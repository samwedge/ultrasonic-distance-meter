/*
Code written by Sam Wedge 2013
samwedge.co.uk
*/

#include <NewPing.h>
#include <SevSegThree.h>

//LED Scale Bar
#define Led1Pin 6
#define Led2Pin 9
#define Led3Pin 11
#define Led4Pin 10
#define Led5Pin 5

//Ultrasonic Sensor
#define trigPin 7
#define echoPin 8

//Alarm
#define alarmBuzzerPin 4
#define alarmIndicatorPin 12
#define alarmButtonPin 3
#define alarmButtonInterrupt 1

//3-Digit 7-Segment LED Display
#define digit1Pin 2
#define digit2Pin 1
#define digit3Pin 0
#define segmentAPin 14
#define segmentBPin 15
#define segmentCPin 16
#define segmentDPin 17
#define segmentEPin 18
#define segmentFPin 19
#define segmentGPin 13
SevSegThree sevseg;

//Ultrasonic
#define sonarMaxPingDistance 500
unsigned long lastPingTime = 0;
unsigned long prf = 5;
NewPing sonar(trigPin, echoPin, sonarMaxPingDistance);
unsigned long distance = sonarMaxPingDistance;

//Default Distances for scaling LEDs and alarm
unsigned long defaultMinDist = 10;
unsigned long defaultMaxDist = 100;
unsigned long minDist = defaultMinDist;
unsigned long maxDist = defaultMaxDist;
signed long pwmVal;

//Variables used by alarm buzzer
unsigned long lastBuzzOnTime = 0;
unsigned long lastBuzzOffTime = 0;
unsigned long buzzDelay; //determined based on distance
unsigned long maxBuzzDelay = 1000;
unsigned int buzzDuration = 20;
unsigned int buzzIntendedState = LOW;
unsigned int alarmIndicatorMode = 0;
unsigned int alarmIndicatorState = LOW;
unsigned int alarmIndicatorDelay = 100;
unsigned long alarmIndicatorTime = 0;

//Test Routine Parameters
#define testRoutineDelay 100

//Generic Variables
unsigned long currentTime = 0;
unsigned int ii,jj;
#define debounceDelay 100

//Defines what happens when the alarm button is pressed
void setAlarmDistance(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > debounceDelay){
    if(alarmIndicatorMode == 0){
      alarmIndicatorMode = 1;
      minDist = distance;
    }
    else if(alarmIndicatorMode == 1){
      alarmIndicatorMode = 2;
      if(distance > minDist){
        maxDist = distance;
      }
      else{
        alarmIndicatorMode = 0;
        minDist = defaultMinDist;
        maxDist = defaultMaxDist;
      }
    }
    else{
      alarmIndicatorMode = 0;
      minDist = defaultMinDist;
      maxDist = defaultMaxDist;
    }
  }
  lastInterruptTime = interruptTime;
}

void testRoutine(){
  analogWrite(Led1Pin,255);
  delay(testRoutineDelay);
  analogWrite(Led2Pin,255);
  delay(testRoutineDelay);
  analogWrite(Led3Pin,255);
  delay(testRoutineDelay);
  analogWrite(Led4Pin,255);
  delay(testRoutineDelay);
  analogWrite(Led5Pin,255);
  delay(testRoutineDelay);
  digitalWrite(alarmIndicatorPin,HIGH);
  delay(testRoutineDelay);

  digitalWrite(digit1Pin,HIGH);
  digitalWrite(digit2Pin,HIGH);
  digitalWrite(digit3Pin,HIGH);

  digitalWrite(segmentFPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentAPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentBPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentCPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentDPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentEPin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentGPin,LOW);
  delay(testRoutineDelay);

  digitalWrite(alarmBuzzerPin,HIGH);
  delay(testRoutineDelay);
  digitalWrite(alarmBuzzerPin,LOW);
  delay(testRoutineDelay);

  digitalWrite(digit3Pin,LOW);
  delay(testRoutineDelay);
  digitalWrite(digit2Pin,LOW);
  delay(testRoutineDelay);
  digitalWrite(digit1Pin,LOW);
  delay(testRoutineDelay);
  digitalWrite(segmentAPin,HIGH);
  digitalWrite(segmentBPin,HIGH);
  digitalWrite(segmentCPin,HIGH);
  digitalWrite(segmentDPin,HIGH);
  digitalWrite(segmentEPin,HIGH);
  digitalWrite(segmentFPin,HIGH);
  digitalWrite(segmentGPin,HIGH);

  digitalWrite(alarmIndicatorPin,LOW);

  for(ii=0;ii<1;ii++){
    for(jj=255;jj>0;jj--){
      analogWrite(Led5Pin,jj);
      analogWrite(Led4Pin,jj);
      analogWrite(Led3Pin,jj);
      analogWrite(Led2Pin,jj);
      analogWrite(Led1Pin,jj);
      delay(2);
    }
    for(jj=0;jj<255;jj++){
      analogWrite(Led5Pin,jj);
      analogWrite(Led4Pin,jj);
      analogWrite(Led3Pin,jj);
      analogWrite(Led2Pin,jj);
      analogWrite(Led1Pin,jj);
      delay(2);
    }
  }
  for(jj=255;jj>0;jj--){
    analogWrite(Led5Pin,jj);
    analogWrite(Led4Pin,jj);
    analogWrite(Led3Pin,jj);
    analogWrite(Led2Pin,jj);
    analogWrite(Led1Pin,jj);
    delay(2);
  }
}

