#include <IRremote.h>
#include <Servo.h>

#define SERVO_DPIN 4
#define IR_DPIN 7
#define ARDUINO_LED 13

Servo servo;
int starting_angle = 0;

uint32_t result;
const unsigned long blink_time = 1000UL;
const unsigned long duration = 3600000UL;
unsigned long blinker_start_time = 0;
int ledState = LOW;
bool blink = false;
bool timerStart = false;
unsigned long startTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ARDUINO_LED, OUTPUT);
  IrReceiver.begin(IR_DPIN, ENABLE_LED_FEEDBACK);


  servo.attach(SERVO_DPIN);
  servo.write(starting_angle);  

  Serial.print("IR enabled at pin: ");
  Serial.println(IR_DPIN, DEC);
}

void loop() {
  if (IrReceiver.decode()) {
    result = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Message received: 0x");
    Serial.println(result, HEX);
    IrReceiver.resume();

    switch(result) {
      case(0xBC43FF00):
        Serial.println("Servo turns right");
        servo.write(0);
        break;
      case(0xBB44FF00):
        Serial.println("Servo turns left");
        // ideal 23
        servo.write(39);
        delay(500);
        servo.write(0);
        break;
      case (0xF30CFF00):
        Serial.println("timer started");
        startTime = millis();
        timerStart = true;
        blink = true;
        break;
      default:
        Serial.println("Command not recognized");
    }

    }

    if (timerStart) {
      if (blink) {
        unsigned long current_milli = millis();
        if (current_milli - blinker_start_time >= blink_time) {
          blinker_start_time = current_milli;

          if (ledState == LOW) {
            ledState = HIGH;
          } else {
            ledState = LOW;
          }
        }
        digitalWrite(ARDUINO_LED, ledState);
      } 
      if (millis() - startTime >= duration) {
        servo.write(39);
        delay(500);
        servo.write(0);
        timerStart = false;
      }
  }
  delay(200);
}

// number 1 0xE916FF00

// number 2 0xE718FF00

// left BB44FF00

// right F30CFF00