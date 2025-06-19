void TEMP_Controller(float work_cycle) {
  float open_time = timer1_tick_count * (1 / SSR_frequency) * 1000; // how long has it been open for in ms
  work_cycle = work_cycle * dt;

  if (open_time < dt && open_time <= work_cycle) {

    // Turn on Relay
    digitalWrite(SSR_SIGNAL_PIN, HIGH);

  }
  else if (open_time < dt && open_time > work_cycle) {

    // Turn off Relay
    digitalWrite(SSR_SIGNAL_PIN, LOW);
    
  }
  else{
    timer1_tick_count = 0;
  }
  
}


void setupTempSensor() {

  sensors.begin();  // Temp sensor
  sensors.setResolution(TEMP_RESOLUTION); // Change resolution to speed up the program, 12bit(original)

  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (12 - TEMP_RESOLUTION));
  lastTempRequest = millis();
  
  // if temp sensor is not detected abort
  if (!checkTempSensor()){
    abort_program(4);
  }
}


// Async temp request
void requestTemp() {

  if (millis() - lastTempRequest >= delayInMillis) {

    temp_reading = sensors.getTempCByIndex(0);

    // immediately after fetching the temperature we request a new sample
    // in the async modus

    sensors.requestTemperatures();

    lastTempRequest = millis();
  }
}



