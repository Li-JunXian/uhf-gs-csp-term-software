/**
 * @file predict.c
 * Satellite prediction for LEO satellites.
 *
 * The Prediction file has been extracted from predict version 2.2.1.
 * However the prediction part for satellites in deep space (orbit period > 225 min)
 * has been removed so it only work with LEO satellites. Also instead of working with multiple satellites
 * it has been modified to work only with one satellite.
 *
 * If it should work with multiple satellites the structs sat and sat_db should be changed to
 * sat[24] and sat_db[24] and the functions where they are used should have added a int x in there deceleration
 * where x in the sat number wanted e.g setTLE(char* line1, char* line2, int x);
 * The functions to be changed are:
 * internalUpdate,AosHappens,Decayed,setTLE
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>

#include "predict.h"

/* Constants used by SGP4 code */
#define	km2mi		0.621371				//! km to miles
#define deg2rad		1.745329251994330E-2	//! Degrees to radians
#define pi			3.14159265358979323846	//! Pi
#define pio2		1.57079632679489656		//! Pi/2
#define x3pio2		4.71238898038468967		//! 3*Pi/2
#define twopi		6.28318530717958623		//! 2*Pi
#define e6a			1.0E-6
#define tothrd		6.6666666666666666E-1	//! 2/3
#define xj2			1.0826158E-3			//! J2 Harmonic (WGS '72)
#define xj3			-2.53881E-6				//! J3 Harmonic (WGS '72)
#define xj4			-1.65597E-6				//! J4 Harmonic (WGS '72)
#define xke			7.43669161E-2
#define xkmper		6.378137E3				//! WGS 84 Earth radius km
#define xmnpda		1.44E3					//! Minutes per day
#define ae			1.0
#define ck2			5.413079E-4
#define ck4			6.209887E-7
#define f			3.35281066474748E-3		//! Flattening factor
#define ge			3.986008E5				//! Earth gravitational constant (WGS '72)
#define s			1.012229
#define qoms2t		1.880279E-09
#define secday		8.6400E4				//! Seconds per day
#define omega_E		1.00273790934			//! Earth rotations/siderial day
#define omega_ER	6.3003879				//! Earth rotations, rads/siderial day
#define zns			1.19459E-5
#define c1ss		2.9864797E-6
#define zes			1.675E-2
#define znl			1.5835218E-4
#define c1l			4.7968065E-7
#define zel			5.490E-2
#define zcosis		9.1744867E-1
#define zsinis		3.9785416E-1
#define zsings		-9.8088458E-1
#define zcosgs		1.945905E-1
#define zcoshs		1
#define zsinhs		0
#define q22			1.7891679E-6
#define q31			2.1460748E-6
#define q33			2.2123015E-7
#define g22			5.7686396
#define g32			9.5240898E-1
#define g44			1.8014998
#define g52			1.0508330
#define g54			4.4108898
#define root22		1.7891679E-6
#define root32		3.7393792E-7
#define root44		7.3636953E-9
#define root52		1.1428639E-7
#define root54		2.1765803E-9
#define thdt		4.3752691E-3
#define rho			1.5696615E-1
#define mfactor		7.292115E-5
#define sr			6.96000E5				//! Solar radius - km (IAU 76)
#define AU			1.49597870691E8			//! Astronomical unit - km (IAU 76)

/* Entry points of Deep() */
#define dpinit   	1						//! Deep-space initialization code
#define dpsec    	2						//! Deep-space secular code
#define dpper    	3						//! Deep-space periodic code

/* Flow control flag definitions */
#define ALL_FLAGS              -1
#define SGP_INITIALIZED_FLAG   0x000001		//! not used
#define SGP4_INITIALIZED_FLAG  0x000002
#define SDP4_INITIALIZED_FLAG  0x000004
#define SGP8_INITIALIZED_FLAG  0x000008		//! not used
#define SDP8_INITIALIZED_FLAG  0x000010		//! not used
#define SIMPLE_FLAG            0x000020
#define DEEP_SPACE_EPHEM_FLAG  0x000040
#define LUNAR_TERMS_DONE_FLAG  0x000080
#define NEW_EPHEMERIS_FLAG     0x000100		//! not used
#define DO_LOOP_FLAG           0x000200
#define RESONANCE_FLAG         0x000400
#define SYNCHRONOUS_FLAG       0x000800
#define EPOCH_RESTART_FLAG     0x001000
#define VISIBLE_FLAG           0x002000
#define SAT_ECLIPSED_FLAG      0x004000

/* Global Variables */
static struct {
	char line1[70];
	char line2[70];
	char name[25];
	long catnum;
	long setnum;
	char designator[10];
	int year;
	double refepoch;
	double incl;
	double raan;
	double eccn;
	double argper;
	double meanan;
	double meanmo;
	double drag;
	double nddot6;
	double bstar;
	long orbitnum;
} sat;

typedef struct {
	double stnlat;
	double stnlong;
	int stnalt;
} QTH;

static struct {
	char name[25];
	long catnum;
	char squintflag;
	double alat;
	double alon;
	unsigned char transponders;
	char transponder_name[10][80];
	double uplink_start[10];
	double uplink_end[10];
	double downlink_start[10];
	double downlink_end[10];
	unsigned char dayofweek[10];
	int phase_start[10];
	int phase_end[10];
} sat_db;

/* Global variables for sharing data among functions... */

static double tsince, jul_epoch, jul_utc, eclipse_depth = 0, sat_azi, sat_ele,
		sat_range, sat_range_rate, sat_lat, sat_lon, sat_alt, sat_vel, phase,
		sun_azi, sun_ele, daynum, fm, fk, age, aostime, lostime, ax, ay, az, rx,
		ry, rz, squint, alat, alon;

static char temp[80], sat_sun_status, findsun, calc_squint;

static int iaz, iel, ma256, isplat, isplong, Flags = 0;

static long rv, irk;

static unsigned char val[256];

/** Type definitions **/

/* Two-line-element satellite orbital data
 structure used directly by the SGP4/SDP4 code. */

typedef struct {
	double epoch, xndt2o, xndd6o, bstar, xincl, xnodeo, eo, omegao, xmo, xno;
	int catnr, elset, revnum;
	char sat_name[25], idesg[9];
} tle_t;

/* Geodetic position structure used by SGP4/SDP4 code. */

typedef struct {
	double lat, lon, alt, theta;
} geodetic_t;

/* General three-dimensional vector structure used by SGP4/SDP4 code. */

typedef struct {
	double x, y, z, w;
} vector_t;

/* Common arguments between deep-space functions used by SGP4/SDP4 code. */

typedef struct {
	/* Used by dpinit part of Deep() */
	double eosq, sinio, cosio, betao, aodp, theta2, sing, cosg, betao2, xmdot,
			omgdot, xnodot, xnodp;

	/* Used by dpsec and dpper parts of Deep() */
	double xll, omgadf, xnode, em, xinc, xn, t;

	/* Used by thetg and Deep() */
	double ds50;
} deep_arg_t;

/* Global structure used by SGP4/SDP4 code. */

static geodetic_t obs_geodetic;

/* Two-line Orbital Elements for the satellite used by SGP4/SDP4 code. */

static tle_t tle;

