// Kyle Martin
// CPTS 315
// Final Project

// This file runs on the reciever board on the plane. It receives 32 byte packets from the transmitter every 10ms.
// The packages contain the controller joystick locations so it can determine the throttle and aeleron movements.
// The decision tree is located in the corrections() function at line 282. This function makes adjustments when 
// the plane enters a none stable position. 

// Including libraries and defining variables
#include "Wire.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP32Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
unsigned long timer = 0;
int roll,pitch,yaw;
RF24 radio(32, 33); // CE, CSN
const byte address[6] = "00001";
int onBoard = 2;

Servo esc1;
Servo esc2;
Servo serv1;
Servo serv2;

int potValue = 0;
int servValHor = 0;
int servValVert = 0;
int adjustL = 0, adjustR = 0;

// Function prototypes
bool adjustServos();
bool corrections();

// Packet
struct dataStruct {
    int right_stick_x;
    int right_stick_y;
    int left_stick_x;
    int left_stick_y;
    byte connection;
    byte trimY;
  };

dataStruct d1;

void setup() 
{
  // Initializing servos and brushless motors, receiver, and gyroscope.
  Serial.begin(115200);
  Wire.begin();
  pinMode(onBoard, OUTPUT);
  
  // 127 is the initial position of the joystick.
  d1.right_stick_x = 127;
  d1.right_stick_y = 127;
  d1.left_stick_x = 127;
  d1.left_stick_y = 127;
  d1.connection = 0;

  esc1.attach(17, 1000, 2000); // left brush
  esc2.attach(16, 1000, 2000); // right brush
  serv1.attach(14, 500, 2400); // left serv
  serv2.attach(12, 500, 2400); // right serv

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  byte status = mpu.begin();
  while (status != 0) { }
  digitalWrite(onBoard, HIGH);
  delay(500);
  digitalWrite(onBoard, LOW);
  delay(500);
  digitalWrite(onBoard, HIGH);
  delay(500);
  digitalWrite(onBoard, LOW);
  delay(500);
  mpu.calcOffsets(); // gyro and accelero

}

void loop() 
{
  // Only move motors when a connection is established.
  if (radio.available())
  {
    d1.connection = 1;
    
    // Read in packet
    radio.read(&d1, sizeof(dataStruct));

    digitalWrite(onBoard, HIGH); 

    // Give the user the option to override corrections with controller, to avoid crash if possible.
    // Unfortuently, I did not have enough time to react and steer the plane to safety in the last test.
    if (!adjustServos())
    {
        corrections();
    }
    
    d1.left_stick_y = map(d1.left_stick_y, 0, 127, 0, 180);
    esc1.write(d1.left_stick_y + adjustL);
    esc2.write(d1.left_stick_y + adjustR);

  }
  delay(10);
  digitalWrite(onBoard, LOW); 
}

// User ranges for motion
void up()
{
  if (d1.right_stick_y >= 10 && d1.right_stick_y < 25) // up
  {
       serv2.write(75);
       serv1.write(105);
  } 
  else if (d1.right_stick_y >= 25 && d1.right_stick_y < 40) // up
  {
       serv2.write(60);
       serv1.write(120);
  } 
  else if (d1.right_stick_y >= 55 && d1.right_stick_y < 70) // up
  {
       serv2.write(45);
       serv1.write(135);
  }
  else if (d1.right_stick_y >= 85 && d1.right_stick_y < 100) // up
  {
       serv2.write(30);
       serv1.write(150);
  }
  else if (d1.right_stick_y >= 100 && d1.right_stick_y < 115) // up
  {
       serv2.write(15);
       serv1.write(165);
  }
  else if (d1.right_stick_y >= 115 && d1.right_stick_y < 128) // up
  {
       serv2.write(0);
       serv1.write(180);
  }
}

// User ranges for motion
void down()
{
  if (d1.right_stick_y <= -10 && d1.right_stick_y > -25) // down
  {
       serv2.write(105);
       serv1.write(75);
  } 
  else if (d1.right_stick_y <= -25 && d1.right_stick_y > -40) // down
  {
       serv2.write(120);
       serv1.write(60);
  } 
  else if (d1.right_stick_y <= -55 && d1.right_stick_y > -70) // down
  {
       serv2.write(135);
       serv1.write(45);
  }
  else if (d1.right_stick_y <= -85 && d1.right_stick_y > -100) // down
  {
       serv2.write(150);
       serv1.write(30);
  }
  else if (d1.right_stick_y <= -100 && d1.right_stick_y > -115) // down
  {
       serv2.write(165);
       serv1.write(15);
  }
  else if (d1.right_stick_y <= -115 && d1.right_stick_y > -128) // down
  {
       serv2.write(180);
       serv1.write(0);
  }
}

