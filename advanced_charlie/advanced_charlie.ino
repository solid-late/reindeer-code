#define NUM_LEDS 12
#define PATTERN_LENGTH 4
#define DELAY 1
#define ITERS 10

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
    0xffff,
    0b000010010000,
    0b000010000000,
    0x0000
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
  for (int pi = 0; pi < PATTERN_LENGTH; ++pi) {
    for (int i = 0; i < ITERS; ++i) {
      for (int ii = 0; ii < NUM_LEDS; ++ii) {
        setup();
        if ((1 << ii) & PATTERN[pi]) {
          ledOn(ii);
          delay(DELAY);
        } else {
          delay(DELAY);
        }
      }
    }
  }
}

