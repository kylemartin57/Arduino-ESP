// // Kyle Martin
// // CPTS 315
// // Final Project

// // This final is temporarly run on the plane's board, to read in the gyroscope data used for training.
//  #include "Wire.h"
//  #include <MPU6050_light.h>
//  MPU6050 mpu(Wire);
//  unsigned long timer = 0;

//  void setup() 
//  {
//    Serial.begin(9600);
//    Wire.begin();
//    byte status = mpu.begin();
//    Serial.print(F("MPU6050 status: "));
//    Serial.println(status);
//    delay(3000);

//    while (status != 0) { }
//    Serial.println(F("Calculating offsets, do not move MPU6050"));
//    mpu.calcOffsets(); // gyro and accelero
//    Serial.println("Done!\n");
//  }
//  void loop() 
//  {
//    mpu.update();

//    // print data every 10ms
//    if ((millis() - timer) > 10) 
//    { 
//      Serial.print(mpu.getAngleX());
//      Serial.print(",");
//      Serial.print(mpu.getAngleY());
//      Serial.print(",");
//      Serial.println(mpu.getAngleZ());
//      timer = millis();
//    }
//  }