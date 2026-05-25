#include <IRremote.h>
#include <Servo.h>

#define SERVO_DPIN 4
#define IR_DPIN 7

Servo servo;
int starting_angle = 0;

uint32_t result;



void setup() {
  Serial.begin(9600);
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
        servo.write(23);
        break;
      default:
        Serial.println("Command not recognized");
    }


  }
  delay(200);
}


// left BB44FF00

// right F30CFF00