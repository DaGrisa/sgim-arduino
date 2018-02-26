#include <Servo.h>

//## PARAMETERS ##//
bool DEBUG = false;
// servo dispenser
int servoPinFirstDispenser = 9;
Servo servo;
int servoAngle = 0;
// vibration dispenser
int motorPinSecondDispenser = 13;
int photoResistorPin  = 0; 
int photoResistorValue = 0;
int photoResistorActivateValue = 900;

//## GLOBAL VARIABLES ##//
int dispenserId;
int amount;
String commands[10];
int commandsIndex = -1;
bool runningFlag;

//## MAIN ##//
void setup() {
  Serial.begin(9600);  
  pinMode(motorPinSecondDispenser, OUTPUT); 
}
void loop() {
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    readCommands();
  } else if (commandsIndex > -1) {
    String command = commands[commandsIndex--];
    runCommand(command);
  }
}
void runCommand(String lineRead){
  runningFlag = true;
      
  int delimiterIndex = lineRead.indexOf(';');
  dispenserId = lineRead.substring(0, delimiterIndex).toInt();
  amount = lineRead.substring(delimiterIndex+1).toInt();
  
  dispense(dispenserId, amount);
  runningFlag = false;
}

//## HELPER ##//
void readCommands() {
  String lineRead = Serial.readString();

  int startIndex = 0;
  int delimiterIndex = 0;
 
  while(delimiterIndex >= 0){
    delimiterIndex = lineRead.indexOf('-', startIndex);
    String command = lineRead.substring(startIndex, delimiterIndex);
    if(command.length()>0){
      commandsIndex++;
      commands[commandsIndex] = command;
    }
    startIndex = delimiterIndex+1;
    if(DEBUG){
      Serial.print("command ");
      Serial.println(command);
    }
  }
}
void dispense(int dispenserId, int amount) {
  if(DEBUG){
    Serial.print("dispenser ");
    Serial.print(dispenserId);
    Serial.print(" with amount ");
    Serial.println(amount);
  }
  
  int servoPin = 0;
  int motorPin = 0;

  switch (dispenserId) {
  case 1:
    servoPin = servoPinFirstDispenser;
    break;
  case 2:
    motorPin = motorPinSecondDispenser;
    break;
  }

  if(servoPin > 0){
    for(int amountDispensed = 0; amountDispensed < amount; amountDispensed++){
      runServo(servoPin);
      delay(500); 
    }
  }

  if(motorPin > 0){
    for(int amountDispensed = 0; amountDispensed < amount; amountDispensed++){
      runMotor(motorPin);
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
void runMotor(int motorPin){
  digitalWrite(motorPin, HIGH);

  long stopCounter = 0;
  photoResistorValue = analogRead(photoResistorPin);
  while(photoResistorValue < photoResistorActivateValue && stopCounter < 50000){
    stopCounter++;
    photoResistorValue = analogRead(photoResistorPin);
    
    if(DEBUG){
      Serial.print("Motor running, resistor value: ");
      Serial.print(photoResistorValue);
      Serial.print(", stopCounter: ");
      Serial.println(stopCounter);
    }
  }
  stopMotor(motorPin);

  if(DEBUG){
    Serial.print("Stop, Photo Resistor: ");
    Serial.print(photoResistorValue);
    Serial.print(", stopCounter: ");
    Serial.println(stopCounter);
  }
}
void stopMotor(int motorPin){
  digitalWrite(motorPin, LOW);
}

