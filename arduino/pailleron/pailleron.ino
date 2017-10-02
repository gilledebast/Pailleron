/* ----------------------------------------------------------------------------------------------------
 * Pailleron, 2017
 * Update: 01/10/17
 * 
 * V1
 * Written by Bastien DIDIER
 * more info :
 *
 * ----------------------------------------------------------------------------------------------------
 */ 

//TB6612FNG Dual Motor Driver pololu
#define STBY         8  // Standby
#define AIN1         9  // Motor A In 1
#define AIN2        10  // Motor A In 2
#define PWMA        11  // Motor A PWM

#define limitSwitch  3
#define led         13

#define soundSensor A0

int soundValue;
const int threshold = 60;

void setup() {

  pinMode(STBY,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  
  pinMode(limitSwitch,INPUT);
  pinMode(led,OUTPUT);

  pinMode(soundSensor,INPUT);

  Serial.begin(9600);

  Serial.println("Initialise...");
  Serial.println("Back to ZERO");
  
  motor_standby(false);
  while (digitalRead(limitSwitch) == LOW) {
    motor_control(0,100);
  }
  motor_control(1,30);
  delay(1000);
  motor_standby(true);

  Serial.println("Ready!");
}

int count = 0;
int frame = 10;

void loop() {

  soundValue=analogRead(soundSensor);   //connect mic sensor to Analog 0
  Serial.println(soundValue,DEC);       //print the sound value to serial        
  
  if(soundValue > threshold){ 
    count++;
    Serial.println("Sound Detected!");
    frame = 10;
  }

    if(count == 2){
      motor_standby(false);
      motor_control(1,100);
      delay(500);
      motor_standby(true);

      count = 0;
    }
  
  if(frame > 0){frame--;}
  if(frame == 0){count = 0;}

  delay(50);

  //Serial.println("frame : "+String(frame)+" conut : "+String(count));
}

void motor_control(boolean direction, char speed){ //speed from 0 to 100

  if (direction == 0) {
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,LOW);
  } else {
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
  }
  
  if(speed < 0){
    speed = 0;
  }else if (speed > 100){
    speed = 100;
  }
  
  byte PWMvalue=0;
  PWMvalue = map(abs(speed),0,100,50,255); //anything below 50 is very weak
  analogWrite(PWMA,PWMvalue);
}

void motor_standby(boolean state) { //low power mode
 if (state == true){
   digitalWrite(STBY,LOW);
} else{
   digitalWrite(STBY,HIGH);
 }
}
