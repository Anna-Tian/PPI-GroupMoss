#include <ChainableLED.h>
#include <SoftwareSerial.h>
#include <Metro.h>              // make multiple output run at the same time
#include <Adafruit_NeoPixel.h>  // LED ring

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

SoftwareSerial mySerial(2, 3);

// ---------------- Buzzer ----------------
#define Buzzer_PIN 3
bool buzzerState = false;
Metro buzzerMetro = Metro(600);  // buzzer interval in millis, increased to 200ms
unsigned long buzzerStartTime = 0;
unsigned long buzzerDuration = 300;      // buzzer duration in milliseconds
unsigned long longBuzzerDuration = 600;  // longer buzzer duration in milliseconds

// ---------------- Vibrator ----------------
#define Vibration_PIN 6
bool vibratorState = false;
Metro vibratorMetro = Metro(300);  // vibrator interval in millis

// ---------------- LED chainable RGB ----------------
#define LED_RGB_Count 5
ChainableLED leds(7, 8, LED_RGB_Count);

// ---------------- Bluetooth ----------------
#define Button_PIN 5
bool isPressed = false;

float initialObjectTemp = 0;
float prevObjectTemp = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(Buzzer_PIN, OUTPUT);
  digitalWrite(Buzzer_PIN, 0);

  pinMode(Vibration_PIN, OUTPUT);
  digitalWrite(Vibration_PIN, 0);

  leds.init();

  pinMode(Button_PIN, INPUT_PULLUP);
  Serial.println("Test begin");
}

void loop() {
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
      initialObjectTemp = objectTemp;  // set initial background environment temperature value
    }

    Serial.println(abs(initialObjectTemp - prevObjectTemp));

    buzzerMetro.check(); // 检查蜂鸣器定时器状态
    vibratorMetro.check(); // 检查振动器定时器状态

    if (abs(initialObjectTemp - prevObjectTemp) >= 1.5) {
      Serial.println("isPressed: " + String(isPressed));
      Serial.println("distance: " + String(distance));
      if (distance < 500 && distance != 0 && !isPressed) {
        buzzerDuration = 600;  // Set buzzer frequency to fast
        Serial.println("close and not pressed");
        vibrator(true);
        buzzer(true);
        leds.setColorRGB(0, 255, 0, 0);  // red color
      } else if (distance < 1500 && distance != 0 && !isPressed) {
        buzzerDuration = 300;  // Set buzzer frequency to slow
        Serial.println("far and not pressed");
        vibrator(true);
        buzzer(true);
        leds.setColorRGB(0, 204, 102, 0);  // yellow color
      } else if (distance < 1000 && distance != 0 && isPressed) {
        Serial.println("far and pressed");
        if (distance < 500) {
          leds.setColorRGB(0, 255, 0, 0);  // red color
        } else {
          leds.setColorRGB(0, 204, 102, 0);  // yellow color
        }
        vibrator(false);
        buzzer(false);
      }
    } else {
      leds.setColorRGB(0, 0, 255, 0);  // green color
      stopPlaying();
      isPressed = false;
    }
    prevObjectTemp = objectTemp;

  }

  // check if the buzzer has been on for longer than the desired duration
  if (buzzerState && (millis() - buzzerStartTime >= buzzerDuration)) {
    Serial.println("time" + String(millis() - buzzerStartTime));
    stopBuzzer();
  }

  // stop buzzer and vibration if button is pressed
  if (digitalRead(Button_PIN) == HIGH) {
    isPressed = true;
  }

}

void buzzer(bool state) {
  buzzerState = state;
  digitalWrite(Buzzer_PIN, buzzerState);
  if (buzzerState) {
    buzzerStartTime = millis();  // record the start time of the buzzer
  }
}

void stopBuzzer() {
  digitalWrite(Buzzer_PIN, LOW);  // turn off the buzzer
  buzzerState = false;
}

void vibrator(bool state) {
  vibratorState = state;
  digitalWrite(Vibration_PIN, vibratorState);
}

void stopPlaying() {
  Serial.println("in the stopPlaying");
  stopBuzzer();
  vibrator(false);
}
