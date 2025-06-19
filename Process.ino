void startProcess() {
  is_running = true;
  is_completed = false;
  status_code = 1;  // running

  timer.resume();
  timer.start();
  
  menu.setScreen(adjustScreen);

  startHeating();
}

void stopProcess() {
  is_running = false;
  is_completed = true;
  status_code = 4;  // Completed
  
  timer.stop();
  
  menu.setScreen(settingsScreen);

  stopHeating();
}

void resumeProcess() {
  status_code = 1;  // running

  timer.resume();

  startHeating();
}

void pauseProcess() {
  if (manual_control) status_code = 2;  // Manual Paused
  else status_code = 3;                 // AUTO Paused

  float tmp_temp = set_temp;

  timer.pause();
  
  stopHeating();

  set_temp = tmp_temp;
  new_set_temp = tmp_temp;

  updateMenus();
}


bool timeIsUp() {
  // check if time is up
  if (timer.get_time_left() <= 0 && timer.is_countDown()) {
    return true;
  }
  else return false;
}


bool inWorkHours() {
  // check if its in working hours
  if (!work_hours_enabled) {
    return true;
  }

  if (work_start_unix < work_end_unix) {
        // Case 1: Normal working hours (same day)
        return (work_start_unix <= RTC_unix && RTC_unix < work_end_unix);
    } else {
        // Case 2: Working hours span midnight
        return (RTC_unix >= work_start_unix || RTC_unix < work_end_unix);
    }
}














