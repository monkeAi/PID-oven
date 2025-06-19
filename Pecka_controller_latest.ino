// Includes


  #include <Arduino.h>
  #include <Wire.h>
  #include <OneButton.h>
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include "Clock.h"
  #include <DFRobot_DS1307.h>

  #include <LiquidCrystal_I2C.h>
  #include <LcdMenu.h>
  #include <display/LiquidCrystal_I2CAdapter.h>
  #include <renderer/CharacterDisplayRenderer.h>
  #include <ItemCommand.h>
  #include <MenuScreen.h>
  #include <ItemIntRange.h>
  #include <ItemFloatRange.h>
  #include <ItemToggle.h>
  #include <ItemBack.h>
  #include <ItemSubMenu.h>


// Control Variables
           
  const int MIN_SET_TEMP = 0;               // [°C]
  const int MAX_SET_TEMP = 75;              // [°C]

  const float dt = 1000.0;                   // Time step [ms]

  const int lcd_refresh_rate = 2000;          // LCD Refresh rate [ms]
  const int clock_update_rate = 5000;         // Clock refresh rate [ms]

  const float MIN_SET_TIME = 0.0;
  const float MAX_SET_TIME = 69.0;


  float work_hours_enabled = false;         // Pecka working hours

  float WORK_START_DEFAULT = 7.00;        
  float WORK_END_DEFAULT = 23.00;

// SAFETY PARAMS

  const float MAX_TEMP_READING = 80.0;       // Turn off system if reached
  const float MIN_TEMP_READING = -10.0;       // Do not turn off if bellow this value

  const int safety_check_time = 5000;        // Safety checking time [ms]

// PID Tunning Settings

  float Kp = 50.0;
  float Ki = 5.0;
  float Kd = 0.5;

  float integralStartRange = 10;           // Degrees from goal temperature when Integral is beeing applied [°C]
  float pidOffset = 35;                     // Offset the final PID calculation to maintain power even at 0 error [%]


// SIMULATIONS

  bool SIM_ON = false;                         // Toggle to true if you want to run simulations

  const int room_temp = 20;

  const float temp_decline_delay = 1500;       // Simualtion decline delay [ms]
  const int temp_incline_delay = 15000;        // Simualtion incline delay [ms]

  const float heating_speed = 0.647;       // Simulated heating speed [°C/s]
  const float cooling_speed = 0.0284;       // Simulated cooling speed [°C/s]

  const int simulation_step = 100;        // Amount of steps in dt

// Other Variables

  bool manual_control = false;

  float set_temp = 0.0;          
  float temp_reading = 20.0;                 // Temp sensor reading
  float control_signal = 0.00;

  float prev_error = 0.00;
  float integral = 0.00;

  float new_set_temp = set_temp;

  volatile bool ssrState = LOW;  // Variable to hold the SSR state
  unsigned long timer1_tick_count = 0;

  bool is_running = false;
  bool is_completed = false;

  unsigned int status_code = 0;  // Program status, check docs for codes

  const int blink_interval = 1000;
  
  const float SSR_frequency = 500;              // Frequency of SSR PWM timer [Hz] , step size of dt in ms
                           
  // PID clamp values
  float min_output = 0.00;   
  float max_output = 100.00;

  // Global pid values

  float Proportional_value = 0;
  float Integral_value = 0;
  float Derivitive_value = 0;

  bool pid_screen_shown = 0;

  // Temp request
  int  TEMP_RESOLUTION = 12;
  unsigned long lastTempRequest = 0;
  int  delayInMillis = 0;               // dependent on set resolution

   // Delays
  int delay_period = dt; 

  unsigned long time_now = 0;
  unsigned long time_now_D1 = 0;
  unsigned long time_now_D2 = 0;

  unsigned long time_prev = millis();
  unsigned long time_prev_2 = millis();
  unsigned long time_prev_3 = millis();
  unsigned long time_prev_4 = millis();
  unsigned long time_prev_5 = millis();
  unsigned long time_prev_6 = millis();
  unsigned long time_prev_7 = millis();
  unsigned long time_prev_8 = millis();
  unsigned long time_prev_9 = millis();
  
  unsigned long open_time_prev = 0;

  bool clock_latch = 1;

  int delay_to_trigger = 0;

  
  int timeCount = 0;

  // RTC

  unsigned long RTC_unix;

  // Work hours

  float work_hours_start = WORK_START_DEFAULT;
  float work_hours_end = WORK_END_DEFAULT;

  unsigned long work_start_unix;
  unsigned long work_end_unix;

  // HUD 
  
  int heating_row = 1;
  int temp_row = 0;
  int pwr_row = 1;
  int timer_row = 2;
  int status_row = 3;

  // Timer

  // unsigned long time_ellapsed = 0; 

// flags

  bool upFlag = false;
  bool downFlag = false;
  bool enterFlag = false;
  bool backFlag = false;


// Simulation variables


  const int maxIndex = temp_incline_delay / int(dt);
  // const int maxIndex = 2;
  float tPow[maxIndex] = {0};
  int tPowIndex = 0;



