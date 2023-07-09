#define ARRAYSIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))
#define GMT_TOKYO       (9*60*60)
#define SECONDS_IN_A_DAY    (24*60*60)
#define EPOCH_DAY       (1969*365L + 1969/4 - 1969/100 + 1969/400 + 306)    //  days from 0000/03/01 to 1970/01/01
#define UNIX_EPOCH_DAY  EPOCH_DAY
#define YEAR_ONE        365
#define YEAR_FOUR       (YEAR_ONE * 4 + 1)  //  it is YEAR_ONE*4+1 so the maximum reminder of day / YEAR_FOUR is YEAR_ONE * 4 and it occurs only on 2/29
#define YEAR_100        (YEAR_FOUR * 25 - 1)
#define YEAR_400        (YEAR_100*4 + 1)    //  it is YEAR_100*4+1 so the maximum reminder of day / YEAR_400 is YEAR_100 * 4 and it occurs only on 2/29
void ConvertUnixTimeToTokyoTime(uint64 unixtime, uint32 *pyear, uint8 *pmonth, uint8 *pday, uint8 *phour, uint8 *pminute, uint8 *psecond, uint8 *pweekday) {
    uint32 unixday;
    uint16 year = 0;
    uint8  leap = 0;
    uint32 n;
    uint8 month, day, weekday;
    uint8 hour, minute, second;
    static const uint16 monthday[] = { 0,31,61,92,122,153,184,214,245,275,306,337 };

    unixtime += GMT_TOKYO;
    second = unixtime % 60;
    minute = (unixtime / 60) % 60;
    hour = (unixtime / 3600) % 24;

    unixday = (uint32)(unixtime / SECONDS_IN_A_DAY);
    weekday = (uint8)((unixday + 3) % 7); //  because the unix epoch day is thursday
    unixday += UNIX_EPOCH_DAY;  //  days from 0000/03/01 to 1970/01/01

    year += 400 * (unixday / YEAR_400);
    unixday %= YEAR_400;

    n = unixday / YEAR_100;
    year += n * 100;
    unixday %= YEAR_100;

    if (n == 4){
        leap = 1;
    } else {
        year += 4 * (unixday / YEAR_FOUR);
        unixday %= YEAR_FOUR;
        
        n = unixday / YEAR_ONE;
        year += n;
        unixday %= YEAR_ONE;
        if (n == 4) {
            leap = 1;
        }
    }
    if (leap != 0) {
        month = 2;
        day = 29;
    }
    else {
        month = (unixday * 5 + 2) / 153;
        day = unixday - monthday[month] + 1;    //  
        month += 3;
        if (month > 12) {
            ++year;
            month -= 12;
        }
    }
    *psecond = second;
    *pminute = minute;
    *phour = hour;
    *pyear = year;
    *pmonth = month;
    *pday = day;
    *pweekday = weekday;
}