void setup(){              
  pinMode(Led1Pin, OUTPUT);
  pinMode(Led2Pin, OUTPUT);
  pinMode(Led3Pin, OUTPUT);
  pinMode(Led4Pin, OUTPUT);
  pinMode(Led5Pin, OUTPUT);

  pinMode(alarmBuzzerPin, OUTPUT);
  pinMode(alarmIndicatorPin, OUTPUT);
  pinMode(alarmButtonPin, INPUT);

  //3-Digit 7-Segment LED Display
  pinMode(digit1Pin, OUTPUT);
  pinMode(digit2Pin, OUTPUT);
  pinMode(digit3Pin, OUTPUT);

  pinMode(segmentAPin, OUTPUT);
  pinMode(segmentBPin, OUTPUT);
  pinMode(segmentCPin, OUTPUT);
  pinMode(segmentDPin, OUTPUT);
  pinMode(segmentEPin, OUTPUT);
  pinMode(segmentFPin, OUTPUT);
  pinMode(segmentGPin, OUTPUT);

  digitalWrite(digit1Pin,LOW);
  digitalWrite(digit2Pin,LOW);
  digitalWrite(digit3Pin,LOW);

  digitalWrite(segmentAPin,HIGH);
  digitalWrite(segmentBPin,HIGH);
  digitalWrite(segmentCPin,HIGH);
  digitalWrite(segmentDPin,HIGH);
  digitalWrite(segmentEPin,HIGH);
  digitalWrite(segmentFPin,HIGH);
  digitalWrite(segmentGPin,HIGH);

  attachInterrupt(alarmButtonInterrupt, setAlarmDistance, RISING);

  sevseg.Begin(1,digit1Pin,digit2Pin,digit3Pin,segmentAPin,segmentBPin,segmentCPin,segmentDPin,segmentEPin,segmentFPin,segmentGPin);

  testRoutine();
}

void setScaleLedsOff(){
  digitalWrite(Led1Pin,LOW);
  digitalWrite(Led2Pin,LOW);
  digitalWrite(Led3Pin,LOW);
  digitalWrite(Led4Pin,LOW);
  digitalWrite(Led5Pin,LOW);
}

