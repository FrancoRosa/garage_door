/*
  Motor.cpp - Library for controling linear
  actuator connected to LM293L module.
*/

#include "Motor.h"

Motor::Motor(int ena, int enb,
             int in1, int in2, int in3, int in4) {
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  _ena = ena;
  _enb = enb;
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;
}

void Motor::on(const int channel, const int direction) {
  digitalWrite(channel ? _in1 : _in3, direction);
  digitalWrite(channel ? _in2 : _in4, !direction);
  digitalWrite(channel ? _ena : _enb, HIGH);
}

void Motor::off(const int channel) {
  digitalWrite(channel ? _ena : _enb, HIGH);
}

