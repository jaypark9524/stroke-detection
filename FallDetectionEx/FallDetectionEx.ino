#include <Wire.h>
const int MPU_addr = 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float   ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

//int data[STORE_SIZE][5]; //array for saving past data
//byte currentIndex=0; //stores current data array index (0-255)
boolean fall     = false; //stores if a fall has occurred
boolean trigger1 = false; //stores if first trigger (lower threshold) has occurred
boolean trigger2 = false; //stores if second trigger (upper threshold) has occurred
boolean trigger3 = false; //stores if third trigger (orientation change) has occurred

byte trigger1count = 0; //stores the counts past since trigger 1 was set true
byte trigger2count = 0; //stores the counts past since trigger 2 was set true
byte trigger3count = 0; //stores the counts past since trigger 3 was set true
int angleChange    = 0;

long acc_x_cal = 0;
long acc_y_cal = 0;
long acc_z_cal = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

// calibrate(2); // takes input # seconds
// Serial.print("Average of AcX: " + (String) acc_x_cal + "\n");
// Serial.print("Average of AcY: " + (String) acc_y_cal + "\n");
// Serial.println("Average of AcZ: " + (String) acc_z_cal + "\n");
}

void loop(){
  mpu_read();
  // 2050, 77, 1947 are values for calibration of accelerometer
  // values may be different for you. 16384.00 divide.
  ax = (AcX - acc_x_cal) / 16384.00;
  ay = (AcY - acc_y_cal) / 16384.00;
  az = (AcZ - acc_z_cal) / 16384.00;
//  Serial.println("X: " + (String) ax);
//  Serial.println("Y: " + (String) ay);
//  Serial.println("Z: " + (String) az + "\n");

  //270, 351, 136 for gyroscope
  gx = (GyX + 270) / 131.07;
  gy = (GyY - 351) / 131.07;
  gz = (GyZ + 136) / 131.07;

  // calculating Amplitute vactor for 3 axis
  float Raw_AM = pow(ax * ax + ay * ay + az * az, 0.5);
  float AM = Raw_AM * 10; // as values are within 0 to 1, I multiplied 
                          // it by for using if else conditions 
  
  Serial.println(AM);
  //Serial.println(PM);
  //delay(500);

  if (trigger3 == true){
    trigger3count++;
    //Serial.println(trigger3count);
    if (trigger3count >= 10) { 
       angleChange = pow(gx * gx + gy * gy + gz * gz, 0.5);
       delay(10);
       //Serial.println("Angle change: " + angleChange); 
       if ((angleChange >= 0) && (angleChange <= 10)) { //if orientation changes remains between 0-10 degrees
           fall          = true; 
           trigger3      = false; 
           trigger3count = 0;
       } else { // user regained normal orientation
          trigger3      = false; 
          trigger3count = 0;
          Serial.println("TRIGGER 3 DEACTIVATED");
       }
     }
    fall = true;
  }
  
  if (fall == true) { //in event of a fall detection
    Serial.println("FALL DETECTED");
    digitalWrite(11, HIGH);
    delay(1000);
    digitalWrite(11, LOW);
 
    fall = false;
    // exit(1);
  }

  if (trigger2count > 5) { // allow 0.5s for orientation change
    trigger2      = false; 
    trigger2count = 0;
    Serial.println("TRIGGER 2 DEACTIVATED");
  }
 
  if (trigger1count > 5) { // allow 0.5s for AM to break upper threshold
    trigger1      = false; 
    trigger1count = 0;
    Serial.println("TRIGGER 1 DEACTIVATED");
  }
   
  if (trigger2 == true) {
    trigger2count++;
    // angleChange=acos(((double)x*(double)bx+(double)y*(double)by+(double)z*(double)bz)/(double)AM/(double)BM);
    angleChange = pow(gx * gx + gy * gy + gz * gz, 0.5); 
    
    if (angleChange >= 30 && angleChange <= 400) { //if orientation changes by between 80-100 degrees
      trigger3      = true; 
      trigger2      = false; 
      trigger2count = 0;
      Serial.println("TRIGGER 3 ACTIVATED");
    }
  }

  if (trigger1 == true) {
    trigger1count++;
    if (AM >= 12) { //if AM breaks upper threshold (3g)
      trigger2 = true;
      Serial.println("TRIGGER 2 ACTIVATED");
      trigger1      = false; 
      trigger1count = 0;
    }
  }
   
  if (AM <= 2 && trigger2 == false) { //if AM breaks lower threshold (0.4g)
    trigger1 = true;
    Serial.println("TRIGGER 1 ACTIVATED");
  }

  //It appears that delay is needed in order not to clog the port
  delay(100);
}

void mpu_read() {
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

void calibrate(int n) {
  for (int i = 0; i < (n * 333); i++) {
    mpu_read();
    acc_x_cal += AcX;
    acc_y_cal += AcY;
    acc_z_cal += AcZ;
    delay(3);
  }
  acc_x_cal /= (n * 333);
  acc_y_cal /= (n * 333);
  acc_z_cal /= (n * 333);
}
