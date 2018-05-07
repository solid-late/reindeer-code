#define NUM_LEDS 16
#define PATTERN_LENGTH 6
#define DELAY 1
#define MAXPWM 255
#define THRESHOLD 300

#define SENSORPIN A0
#define FUCKEDUP_SERVOPIN 6
#define SERVOPIN 10
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6

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
  /* 11 */  {B, A},
  /* 12 */  {C, E},
  /* 13 */  {E, C},
  /* 14 */  {D, E},
  /* 15 */  {E, D},
};

const int PATTERN[PATTERN_LENGTH] = {
    0b1111111111111111,
    0b0000100100000100,
    0b0000100000000000,
    0b0000000000000000,
    0b0000100000000000,
    0b0000100100000100
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

#if SERVOPIN==FUCKEDUP_SERVOPIN
/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
#else
void setPwmFrequency(int pin, int divisor) { }
#endif

void resetPins() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
  pinMode(E, INPUT);
}

void setup() {
  resetPins();
  setUpTimer();
  pinMode(SERVOPIN, OUTPUT);
  setPwmFrequency(SERVOPIN, 256);
}

void ledOn(int index) {
  pinMode(LED_MAP[index][0], OUTPUT);
  digitalWrite(LED_MAP[index][0], HIGH);
  pinMode(LED_MAP[index][1], OUTPUT);
  digitalWrite(LED_MAP[index][1], LOW);
}

void turnHead(bool newEnabled) {
  const int pwmVal = newEnabled ?
    MAXPWM * 1/4 + MAXPWM * 1/6 :
    MAXPWM * 3/4 - MAXPWM * 1/6;
  analogWrite(SERVOPIN, pwmVal);
  delay(DELAY * 100);
}

void loop() {
  int iter = 0;
  bool enabled = true;
  if (iter++ % 200 == 0) {
    const bool newEnabled = analogRead(SENSORPIN) > THRESHOLD;
//    if (enabled != newEnabled) {
//      turnHead(newEnabled);
//    }
    enabled = newEnabled;
  }
  if (!enabled) {
    return;  
  }
  for (int ledi = 0; ledi < NUM_LEDS; ++ledi) {
    resetPins();
    if ((1 << ledi) & PATTERN[patternIndex]) {
      ledOn(ledi);
      delay(DELAY);
    }
  }
}

