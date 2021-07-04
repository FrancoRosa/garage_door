#include <NilRTOS.h>
#include <SoftwareSerial.h>

// Sensor inputs to know if the doors are fully open or closed
#define s1_cls  2
#define s1_opn  3
#define s2_cls  4
#define s2_opn  5

// Relay outputs
#define buzzer   6
#define doorlock 7
#define led 13

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
  Serial.println("... opening door");
  flag_open=false;
}

void close_door() {
  Serial.println("... closing door");
  flag_close=false;
}

void stop_door() {
  Serial.println("... stopping door");
  flag_close=false;
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
  Serial.println("... start debug serial");
  SerialBT.println("... start bluetooth serial");
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

NIL_THREAD(commandReader, arg) {
  while (true) {
    if (SerialBT.available()) {
      processCommand();
    }
    nilThdSleepMilliseconds(100); 
  }
}

NIL_THREAD(commandExecution, arg) {
  while (true) {
    if (flag_open) open_door(); 
    if (flag_close) close_door(); 
    if (flag_stop) stop_door();
    nilThdSleepMilliseconds(100); 
  }
}

NIL_WORKING_AREA(waThread1, 64);
NIL_WORKING_AREA(waThread2, 64);

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY("thread1", commandReader, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY("thread2", commandExecution, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()


void setup() {
  pinConfig();
  serialConfig();
  nilSysBegin();
}

void loop() {
}

