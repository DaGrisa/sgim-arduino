#include <Servo.h>

int servoPinFirstDispenser = 9;
Servo servo;
int servoAngle = 0;

String lineRead;
int dispenserId;
int amount;

int photoResistorPin = 0; 
int photoResistorValue = 0;

void setup() {
  Serial.begin(9600);  
}

void loop() {
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    lineRead = Serial.readString();
                
    int delimiterIndex = lineRead.indexOf(';');
    dispenserId = lineRead.substring(0, delimiterIndex).toInt();
    amount = lineRead.substring(delimiterIndex+1).toInt();

    dispense(dispenserId, amount);
  }

  photoResistorValue = analogRead(photoResistorPin);
  if(photoResistorValue >= 800){
    Serial.println("Photo Resistor Activation!");
  }
    
}

void dispense(int dispenserId, int amount) {
  Serial.print("dispenser ");
  Serial.print(dispenserId);
  Serial.print(" with amount ");
  Serial.println(amount);

  int servoPin;

  switch (dispenserId) {
  case 1:
    servoPin = servoPinFirstDispenser;
    break;
  default:
    servoPin = 0;
  }

  if(servoPin > 0){
    for(int amountDispensed = 0; amountDispensed < amount; amountDispensed++){
      runServo(servoPin);
      delay(500); 
    }
  }
}

void runServo(int servoPin){
  servo.attach(servoPin);
  
  for(servoAngle = 0; servoAngle < 180; servoAngle++)  //move the micro servo from 0 degrees to 180 degrees
  {                                  
    servo.write(servoAngle);              
    delay(10);                  
  }

  for(servoAngle = 180; servoAngle > 0; servoAngle--)  //now move back the micro servo from 0 degrees to 180 degrees
  {                                
    servo.write(servoAngle);          
    delay(10);      
  }
}

