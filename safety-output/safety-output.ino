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
#define Vibration_PIN 6
bool vibratorState = false;
Metro vibratorMetro = Metro(300); // vibrator interval in millis

// ---------------- LED chainable RGB ----------------
#define LED_RGB_Count 5
ChainableLED leds(7, 8, LED_RGB_Count);

// ---------------- Bluetooth ----------------
#define Button_PIN 5
bool isPressed = false;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(Buzzer_PIN, OUTPUT);
  digitalWrite(Buzzer_PIN, 0);

  pinMode(Vibration_PIN, OUTPUT );
  digitalWrite(Vibration_PIN, 0);

  leds.init();

  pinMode(Button_PIN, INPUT);
  Serial.println("Test begin");
}

void loop()
{
  if (mySerial.available()) {
    int distance = mySerial.readStringUntil('\n').toInt();
    if (distance < 300 && distance !=0 && !isPressed) {
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
      // ---------------- LED ----------------
      leds.setColorRGB(0, 255, 0, 0);

      // ---------------- press button to stop playing ----------------
      if (digitalRead(Button_PIN) == HIGH) {
        isPressed = true;
      }
    } else if (distance < 300 && distance !=0 && isPressed) {
      leds.setColorRGB(0, 0, 0, 255);
      stopPlaying();
    } else {
      isPressed = false;
      leds.setColorRGB(0, 0, 255, 0);
      stopPlaying();
    }
  }
}

void stopPlaying() {
  digitalWrite(Buzzer_PIN, false);
  digitalWrite(Vibration_PIN, false);
}