void setTemp(int value) {
  // set temp
  new_set_temp = value;
  Serial.println(F("New temp set."));
}

void changeTemp(int value) {
  set_temp = value;
  new_set_temp = value;
  Serial.println(F("Set temp changed."));
} 

void startHeating() {
  set_temp = new_set_temp;
  manual_control = false;

  updateMenus();
  
  Serial.println(F("Heating started."));
  
  homeScreen();
  
}

void stopHeating() {
  set_temp = 0;
  new_set_temp = 0;
  control_signal = 0;

  updateMenus();

  Serial.println(F("Heating stoped."));

  homeScreen();
}

void homeScreen() {
  // Go back to home screen
  menu.hide();
  lcd.clear();
  menu_hidden = true;
  pid_screen_shown = false;
  drawHUD();
}


// TIMERS

void setTimer(float time) {
  timer.setCountDown(time);
}

void enableTimer(bool state) {
  timer.enableCountDown(state);
  Serial.println(F("Timer ON/OFF."));
  // enable timer
}

void resetTimer() {
  // restart timer count
  timer.reset();
  homeScreen();

}


void enableWorkHours(bool state) {
  // enable work hours
  work_hours_enabled = state;
}

void setWorkStart(float time) {
  // set start time 
  work_hours_start = time;
  work_start_unix = convertToUnix(time);
}

void setWorkEnd(float time) {
  // set stop time
  work_hours_end = time;
  work_end_unix = convertToUnix(time);
}

void manual_Pause_Resume() {
  if (!timer.is_paused()) {
    manual_control = true;
    pauseProcess();
  }
  else {
    manual_control = false;
    resumeProcess();
  }
}

void pidScreen() {
  menu.hide();
  pid_screen_shown = true;
  drawPIDscreen();
}



// Menu update

void updateMenus() {

  // This function keeps all of the settings items in diffeerent menus updated with the correct values

  // Set temp setting
  ItemIntRange* item_setTemp = static_cast<ItemIntRange*>(settingsItems[1]);  // lokacija set_temp itema v meniju
  ItemIntRange* item_changeTemp = static_cast<ItemIntRange*>(adjustItems[3]); // -//-

  if (item_setTemp) {
    item_setTemp->setCurrentValue(set_temp); 
  }
  if (item_changeTemp) {
    item_changeTemp->setCurrentValue(set_temp); 
  }

  // Enable timer setting
  ItemToggle* item_toggleTimer_settings = static_cast<ItemToggle*>(settingsItems[2]); // Toggle timer
  ItemToggle* item_toggleTimer_adjust = static_cast<ItemToggle*>(adjustItems[4]); // Toggle timer

  if (item_toggleTimer_settings) {
    item_toggleTimer_settings->setIsOn(timer.is_countDown()); 
  }
  if (item_toggleTimer_adjust) {
    item_toggleTimer_adjust->setIsOn(timer.is_countDown()); 
  }

  // Set timer duration setting
  ItemFloatRange* item_setTimer_settings = static_cast<ItemFloatRange*>(settingsItems[3]); // Toggle timer
  ItemFloatRange* item_setTimer_adjust = static_cast<ItemFloatRange*>(adjustItems[5]); // Toggle timer

  if (item_setTimer_settings) {
    item_setTimer_settings->setCurrentValue(timer.getSetTime()); 
  }
  if (item_setTimer_adjust) {
    item_setTimer_adjust->setCurrentValue(timer.getSetTime()); 
  }

  ItemToggle* item_workingHours_additional = static_cast<ItemToggle*>(aditionalSettingsItems[1]); // Toggle timer
  if (item_workingHours_additional) {
    item_workingHours_additional->setIsOn(work_hours_enabled); 
  }

  // Refresh at the end to update the menus
  menu.refresh();
}


void updateClockItem() {
  MenuItem* item_rtc = static_cast<MenuItem*>(aditionalSettingsItems[6]); // get rtc item from menu

  int rtc_h = ( RTC_unix / 3600 ) % 24;
  int rtc_m = ( RTC_unix / 60 ) % 60;
  int rtc_s = RTC_unix % 60;

  char rtc_text[10]; // Ensure enough size to hold the formatted string
  sprintf(rtc_text, "RTC: %02d:%02d:%02d", rtc_h, rtc_m, rtc_s);

  if (item_rtc) {
    item_rtc->setText(rtc_text);   // set the string for this rtc item
  }

  menu.refresh();
}
















