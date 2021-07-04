#include <NilRTOS.h>
#include <SoftwareSerial.h>
#include <Motor.h>

// Sensor inputs to know if the doors are fully open or closed
#define s1_cls  2
#define s1_opn  3
#define s2_cls  4
#define s2_opn  5

// Relay outputs
#define buzzer   6
#define doorlock 7
#define led 13

#define motor1 0
#define motor2 1
#define open  0
#define close 1

Motor motor(A0,A1,A2,A3,A4,A5,A6);
SoftwareSerial SerialBT(11, 12); // RX, TX

volatile bool flag_open= false;
volatile bool flag_close = false;
volatile bool flag_stop = false;

char message[20];

void open_door() {
  flag_open=false;
  motor.on(motor1, open);
  motor.on(motor2, open);
  sprintf(message, "opening");
  nilThdSleepS(5);
}

void close_door() {
  flag_close=false;
  motor.on(motor1, close);
  motor.on(motor2, close);
  sprintf(message, "closing");
  nilThdSleepS(5);
}

void stop_door() {
  flag_stop=false;
  motor.off(motor1);
  motor.off(motor2);
  sprintf(message, "stop");
  nilThdSleepS(5);
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
  sprintf(message, "begin");
  SerialBT.println("... start bluetooth serial");
}

void processCommand() {
  char c = SerialBT.read();
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
    nilThdSleepMilliseconds(10); 
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

NIL_THREAD(processStatus, arg) {
  char verbose[100];
  while (true) {
    sprintf(
      verbose,
      "s1Op: %d, s2Op: %d, s1Cl: %d, s2Cl: %d, status: %s",
      digitalRead(s1_opn),
      digitalRead(s2_opn),
      digitalRead(s1_cls),
      digitalRead(s2_cls),
      message
    );
    Serial.println(verbose);
    nilThdSleepMilliseconds(1000);
  }
}

NIL_WORKING_AREA(waThread1, 64);
NIL_WORKING_AREA(waThread2, 64);
NIL_WORKING_AREA(waThread3, 64);

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY("thread1", commandReader, NULL, waThread1, sizeof(waThread1))
NIL_THREADS_TABLE_ENTRY("thread2", commandExecution, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_ENTRY("thread3", processStatus, NULL, waThread3, sizeof(waThread3))
NIL_THREADS_TABLE_END()


void setup() {
  pinConfig();
  serialConfig();
  nilSysBegin();
}

void loop() {
}

