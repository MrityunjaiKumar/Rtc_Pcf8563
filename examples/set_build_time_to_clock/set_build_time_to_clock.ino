/* Demonstration of Rtc_Pcf8563 Set Time.
   Set the clock to a time then loop over reading time and
   output the time and date to the serial console.

   I used a RBBB with Arduino IDE, the pins are mapped a
   bit differently. Change for your hw
   SCK - A5, SDA - A4, INT - D3/INT1

   After loading and starting the sketch, use the serial monitor
   to see the clock output.

   setup:  see Pcf8563 data sheet.
           1x 10Kohm pullup on Pin3 INT
           No pullups on Pin5 or Pin6 (I2C internals used)
           1x 0.1pf on power
           1x 32khz chrystal

   Joe Robertson, jmr
   orbitalair@bellsouth.net
*/
#include <Wire.h>
#include <Rtc_Pcf8563.h>

// Parse the __DATE__ predefined macro to generate date defaults:
// __Date__ Format: MMM DD YYYY (First D may be a space if <10)
// <MONTH>
#define BUILD_MONTH_JAN ((__DATE__[0] == 'J') && (__DATE__[1] == 'a')) ? 1 : 0
#define BUILD_MONTH_FEB (__DATE__[0] == 'F') ? 2 : 0
#define BUILD_MONTH_MAR ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'r')) ? 3 : 0
#define BUILD_MONTH_APR ((__DATE__[0] == 'A') && (__DATE__[1] == 'p')) ? 4 : 0
#define BUILD_MONTH_MAY ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'y')) ? 5 : 0
#define BUILD_MONTH_JUN ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'n')) ? 6 : 0
#define BUILD_MONTH_JUL ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'l')) ? 7 : 0
#define BUILD_MONTH_AUG ((__DATE__[0] == 'A') && (__DATE__[1] == 'u')) ? 8 : 0
#define BUILD_MONTH_SEP (__DATE__[0] == 'S') ? 9 : 0
#define BUILD_MONTH_OCT (__DATE__[0] == 'O') ? 10 : 0
#define BUILD_MONTH_NOV (__DATE__[0] == 'N') ? 11 : 0
#define BUILD_MONTH_DEC (__DATE__[0] == 'D') ? 12 : 0
#define BUILD_MONTH BUILD_MONTH_JAN | BUILD_MONTH_FEB | BUILD_MONTH_MAR |     \
                        BUILD_MONTH_APR | BUILD_MONTH_MAY | BUILD_MONTH_JUN | \
                        BUILD_MONTH_JUL | BUILD_MONTH_AUG | BUILD_MONTH_SEP | \
                        BUILD_MONTH_OCT | BUILD_MONTH_NOV | BUILD_MONTH_DEC
// <DATE>
#define BUILD_DATE_0 ((__DATE__[4] == ' ') ? 0 : (__DATE__[4] - 0x30))
#define BUILD_DATE_1 (__DATE__[5] - 0x30)
#define BUILD_DATE ((BUILD_DATE_0 * 10) + BUILD_DATE_1)
// <YEAR>
#define BUILD_YEAR (((__DATE__[7] - 0x30) * 1000) + ((__DATE__[8] - 0x30) * 100) + \
                    ((__DATE__[9] - 0x30) * 10) + ((__DATE__[10] - 0x30) * 1))

#define BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define BUILD_MIN ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define BUILD_SEC ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')
//init the real time clock
Rtc_Pcf8563 rtc;
uint8_t DECtoBCD(uint8_t val)
{
    return ((val / 10) * 0x10) + (val % 10);
}
void setup()
{

    Serial.begin(115200);

    Serial.print("\r\n");
    Serial.print(BUILD_DATE);
    Serial.print(" - ");
    Serial.print(BUILD_MONTH);
    Serial.print(" - ");
    Serial.print(BUILD_YEAR);
    Serial.print("\r\n");
    Serial.print(BUILD_HOUR);
    Serial.print(":");
    Serial.print(BUILD_MIN);
    Serial.print(":");
    Serial.print(BUILD_SEC);
    Serial.println("\r\n");

    //clear out the registers
    rtc.initClock();
    int d = BUILD_DATE;
    int m = BUILD_MONTH;
    int y = BUILD_YEAR;
    int weekday = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7 + 1;

    //set a time to start with.
    //day, weekday, month, century(1=1900, 0=2000), year(0-99)
    rtc.setDate(BUILD_DATE, DECtoBCD(weekday), BUILD_MONTH, 0, BUILD_YEAR % 100);
    //hr, min, sec
    rtc.setTime(BUILD_HOUR, BUILD_MIN, BUILD_SEC);
}

void loop()
{
    //both format functions call the internal getTime() so that the
    //formatted strings are at the current time/date.
    Serial.print(rtc.formatTime());
    Serial.print("\r\n");
    Serial.print(rtc.formatDate());
    Serial.print("\r\n");
    delay(1000);
}