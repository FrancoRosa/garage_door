#include <NilRTOS.h>
#include <SoftwareSerial.h>
#include <Motor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Sensor inputs to know if the doors are fully open or closed
#define s1_cls_pin 2
#define s1_opn_pin 3
#define s2_cls_pin 4
#define s2_opn_pin 5

// Relay outputs
#define buzzer   6
#define doorlock 7
#define led 13

// bluetooth serial
#define rx_bt 11
#define tx_bt 12

// motor outputs
#define ena A6
#define enb A7
#define in1 A0
#define in2 A1
#define in3 A2
#define in4 A3

const int motor1 = 0;
const int motor2 = 1;
const int open = 0;
const int close = 1;

Motor motor(ena, enb, in1, in2, in3, in4);
SoftwareSerial SerialBT(rx_bt, tx_bt);

volatile bool flag_open= false;
volatile bool flag_close = false;
volatile bool flag_stop = false;

volatile bool sensor1_open, sensor2_open, sensor1_close, sensor2_close;

char message[20];
char m1[15];
char m2[15];

void read_sensors() {
  sensor1_open = digitalRead(s1_opn_pin);
  sensor2_open = digitalRead(s2_opn_pin);
  sensor1_close = digitalRead(s1_cls_pin);
  sensor2_close = digitalRead(s2_cls_pin);
}

void open_door() {
  Serial.println("... opening");
  sprintf(message, "opening");
  while (flag_open) {
    read_sensors();
    if (!sensor1_close) {
      motor.on(motor1, open);
      sprintf(m1, "opening");
    }
    if (!sensor2_close && !sensor1_open) {
      motor.on(motor2, open);
      sprintf(m2, "opening");
    }
    if (sensor1_close && sensor2_close) {
      sprintf(message, "opened");
      break;
    }
    nilThdSleepMilliseconds(10);
  }
  motor.off(motor1);
  sprintf(m1, "off");
  motor.off(motor2);
  sprintf(m2, "off");
  flag_open=false;
}

void close_door() {
  Serial.println("... clossing");
  sprintf(message, "closing");
  while (flag_close) {
    read_sensors();
    if (!sensor1_open) {
      motor.on(motor1, close);
      sprintf(m1, "closing");
    }
    if (!sensor2_open && !sensor2_close) {
      motor.on(motor2, close);
      sprintf(m2, "closing");
    }
    if (sensor1_open && sensor2_open) {
      sprintf(message, "closed");
      break;
    }
    nilThdSleepMilliseconds(10);
  }
  motor.off(motor1);
  sprintf(m1, "off");
  motor.off(motor2);
  sprintf(m2, "off");
  flag_close=false;
}

void stop_door() {
  Serial.println("... stopping");
  motor.off(motor1);
  sprintf(m1, "off");
  motor.off(motor2);
  sprintf(m2, "off");
  sprintf(message, "stopped");
  flag_stop=false;
}

void pinConfig() {
  pinMode(s1_cls_pin, INPUT_PULLUP);
  pinMode(s1_opn_pin, INPUT_PULLUP);
  pinMode(s2_cls_pin, INPUT_PULLUP);
  pinMode(s2_opn_pin, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(doorlock, OUTPUT);
}

void serialConfig() {
  Serial.begin(115200);
  SerialBT.begin(9600);
  Serial.println("... start debug serial");
  SerialBT.println("... start bluetooth serial");
}

void processCommand() {
  char c = SerialBT.read();
  if ((c=='o') || (c=='c') || (c=='s')) {
    flag_open  = false;
    flag_close = false;
    flag_stop = false;

    switch (c) {
      case 'o':
        flag_open = true;
        break;
      case 'c':
        flag_close = true;
        break;
      case 's':
        flag_stop = true;
        break;
      default:
        break;
    }
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
    nilThdSleepMilliseconds(10); 
  }
}

NIL_THREAD(processStatus, arg) {
  char verbose[120];
  sprintf(message, "begin");
  sprintf(m1, "begin");
  sprintf(m2, "begin");
  
  while (true) {
    sprintf(
      verbose,
      "sensors: %d%d%d%d,   motors: %s - %s,    process: %s\r",
      sensor1_open,
      sensor1_close,
      sensor2_open,
      sensor2_close,
      m1, m2, message
    );
    Serial.print(verbose);
    read_sensors();
    nilThdSleepMilliseconds(500);
  }
}

NIL_WORKING_AREA(waThread1, 128);
NIL_WORKING_AREA(waThread2, 128);
NIL_WORKING_AREA(waThread3, 128);

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
