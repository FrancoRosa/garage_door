// Sensor inputs to know if the doors are fully open or closed
#define s1_cls  2
#define s1_opn  3
#define s2_cls  4
#define s2_opn  5

// Relay outputs
#define buzzer   6
#define doorlock 7

// Motor driver
#define en1     A0
#define en2     A1
#define dir1_a  9
#define dir1_b  10
#define dir2_a  11
#define dir2_b  12

void pinConfig() {
  pinMode(s1_cls, INPUT_PULLUP);
  pinMode(s1_opn, INPUT_PULLUP);
  pinMode(s2_cls, INPUT_PULLUP);
  pinMode(s2_opn, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(doorlock, OUTPUT);

  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(dir1_a, OUTPUT);
  pinMode(dir1_b, OUTPUT);
  pinMode(dir2_a, OUTPUT);
  pinMode(dir2_b, OUTPUT);
}

void setup() {
  pinConfig();
}

void loop() {
  
}
