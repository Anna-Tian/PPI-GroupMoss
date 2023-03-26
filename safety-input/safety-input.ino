#include <SoftwareSerial.h>

#include <Ultrasonic.h>

Ultrasonic ultrasonic(5);
SoftwareSerial mySerial(2,3);
char distance[10];
void setup() {
  Serial.begin(19200);
  mySerial.begin(9600);
  Serial.println("Test begin");
}

void loop() {
  int ultrasonicDistance = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  sprintf(distance,"%i\r",ultrasonicDistance);
  mySerial.write(distance);
  mySerial.write("\n");
  delay(500);
}
