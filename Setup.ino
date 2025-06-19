void runSetup() {
  
  Serial.begin(9600);
  
  Serial.println(F("DBF Edvard Rusjan Team - Pečka starting up ..."));

  // Set the Directions of the I/O Pins
  pinMode(SSR_SIGNAL_PIN, OUTPUT);

  initStatusLED();

  
  menu.setScreen(settingsScreen);
  menu.hide();

  Serial.println(F("Init LCD."));
	initializeLcd();
  delay(2000);
  
  Serial.println(F("Init Rotary encoder."));
  initializeRotaryEncoder();
  delay(1000);

  Serial.println(F("Init Temp sensor."));
  if (!SIM_ON) {
    setupTempSensor();
  }
  delay(500);

  initializeRTC();

  setupTimer1();    // Setup independant timer for SSR

  Serial.println(F("Setup Complete."));

  drawLogo();
  delay(2000);

  

  drawHUD();

  Serial.print(F("Free memory: "));
  Serial.println(freeMemory());

  // set starting work hours in unix
  work_start_unix = convertToUnix(work_hours_start);
  work_end_unix = convertToUnix(work_hours_end);

  // update lcd menus with default values
  updateMenus();

  status_code = 0;  // All good 
  
}