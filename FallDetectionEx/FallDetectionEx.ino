#include <Wire.h>
const int MPU_addr = 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

boolean fall = false;
boolean trigger0 = true;
boolean trigger1 = false;
boolean trigger2 = false;
boolean trigger3 = false;

byte trigger1Count = 0;
byte trigger2Count = 0;
byte trigger3Count = 0;
int angleChange = 0;

long avgAcX = 0;
long avgAcY = 0;
long avgAcZ = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

//  calibrate(3); // takes input # seconds
//  Serial.print("Average of AcX: " + (String) avgAcX + "\n");
//  Serial.print("Average of AcY: " + (String) avgAcY + "\n");
//  Serial.println("Average of AcZ: " + (String) avgAcZ + "\n");

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  
  Serial.begin(9600);
}

void loop(){
  mpuRead();
  // 2050, 77, 1947 are original values for calibrating the accelerometer manually.
  // Values may be different based on config. Divide by 16384.0
  ax = (AcX - avgAcX) / 16384.;
  ay = (AcY - avgAcY) / 16384.;
  az = (AcZ - avgAcZ) / 16384.;
//  Serial.println("AcX: " + (String) ax);
//  Serial.println("AcY: " + (String) ay);
//  Serial.println("AcZ: " + (String) az + "\n");

  // 270, - 351, 136 for gyroscope
  gx = (GyX) / 131.07;
  gy = (GyY) / 131.07;
  gz = (GyZ) / 131.07;
//  Serial.println("GyX: " + (String) gx);
//  Serial.println("GyY: " + (String) gy);
//  Serial.println("GyZ: " + (String) gz + "\n");
  

  // amplitude vector for the 3 axes
  float rawAM = pow(ax * ax + ay * ay + az * az, 0.5);
  float AM = rawAM * 10; // scaled AM
  
  Serial.println(AM);
//  float rawGyro = pow(gx * gx + gy * gy + gz * gz, 0.5);
//  Serial.println(rawGyro);
//  delay(500);

  // First step: free fall
  if (trigger0) {
    if (AM <= 3) { // if AM breaks lower threshold (0.4g)
      trigger0 = false;
      trigger1 = true;
      Serial.println("TRIGGER 1 ACTIVATED");
    }
  } else if (trigger1) {
    trigger1Count++;
    if (AM >= 12) { // if AM breaks upper threshold (3g)
      trigger1 = false;
      trigger1Count = 0;
      trigger2 = true;
      Serial.println("TRIGGER 2 ACTIVATED");
    } else if (trigger1Count >= 5) {
      trigger1 = false; 
      trigger1Count = 0;
      trigger0 = true;
      Serial.println("TRIGGER 1 DEACTIVATED");
    }
  } else if (trigger2) {
    trigger2Count++;
    angleChange = pow(gx * gx + gy * gy + gz * gz, 0.5); 
    if (angleChange >= 30 && angleChange <= 400) { // if orientation changes by between 80-100 degrees
      trigger2 = false; 
      trigger2Count = 0;
      trigger3 = true; 
      Serial.println("TRIGGER 3 ACTIVATED");
    } else if (trigger2Count > 5) { // allow 0.5s for orientation change
      trigger2 = false; 
      trigger2Count = 0;
      trigger0 = true;
      Serial.println("TRIGGER 2 DEACTIVATED");
      }
  } else if (trigger3) {
    trigger3Count++;
    if (trigger3Count >= 50) { // 5 seconds
      angleChange = pow(gx * gx + gy * gy + gz * gz, 0.5);
      delay(10);
      // if orientation changes remains between 0-20 degrees
      if (angleChange >= 0 && angleChange <= 20) {
        trigger3 = false; 
        trigger3Count = 0;
        fall = true; 
      } else { // user regained normal orientation
        trigger3 = false; 
        trigger3Count = 0;
        trigger0 = true;
        Serial.println("TRIGGER 3 DEACTIVATED");
      }
    }
  } else if (fall) {
    Serial.println("FALL DETECTED");
    for (int i = 0; i < 5; i++) {
      tone(10,1175);
      delay(600);
      tone(10,880);
      delay(600);
    }
    fall = false;
    trigger0 = true;
  }
  
  //It appears that delay is needed in order not to clog the port
  delay(100);
}

void mpuRead() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);    // request a total of 14 registers
  AcX = (Wire.read() << 8) | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY = (Wire.read() << 8) | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = (Wire.read() << 8) | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = (Wire.read() << 8) | Wire.read();  // 0x41 (TEMP_OUT_H)   & 0x42 (TEMP_OUT_L)
  GyX = (Wire.read() << 8) | Wire.read();  // 0x43 (GYRO_XOUT_H)  & 0x44 (GYRO_XOUT_L)
  GyY = (Wire.read() << 8) | Wire.read();  // 0x45 (GYRO_YOUT_H)  & 0x46 (GYRO_YOUT_L)
  GyZ = (Wire.read() << 8) | Wire.read();  // 0x47 (GYRO_ZOUT_H)  & 0x48 (GYRO_ZOUT_L)
}

// Not fully functional as of right now. 
// Better left uncalibrated.
void calibrate(int n) {
  for (int i = 0; i < (n * 333); i++) {
    mpuRead();
    avgAcX += AcX;
    avgAcY += AcY;
    avgAcZ += AcZ;
    delay(3);
  }
  avgAcX /= (n * 333);
  avgAcY /= (n * 333);
  avgAcZ /= (n * 333);
}
