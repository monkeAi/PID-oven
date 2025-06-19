void setupTimer1() {
  // Set up Timer1
  cli(); // Disable interrupts

  TCCR1A = 0; // Clear Timer/Counter Control Registers
  TCCR1B = 0;
  TCNT1 = 0;  // Initialize counter to 0

  // Set CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set prescaler to 256
  TCCR1B |= (1 << CS12);

  // Calculate and set OCR1A value for 1-second interrupt
  OCR1A = (62500 / SSR_frequency) - 1;  // (16 MHz / 256 prescaler / 1 Hz) - 1

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); // Enable interrupts
}