// Defines


  #define ONE_WIRE_BUS      6      // Temperature sensor Pin
  #define SSR_SIGNAL_PIN    5      // SSR Pin
  #define STATUS_LED_G_PIN    7     // Status led pin Green
  #define STATUS_LED_R_PIN    8     // Status led pin Red

                                // Rotary encoder
  #define PIN_ROTARY_CLK    2   // Used for generating interrupts using CLK signal
  #define PIN_ROTARY_DAT    3   // Used for reading DT signal
  #define PIN_ROTARY_SW     4   // Used for the Rotary push button switch

  // If the LCD doesn't work with one address, try the other
  #define LCD_I2C_ADDRESS 0x27
  // #define LCD_I2C_ADDRESS     0x3F

  // Define the size of the LCD.  Most LCD's are either 16x2 or 20x4
  #define LCD_ROW_COUNT       4    // Number of Rows
  #define LCD_COL_COUNT       20   // Number of Characters per Row



// Library setups


  OneWire oneWire(ONE_WIRE_BUS);          // Setup a oneWire instance to communicate with any OneWire devices
  DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 

  // OneButton class handles Debounce and detects button press
  OneButton btnRot(PIN_ROTARY_SW, HIGH);		  // Rotary Select button

  LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);

  DFRobot_DS1307 DS1307; // real time clock module

  volatile int rotaryCount = 0; // Used for the Rotary Encoder interrupt routines PinA() and PinB()

  byte rotaryDisabled; // Disables the Rotary Encoder interrupts while the LCD is being updated

  volatile byte aFlag = 0; // lets us know when we're expecting a rising edge on pinA 
  volatile byte bFlag = 0; // lets us know when we're expecting a rising edge on pinB 
  volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt


// LCD Menu setup
  bool menu_hidden = true;

  void setTemp(int value);
  void startProcess();
  void stopProcess();
  void homeScreen();
  void setTimer(float time);
  void enableTimer(bool state);
  void resetTimer();
  void manual_Pause_Resume();
  void changeTemp(int value);
  void pidScreen();
  void enableWorkHours(bool state);
  void setWorkStart(float time);
  void setWorkEnd(float time);

  extern MenuScreen* aditionalSettingsScreen;

  MENU_SCREEN(settingsScreen, settingsItems,
      ITEM_COMMAND("..", homeScreen),
      ITEM_INT_RANGE("Set temp", MIN_SET_TEMP, MAX_SET_TEMP, set_temp, setTemp, "%01d C"),
      ITEM_TOGGLE("Timer", enableTimer),
      ITEM_FLOAT_RANGE("Set timer", MIN_SET_TIME, MAX_SET_TIME, 0, setTimer, "%.2f h", 1.0f),
      ITEM_SUBMENU("More settings", aditionalSettingsScreen),
      ITEM_COMMAND("START", startProcess)
  );

  MENU_SCREEN(adjustScreen, adjustItems,
      ITEM_COMMAND("..", homeScreen),
      ITEM_COMMAND("PAUSE / RESUME", manual_Pause_Resume),
      ITEM_COMMAND("STOP", stopProcess),
      ITEM_INT_RANGE("Change temp", MIN_SET_TEMP, MAX_SET_TEMP, set_temp, changeTemp, "%01d C"),
      ITEM_TOGGLE("Timer", enableTimer),
      ITEM_FLOAT_RANGE("Set timer", MIN_SET_TIME, MAX_SET_TIME, 0, setTimer, "%.2f h", 0.5f),
      ITEM_SUBMENU("More settings", aditionalSettingsScreen)
  );

  MENU_SCREEN(aditionalSettingsScreen, aditionalSettingsItems,
      ITEM_BACK(".."),
      ITEM_TOGGLE("Work hours", enableWorkHours),
      ITEM_FLOAT_RANGE("Work start", 0.0, 23.00, WORK_START_DEFAULT, setWorkStart, "%.2f h", 0.5f),
      ITEM_FLOAT_RANGE("Work end", 0.0, 23.00, WORK_END_DEFAULT, setWorkEnd, "%.2f h", 0.5f),
      ITEM_COMMAND("Reset timer", resetTimer), 
      ITEM_COMMAND("Show PID", pidScreen),
      ITEM_BASIC("RTC"),
      ITEM_BASIC("Credits:"),
      ITEM_BASIC("  - Viktor PG -"),
      ITEM_BASIC("  -  Matej M  -"),
      ITEM_BASIC("  -  Uros B   -"),
      ITEM_BASIC("  -  Aljaz H  -")

  
  );
  
  LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
  CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COL_COUNT, LCD_ROW_COUNT);
  LcdMenu menu(renderer);

// CLOCK SETUP

  Clock timer; // init timer


byte PauseChar[] = {
  B00000,
  B00000,
  B01010,
  B01010,
  B01010,
  B01010,
  B00000,
  B00000,
};



void setup () {
  runSetup();
}

void loop () {
  runLoop();
}





// Interupt for the SSR
// Utilizes timer 1 on arduino so SSR has uninterupted routine

ISR(TIMER1_COMPA_vect) {
  timer1_tick_count += 1; // increment current tick every interupt

  TEMP_Controller(control_signal); 
  
}












