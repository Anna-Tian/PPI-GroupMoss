#include <ChainableLED.h>
#include <SoftwareSerial.h>
#include <Metro.h> // make multiple output run at the same time
#include <Adafruit_NeoPixel.h>  // LED ring

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

SoftwareSerial mySerial(2,3);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
bool buzzerState = false;
Metro buzzerMetro = Metro(600); // buzzer interval in millis, increased to 200ms
unsigned long buzzerStartTime = 0;
unsigned long buzzerDuration = 300; // buzzer duration in milliseconds
unsigned long longBuzzerDuration = 600; // longer buzzer duration in milliseconds

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

float initialObjectTemp = 0;
float prevObjectTemp = 0; 

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(Buzzer_PIN, OUTPUT);
  digitalWrite(Buzzer_PIN, 0);

  pinMode(Vibration_PIN, OUTPUT );
  digitalWrite(Vibration_PIN, 0);

  leds.init();

  // pinMode(Button_PIN, INPUT);
  pinMode(Button_PIN, INPUT_PULLUP);
  Serial.println("Test begin");
}

void loop()
{
  if (mySerial.available()) {
    String receivedMessage = mySerial.readStringUntil('\n');
    int separatorIndex = receivedMessage.indexOf(',');

    // Extract the distance value from the received message
    String distanceStr = receivedMessage.substring(receivedMessage.indexOf(':') + 2, separatorIndex);
    int distance = distanceStr.toInt();

    // Extract the object temperature value from the received message
    String objectTempStr = receivedMessage.substring(receivedMessage.lastIndexOf(':') + 2, receivedMessage.length() - 1);
    float objectTemp = objectTempStr.toFloat();

    if (initialObjectTemp == 0) {
      initialObjectTemp = objectTemp; // set initial background environment temperature value
    }
    
    Serial.println(abs(initialObjectTemp - prevObjectTemp));

    if (abs(initialObjectTemp - prevObjectTemp) >= 2.5) {
      if (distance < 1000 && distance !=0 && !isPressed) {
        buzzerDuration = 300; // Set buzzer duration to normal

        // ---------------- Buzzer ----------------
        if(buzzerMetro.check()) {
          buzzerState = !buzzerState;
          if(buzzerState) {
            digitalWrite(Buzzer_PIN, HIGH); // turn on the buzzer
            buzzerStartTime = millis(); // record the start time of the buzzer
          } else {
            digitalWrite(Buzzer_PIN, LOW); // turn off the buzzer
          }
        }
        // ---------------- Vibrator ----------------
        if(vibratorMetro.check()) {
          vibratorState = !vibratorState;
          digitalWrite(Vibration_PIN, vibratorState);
        }
        // ---------------- LED ----------------
        leds.setColorRGB(0, 255, 0, 0); // red color

        // ---------------- press button to stop playing ----------------
        if (digitalRead(Button_PIN) == HIGH) {
          isPressed = true;
        }
      } else if (distance < 1000 && distance !=0 && isPressed) {
        buzzerDuration = longBuzzerDuration; // Set buzzer duration to shorter duration

        // ---------------- Buzzer ----------------
        if(buzzerMetro.check()) {
          buzzerState = !buzzerState;
          if(buzzerState) {
            digitalWrite(Buzzer_PIN, HIGH); // turn on the buzzer
            buzzerStartTime = millis(); // record the start time of the buzzer
          } else {
            digitalWrite(Buzzer_PIN, LOW); // turn off the buzzer
          }
        }
        leds.setColorRGB(0, 204, 102, 0); // yellow color
        digitalWrite(Vibration_PIN, false);
      } else {
        isPressed = false;
        leds.setColorRGB(0, 0, 255, 0); //green color
        stopPlaying();
      }
    } else {
      isPressed = false;
      leds.setColorRGB(0, 0, 255, 0); //green color
      stopPlaying();
    }
    prevObjectTemp = objectTemp;
  }

  // check if the buzzer has been on for longer than the desired duration
  if(buzzerState && (millis() - buzzerStartTime >= buzzerDuration)) {
    digitalWrite(Buzzer_PIN, LOW); // turn off the buzzer
    buzzerState = false;
  }
}

void stopPlaying() {
  digitalWrite(Buzzer_PIN, false);
  digitalWrite(Vibration_PIN, false);
}
