#define NUM_LEDS 12
#define PATTERN_LENGTH 5
#define DELAY 1

#define A 2
#define B 3
#define C 4
#define D 5

const int LED_MAP[NUM_LEDS][2] = {
  /*  0 */  {D, A},
  /*  1 */  {B, D},
  /*  2 */  {A, B},
  /*  3 */  {D, C},
  /*  4 */  {B, C},
  /*  5 */  {C, B},
  /*  6 */  {A, C},
  /*  7 */  {C, D},
  /*  8 */  {C, A},
  /*  9 */  {A, D},
  /* 10 */  {D, B},
  /* 11 */  {B, A}
};

const int PATTERN[PATTERN_LENGTH] = {
    0b111111111111,
    0b000010010000,
    0b000010000000,
    0b000000000000,
    0b000010010000
};

int patternIndex = 0;

void setUpTimer() {
  cli();

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 2000;//15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();
}

ISR(TIMER1_COMPA_vect){
  patternIndex = (patternIndex + 1) % PATTERN_LENGTH;
}

void resetPins() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
}

void setup() {
  resetPins();
  setUpTimer();
}

void ledOn(int index) {
  pinMode(LED_MAP[index][0], OUTPUT);
  digitalWrite(LED_MAP[index][0], HIGH);
  pinMode(LED_MAP[index][1], OUTPUT);
  digitalWrite(LED_MAP[index][1], LOW);
}

void loop() {
  for (int ledi = 0; ledi < NUM_LEDS; ++ledi) {
    resetPins();
    if ((1 << ledi) & PATTERN[patternIndex]) {
      ledOn(ledi);
      delay(DELAY);
    }
  }
}

