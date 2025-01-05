#include "Wire.h"         
#include "MPU6050.h" 
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};

SoftwareSerial gpsSerial(9,8);
SoftwareSerial b(5,4); //tx , rx
TinyGPSPlus gps;
float lattitude,longitude;
MyData data;
const int gasSensorPin = A0;// MQ-2 sensor connected to A0s
int gasLevel;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();
  mpu.initialize();
  pinMode(gasSensorPin, INPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  b.begin(9600);//initialize blutooth 
}

void loop() {
  // put your main code here, to run repeatedly:
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255 ); // X axis data
  data.Y = map(ay, -17000, 17000, 0, 255); 
  data.Z = map(az, -17000, 17000, 0, 255);  // Y axis data
  gasLevel = analogRead(gasSensorPin);

  if (gasLevel > 65) {       // Set threshold according to your needs
    Serial.println("Gas Leakage Detected!");
    b.println("Gas Leakage Detected!");
     digitalWrite(12, HIGH);
      digitalWrite(11, HIGH);
    gpsInit();
    while (gpsSerial.available())
  {
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      lattitude = (gps.location.lat());
      longitude = (gps.location.lng());
      
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }
   delay(3000);
    
  }else{
     digitalWrite(12, LOW);
     digitalWrite(11, LOW);
  } 
  if(data.X<15||data.Y>240||data.Z<10||data.Z>240){
    delay(3000);
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    data.X = map(ax, -17000, 17000, 0, 255 ); // X axis data
    data.Y = map(ay, -17000, 17000, 0, 255); 
    data.Z = map(az, -17000, 17000, 0, 255);  // Y axis data
    
    if(data.X<15||data.Y>240||data.Z<10||data.Z>240){
    Serial.println("Accident Detected!");
    b.println("Accident Detected!");
    digitalWrite(12, HIGH);
     digitalWrite(11, HIGH);
    gpsInit();
    
    while (gpsSerial.available())
  {
    int data = gpsSerial.read();
    if (gps.encode(data))
    {
      lattitude = (gps.location.lat());
      longitude = (gps.location.lng());
      
      Serial.print ("lattitude: ");
      Serial.println (lattitude);
      Serial.print ("longitude: ");
      Serial.println (longitude);
    }
  }
  delay(3000);
    }
  }else{
     digitalWrite(12, LOW);
     digitalWrite(11, LOW);
  } 
  
}
