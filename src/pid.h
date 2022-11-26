#include <Arduino.h>
#define dir1 12
#define pwm1 10
#define pwm2 11
#define dir2 13

int pwmA;
int pwmB;
int pwmKa;
int pwmKi;
int Upper = 255;
int Lower = 15;
int basePWM = 50;

float sp = 5.00;
float dt = 0.001;
float error, integralE, derivativeE, lastError;
float PID;

void start_dcMotor()
{
    pinMode(dir1, OUTPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(dir2, OUTPUT);
    pinMode(pwm2, OUTPUT);
}

void pid(float Kp, float Ki, float Kd, float depan, float kanan)
{
    if (kanan > 20)
    {
        kanan = 20;
    }
    else
    {
        kanan = kanan;
    }

    if (depan <= 4)
    {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pwm1, 155);
        analogWrite(pwm2, 100);
    }

    else
    {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, HIGH);

        error = sp - kanan;
        integralE = integralE + error;
        derivativeE = error - lastError;

        float P = Kp * error;
        float I = (Ki * integralE) * dt;
        float D = (Kd / dt) * derivativeE;

        lastError = error;

        PID = P + I + D;

        pwmA = basePWM + PID;
        if (pwmA > Upper)
        {
            pwmA = Upper;
        }
        if (pwmA < Lower)
        {
            pwmA = Lower;
        }
        pwmKa = pwmA;

        pwmB = basePWM - PID;
        if (pwmB > Upper)
        {
            pwmB = Upper;
        }
        if (pwmB < Lower)
        {
            pwmB = Lower;
        }
        pwmKi = pwmB;

        analogWrite(pwm1, pwmKa);
        analogWrite(pwm2, pwmKi);
    }
}
