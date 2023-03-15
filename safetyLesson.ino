#include <Ultrasonic.h>

#include <Adafruit_NeoPixel.h>  // LED ring
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

Ultrasonic ultrasonic(7);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
int buzzerLength = 4;
char buzzerNotes[] = "gec ";
int buzzerBeats[] = {1, 1, 2};
int buzzerTempo = 300;
// ---------------- Buzzer end ----------------

#define Vibration_PIN 2

// ---------------- LED ring ----------------
#define LED_PIN     6
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  60
// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// ---------------- LED ring end ----------------

void setup()
{
  Serial.begin(9600);
  pinMode(Buzzer_PIN, OUTPUT);
  pinMode(Vibration_PIN, OUTPUT );

  // ---------------- LED ring ----------------
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  // ---------------- LED ring end ----------------
}

void loop()
{
  //---------------- Ultrasonic distance sensor ----------------
  long ultrasonicDistance;
  ultrasonicDistance = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  
  if (ultrasonicDistance < 11) {
    // ---------------- Buzzer ----------------
    for(int i = 0; i < buzzerLength; i++) {
      if(buzzerNotes[i] != ' ') {
        playBuzzer(buzzerNotes[i], buzzerBeats[i] * buzzerTempo);
      }
        delay(buzzerTempo / 2);    /* delay between notes */
    }
    // ---------------- Buzzer end ----------------

    Serial.println((String)"Stay Away! It's too close: " + ultrasonicDistance + " cm");
  } else {
    Serial.print(ultrasonicDistance);//0~400cm
    Serial.println(" cm");
  }
  //---------------- Ultrasonic distance sensor end ----------------

  // ---------------- Vibrator ----------------
  // digitalWrite(Vibration_PIN, HIGH);
  // delay(1000);

  // digitalWrite(Vibration_PIN, LOW);
  // delay(1000);
  // ---------------- Vibrator end----------------

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