// User ranges for motion
void right()
{
  if (d1.right_stick_x >= 10 && d1.right_stick_x < 25) // right
  {
       serv1.write(75);
       serv2.write(75);
  } 
  else if (d1.right_stick_x >= 25 && d1.right_stick_x < 40) // right
  {
       serv1.write(60);
       serv2.write(60);
  } 
  else if (d1.right_stick_x >= 55 && d1.right_stick_x < 70) // right
  {
       serv1.write(45);
       serv2.write(45);
  }
  else if (d1.right_stick_x >= 85 && d1.right_stick_x < 100) // right
  {
       serv1.write(30);
       serv2.write(30);
  }
  else if (d1.right_stick_x >= 100 && d1.right_stick_x < 115) // right
  {
       serv1.write(15);
       serv2.write(15);
  }
  else if (d1.right_stick_x >= 115 && d1.right_stick_x < 128) // right
  {
       serv1.write(0);
       serv2.write(0);
  }
}

// User ranges for motion
void left()
{
  if (d1.right_stick_x <= -10 && d1.right_stick_x > -25) // left
  {
       serv1.write(105);
       serv2.write(105) ;
  } 
  else if (d1.right_stick_x <= -25 && d1.right_stick_x > -40) // left
  {
       serv1.write(120);
       serv2.write(120);
  } 
  else if (d1.right_stick_x <= -55 && d1.right_stick_x > -70) // left
  {
       serv1.write(135);
       serv2.write(135);
  }
  else if (d1.right_stick_x <= -85 && d1.right_stick_x > -100) // left
  {
       serv1.write(150);
       serv2.write(150);
  }
  else if (d1.right_stick_x <= -100 && d1.right_stick_x > -115) // left
  {
       serv1.write(165);
       serv2.write(165);
  }
  else if (d1.right_stick_x <= -115 && d1.right_stick_x > -128) // left
  {
       serv1.write(180);
       serv2.write(180);
  }
}

// Allows for user controlled motion
bool adjustServos()
{
  if (d1.right_stick_y < 10 && d1.right_stick_y > -10 && d1.right_stick_x < 10 && d1.right_stick_x > -10) // flat
  {
     //  serv1.write(90);
     //  serv2.write(90);
      return false;
  }
  else
  {
     if (abs(d1.right_stick_x) > abs(d1.right_stick_y))
     {
          right();
          left();
          return true;
     }
     else
     {
          up();
          down();
          return true;
     }
  }
}

// Decision tree
bool corrections()
{
    // Read in gyroscope data
    mpu.update();
    roll = mpu.getAngleX();
    pitch = mpu.getAngleY();
    yaw = mpu.getAngleZ();

    // Start of tree. Accuracies for each output from Weka are shown.
    if (roll < 6.66)
     {
        if (roll < -6.97)
        {
            if (pitch < -18.42)
            {
                adjustR = 0;
                adjustL = 0;
                serv2.write(105);
                serv1.write(75);
                // Serial.println("pitch down"); // (21/0) [9/0]
                return true;
            }
            else if (pitch >= -18.42)
            {
                if (pitch < 17.71)
                {
                    adjustL = 2;
                    // Serial.println("roll right"); // (166/15) [85/6]
                    return true;
                }
                else if (pitch >= 17.71)
                {
                    adjustR = 0;
                    adjustL = 0;
                    serv2.write(70);
                    serv1.write(110);
                    // Serial.println("pitch up"); // (17/0) [4/0]
                    return true;
                }
            }
        }
        else if (roll >= -7)
        {
            if (pitch < 4.63)
            {
                if (pitch < -8.65)
                {
                    adjustR = 0;
                    adjustL = 0;
                    serv2.write(105);
                    serv1.write(75);
                    // Serial.println("pitch down"); // (49/11) [39/15]
                    return true;
                }
                else if (pitch >= -8.65)
                {
                    adjustR = 0;
                    adjustL = 0;
                    serv2.write(90);
                    serv1.write(90);
                    return false;
                    // Serial.println("none"); // (277/59) [132/31]
                }
            }
            else if (pitch >= 4.63)
            {
                adjustR = 0;
                adjustL = 0;
                serv2.write(70);
                serv1.write(110);
                // Serial.println("pitch up"); // (111/7) [51/3]
                return true;
            }
        }
    }
    else if (roll >= 6.66)
    {
        if (pitch < -16.3)
        {
            adjustR = 0;
            adjustL = 0;
            serv2.write(105);
            serv1.write(75);
            // Serial.println("pitch down"); // (63/0) [23/0]
            return true;
        }
        else if (pitch >= -16.3)
        {
            if (pitch < 9.06)
            {
                adjustR = 2;
                // Serial.println("roll left"); // (126/6) [67/4]
                return true;
            }
            else if (pitch >= 9.06)
            {
                adjustR = 0;
                adjustL = 0;
                serv2.write(70);
                serv1.write(110);
                // Serial.println("pitch up"); // (21/0) [16/0]
                return true;
            }
        }
    }
}
     
