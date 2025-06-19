void abort_program(int err_code) {

  Serial.println("Program Terminated.");
  status_code = 5;

  switch (err_code) {
    case 2:
      printError(err_code, "Overheating.");
      delay(1000);
      abort();
    case 3:
      printError(err_code, "Too cold.");
      delay(1000);
      abort();
    case 4:
      printError(err_code, "Temperature sensor not detected.");
      delay(1000);
      abort();
    default:
      printError(1, "Program terminated.");
      delay(1000);
      abort();
  }

}



void printError(int err_code, char *err_message) {
  
  Serial.print(F("Error code: "));
  Serial.println(err_code);    // Error code
  Serial.println(err_message); // Error description

  lcd.clear();
  lcd.setCursor(0,0);
	lcd.print(F("Error: "));
  lcd.print(err_code);
  lcd.setCursor(0,1);
	lcd.print(err_message); 

}