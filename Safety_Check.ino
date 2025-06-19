void safetyCheck() {

  // if temp_reading > max_temp_reading
    // send interrupt and flag to stop heating
    // status = 300 heating out of control

  // Too hot
  if (temp_reading >= MAX_TEMP_READING) {
    abort_program(2);
  }

  // Too cold
  if (temp_reading <= MIN_TEMP_READING) {
    abort_program(3);
  }

  if (!SIM_ON) {

    // if temp sensor is not detected abort
    if (!checkTempSensor()){
      abort_program(4);
    }

  }

}


bool checkTempSensor() {

  DeviceAddress tempDeviceAddress;
  if (sensors.getAddress(tempDeviceAddress, 0)) {
      // Temp sensor found
      return 1;
  } else {
      // Temp sernsor not found
      return 0;
  }
}

int freeMemory() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}