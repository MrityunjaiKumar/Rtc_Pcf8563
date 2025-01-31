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
    Wire.begin();
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