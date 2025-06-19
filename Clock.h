#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

class Clock {
  private:
    unsigned long start_time;
    unsigned long ellapsed_time;
    unsigned long left_time;
    unsigned long set_time = 0;
    bool timeRunning = false;
    bool countDownEnabled = false;
    float floatSetTime = set_time;
    bool timePaused = false;

  public:
    Clock();                          // Constructor
    void start();
    void setCountDown(float time);
    void stop();
    void pause();
    void resume();
    void update();
    int getM();
    int getH();
    int getM_left();
    int getH_left();
    int get_time_left();
    void reset();
    void enableCountDown(bool state);
    bool is_countDown();
    float getSetTime();
    bool is_paused();
};

#endif




/*
class Clock {
  private:
    unsigned long start_time;
    unsigned long ellapsed_time;
    unsigned long left_time;
    unsigned long set_time = 0;
    bool timeRunning = false;
    bool countDownEnabled = false;

  public:
    Clock() {
      // init clock
    }

    void start() {
      start_time = millis() / 1000;
      ellapsed_time = 0;
      timeRunning = true;

      if (countDownEnabled) {
        left_time = set_time;
      }
    }

    void setCountDown(float time) {
      // convert float time in hours to seconds
      float h = floor(time);
      float m = map((time - h) * 100, 0, 100, 0, 60); // map from 0.5h -> 30 min

      set_time = (int(h) * 3600) + (int(m) * 60);

      Serial.println(set_time);

      left_time = set_time;

    }

    void stop() {
      timeRunning = false;
    }

    void update() {
      if (timeRunning) {
        ellapsed_time = (millis() / 1000) - start_time;

        if (countDownEnabled) {
          if (set_time <= 0){
            left_time = 0;
          }
          else{
            left_time = set_time - ellapsed_time;
          }
        }
      }
    }

    int getM() {
      return (ellapsed_time / 60) % 60;
    }
    
    int getH() {
      return ellapsed_time / 3600;
    }

    int getM_left() {
      return (left_time / 60) % 60;
    }
    
    int getH_left() {
      return left_time / 3600;
    }

    void reset() {
      start();
    }

    void enableCountDown(bool state) {
      countDownEnabled = state;
    }
    
    bool is_countDown() {
      return countDownEnabled;
    }

};

#endif

*/