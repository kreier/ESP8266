#include <Wire.h>
#include <DS3231.h>

DS3231 rtc;

bool century = false;
bool h12Flag;
bool pmFlag;

String datetime;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nRTC clock");
}

void loop() {
  update_datetime();
  Serial.print(datetime);
  Serial.print("  temperature: ");
  Serial.println(rtc.getTemperature());
  delay(10000);
}

void update_datetime() {
  datetime = "20";
  datetime += rtc.getYear();
  datetime += "-";
  if(rtc.getMonth(century) < 10) datetime += "0";
  datetime += rtc.getMonth(century);
  datetime += "-";
  if(rtc.getDate() < 10) datetime += "0";
  datetime += rtc.getDate();
  datetime += " ";
  if(rtc.getHour(h12Flag, pmFlag) < 10) datetime += "0";  
  datetime += rtc.getHour(h12Flag, pmFlag); //24-hr
  datetime += ":";
  if(rtc.getMinute() < 10) datetime += "0";
  datetime += rtc.getMinute();
  datetime += ":";
  if(rtc.getSecond() < 10) datetime += "0";
  datetime += rtc.getSecond(); 
}
