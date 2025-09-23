#ifndef __MINMAX_H__
#define __MINMAX_H__

#define min(x,y) ({			\
	__typeof__ (x) _x = (x);	\
	__typeof__ (y) _y = (y);	\
	_x < _y ? _x : _y; })

#define max(x,y) ({			\
	__typeof__ (x) _x = (x);	\
	__typeof__ (y) _y = (y);	\
	_x > _y ? _x : _y; })

#define min3(x,y,z)	min(min((x),(y)), (z))

#define max3(x,y,z)	max(max((x),(y)), (z))

#define clamp(x, _min, _max) ({		\
	min(max((x), (_min)), (_max)); })

#endif /* __MINMAX_H__ */
