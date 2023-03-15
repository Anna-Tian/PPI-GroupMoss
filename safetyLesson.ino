#include <Ultrasonic.h>


Ultrasonic ultrasonic(7);
#define BUZZER_PIN 6


void setup()
{
  Serial.begin(9600);
  // pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  long RangeInInches;
  long RangeInCentimeters;

  Serial.println("The distance to obstacles in front is: ");
  RangeInInches = ultrasonic.MeasureInInches();
  Serial.print(RangeInInches);//0~157 inches
  Serial.println(" inch");
  delay(250);

  RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  Serial.print(RangeInCentimeters);//0~400cm
  Serial.println(" cm");
  delay(250);
  // digitalWrite(BUZZER_PIN, HIGH);
  // delay(1000);
  // digitalWrite(BUZZER_PIN, LOW);
  // delay(1000);
}