/* init qth/groundstation data */
static QTH qth = { .stnlat = 57.0131, .stnlong = 9.998, .stnalt = 5 };

/* Functions for testing and setting/clearing flags used in SGP4/SDP4 code */

int isFlagSet(int flag) {
	return (Flags & flag);
}

int isFlagClear(int flag) {
	return (~Flags & flag);
}

void SetFlag(int flag) {
	Flags |= flag;
}

void ClearFlag(int flag) {
	Flags &= ~flag;
}

/* Remaining SGP4/SDP4 code follows... */

int Sign(double arg) {
	/* Returns sign of a double */

	if (arg > 0)
		return 1;

	else if (arg < 0)
		return -1;

	else
		return 0;
}

double Sqr(double arg) {
	/* Returns square of a double */
	return (arg * arg);
}

double Cube(double arg) {
	/* Returns cube of a double */
	return (arg * arg * arg);
}

double Radians(double arg) {
	/* Returns angle in radians from argument in degrees */
	return (arg * deg2rad);
}

double Degrees(double arg) {
	/* Returns angle in degrees from argument in radians */
	return (arg / deg2rad);
}

double ArcSin(double arg) {
	/* Returns the arcsine of the argument */

	if (fabs(arg) >= 1.0)
		return (Sign(arg) * pio2);
	else
		return (atan(arg / sqrt(1.0 - arg * arg)));
}

double ArcCos(double arg) {
	/* Returns arccosine of argument */
	return (pio2 - ArcSin(arg));
}

void Magnitude(vector_t *v) {
	/* Calculates scalar magnitude of a vector_t argument */
	v->w = sqrt(Sqr(v->x) + Sqr(v->y) + Sqr(v->z));
}

void Vec_Add(vector_t *v1, vector_t *v2, vector_t *v3) {
	/* Adds vectors v1 and v2 together to produce v3 */
	v3->x = v1->x + v2->x;
	v3->y = v1->y + v2->y;
	v3->z = v1->z + v2->z;
	Magnitude(v3);
}

void Vec_Sub(vector_t *v1, vector_t *v2, vector_t *v3) {
	/* Subtracts vector v2 from v1 to produce v3 */
	v3->x = v1->x - v2->x;
	v3->y = v1->y - v2->y;
	v3->z = v1->z - v2->z;
	Magnitude(v3);
}

void Scalar_Multiply(double k, vector_t *v1, vector_t *v2) {
	/* Multiplies the vector v1 by the scalar k to produce the vector v2 */
	v2->x = k * v1->x;
	v2->y = k * v1->y;
	v2->z = k * v1->z;
	v2->w = fabs(k) * v1->w;
}

void Scale_Vector(double k, vector_t *v) {
	/* Multiplies the vector v1 by the scalar k */
	v->x *= k;
	v->y *= k;
	v->z *= k;
	Magnitude(v);
}

