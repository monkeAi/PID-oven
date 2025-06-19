void runLoop() {
  
  time_now = millis();

  handleFlags(); // Handle all procces flags
  
  btnRot.tick();  // Rotary encoder update

  if (!SIM_ON) {
    requestTemp();  // Get temp reading
  }

  // Check safety
  if (time_now - time_prev_8 > safety_check_time) {
    safetyCheck();
    time_prev_8 = time_now;
  }

  // STATUS LED
  updateStatusLED();

  // PID LOOP
  if (time_now - time_prev > dt) {

    if (is_running && !timer.is_paused()) {

      // Get control signal
      control_signal = PID_Controller(temp_reading);

      // printStats();
    }

    // Simulated delayed temperature increase
    if (SIM_ON) {
      tPow[tPowIndex] = control_signal;
      tPowIndex++;
      if (tPowIndex > maxIndex - 1) {
        tPowIndex = 0;
      }
    }

    time_prev = time_now;
  }

  // SIMULATED Heating
  if (SIM_ON) {
      povecajTemp(tPow[tPowIndex]);
      
  }
  // SIMULATED Heat decipation through isolation
  if (SIM_ON) {
    zmansajTemp();
  }

  // LCD Update
  if (time_now - time_prev_2 > lcd_refresh_rate) {
    if(menu_hidden) {
      updateHUD();
    }
    else if(pid_screen_shown){
      updatePIDscreen();
    }
    else {
      updateClockItem();
    }
    time_prev_2 = time_now;
  } 

  // CLOCK Update
  if (time_now - time_prev_5 > clock_update_rate) {
    timer.update();

    getRTC();

    if (is_running && timeIsUp()) {
      // stop process if time is up
      stopProcess(); 
    }
    else if(is_running && !inWorkHours() && !timer.is_paused() && !manual_control) {
      // pause process if its not in work hours and it hasn't been paused yet
      pauseProcess();
    }
    else if(is_running && inWorkHours() && timer.is_paused() && !manual_control) {
      // resume process if its in work hours and it hasn't been resumed yet
      resumeProcess();
    }

    time_prev_5 = time_now;

  }
}


// void printStats() {

//   Serial.print(F("Temp reading:"));
//   Serial.print(temp_reading);
//   Serial.print(F(","));
//   Serial.print(F("Set temp:"));
//   Serial.print(set_temp);
//   Serial.print(F("control signal:"));
//   Serial.println(control_signal * 100);

// }

