float PID_Controller(float measured_temp) {
  
  // Calculate error
  float error = set_temp - measured_temp;

  // Proportional
  float P_value = Kp * error;

  Proportional_value = P_value;

  // Integral
  integral = integral + error * dt;
  integral = (integral - (-40000.0)) * (10.0 - (-10.0)) / (40000.0 - (-40000.0)) + (-10.0); // map 
  
  // Serial.print("Integral value:");
  // Serial.println(integral);

  // Removes Integral overshoot at start
  float movingKi = Ki;
  if (error >= integralStartRange) {
    movingKi = Ki / 4;
  }

  float I_value = movingKi * integral;

  Integral_value = I_value;

  // Derivative
  float derivative = (error - prev_error) / dt;

  float mappedDerivative = (derivative - (-1 / dt)) * (50.0 - (-50.0)) / (1 / dt - (-1 / dt)) + (-50.0); // map the small error diference to a usable value

  float D_value = Kd * mappedDerivative;

  Derivitive_value = D_value;

  // Sum of all values
  float PID_output = P_value + I_value + D_value;

  // Serial.print("Proportional:");
  // Serial.print(P_value);
  // Serial.print(",");
  // Serial.print("Integral:");
  // Serial.print(I_value);
  // Serial.print(",");
  // Serial.print("Integral value:");
  // Serial.println(integral);
  // Serial.print("Derivative:");
  // Serial.print(D_value);
  // Serial.print(", ");
  // Serial.print("Error:");
  // Serial.print(error);
  // Serial.print(", ");
  // Serial.print("prev_error:");
  // Serial.println(prev_error);

  //  Serial.println(PID_output);

  // Update previous error
  prev_error = error;

  // MAP Pid_output to normal values
  // map(PID_output, -200.0, 180.0, -100.0, 100.0); // ofsets the 0 so it still heats even if error is 0, helps with oscilation
  PID_output = constrain(PID_output, -200.0, 200.0);
  PID_output = (PID_output - (-200.0)) * (100.0 - (-100.0)) / ((200 - pidOffset) - (-200.0)) + (-100.0);


  // Clamp output to min/max bounds
  if (PID_output < min_output) { PID_output = min_output; }
  else if (PID_output > max_output) { PID_output = max_output; }

  // Calculate percantage
  float PID_output_percentage = (PID_output - min_output) / (max_output - min_output);

  return PID_output_percentage;
}