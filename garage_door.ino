// Software serial to stablish communication by bluetooth
#include <SoftwareSerial.h>

// Sensor inputs to know if the doors are fully open or closed
#define s1_cls  2
#define s1_opn  3
#define s2_cls  4
#define s2_opn  5

// Relay outputs
#define buzzer   6
#define doorlock 7

// Motor driver
#define en1     8
#define en2     9
#define dir1_a  A0
#define dir1_b  A1
#define dir2_a  A2
#define dir2_b  A3

SoftwareSerial SerialBT(11, 12); // RX, TX

bool flag_open= false;
bool flag_close = false;
bool flag_stop = false;

void open_door() {

}

void close_door() {
  
}

void stop_door() {
  
}

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

void serialConfig() {
  Serial.begin(115200);
  SerialBT.begin(9600);
}

void processCommand() {
  char c = SerialBT.read();
  Serial.print(c);
  flag_open = flag_close = flag_stop = false;
  switch (c) {
    case 'o':
      flag_open = true; break;
    case 'c':
      flag_close = true; break;
    case 's':
      flag_stop = true; break;
    default:
      break;
  }
}

void setup() {
  pinConfig();
  serialConfig();
}

void loop() {
  if (SerialBT.available()) {
    processCommand();
  }

  if (flag_open) open_door(); 
  if (flag_close) close_door(); 
  if (flag_stop) stop_door(); 
}
