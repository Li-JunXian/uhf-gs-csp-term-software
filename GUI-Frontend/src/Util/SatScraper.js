import * as cheerio from 'cheerio'


export async function fetchSatData() {
    const url = 'https://db.satnogs.org/satellite/AGMX-2508-1337-4945-9939#data';
    const response = await fetch(url);

    const $ = cheerio.load(await response.text());
    const lastUpdated = $('dd.col-sm-10:eq(1)').text().trim();
    const TLE = ($('.tle-set').html()).replace("<br>", "\n");
    console.log(lastUpdated);
    console.log(TLE);  
}

fetchSatData();


