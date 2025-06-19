void initializeLcd() {
	renderer.begin();

  // Create custom characters
  lcd.createChar(7, PauseChar);
  
  lcd.setCursor(1,1);
	lcd.print(F("Pecka starting up."));

  delay(30);

  if (SIM_ON) {
    lcd.setCursor(2,2);
    lcd.print(F("Simulation mode.")); 
    Serial.println(F("Sim on."));
  }


  Serial.println(F("LCD init complete."));

}

void drawLogo() {
  lcd.clear();

  lcd.setCursor(7,1);
	lcd.print(F("D B F"));
  lcd.setCursor(1,2);
	lcd.print(F("Edvard Rusjan Team")); 

}

// HUD SCREEN

void drawHUD() {
  // Heating row
  // lcd.setCursor(0, heating_row);
  // lcd.print(F("Heat: "));

  lcd.clear();

  updateHUDheating(6, heating_row);

  // Temp row
  lcd.setCursor(0, temp_row);
  lcd.print(F("Temp: "));
  
  updateHUDtemp(6, temp_row);      // draw temperature

  lcd.setCursor(16, temp_row);
  lcd.print((char)223);
  lcd.print(F("C"));
  lcd.print(F("  "));

  // Power row

  lcd.setCursor(0, pwr_row);
  lcd.print(F("Powr: "));

  updateHUDpower(10, pwr_row);

  // Timer row

  lcd.setCursor(0, timer_row);
  lcd.print(F("Time: "));

  updateHUDtimer(6, timer_row);

  // Status row

  lcd.setCursor(0, status_row);
  lcd.print(F("Stat: "));

  updateHUDstatus(6, status_row);

}

void updateHUD() {
  rotaryDisabled = 1;

  updateHUDtemp(6, temp_row);
  updateHUDheating(6, heating_row);
  updateHUDpower(10, pwr_row);
  updateHUDtimer(6, timer_row);
  updateHUDstatus(6, status_row);

  rotaryDisabled = 0;
}


void updateHUDtemp(int col, int row) {
  lcd.setCursor(col, row);

  lcd.print(temp_reading, 1);
  lcd.print(F("/"));
  if (set_temp < 10) {
    lcd.print(F("0"));
  }
  lcd.print(set_temp, 1);

}

void updateHUDheating(int col, int row) {
  lcd.setCursor(col, row);

  if (is_running && !timer.is_paused()) {
    lcd.print(F("ON"));
  }
  else {
    lcd.print(F("OFF"));
  }
}

void updateHUDpower(int col, int row) {
  lcd.setCursor(col, row);

  lcd.print(control_signal * 100);
  lcd.print(F(" %"));
  lcd.print(F("  ")); // clear afterwards
}

void updateHUDtimer(int col, int row) {
  lcd.setCursor(col, row);

  int min, hour;
  int state;

  if (!timer.is_countDown()) {
    min = timer.getM();
    hour = timer.getH(); 
    state = 0;
  }
  else {
    min = timer.getM_left();
    hour = timer.getH_left();
    state = 1;
  }
  if (hour < 10) {
    lcd.print(F("0"));
  }
  lcd.print(hour);

  lcd.print(F(":"));


  if (min < 10) {
    lcd.print(F("0"));
  }
  lcd.print(min);

  lcd.print(F(" "));

  // Only animate when heating
  if (is_running && !timer.is_paused()) {
    
    // Animated sign for the clock
    if (clock_latch == 1) {
      lcd.write(byte(state));
      clock_latch = 0;
    }
    else {
      lcd.print(F(" "));
      clock_latch = 1;
    }
  }
  else {
    if (timer.is_paused()) {
      lcd.write(byte(7)); // pause char
    } 
    else {
      lcd.write(byte(state));
    }
  }

  

}

void updateHUDstatus(int col, int row) {
  lcd.setCursor(col, row);

  switch (status_code) {
    case 0:
      lcd.print(F("Ready"));
      lcd.print(F("         "));
      return;
    case 1:
      lcd.print(F("Running"));
      lcd.print(F("       "));
      return;
    case 2:
      lcd.print(F("Paused"));
      lcd.print(F("        "));
      return;
    case 3:
      lcd.print(F("AUTO Paused"));
      lcd.print(F("   "));
      return;
    case 4:
      lcd.print(F("COMPLETED"));
      lcd.print(F("     "));
      return;
    case 5:
      lcd.print(F("ABORTED"));
      lcd.print(F("       "));
      return;
  }


}



// PID SCREEN

void updatePIDvalues(int col, int row) {
  lcd.setCursor(col, row);

  lcd.print(Proportional_value, 1);
  lcd.print(F("/"));
  lcd.print(Integral_value, 1);
  lcd.print(F("/"));
  lcd.print(Derivitive_value, 1);
}

void drawPIDscreen() {
  // Temp row
  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  
  updateHUDtemp(6, 0);      // draw temperature

  lcd.setCursor(16, 0);
  lcd.print((char)223);
  lcd.print(F("C"));
  lcd.print(F("  "));

  // PID row

  lcd.setCursor(0, 1);
  lcd.print(F("PID: "));

  updatePIDvalues(5, 1);

  // Power row

  lcd.setCursor(0, 2);
  lcd.print(F("Powr: "));

  updateHUDpower(6, 2);
}


void updatePIDscreen() {
  rotaryDisabled = 1;

  updateHUDtemp(6, 0);
  updatePIDvalues(5, 1);
  updateHUDpower(6, 2);

  rotaryDisabled = 0;
}


