#define NUM_LEDS 12

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

void setup() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
}

void ledOn(int index) {
  pinMode(LED_MAP[index][0], OUTPUT);
  digitalWrite(LED_MAP[index][0], HIGH);
  pinMode(LED_MAP[index][1], OUTPUT);
  digitalWrite(LED_MAP[index][1], LOW);
}

void loop() {
  for(int i = 1; i < NUM_LEDS; ++i) {
    setup();
    ledOn(i);
    delay(40);
  }
  for(int i = NUM_LEDS - 2; i >= 0; --i) {
    setup();
    ledOn(i);
    delay(40);
  }
}
