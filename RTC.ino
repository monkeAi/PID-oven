void initializeRTC() {

  Serial.println("Init RTC module.");

  // Initialize sensor
  while( !(DS1307.begin()) ){
    Serial.println(F("Communication with RTC device failed, please check connection"));
    delay(3000);
  }

  // Serial.println(F("RTC init complete."));

  // setRTC();   // Set the correct date -> comment this out after setting the new date and upload the program again

  DS1307.start();   // Start RTC timing function

  DS1307.setSqwPinMode(DS1307.eSquareWave_1Hz);     // Control the operation of the SQW/OUT pin
}

void setRTC() {

  // s/m/h/dow/d/m/y

  uint16_t setTimeBuff[7] = {0, 9, 2, 2, 18, 13, 2025};
  DS1307.setTime(setTimeBuff);
}

void getRTC() {

    /**
   *  Get the time from rtc module and convert it to uint16_t
   *  getTimeBuff Array for buffering the obtained time, uint16_t *
   *    getTimeBuff[0] for eSEC type, range: 00-59
   *    getTimeBuff[1] for eMIN type, range: 00-59
   *    getTimeBuff[2] for eHR type, range: 00-23
   *    getTimeBuff[3] for eDOW type, range: 01-07
   *    getTimeBuff[4] for eDATE type, range: 01-31
   *    getTimeBuff[5] for eMTH type, range: 01-12
   *    getTimeBuff[6] for eYR type, range: 2000-2099
   */

  // Get time from RTC module

  uint16_t getTimeBuff[3] = {0};
  DS1307.getTime(getTimeBuff);

  // Save values

  long RTC_sec = getTimeBuff[0];
  long RTC_min = getTimeBuff[1];
  long RTC_hour = getTimeBuff[2];

  // Create unix (total seconds)

  RTC_unix = (RTC_hour * 3600) + (RTC_min * 60) + RTC_sec;

}



unsigned long convertToUnix(float time) {
  // Convert float time to hours and minutes
  long h = floor(time);
  long m = map((time - h) * 100, 0, 100, 0, 60); // Map from 0.5h -> 30 min

  // Convert to unix time

  return (h * 3600) + (m * 60);
}



