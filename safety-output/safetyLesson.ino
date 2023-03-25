#include <SoftwareSerial.h>

#include <Metro.h>

#include <Ultrasonic.h>

#include <Adafruit_NeoPixel.h>  // LED ring
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

SoftwareSerial mySerial(2,3);
Ultrasonic ultrasonic(7);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
bool buzzerState = false;
Metro buzzerMetro = Metro(600); // buzzer interval in millis

// ---------------- Vibrator ----------------
#define Vibration_PIN 2
bool vibratorState = false;
Metro vibratorMetro = Metro(300); // vibrator interval in millis

// ---------------- LED ring ----------------
#define LED_PIN     6
#define LED_COUNT  60 // How many NeoPixels are attached to the Arduino?
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Declare our NeoPixel strip object:

// bluetooth
int info = 0;

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  // pinMode(Buzzer_PIN, OUTPUT);
  pinMode(Vibration_PIN, OUTPUT );
  digitalWrite(Vibration_PIN, 0);
  // Serial.println("Test begin");
  // // ---------------- LED ring ----------------
  // strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  // strip.show();            // Turn OFF all pixels ASAP
  // strip.setBrightness(BRIGHTNESS);
}

void loop()
{
  long ultrasonicDistance = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  
  if (mySerial.available()) {
    // Serial.println("DATA RECEIVED:");
    if(ultrasonicDistance < 11 && ultrasonicDistance !=0) {
      digitalWrite(Vibration_PIN,1);
      Serial.println((String)"led on: " + ultrasonicDistance + " cm");
      mySerial.println((String)"bluetooth: led on: " + ultrasonicDistance + " cm");
    }else {
      digitalWrite(Vibration_PIN,0);
      // Serial.println("led off");
      // mySerial.println("bluetooth: led off");
    }
  }
  
  // if (ultrasonicDistance < 11) {
  //   // ---------------- Buzzer ----------------
  //   if(buzzerMetro.check()) {
  //     buzzerState = !buzzerState;
  //     digitalWrite(Buzzer_PIN, buzzerState);
  //   }

  //   // ---------------- Vibrator ----------------
  //   if(vibratorMetro.check()) {
  //     vibratorState = !vibratorState;
  //     digitalWrite(Vibration_PIN, vibratorState);
  //   }

  //   // ---------------- LED ring ----------------
  //   strip.fill(strip.Color(0, 0, 0, 255));
  //   strip.show();

    
  //   Serial.println((String)"Stay Away! It's too close: " + ultrasonicDistance + " cm");
  // } else {
  //   digitalWrite(Buzzer_PIN, LOW);
  //   digitalWrite(Vibration_PIN, LOW);
  //   strip.fill(strip.Color(0, 0, 0, 0));
  //   strip.show();
  //   // Serial.println((String)ultrasonicDistance + " cm");
  // }
}