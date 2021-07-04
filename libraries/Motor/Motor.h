/*
  Motor.h - Library for controling linear
  actuator connected to LM293L module.
*/

#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
  public:
    Motor(int ena, int enb, int in1, int in2, int in3, int in4);
    void on(bool channel, bool direction);
    void off(bool channel);
  private:
    int _ena;
    int _enb;
    int _in1;
    int _in2;
    int _in3;
    int _in4;
};

#endif

