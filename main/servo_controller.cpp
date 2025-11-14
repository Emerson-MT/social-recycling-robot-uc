#include "servo_controller.h"

ServoController::ServoController(int s1c, int s2c)
  : servo1Channel(s1c), servo2Channel(s2c) {}

void ServoController::init() {
    pca.begin();
    pca.setPWMFreq(50); // 50Hz

    currentAngle[servo1Channel] = targetAngle[servo1Channel] = 90;
    currentAngle[servo2Channel] = targetAngle[servo2Channel] = 90;
    speedPercent[servo1Channel] = speedPercent[servo2Channel] = 100;

    pca.setPWM(servo1Channel, 0, angleToPulse(90));
    pca.setPWM(servo2Channel, 0, angleToPulse(90));

    delay(1000);
}

int ServoController::angleToPulse(int ang) {
    return map(ang, 0, 180, SERVOMIN, SERVOMAX);
}

void ServoController::setTargetAngles(int t) {
    switch(t) {
        case 0: targetAngle[servo1Channel]=45; targetAngle[servo2Channel]=45; break;
        case 1: targetAngle[servo1Channel]=135; targetAngle[servo2Channel]=45; break;
        case 2: targetAngle[servo1Channel]=45; targetAngle[servo2Channel]=135; break;
        case 3: targetAngle[servo1Channel]=135; targetAngle[servo2Channel]=135; break;
        case 4: targetAngle[servo1Channel]=90; targetAngle[servo2Channel]=90; break;
        default: Serial.println(F("⚠ Tacho inválido")); break;
    }
}

void ServoController::moveServoSmooth(int channel) {
    static unsigned long lastMoveTime[16] = {0};

    if(currentAngle[channel] == targetAngle[channel]) return;

    int delayTime = map(100 - speedPercent[channel], 0, 100, 0, 50);

    if(millis() - lastMoveTime[channel] >= (unsigned long)delayTime){
        lastMoveTime[channel] = millis();

        if(currentAngle[channel] < targetAngle[channel]) currentAngle[channel]++;
        else if(currentAngle[channel] > targetAngle[channel]) currentAngle[channel]--;

        currentAngle[channel] = constrain(currentAngle[channel],0,180);
        pca.setPWM(channel,0,angleToPulse(currentAngle[channel]));
    }
}

void ServoController::moveAllServosSmooth() {
    moveServoSmooth(servo1Channel);
    moveServoSmooth(servo2Channel);
}

bool ServoController::isMotionComplete() {
    return (currentAngle[servo1Channel]==targetAngle[servo1Channel] &&
            currentAngle[servo2Channel]==targetAngle[servo2Channel]);
}
