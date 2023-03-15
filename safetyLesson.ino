#include <Metro.h>

#include <Ultrasonic.h>

#include <Adafruit_NeoPixel.h>  // LED ring
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

Ultrasonic ultrasonic(7);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
bool buzzerState = false;
Metro buzzerMetro = Metro(500); //buzzer interval in millis

// ---------------- Vibrator ----------------
#define Vibration_PIN 2
bool vibratorState = false;
Metro vibratorMetro = Metro(600); //vibrator interval in millis

// ---------------- LED ring ----------------
#define LED_PIN     6
#define LED_COUNT  60 // How many NeoPixels are attached to the Arduino?
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Declare our NeoPixel strip object:

void setup()
{
  Serial.begin(9600);
  pinMode(Buzzer_PIN, OUTPUT);
  pinMode(Vibration_PIN, OUTPUT );

  // ---------------- LED ring ----------------
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
}

void loop()
{
  long ultrasonicDistance = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  Serial.println((String)ultrasonicDistance + " cm");
  

  if (ultrasonicDistance < 11) {
    // ---------------- Buzzer ----------------
    if(buzzerMetro.check()) {
      buzzerState = !buzzerState;
      digitalWrite(Buzzer_PIN, buzzerState);
    }

    // ---------------- Vibrator ----------------
    if(vibratorMetro.check()) {
      vibratorState = !vibratorState;
      digitalWrite(Vibration_PIN, vibratorState);
    }
  } else {
    digitalWrite(Buzzer_PIN, LOW);
    digitalWrite(Vibration_PIN, LOW);
  }

  // Serial.println((String)"Stay Away! It's too close: " + ultrasonicDistance + " cm");

  // ringFlesh(5);
}

void ringFlesh(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}