void setScaleLeds(unsigned int distance, unsigned int minDist, unsigned int maxDist){

  pwmVal = (255*4)-(((distance-minDist)*(long)255*(long)4)/(maxDist-minDist));

  if(distance <= minDist){
    digitalWrite(Led1Pin,HIGH);
    digitalWrite(Led2Pin,HIGH);
    digitalWrite(Led3Pin,HIGH);
    digitalWrite(Led4Pin,HIGH);
    digitalWrite(Led5Pin,HIGH);
  }
  else{
    digitalWrite(Led5Pin,LOW);

    if(pwmVal > (255*3)){
      analogWrite(Led4Pin,pwmVal-255*3);
      digitalWrite(Led3Pin,HIGH);
      digitalWrite(Led2Pin,HIGH);
      digitalWrite(Led1Pin,HIGH);
    }
    else if(pwmVal > (255*2)){
      digitalWrite(Led4Pin,LOW);
      analogWrite(Led3Pin,pwmVal-255*2);
      digitalWrite(Led2Pin,HIGH);
      digitalWrite(Led1Pin,HIGH);
    }
    else if(pwmVal > 255){
      digitalWrite(Led4Pin,LOW);
      digitalWrite(Led3Pin,LOW);
      analogWrite(Led2Pin,pwmVal-255);
      digitalWrite(Led1Pin,HIGH);
    }
    else if(pwmVal > 0){
      digitalWrite(Led4Pin,LOW);
      digitalWrite(Led3Pin,LOW);
      digitalWrite(Led2Pin,LOW);
      analogWrite(Led1Pin,pwmVal);
    }
    else{
      digitalWrite(Led4Pin,LOW);
      digitalWrite(Led3Pin,LOW);
      digitalWrite(Led2Pin,LOW);
      digitalWrite(Led1Pin,LOW);
    }
  }

}

void loop(){

  currentTime = millis();

  //Fire the transducer, if the PRF criteria has been met
  if((currentTime - lastPingTime) > (1000/prf)){
      distance = sonar.ping()/US_ROUNDTRIP_CM;
      //Prevent an unintended alarm shortly after everything has fired up
      if(currentTime < 5000){
        distance = 0;
      }

      sevseg.NewNum(distance);//Update the LED display

      lastPingTime = currentTime;
  }

  if(alarmIndicatorMode == 1){
    setScaleLedsOff();
    digitalWrite(alarmBuzzerPin,LOW);
  }
  else{

    //Light up the scale-bar LEDs based upon the distance
    if(distance != 0){
      setScaleLeds(distance,minDist,maxDist);
    }
    else{
      setScaleLedsOff();
    }

    //Turn the buzzer on or off
    //--Should the last-initiated buzz be switched off?
    if(digitalRead(alarmBuzzerPin)==HIGH){
      if((currentTime - lastBuzzOnTime) >= buzzDuration){
        buzzIntendedState = LOW;
        lastBuzzOffTime = currentTime;
      }
    }
    //--Has the buzzer delay been exceeded?
    if((distance < maxDist) & (distance > minDist)){
      //1000 on next line to prevent flooring of a float<1 to 0
      buzzDelay = (((distance-minDist))*1000/(maxDist-minDist))*maxBuzzDelay/1000;
      if((currentTime - lastBuzzOnTime) >= buzzDelay){
        buzzIntendedState = HIGH;
        lastBuzzOnTime = currentTime;
      }
    }
    else if(distance >= maxDist){
      buzzIntendedState = LOW;
    }
    else if((distance <= minDist) & (distance > 0)){
      buzzIntendedState = HIGH;
    }
    else{
      buzzIntendedState = LOW;
    }
    //--Change buzzer state, if required
    if(digitalRead(alarmBuzzerPin) != buzzIntendedState){
      digitalWrite(alarmBuzzerPin,buzzIntendedState);
    }

  }

  //Set the alarm indicator LED on or off
  if(alarmIndicatorMode == 0){
    alarmIndicatorState = LOW;
  }
  else if(alarmIndicatorMode == 1){
    if((currentTime - alarmIndicatorTime) >= alarmIndicatorDelay){
      if(alarmIndicatorState == LOW){
        alarmIndicatorState = HIGH;
      }
      else if(alarmIndicatorState == HIGH){
        alarmIndicatorState = LOW;
      }
      alarmIndicatorTime = currentTime;
    }
  }
  else if(alarmIndicatorMode == 2){
    alarmIndicatorState = HIGH;
  }
  if(digitalRead(alarmIndicatorPin) != alarmIndicatorState){
    digitalWrite(alarmIndicatorPin,alarmIndicatorState);
  }

  //Display the number on the sev seg display
  if(distance != 0){
    sevseg.PrintOutput();
  }
  else{
    digitalWrite(digit1Pin,HIGH);
    digitalWrite(digit2Pin,HIGH);
    digitalWrite(digit3Pin,HIGH);
    digitalWrite(segmentGPin,LOW);
    digitalWrite(digit1Pin,LOW);
    digitalWrite(digit2Pin,LOW);
    digitalWrite(digit3Pin,LOW);
    digitalWrite(segmentGPin,HIGH);
  }

}
