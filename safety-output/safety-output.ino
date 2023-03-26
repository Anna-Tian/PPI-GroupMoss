#include <ChainableLED.h>

#include <SoftwareSerial.h>

#include <Metro.h> // make multiple output run at same time

#include <Adafruit_NeoPixel.h>  // LED ring
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

SoftwareSerial mySerial(2,3);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
bool buzzerState = false;
Metro buzzerMetro = Metro(600); // buzzer interval in millis

// ---------------- Vibrator ----------------
#define Vibration_PIN 2
bool vibratorState = false;
Metro vibratorMetro = Metro(300); // vibrator interval in millis

// ---------------- LED ring ----------------
#define LED_Ring_PIN 6
#define LED_Ring_Count  60 // How many NeoPixels are attached to the Arduino?
#define LED_Ring_Brightness 50 // Set LED_Ring_Brightness to about 1/5 (max = 255)
Adafruit_NeoPixel strip(LED_Ring_Count, LED_Ring_PIN, NEO_GRBW + NEO_KHZ800); // Declare our NeoPixel strip object:

// ---------------- LED socket kit ----------------
#define LED_Kit_PIN 4

// ---------------- LED chainable RGB ----------------
#define LED_RGB_Count 5
ChainableLED leds(7, 8, LED_RGB_Count);

// bluetooth
int info = 0;

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  // pinMode(Buzzer_PIN, OUTPUT);
  pinMode(Vibration_PIN, OUTPUT );
  digitalWrite(Vibration_PIN, 0);
  pinMode(LED_Kit_PIN, OUTPUT);
  leds.init();
  // Serial.println("Test begin");
  // // ---------------- LED ring ----------------
  // strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  // strip.show();            // Turn OFF all pixels ASAP
  // strip.setLED_Ring_Brightness(LED_Ring_Brightness);
}

void loop()
{
  digitalWrite(LED_Kit_PIN, HIGH); 
  leds.setColorRGB(0, 0, 255, 0);
  
  if (mySerial.available()) {
    // Serial.println("DATA RECEIVED:");
    
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