double Dot(vector_t *v1, vector_t *v2) {
	/* Returns the dot product of two vectors */
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

double Angle(vector_t *v1, vector_t *v2) {
	/* Calculates the angle between vectors v1 and v2 */
	Magnitude(v1);
	Magnitude(v2);
	return (ArcCos(Dot(v1, v2) / (v1->w * v2->w)));
}

void Cross(vector_t *v1, vector_t *v2, vector_t *v3) {
	/* Produces cross product of v1 and v2, and returns in v3 */
	v3->x = v1->y * v2->z - v1->z * v2->y;
	v3->y = v1->z * v2->x - v1->x * v2->z;
	v3->z = v1->x * v2->y - v1->y * v2->x;
	Magnitude(v3);
}

void Normalize(vector_t *v) {
	/* Normalizes a vector */
	v->x /= v->w;
	v->y /= v->w;
	v->z /= v->w;
}

double AcTan(double sinx, double cosx) {
	/* Four-quadrant arctan function */

	if (cosx == 0.0) {
		if (sinx > 0.0)
			return (pio2);
		else
			return (x3pio2);
	}

	else {
		if (cosx > 0.0) {
			if (sinx > 0.0)
				return (atan(sinx / cosx));
			else
				return (twopi + atan(sinx / cosx));
		}

		else
			return (pi + atan(sinx / cosx));
	}
}

double FMod2p(double x) {
	/* Returns mod 2PI of argument */

	int i;
	double ret_val;

	ret_val = x;
	i = ret_val / twopi;
	ret_val -= i * twopi;

	if (ret_val < 0.0)
		ret_val += twopi;

	return ret_val;
}

double Modulus(double arg1, double arg2) {
	/* Returns arg1 mod arg2 */

	int i;
	double ret_val;

	ret_val = arg1;
	i = ret_val / arg2;
	ret_val -= i * arg2;

	if (ret_val < 0.0)
		ret_val += arg2;

	return ret_val;
}

double Frac(double arg) {
	/* Returns fractional part of double argument */
	return (arg - floor(arg));
}

int Round(double arg) {
	/* Returns argument rounded up to nearest integer */
	return ((int) floor(arg + 0.5));
}

double Int(double arg) {
	/* Returns the floor integer of a double arguement, as double */
	return (floor(arg));
}

double CurrentDaynum() {
	time_t ts;
	time(&ts);
	return (((double) ts) / 86400.0) - 3651.0;
}

void Convert_Sat_State(vector_t *pos, vector_t *vel) {
	/* Converts the satellite's position and velocity  */
	/* vectors from normalized values to km and km/sec */
	Scale_Vector(xkmper, pos);
	Scale_Vector(xkmper * xmnpda / secday, vel);
}

double Julian_Date_of_Year(double year) {
	/* The function Julian_Date_of_Year calculates the Julian Date  */
	/* of Day 0.0 of {year}. This function is used to calculate the */
	/* Julian Date of any date by using Julian_Date_of_Year, DOY,   */
	/* and Fraction_of_Day. */

	/* Astronomical Formulae for Calculators, Jean Meeus, */
	/* pages 23-25. Calculate Julian Date of 0.0 Jan year */

	long A, B, i;
	double jdoy;

	year = year - 1;
	i = year / 100;
	A = i;
	i = A / 4;
	B = 2 - A + i;
	i = 365.25 * year;
	i += 30.6001 * 14;
	jdoy = i + 1720994.5 + B;

	return jdoy;
}

double Julian_Date_of_Epoch(double epoch) {
	/* The function Julian_Date_of_Epoch returns the Julian Date of     */
	/* an epoch specified in the format used in the NORAD two-line      */
	/* element sets. It has been modified to support dates beyond       */
	/* the year 1999 assuming that two-digit years in the range 00-56   */
	/* correspond to 2000-2056. Until the two-line element set format   */
	/* is changed, it is only valid for dates through 2056 December 31. */

	double year, day;

	/* Modification to support Y2K */
	/* Valid 1957 through 2056     */

	day = modf(epoch * 1E-3, &year) * 1E3;

	if (year < 57)
		year = year + 2000;
	else
		year = year + 1900;

	return (Julian_Date_of_Year(year) + day);
}

int DOY(int yr, int mo, int dy) {
	/* The function DOY calculates the day of the year for the specified */
	/* date. The calculation uses the rules for the Gregorian calendar   */
	/* and is valid from the inception of that calendar system.          */

	const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int i, day;

	day = 0;

	for (i = 0; i < mo - 1; i++)
		day += days[i];

	day = day + dy;

	/* Leap year correction */

	if ((yr % 4 == 0) && ((yr % 100 != 0) || (yr % 400 == 0)) && (mo > 2))
		day++;

	return day;
}

double Fraction_of_Day(int hr, int mi, double se) {
	/* Fraction_of_Day calculates the fraction of */
	/* a day passed at the specified input time.  */

	double dhr, dmi;

	dhr = (double) hr;
	dmi = (double) mi;

	return ((dhr + (dmi + se / 60.0) / 60.0) / 24.0);
}

double Julian_Date(struct tm *cdate) {
	/* The function Julian_Date converts a standard calendar   */
	/* date and time to a Julian Date. */

	double julian_date;

	julian_date = Julian_Date_of_Year(cdate->tm_year)
			+ DOY(cdate->tm_year, cdate->tm_mon, cdate->tm_mday)
			+ Fraction_of_Day(cdate->tm_hour, cdate->tm_min, cdate->tm_sec)
			+ 5.787037e-06; /* Round up to nearest 1 sec */

	return julian_date;
}

double Delta_ET(double year) {
	/* The function Delta_ET has been added to allow calculations on   */
	/* the position of the sun.  It provides the difference between UT */
	/* (approximately the same as UTC) and ET (now referred to as TDT).*/
	/* This function is based on a least squares fit of data from 1950 */
	/* to 1991 and will need to be updated periodically. */

	/* Values determined using data from 1950-1991 in the 1990
	 Astronomical Almanac.  See DELTA_ET.WQ1 for details. */

	double delta_et;

	delta_et = 26.465 + 0.747622 * (year - 1950)
			+ 1.886913 * sin(twopi * (year - 1975) / 33);

	return delta_et;
}

double ThetaG(double epoch, deep_arg_t *deep_arg) {
	/* The function ThetaG calculates the Greenwich Mean Sidereal Time */
	/* for an epoch specified in the format used in the NORAD two-line */
	/* element sets. It has now been adapted for dates beyond the year */
	/* 1999, as described above. The function ThetaG_JD provides the   */
	/* same calculation except that it is based on an input in the     */
	/* form of a Julian Date. */

	/* Reference:  The 1992 Astronomical Almanac, page B6. */

	double year, day, UT, jd, TU, GMST, ThetaG;

	/* Modification to support Y2K */
	/* Valid 1957 through 2056     */

	day = modf(epoch * 1E-3, &year) * 1E3;

	if (year < 57)
		year += 2000;
	else
		year += 1900;

	UT = modf(day, &day);
	jd = Julian_Date_of_Year(year) + day;
	TU = (jd - 2451545.0) / 36525;
	GMST = 24110.54841 + TU * (8640184.812866 + TU * (0.093104 - TU * 6.2E-6));
	GMST = Modulus(GMST + secday * omega_E * UT, secday);
	ThetaG = twopi * GMST / secday;
	deep_arg->ds50 = jd - 2433281.5 + UT;
	ThetaG = FMod2p(6.3003880987 * deep_arg->ds50 + 1.72944494);

	return ThetaG;
}

double ThetaG_JD(double jd) {
	/* Reference:  The 1992 Astronomical Almanac, page B6. */

	double UT, TU, GMST;

	UT = Frac(jd + 0.5);
	jd = jd - UT;
	TU = (jd - 2451545.0) / 36525;
	GMST = 24110.54841 + TU * (8640184.812866 + TU * (0.093104 - TU * 6.2E-6));
	GMST = Modulus(GMST + secday * omega_E * UT, secday);

	return (twopi * GMST / secday);
}

void Calculate_Solar_Position(double time, vector_t *solar_vector) {
	/* Calculates solar position vector */

	double mjd, year, T, M, L, e, C, O, Lsa, nu, R, eps;

	mjd = time - 2415020.0;
	year = 1900 + mjd / 365.25;
	T = (mjd + Delta_ET(year) / secday) / 36525.0;
	M = Radians(
			Modulus(
					358.47583 + Modulus(35999.04975 * T, 360.0)
							- (0.000150 + 0.0000033 * T) * Sqr(T), 360.0));
	L = Radians(
			Modulus(
					279.69668 + Modulus(36000.76892 * T, 360.0)
							+ 0.0003025 * Sqr(T), 360.0));
	e = 0.01675104 - (0.0000418 + 0.000000126 * T) * T;
	C = Radians(
			(1.919460 - (0.004789 + 0.000014 * T) * T) * sin(M)
					+ (0.020094 - 0.000100 * T) * sin(2 * M)
					+ 0.000293 * sin(3 * M));
	O = Radians(Modulus(259.18 - 1934.142 * T, 360.0));
	Lsa = Modulus(L + C - Radians(0.00569 - 0.00479 * sin(O)), twopi);
	nu = Modulus(M + C, twopi);
	R = 1.0000002 * (1.0 - Sqr(e)) / (1.0 + e * cos(nu));
	eps = Radians(
			23.452294 - (0.0130125 + (0.00000164 - 0.000000503 * T) * T) * T
					+ 0.00256 * cos(O));
	R = AU * R;
	solar_vector->x = R * cos(Lsa);
	solar_vector->y = R * sin(Lsa) * cos(eps);
	solar_vector->z = R * sin(Lsa) * sin(eps);
	solar_vector->w = R;
}

int Sat_Eclipsed(vector_t *pos, vector_t *sol, double *depth) {
	/* Calculates stellite's eclipse status and depth */

	double sd_sun, sd_earth, delta;
	vector_t Rho, earth;

	/* Determine partial eclipse */

	sd_earth = ArcSin(xkmper / pos->w);
	Vec_Sub(sol, pos, &Rho);
	sd_sun = ArcSin(sr / Rho.w);
	Scalar_Multiply(-1, pos, &earth);
	delta = Angle(sol, &earth);
	*depth = sd_earth - sd_sun - delta;

	if (sd_earth < sd_sun)
		return 0;
	else if (*depth >= 0)
		return 1;
	else
		return 0;
}

void select_ephemeris(tle_t *tle) {
	/* Selects the apropriate ephemeris type to be used */
	/* for predictions according to the data in the TLE */
	/* It also processes values in the tle set so that  */
	/* they are apropriate for the sgp4/sdp4 routines   */

	double temp;

	/* Preprocess tle set */
	tle->xnodeo *= deg2rad;
	tle->omegao *= deg2rad;
	tle->xmo *= deg2rad;
	tle->xincl *= deg2rad;
	temp = twopi / xmnpda / xmnpda;
	tle->xno = tle->xno * temp * xmnpda;
	tle->xndt2o *= temp;
	tle->xndd6o = tle->xndd6o * temp / xmnpda;
	tle->bstar /= ae;

}

void SGP4(double tsince, tle_t * tle, vector_t * pos, vector_t * vel) {
	/* This function is used to calculate the position and velocity */
	/* of near-earth (period < 225 minutes) satellites. tsince is   */
	/* time since epoch in minutes, tle is a pointer to a tle_t     */
	/* structure with Keplerian orbital elements and pos and vel    */
	/* are vector_t structures returning ECI satellite position and */
	/* velocity. Use Convert_Sat_State() to convert to km and km/s. */

	static double aodp, aycof, c1, c4, c5, cosio, d2, d3, d4, delmo, omgcof,
			eta, omgdot, sinio, xnodp, sinmo, t2cof, t3cof, t4cof, t5cof,
			x1mth2, x3thm1, x7thm1, xmcof, xmdot, xnodcf, xnodot, xlcof;

	double cosuk, sinuk, rfdotk, vx, vy, vz, ux, uy, uz, xmy, xmx, cosnok,
			sinnok, cosik, sinik, rdotk, xinck, xnodek, uk, rk, cos2u, sin2u, u,
			sinu, cosu, betal, rfdot, rdot, r, pl, elsq, esine, ecose, epw,
			cosepw, x1m5th, xhdot1, tfour, sinepw, capu, ayn, xlt, aynl, xll,
			axn, xn, beta, xl, e, a, tcube, delm, delomg, templ, tempe, tempa,
			xnode, tsq, xmp, omega, xnoddf, omgadf, xmdf, a1, a3ovk2, ao, betao,
			betao2, c1sq, c2, c3, coef, coef1, del1, delo, eeta, eosq, etasq,
			perigee, pinvsq, psisq, qoms24, s4, temp, temp1, temp2, temp3,
			temp4, temp5, temp6, theta2, theta4, tsi;

	int i;

	/* Initialization */

	if (isFlagClear(SGP4_INITIALIZED_FLAG)) {
		SetFlag(SGP4_INITIALIZED_FLAG);

		/* Recover original mean motion (xnodp) and   */
		/* semimajor axis (aodp) from input elements. */

		a1 = pow(xke / tle->xno, tothrd);
		cosio = cos(tle->xincl);
		theta2 = cosio * cosio;
		x3thm1 = 3 * theta2 - 1.0;
		eosq = tle->eo * tle->eo;
		betao2 = 1.0 - eosq;
		betao = sqrt(betao2);
		del1 = 1.5 * ck2 * x3thm1 / (a1 * a1 * betao * betao2);
		ao = a1
				* (1.0
						- del1
								* (0.5 * tothrd
										+ del1 * (1.0 + 134.0 / 81.0 * del1)));
		delo = 1.5 * ck2 * x3thm1 / (ao * ao * betao * betao2);
		xnodp = tle->xno / (1.0 + delo);
		aodp = ao / (1.0 - delo);

		/* For perigee less than 220 kilometers, the "simple"     */
		/* flag is set and the equations are truncated to linear  */
		/* variation in sqrt a and quadratic variation in mean    */
		/* anomaly.  Also, the c3 term, the delta omega term, and */
		/* the delta m term are dropped.                          */

		if ((aodp * (1 - tle->eo) / ae) < (220 / xkmper + ae))
			SetFlag(SIMPLE_FLAG);

		else
			ClearFlag(SIMPLE_FLAG);

		/* For perigees below 156 km, the      */
		/* values of s and qoms2t are altered. */

		s4 = s;
		qoms24 = qoms2t;
		perigee = (aodp * (1 - tle->eo) - ae) * xkmper;

		if (perigee < 156.0) {
			if (perigee <= 98.0)
				s4 = 20;
			else
				s4 = perigee - 78.0;

			qoms24 = pow((120 - s4) * ae / xkmper, 4);
			s4 = s4 / xkmper + ae;
		}

		pinvsq = 1 / (aodp * aodp * betao2 * betao2);
		tsi = 1 / (aodp - s4);
		eta = aodp * tle->eo * tsi;
		etasq = eta * eta;
		eeta = tle->eo * eta;
		psisq = fabs(1 - etasq);
		coef = qoms24 * pow(tsi, 4);
		coef1 = coef / pow(psisq, 3.5);
		c2 = coef1 * xnodp
				* (aodp * (1 + 1.5 * etasq + eeta * (4 + etasq))
						+ 0.75 * ck2 * tsi / psisq * x3thm1
								* (8 + 3 * etasq * (8 + etasq)));
		c1 = tle->bstar * c2;
		sinio = sin(tle->xincl);
		a3ovk2 = -xj3 / ck2 * pow(ae, 3);
		c3 = coef * tsi * a3ovk2 * xnodp * ae * sinio / tle->eo;
		x1mth2 = 1 - theta2;

		c4 = 2 * xnodp * coef1 * aodp * betao2
				* (eta * (2 + 0.5 * etasq) + tle->eo * (0.5 + 2 * etasq)
						- 2 * ck2 * tsi / (aodp * psisq)
								* (-3 * x3thm1
										* (1 - 2 * eeta
												+ etasq * (1.5 - 0.5 * eeta))
										+ 0.75 * x1mth2
												* (2 * etasq
														- eeta * (1 + etasq))
												* cos(2 * tle->omegao)));
		c5 = 2 * coef1 * aodp * betao2
				* (1 + 2.75 * (etasq + eeta) + eeta * etasq);

		theta4 = theta2 * theta2;
		temp1 = 3 * ck2 * pinvsq * xnodp;
		temp2 = temp1 * ck2 * pinvsq;
		temp3 = 1.25 * ck4 * pinvsq * pinvsq * xnodp;
		xmdot = xnodp + 0.5 * temp1 * betao * x3thm1
				+ 0.0625 * temp2 * betao * (13 - 78 * theta2 + 137 * theta4);
		x1m5th = 1 - 5 * theta2;
		omgdot = -0.5 * temp1 * x1m5th
				+ 0.0625 * temp2 * (7 - 114 * theta2 + 395 * theta4)
				+ temp3 * (3 - 36 * theta2 + 49 * theta4);
		xhdot1 = -temp1 * cosio;
		xnodot = xhdot1
				+ (0.5 * temp2 * (4 - 19 * theta2)
						+ 2 * temp3 * (3 - 7 * theta2)) * cosio;
		omgcof = tle->bstar * c3 * cos(tle->omegao);
		xmcof = -tothrd * coef * tle->bstar * ae / eeta;
		xnodcf = 3.5 * betao2 * xhdot1 * c1;
		t2cof = 1.5 * c1;
		xlcof = 0.125 * a3ovk2 * sinio * (3 + 5 * cosio) / (1 + cosio);
		aycof = 0.25 * a3ovk2 * sinio;
		delmo = pow(1 + eta * cos(tle->xmo), 3);
		sinmo = sin(tle->xmo);
		x7thm1 = 7 * theta2 - 1;

		if (isFlagClear(SIMPLE_FLAG)) {
			c1sq = c1 * c1;
			d2 = 4 * aodp * tsi * c1sq;
			temp = d2 * tsi * c1 / 3;
			d3 = (17 * aodp + s4) * temp;
			d4 = 0.5 * temp * aodp * tsi * (221 * aodp + 31 * s4) * c1;
			t3cof = d2 + 2 * c1sq;
			t4cof = 0.25 * (3 * d3 + c1 * (12 * d2 + 10 * c1sq));
			t5cof = 0.2
					* (3 * d4 + 12 * c1 * d3 + 6 * d2 * d2
							+ 15 * c1sq * (2 * d2 + c1sq));
		}
	}

	/* Update for secular gravity and atmospheric drag. */
	xmdf = tle->xmo + xmdot * tsince;
	omgadf = tle->omegao + omgdot * tsince;
	xnoddf = tle->xnodeo + xnodot * tsince;
	omega = omgadf;
	xmp = xmdf;
	tsq = tsince * tsince;
	xnode = xnoddf + xnodcf * tsq;
	tempa = 1 - c1 * tsince;
	tempe = tle->bstar * c4 * tsince;
	templ = t2cof * tsq;

	if (isFlagClear(SIMPLE_FLAG)) {
		delomg = omgcof * tsince;
		delm = xmcof * (pow(1 + eta * cos(xmdf), 3) - delmo);
		temp = delomg + delm;
		xmp = xmdf + temp;
		omega = omgadf - temp;
		tcube = tsq * tsince;
		tfour = tsince * tcube;
		tempa = tempa - d2 * tsq - d3 * tcube - d4 * tfour;
		tempe = tempe + tle->bstar * c5 * (sin(xmp) - sinmo);
		templ = templ + t3cof * tcube + tfour * (t4cof + tsince * t5cof);
	}

	a = aodp * pow(tempa, 2);
	e = tle->eo - tempe;
	xl = xmp + omega + xnode + xnodp * templ;
	beta = sqrt(1 - e * e);
	xn = xke / pow(a, 1.5);

	/* Long period periodics */
	axn = e * cos(omega);
	temp = 1 / (a * beta * beta);
	xll = temp * xlcof * axn;
	aynl = temp * aycof;
	xlt = xl + xll;
	ayn = e * sin(omega) + aynl;

	/* Solve Kepler's Equation */
	capu = FMod2p(xlt - xnode);
	temp2 = capu;
	i = 0;

	do {
		sinepw = sin(temp2);
		cosepw = cos(temp2);
		temp3 = axn * sinepw;
		temp4 = ayn * cosepw;
		temp5 = axn * cosepw;
		temp6 = ayn * sinepw;
		epw = (capu - temp4 + temp3 - temp2) / (1 - temp5 - temp6) + temp2;

		if (fabs(epw - temp2) <= e6a)
			break;

		temp2 = epw;

	} while (i++ < 10);

	/* Short period preliminary quantities */
	ecose = temp5 + temp6;
	esine = temp3 - temp4;
	elsq = axn * axn + ayn * ayn;
	temp = 1 - elsq;
	pl = a * temp;
	r = a * (1 - ecose);
	temp1 = 1 / r;
	rdot = xke * sqrt(a) * esine * temp1;
	rfdot = xke * sqrt(pl) * temp1;
	temp2 = a * temp1;
	betal = sqrt(temp);
	temp3 = 1 / (1 + betal);
	cosu = temp2 * (cosepw - axn + ayn * esine * temp3);
	sinu = temp2 * (sinepw - ayn - axn * esine * temp3);
	u = AcTan(sinu, cosu);
	sin2u = 2 * sinu * cosu;
	cos2u = 2 * cosu * cosu - 1;
	temp = 1 / pl;
	temp1 = ck2 * temp;
	temp2 = temp1 * temp;

	/* Update for short periodics */
	rk = r * (1 - 1.5 * temp2 * betal * x3thm1) + 0.5 * temp1 * x1mth2 * cos2u;
	uk = u - 0.25 * temp2 * x7thm1 * sin2u;
	xnodek = xnode + 1.5 * temp2 * cosio * sin2u;
	xinck = tle->xincl + 1.5 * temp2 * cosio * sinio * cos2u;
	rdotk = rdot - xn * temp1 * x1mth2 * sin2u;
	rfdotk = rfdot + xn * temp1 * (x1mth2 * cos2u + 1.5 * x3thm1);

	/* Orientation vectors */
	sinuk = sin(uk);
	cosuk = cos(uk);
	sinik = sin(xinck);
	cosik = cos(xinck);
	sinnok = sin(xnodek);
	cosnok = cos(xnodek);
	xmx = -sinnok * cosik;
	xmy = cosnok * cosik;
	ux = xmx * sinuk + cosnok * cosuk;
	uy = xmy * sinuk + sinnok * cosuk;
	uz = sinik * sinuk;
	vx = xmx * cosuk - cosnok * sinuk;
	vy = xmy * cosuk - sinnok * sinuk;
	vz = sinik * cosuk;

	/* Position and velocity */
	pos->x = rk * ux;
	pos->y = rk * uy;
	pos->z = rk * uz;
	vel->x = rdotk * ux + rfdotk * vx;
	vel->y = rdotk * uy + rfdotk * vy;
	vel->z = rdotk * uz + rfdotk * vz;

	/* Phase in radians */
	phase = xlt - xnode - omgadf + twopi;

	if (phase < 0.0)
		phase += twopi;

	phase = FMod2p(phase);
}

void Calculate_User_PosVel(double time, geodetic_t *geodetic, vector_t *obs_pos,
		vector_t *obs_vel) {
	/* Calculate_User_PosVel() passes the user's geodetic position
	 and the time of interest and returns the ECI position and
	 velocity of the observer.  The velocity calculation assumes
	 the geodetic position is stationary relative to the earth's
	 surface. */

	/* Reference:  The 1992 Astronomical Almanac, page K11. */

	double c, sq, achcp;

	geodetic->theta = FMod2p(ThetaG_JD(time) + geodetic->lon); /* LMST */
	c = 1 / sqrt(1 + f * (f - 2) * Sqr(sin(geodetic->lat)));
	sq = Sqr(1 - f) * c;
	achcp = (xkmper * c + geodetic->alt) * cos(geodetic->lat);
	obs_pos->x = achcp * cos(geodetic->theta); /* kilometers */
	obs_pos->y = achcp * sin(geodetic->theta);
	obs_pos->z = (xkmper * sq + geodetic->alt) * sin(geodetic->lat);
	obs_vel->x = -mfactor * obs_pos->y; /* kilometers/second */
	obs_vel->y = mfactor * obs_pos->x;
	obs_vel->z = 0;
	Magnitude(obs_pos);
	Magnitude(obs_vel);

}

void Calculate_LatLonAlt(double time, vector_t *pos, geodetic_t *geodetic) {
	/* Procedure Calculate_LatLonAlt will calculate the geodetic  */
	/* position of an object given its ECI position pos and time. */
	/* It is intended to be used to determine the ground track of */
	/* a satellite.  The calculations  assume the earth to be an  */
	/* oblate spheroid as defined in WGS '72.                     */

	/* Reference:  The 1992 Astronomical Almanac, page K12. */

	double r, e2, phi, c;

	geodetic->theta = AcTan(pos->y, pos->x); /* radians */
	geodetic->lon = FMod2p(geodetic->theta - ThetaG_JD(time)); /* radians */
	r = sqrt(Sqr(pos->x) + Sqr(pos->y));
	e2 = f * (2 - f);
	geodetic->lat = AcTan(pos->z, r); /* radians */

	do {
		phi = geodetic->lat;
		c = 1 / sqrt(1 - e2 * Sqr(sin(phi)));
		geodetic->lat = AcTan(pos->z + xkmper * c * e2 * sin(phi), r);

	} while (fabs(geodetic->lat - phi) >= 1E-10);

	geodetic->alt = r / cos(geodetic->lat) - xkmper * c; /* kilometers */

	if (geodetic->lat > pio2)
		geodetic->lat -= twopi;
}

void Calculate_Obs(double time, vector_t *pos, vector_t *vel,
		geodetic_t *geodetic, vector_t *obs_set) {
	/* The procedures Calculate_Obs and Calculate_RADec calculate         */
	/* the *topocentric* coordinates of the object with ECI position,     */
	/* {pos}, and velocity, {vel}, from location {geodetic} at {time}.    */
	/* The {obs_set} returned for Calculate_Obs consists of azimuth,      */
	/* elevation, range, and range rate (in that order) with units of     */
	/* radians, radians, kilometers, and kilometers/second, respectively. */
	/* The WGS '72 geoid is used and the effect of atmospheric refraction */
	/* (under standard temperature and pressure) is incorporated into the */
	/* elevation calculation; the effect of atmospheric refraction on     */
	/* range and range rate has not yet been quantified.                  */

	/* The {obs_set} for Calculate_RADec consists of right ascension and  */
	/* declination (in that order) in radians.  Again, calculations are   */
	/* based on *topocentric* position using the WGS '72 geoid and        */
	/* incorporating atmospheric refraction.                              */

	double sin_lat, cos_lat, sin_theta, cos_theta, el, azim, top_s, top_e,
			top_z;

	vector_t obs_pos, obs_vel, range, rgvel;

	Calculate_User_PosVel(time, geodetic, &obs_pos, &obs_vel);

	range.x = pos->x - obs_pos.x;
	range.y = pos->y - obs_pos.y;
	range.z = pos->z - obs_pos.z;

	/* Save these values globally for calculating squint angles later... */

	rx = range.x;
	ry = range.y;
	rz = range.z;

	rgvel.x = vel->x - obs_vel.x;
	rgvel.y = vel->y - obs_vel.y;
	rgvel.z = vel->z - obs_vel.z;

	Magnitude(&range);

	sin_lat = sin(geodetic->lat);
	cos_lat = cos(geodetic->lat);
	sin_theta = sin(geodetic->theta);
	cos_theta = cos(geodetic->theta);
	top_s = sin_lat * cos_theta * range.x + sin_lat * sin_theta * range.y
			- cos_lat * range.z;
	top_e = -sin_theta * range.x + cos_theta * range.y;
	top_z = cos_lat * cos_theta * range.x + cos_lat * sin_theta * range.y
			+ sin_lat * range.z;
	azim = atan(-top_e / top_s); /* Azimuth */

	if (top_s > 0.0)
		azim = azim + pi;

	if (azim < 0.0)
		azim = azim + twopi;

	el = ArcSin(top_z / range.w);
	obs_set->x = azim; /* Azimuth (radians)   */
	obs_set->y = el; /* Elevation (radians) */
	obs_set->z = range.w; /* Range (kilometers)  */

	/* Range Rate (kilometers/second) */

	obs_set->w = Dot(&range, &rgvel) / range.w;

	/* Corrections for atmospheric refraction */
	/* Reference:  Astronomical Algorithms by Jean Meeus, pp. 101-104    */
	/* Correction is meaningless when apparent elevation is below horizon */

	/*** Temporary bypass for PREDICT-2.2.x ***/

	/* obs_set->y=obs_set->y+Radians((1.02/tan(Radians(Degrees(el)+10.3/(Degrees(el)+5.11))))/60); */

	obs_set->y = el;

	/**** End bypass ****/

	if (obs_set->y >= 0.0)
		SetFlag(VISIBLE_FLAG);
	else {
		obs_set->y = el; /* Reset to true elevation */
		ClearFlag(VISIBLE_FLAG);
	}
}

void Calculate_RADec(double time, vector_t *pos, vector_t *vel,
		geodetic_t *geodetic, vector_t *obs_set) {
	/* Reference:  Methods of Orbit Determination by  */
	/*             Pedro Ramon Escobal, pp. 401-402   */

	double phi, theta, sin_theta, cos_theta, sin_phi, cos_phi, az, el, Lxh, Lyh,
			Lzh, Sx, Ex, Zx, Sy, Ey, Zy, Sz, Ez, Zz, Lx, Ly, Lz, cos_delta,
			sin_alpha, cos_alpha;

	Calculate_Obs(time, pos, vel, geodetic, obs_set);

	if (isFlagSet(VISIBLE_FLAG)) {
		az = obs_set->x;
		el = obs_set->y;
		phi = geodetic->lat;
		theta = FMod2p(ThetaG_JD(time) + geodetic->lon);
		sin_theta = sin(theta);
		cos_theta = cos(theta);
		sin_phi = sin(phi);
		cos_phi = cos(phi);
		Lxh = -cos(az) * cos(el);
		Lyh = sin(az) * cos(el);
		Lzh = sin(el);
		Sx = sin_phi * cos_theta;
		Ex = -sin_theta;
		Zx = cos_theta * cos_phi;
		Sy = sin_phi * sin_theta;
		Ey = cos_theta;
		Zy = sin_theta * cos_phi;
		Sz = -cos_phi;
		Ez = 0.0;
		Zz = sin_phi;
		Lx = Sx * Lxh + Ex * Lyh + Zx * Lzh;
		Ly = Sy * Lxh + Ey * Lyh + Zy * Lzh;
		Lz = Sz * Lxh + Ez * Lyh + Zz * Lzh;
		obs_set->y = ArcSin(Lz); /* Declination (radians) */
		cos_delta = sqrt(1.0 - Sqr(Lz));
		sin_alpha = Ly / cos_delta;
		cos_alpha = Lx / cos_delta;
		obs_set->x = AcTan(sin_alpha, cos_alpha); /* Right Ascension (radians) */
		obs_set->x = FMod2p(obs_set->x);
	}
}

/* PREDICT functions follow... */

char *SubString(char *string, unsigned char start, unsigned char end) {
	/* This function returns a substring based on the starting
	 and ending positions provided.  It is used heavily in the
	 AutoUpdate function when parsing 2-line element data. */

	unsigned x, y;

	if (end >= start) {
		for (x = start, y = 0; x <= end && string[x] != 0; x++)
			if (string[x] != ' ') {
				temp[y] = string[x];
				y++;
			}

		temp[y] = 0;
		return temp;
	} else
		return NULL;
}

char KepCheck(char *line1, char *line2) {
	/* This function scans line 1 and line 2 of a NASA 2-Line element
	 set and returns a 1 if the element set appears to be valid or
	 a 0 if it does not.  If the data survives this torture test,
	 it's a pretty safe bet we're looking at a valid 2-line
	 element set and not just some random text that might pass
	 as orbital data based on a simple checksum calculation alone. */

	int x;
	unsigned sum1, sum2;

	/* Compute checksum for each line */

	for (x = 0, sum1 = 0, sum2 = 0; x <= 67;
			sum1 += val[(int) line1[x]], sum2 += val[(int) line2[x]], x++)
		;

	/* Perform a "torture test" on the data */

	x = (val[(int) line1[68]] ^ (sum1 % 10))
			| (val[(int) line2[68]] ^ (sum2 % 10)) | (line1[0] ^ '1')
			| (line1[1] ^ ' ') | (line1[7] ^ 'U') | (line1[8] ^ ' ')
			| (line1[17] ^ ' ') | (line1[23] ^ '.') | (line1[32] ^ ' ')
			| (line1[34] ^ '.') | (line1[43] ^ ' ') | (line1[52] ^ ' ')
			| (line1[61] ^ ' ') | (line1[62] ^ '0') | (line1[63] ^ ' ')
			| (line2[0] ^ '2') | (line2[1] ^ ' ') | (line2[7] ^ ' ')
			| (line2[11] ^ '.') | (line2[16] ^ ' ') | (line2[20] ^ '.')
			| (line2[25] ^ ' ') | (line2[33] ^ ' ') | (line2[37] ^ '.')
			| (line2[42] ^ ' ') | (line2[46] ^ '.') | (line2[51] ^ ' ')
			| (line2[54] ^ '.') | (line1[2] ^ line2[2]) | (line1[3] ^ line2[3])
			| (line1[4] ^ line2[4]) | (line1[5] ^ line2[5])
			| (line1[6] ^ line2[6]) | (isdigit(line1[68]) ? 0 : 1)
			| (isdigit(line2[68]) ? 0 : 1) | (isdigit(line1[18]) ? 0 : 1)
			| (isdigit(line1[19]) ? 0 : 1) | (isdigit(line2[31]) ? 0 : 1)
			| (isdigit(line2[32]) ? 0 : 1);

	return (x ? 0 : 1);
}

void InternalUpdate() {
	/* Updates data in TLE structure based on
	 line1 and line2 stored in structure. */

	double tempnum;

	strncpy(sat.designator, SubString(sat.line1, 9, 16), 8);
	sat.designator[9] = 0;
	sat.catnum = atol(SubString(sat.line1, 2, 6));
	sat.year = atoi(SubString(sat.line1, 18, 19));
	sat.refepoch = atof(SubString(sat.line1, 20, 31));
	tempnum = 1.0e-5 * atof(SubString(sat.line1, 44, 49));
	sat.nddot6 = tempnum / pow(10.0, (sat.line1[51] - '0'));
	tempnum = 1.0e-5 * atof(SubString(sat.line1, 53, 58));
	sat.bstar = tempnum / pow(10.0, (sat.line1[60] - '0'));
	sat.setnum = atol(SubString(sat.line1, 64, 67));
	sat.incl = atof(SubString(sat.line2, 8, 15));
	sat.raan = atof(SubString(sat.line2, 17, 24));
	sat.eccn = 1.0e-07 * atof(SubString(sat.line2, 26, 32));
	sat.argper = atof(SubString(sat.line2, 34, 41));
	sat.meanan = atof(SubString(sat.line2, 43, 50));
	sat.meanmo = atof(SubString(sat.line2, 52, 62));
	sat.drag = atof(SubString(sat.line1, 33, 42));
	sat.orbitnum = atof(SubString(sat.line2, 63, 67));
}

long DayNum(int m, int d, int y) {
	/* This function calculates the day number from m/d/y. */

	long dn;
	double mm, yy;

	if (m < 3) {
		y--;
		m += 12;
	}

	/* Correct for Y2K... */

	if (y <= 50)
		y += 100;

	yy = (double) y;
	mm = (double) m;
	dn = (long) (floor(365.25 * (yy - 80.0)) - floor(19.0 + yy / 100.0)
			+ floor(4.75 + yy / 400.0) - 16.0);
	dn += d + 30 * m + (long) floor(0.6 * mm - 0.3);
	return dn;
}

void set_calc_time(long time) {
	daynum = (((double) time) / 86400.0) - 3651.0;
}

void PreCalc() {
	/* This function copies TLE data from PREDICT's sat structure
	 to the SGP4/SDP4's single dimensioned tle structure, and
	 prepares the tracking code for the update. */

	tle.catnr = sat.catnum;
	tle.epoch = (1000.0 * (double) sat.year) + sat.refepoch;
	tle.xndt2o = sat.drag;
	tle.xndd6o = sat.nddot6;
	tle.bstar = sat.bstar;
	tle.xincl = sat.incl;
	tle.xnodeo = sat.raan;
	tle.eo = sat.eccn;
	tle.omegao = sat.argper;
	tle.xmo = sat.meanan;
	tle.xno = sat.meanmo;
	tle.revnum = sat.orbitnum;

	if (sat_db.squintflag) {
		calc_squint = 1;
		alat = deg2rad * sat_db.alat;
		alon = deg2rad * sat_db.alon;
	} else
		calc_squint = 0;

	ClearFlag(ALL_FLAGS);

	/* Select ephemeris type.  This function will set or clear the
	 DEEP_SPACE_EPHEM_FLAG depending on the TLE parameters of the
	 satellite.  It will also pre-process tle members for the
	 ephemeris functions SGP4 or SDP4, so this function must
	 be called each time a new tle set is used. */

	select_ephemeris(&tle);
}

void Calc() {

	/* This is the stuff we need to do repetitively... */

	/*convert qth data to geodetic */
	obs_geodetic.lat = qth.stnlat * deg2rad;
	obs_geodetic.lon = qth.stnlong * deg2rad;
	obs_geodetic.alt = ((double) qth.stnalt) / 1000.0;
	obs_geodetic.theta = 0.0;

	/* Zero vector for initializations */
	vector_t zero_vector = { 0, 0, 0, 0 };

	/* Satellite position and velocity vectors */
	vector_t vel = zero_vector;
	vector_t pos = zero_vector;

	/* Satellite Az, El, Range, Range rate */
	vector_t obs_set;

	/* Solar ECI position vector  */
	vector_t solar_vector = zero_vector;

	/* Solar observed azi and ele vector  */
	vector_t solar_set;

	/* Satellite's predicted geodetic position */
	geodetic_t sat_geodetic;

	jul_utc = daynum + 2444238.5;

	/* Convert satellite's epoch time to Julian  */
	/* and calculate time since epoch in minutes */

	jul_epoch = Julian_Date_of_Epoch(tle.epoch);
	tsince = (jul_utc - jul_epoch) * xmnpda;
	age = jul_utc - jul_epoch;

	/* Call NORAD SGP4 routines For LEO satellites */

	SGP4(tsince, &tle, &pos, &vel);

	/* Scale position and velocity vectors to km and km/sec */

	Convert_Sat_State(&pos, &vel);

	/* Calculate velocity of satellite */

	Magnitude(&vel);
	sat_vel = vel.w;

	/** All angles in rads. Distance in km. Velocity in km/s **/
	/* Calculate satellite Azi, Ele, Range and Range-rate */

	Calculate_Obs(jul_utc, &pos, &vel, &obs_geodetic, &obs_set);

	/* Calculate satellite Lat North, Lon East and Alt. */

	Calculate_LatLonAlt(jul_utc, &pos, &sat_geodetic);

	/* Calculate squint angle */

	if (calc_squint)
		squint = (acos(-(ax * rx + ay * ry + az * rz) / obs_set.z)) / deg2rad;

	/* Calculate solar position and satellite eclipse depth. */
	/* Also set or clear the satellite eclipsed flag accordingly. */

	Calculate_Solar_Position(jul_utc, &solar_vector);
	Calculate_Obs(jul_utc, &solar_vector, &zero_vector, &obs_geodetic,
			&solar_set);

	if (Sat_Eclipsed(&pos, &solar_vector, &eclipse_depth))
		SetFlag(SAT_ECLIPSED_FLAG);
	else
		ClearFlag(SAT_ECLIPSED_FLAG);

	if (isFlagSet(SAT_ECLIPSED_FLAG))
		sat_sun_status = 0; /* Eclipse */
	else
		sat_sun_status = 1; /* In sunlight */

	/* Convert satellite and solar data */
	sat_azi = Degrees(obs_set.x);
	sat_ele = Degrees(obs_set.y);
	sat_range = obs_set.z;
	sat_range_rate = obs_set.w;
	sat_lat = Degrees(sat_geodetic.lat);
	sat_lon = Degrees(sat_geodetic.lon);
	sat_alt = sat_geodetic.alt;

	fk = 12756.33 * acos(xkmper / (xkmper + sat_alt));
	fm = fk / 1.609344;

	rv = (long) floor(
			(tle.xno * xmnpda / twopi + age * tle.bstar * ae)
					* age+tle.xmo/twopi)+tle.revnum;

	sun_azi = Degrees(solar_set.x);
	sun_ele = Degrees(solar_set.y);

	irk = (long) Round(sat_range);
	isplat = (int) Round(sat_lat);
	isplong = (int) Round(360.0 - sat_lon);
	iaz = (int) Round(sat_azi);
	iel = (int) Round(sat_ele);
	ma256 = (int) Round(256.0 * (phase / twopi));

	if (sat_sun_status) {
		if (sun_ele <= -12.0 && sat_ele >= 0.0)
			findsun = '+';
		else
			findsun = '*';
	} else
		findsun = ' ';
}

char AosHappens() {
	/* This function returns a 1 if the satellite pointed to by
	 "x" can ever rise above the horizon of the ground station. */

	double lin, sma, apogee;

	if (sat.meanmo == 0.0)
		return 0;
	else {
		lin = sat.incl;

		if (lin >= 90.0)
			lin = 180.0 - lin;

		sma = 331.25 * exp(log(1440.0 / sat.meanmo) * (2.0 / 3.0));
		apogee = sma * (1.0 + sat.eccn) - xkmper;

		if ((acos(xkmper / (apogee + xkmper)) + (lin * deg2rad))
				> fabs(qth.stnlat * deg2rad))
			return 1;
		else
			return 0;
	}
}

char Decayed(double time) {
	/* This function returns a 1 if it appears that the
	 satellite pointed to by 'x' has decayed at the
	 time of 'time'.  If 'time' is 0.0, then the
	 current date/time is used. */

	double satepoch;

	if (time == 0.0){
		time = CurrentDaynum();
	}

	satepoch = DayNum(1, 0, sat.year) + sat.refepoch;

	if (satepoch + ((16.666666 - sat.meanmo) / (10.0 * fabs(sat.drag))) < time)
		return 1;
	else
		return 0;
}

double FindAOS() {
	/* This function finds and returns the time of AOS (aostime). */

	aostime = 0.0;

	if (AosHappens() && Decayed(daynum) == 0) {
		Calc();

		/* Get the satellite in range */

		while (sat_ele < -1.0) {
			daynum -= 0.00035 * (sat_ele * (((sat_alt / 8400.0) + 0.46)) - 2.0);

			/* Technically, this should be:

			 daynum-=0.0007*(sat_ele*(((sat_alt/8400.0)+0.46))-2.0);

			 but it sometimes skipped passes for
			 satellites in highly elliptical orbits. */

			Calc();
		}

		/* Find AOS */

		/** Users using Keplerian data to track the Sun MAY find
		 this section goes into an infinite loop when tracking
		 the Sun if their QTH is below 30 deg N! **/

		while (aostime == 0.0) {
			if (fabs(sat_ele) < 0.03)
				aostime = daynum;
			else {
				daynum -= sat_ele * sqrt(sat_alt) / 530000.0;
				Calc();
			}
		}
	}

	return aostime;
}

double FindLOS() {
	lostime = 0.0;

	if (AosHappens() == 1 && Decayed(daynum) == 0) {
		Calc();

		do {
			daynum += sat_ele * sqrt(sat_alt) / 502500.0;
			Calc();

			if (fabs(sat_ele) < 0.03)
				lostime = daynum;

		} while (lostime == 0.0);
	}

	return lostime;
}

double FindLOS2() {
	/* This function steps through the pass to find LOS.
	 FindLOS() is called to "fine tune" and return the result. */

	do {
		daynum += cos((sat_ele - 1.0) * deg2rad) * sqrt(sat_alt) / 25000.0;
		Calc();

	} while (sat_ele >= 0.0);

	return (FindLOS());
}

double NextAOS() {
	/* This function finds and returns the time of the next
	 AOS for a satellite that is currently in range. */

	aostime = 0.0;

	if (AosHappens() && Decayed(daynum) == 0)
		daynum = FindLOS2() + 0.014; /* Move to LOS + 20 minutes */

	return (FindAOS());
}

void getSatInfo(sat_info_t *info) {
	info->tsince = tsince;
	info->jul_epoch = jul_epoch;
	info->jul_utc = jul_utc;
	info->eclipse_depth = eclipse_depth;
	info->sat_azi = sat_azi;
	info->sat_ele = sat_ele;
	info->sat_range = sat_range;
	info->sat_range_rate = sat_range_rate;
	info->sat_lat = sat_lat;
	info->sat_lon = sat_lon;
	info->sat_alt = sat_alt;
	info->sat_vel = sat_vel;
	info->phase = phase;
	info->sun_azi = sun_azi;
	info->sun_ele = sun_ele;
	info->daynum = daynum;
	info->fm = fm;
	info->fk = fk;
	info->age = age;
	info->aostime = aostime;
	info->lostime = lostime;
	info->ax = ax;
	info->ay = ay;
	info->az = az;
	info->rx = rx;
	info->ry = ry;
	info->rz = rz;
	info->squint = squint;
	info->alat = alat;
	info->alon = alon;
}

int setTLE(char* line1, char* line2) {
	/* Read element set */
	if (KepCheck(line1, line2)) {
		/* We found a valid TLE! */
		/* Copy TLE data into the sat data structure */
		strncpy(sat.line1, line1, 69);
		strncpy(sat.line2, line2, 69);
		/* Update individual parameters */
		InternalUpdate();
		return 1;
	}
	return 0;
}

void setStation(double lat, double lon, int alt) {
	qth.stnlat = lat;
	qth.stnlong = lon;
	qth.stnalt = alt;
}

long int comp_dopp_frq(long int frq, int direction) {

	
	long dopp;
	if (direction == 1) {
		dopp = frq - (-frq *((sat_range_rate*1000.0)/299792458.0));
	} else {
		dopp = frq + (-frq *((sat_range_rate*1000.0)/299792458.0));
	}
	return dopp;

}

double get_azi(void) {
	return sat_azi;
}

double get_ele(void) {
	return sat_ele;
}

double get_satlat(void) {
	return sat_lat;
}

double get_satlon(void) {
	return sat_lon;
}

double get_satalt(void) {
	return sat_alt;
}

double get_satvel(void) {
	return sat_vel;
}

double get_sataos(void) {
	return aostime;
}

double get_satlos(void) {
	return lostime;
}
