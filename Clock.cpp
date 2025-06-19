#include "Clock.h"

Clock::Clock() {
    // Init
}

void Clock::start() {
  start_time = millis() / 1000;
  ellapsed_time = 0;
  timeRunning = true;

  if (countDownEnabled) {
    left_time = set_time;
  }
}

void Clock::setCountDown(float time) {
  floatSetTime = time;
  // Convert float time in hours to seconds
  long h = floor(time);
  long m = map((time - h) * 100, 0, 100, 0, 60); // Map from 0.5h -> 30 min
  set_time = (h * 3600) + (m * 60);

  left_time = set_time;
}

float Clock::getSetTime() {
  return floatSetTime;
}

void Clock::stop() {
  timeRunning = false;
}

void Clock::pause() {
  timeRunning = false;
  timePaused = true;
}

void Clock::resume() {
  timeRunning = true;
  timePaused = false;
}

void Clock::update() {
  if (timeRunning) {
    ellapsed_time = (millis() / 1000) - start_time;

    if (countDownEnabled) {
      if (set_time <= 0) {
        left_time = 0;
      } else {
        left_time = set_time - ellapsed_time;
      }
    }
  }
}

int Clock::getM() {
  return (ellapsed_time / 60) % 60;
}

int Clock::getH() {
  return ellapsed_time / 3600;
}

int Clock::getM_left() {
  return (left_time / 60) % 60;
}

int Clock::getH_left() {
  return left_time / 3600;
}

int Clock::get_time_left() {
  return left_time;
}

void Clock::reset() {
  start();
}

void Clock::enableCountDown(bool state) {
  countDownEnabled = state;
}

bool Clock::is_countDown() {
  return countDownEnabled;
}

bool Clock::is_paused() {
  return timePaused;
}






