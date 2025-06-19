void povecajTemp(float i_control_signal) {

  // Serial.println(i_control_signal);
  if (i_control_signal == 0) {
    return;
  }
  
  unsigned long open_time = time_now - time_prev_6;

  i_control_signal = i_control_signal * dt; // delez dt-ja kjer je aktiven

  // Inside of dt and active working cycle
  if (open_time <= dt && open_time <= i_control_signal) {

    // On each step
    if (open_time - open_time_prev >= dt / simulation_step) {
      // increase temp for koeficent / sim_step
  
      // Temp increase
      temp_reading += heating_speed / simulation_step;

      // reset open_time_step 
      open_time_prev = open_time;
    }
  }

  // Outside of dt
  else if (open_time > dt){
    // reset to start of cycle
    time_prev_6 = time_now;
  }
}


void zmansajTemp() {

  // Lower temp each step of dt if temperature is above room temperature
  if (time_now - time_prev_3 >= dt / simulation_step && temp_reading > room_temp) {
  
    temp_reading -= cooling_speed / simulation_step;

    time_prev_3 = time_now;
  }  
  
}





