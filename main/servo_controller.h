#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

#define SERVOMIN 150
#define SERVOMAX 600

class ServoController {
public:
    ServoController(int s1Channel, int s2Channel);
    void init();
    void setTargetAngles(int t);
    void moveServoSmooth(int channel);
    void moveAllServosSmooth();
    bool isMotionComplete();

private:
    Adafruit_PWMServoDriver pca;
    int servo1Channel;
    int servo2Channel;
    int currentAngle[16];
    int targetAngle[16];
    int speedPercent[16];
    int angleToPulse(int ang);
};

#endif
