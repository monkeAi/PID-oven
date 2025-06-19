class StatusLED {
  private:
    int pin;
    bool state = 0;
    unsigned long time_prev_blink = millis();

  public:
    StatusLED(int pin_slot) {
      pin = pin_slot;
      pinMode(pin, OUTPUT);
    }

    void turnOff() {
      state = 0;
      digitalWrite(pin, 0);
    }

    void turnOn() {
      state = 1;
      digitalWrite(pin, 1);
    }

    void toggle() {
      if (state == 1) { turnOff(); }
      else { turnOn(); }
    }

    void blink(int interval) {
      if (time_now - time_prev_blink > interval) {

        toggle();

        time_prev_blink = time_now;
      }

    }
};

StatusLED sLED_green(STATUS_LED_G_PIN);
StatusLED sLED_red(STATUS_LED_R_PIN);



void initStatusLED() {
  sLED_green.turnOn();
  status_code = 0;  // All good 
}


void updateStatusLED() {

  switch (status_code) {
    case 0:
      // Ready
      sLED_red.turnOn();
      sLED_green.blink(1000);
      return;
    case 1:
      // Running
      sLED_red.turnOff();
      sLED_green.turnOn();
      return;
    case 2:
      // Paused
      sLED_red.turnOn();
      sLED_green.turnOff();
      return;
    case 3:
      // Auto Paused
      sLED_red.turnOn();
      sLED_green.turnOff();
      return;
    case 4:
      // Completed
      sLED_red.turnOn();
      sLED_green.blink(1000);
      return;
    case 5:
      // Error, Aborted
      sLED_red.turnOn();
      sLED_green.turnOff();
      return;
  }
}











