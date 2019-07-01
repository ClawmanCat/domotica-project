
#include <Stepper.h>

const float STEPS_PER_REV = 32;

const float GEAR_RED = 64;

const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

int StepsRequired;

int Sensor = A0;
int sensv = 0;
int prevsensv = 0;

Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensv = analogRead(Sensor);
  Serial.println(sensv);
  if (prevsensv > 140 || prevsensv == 0)
  {
    if (sensv < 140)
    {
      Serial.println(sensv);
      prevsensv = sensv;
      StepsRequired  =  STEPS_PER_OUT_REV * 2;
      steppermotor.setSpeed(1000);
      steppermotor.step(StepsRequired);
      delay(500);
    }
  }
  if (prevsensv < 140 || prevsensv == 0)
  {
    if (sensv > 140)
    {
      prevsensv = sensv;
      StepsRequired  =  - STEPS_PER_OUT_REV * 2;
      steppermotor.setSpeed(1000);
      steppermotor.step(StepsRequired);
      delay(500);
    }
  }
}
