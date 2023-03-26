#include <Ultrasonic.h>
Ultrasonic ultrasonic(5);

void setup() {
  Serial.begin(9600);

}

void loop() {
  long ultrasonicDistance = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  if(ultrasonicDistance < 30 && ultrasonicDistance !=0) {
    Serial.println((String)"Stay Away! It's too close: " + ultrasonicDistance + " cm");
  }else {
    Serial.println((String)"all good");
  }
}
