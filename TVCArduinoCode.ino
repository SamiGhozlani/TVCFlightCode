//Sami TVC Mount Code
//Uses PID loop to stabalize pitch and roll axis


//Libraries
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SD.h>

//2 Servos
Servo servoX;
Servo servoY;

//For this project I used the Adafruit BNO055
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

//PID constants
double Kp = 0.6;
double Ki = 0.05;
double Kd = 0.12;

//Error
double error = 0;
double prevErrorY = 0;
double prevErrorZ = 0;
double xOffset = 0;
double yOffset = 0;
int DError = 0;
double iErrorY = 0;
double iErrorZ = 0;
double RADTODEG = 57.2957795;
//Calibrated upright values
int desiredOrientationY = 0;
int desiredOrientationZ = 0;
int firstRound = 0;
double outputCorrection;

//Setting the starting values for the servos
double servoXLocation = 90;
double servoYLocation = 90;
double currentTime = micros();
double previousTimeY = currentTime;
double previousTimeZ = currentTime;
double previousTimeGyro = currentTime;
double dt=0.0001;
double relativeOrientationY = 90;
double relativeOrientationZ = 90; 
bool calibrated = false;
bool launch = false;
bool rotated = false;
double launchTime = 0;
double firingTime = 1800;
int i = 0;
File myFile;
String fileName = "TVC5.txt";
bool fired = false;
int rotation = 0;

void setup() {
  // Setting up the serial
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }  
  // Setting up Servo, SD Card and BNO055 locations
  delay(50);
  pinMode(10, OUTPUT);
  
  if(!SD.begin(10)){
    Serial.print("SD Initialization failed");
  }
  Serial.print("SD Initialization Successfull \n");
  myFile = SD.open(fileName, FILE_WRITE);
  if(myFile){
    Serial.print("Writing to " + fileName);
    myFile.println("Time,orientationX,orientationY,servoX,servoY");
    myFile.close();
  }
  
  //pinMode(2, OUTPUT);
  servoX.attach(3);
  servoY.attach(5);
  // Initialise the BNO055 sensor
  if(!bno.begin())
  {
    // Not good: there was a problem detecting the BNO055
    Serial.print("No BNO055 detected ... ");
    while(1);
  }
  servoX.write(50);
  servoY.write(105);
  delay(8000);
  servoX.write(50);
  servoY.write(105);
  delay(1800);
  sensors_event_t event;
  bno.getEvent(&event);
  desiredOrientationY = event.orientation.y;
  desiredOrientationZ = event.orientation.z;

  //Logging calibration values
  myFile = SD.open(fileName, FILE_WRITE);
  myFile.print(millis());
  myFile.print(",");
  myFile.print(desiredOrientationY);
  myFile.print(",");
  myFile.print(desiredOrientationZ);
  myFile.print(",");
  myFile.print(60);
  myFile.print(",");
  myFile.println(100);
  myFile.close();
  
  previousTimeY = micros();
  previousTimeZ = micros();
}

void loop() {
  // Idling while the rocket waits to take off:
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.println(event.acceleration.y);

  // Detecting launch if there is a large enough upwards acceleration
  if (event.acceleration.y <= -6){
    if (launch == false){
      Serial.println("LActivated  ");
      Serial.println("  ");
      launchTime = millis();
      launch = true;
    }
  }
  
  if (launch == true && rotated == false){
    //Start rotating
    desiredOrientationZ += 1;
  }
  //Stop at 30 degrees
  if (desiredOrientationZ >= 30){
    rotated = true;
  }
  
  //Inputting orientation to PID loops
  relativeOrientationY = event.orientation.y;
  relativeOrientationZ = event.orientation.z;
  servoXLocation = 50+2*PIDCorrectionY(0, relativeOrientationY);
  servoYLocation = 110+2*PIDCorrectionZ(desiredOrientationZ, relativeOrientationZ);
  if(servoXLocation > 30 && servoXLocation < 70){
      //Updating Servo Position:
      servoX.write(servoXLocation);
  }
  if(servoYLocation > 90 && servoYLocation < 130){
      //Updating Servo Position:
      servoY.write(servoYLocation);
  }
  
  currentTime = micros();
  
  myFile = SD.open(fileName, FILE_WRITE);
  myFile.print(millis());
  myFile.print(",");
  myFile.print(relativeOrientationY);
  myFile.print(",");
  myFile.print(relativeOrientationZ);
  myFile.print(",");
  myFile.print(servoXLocation);
  myFile.print(",");
  myFile.println(servoYLocation);
  myFile.close();
  
  Serial.println(relativeOrientationY);
  Serial.println(relativeOrientationZ);
  Serial.print("PID Correction_y: ");
  Serial.print(servoXLocation);
  Serial.print("\t PID Correction_z: ");
  Serial.print(servoYLocation);
  Serial.println(dt);
  Serial.println(" ");
}


double PIDCorrectionY (double desiredOrientation, double currentOrientation) {
  currentTime = micros();
  //Converting dt into seconds using a microsecond timestamp
  dt = (currentTime - previousTimeY)/1000000;
  error = desiredOrientation-currentOrientation;
  iErrorY += (desiredOrientation-currentOrientation)*dt;
  if (firstRound > 1){
    DError = (error-prevErrorY)/dt;
    Serial.print("DError: ");
    Serial.println(DError);
    Serial.print("\t");
  }
  else{
    //Removing the Derivative error value from the first loop
    firstRound += 1;
    DError = 0;
  }

  outputCorrection = error*Kp+iErrorY*Ki+DError*Kd;
  //limiting correction if it exceeds Servo limitations
  if (outputCorrection > 10){
    outputCorrection=10;
  }
  if (outputCorrection < -10){
    outputCorrection=-10;
  }
  //Saving error and time for next iteration
  prevErrorY = error;
  previousTimeY = currentTime;
  return outputCorrection;
}

double PIDCorrectionZ (double desiredOrientation, double currentOrientation) {
  currentTime = micros();
  //Converting dt into seconds using a microsecond timestamp
  dt = (currentTime - previousTimeZ)/1000000;
  error = desiredOrientation-currentOrientation;
  iErrorZ += (desiredOrientation-currentOrientation)*dt;
  if (firstRound > 1){
    DError = (error-prevErrorZ)/dt;
  }
  else{
    //Removing the Derivative error value from the first loop
    firstRound += 1;
    DError = 0;
  }

  outputCorrection = error*Kp+iErrorZ*Ki+DError*Kd;
  
  //limiting correction if it exceeds Servo limitations
  
  if (outputCorrection > 10){
    outputCorrection = 10;
  }
  else if (outputCorrection < -10){
    outputCorrection = -10;
  }
  
  //Saving error and time for next iteration
  prevErrorZ = error;
  previousTimeZ = currentTime;
  return outputCorrection;
}