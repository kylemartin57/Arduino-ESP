// /* Get tilt angles on X and Y, and rotation angle on Z
//     Angles are given in degrees
//  */
// #include "Wire.h"
// #include <MPU6050_light.h>
// MPU6050 mpu(Wire);
// unsigned long timer = 0;
// int roll,pitch,yaw;
// void setup() 
// {
//   Serial.begin(9600);
//   Wire.begin();
//   byte status = mpu.begin();
//   Serial.print(F("MPU6050 status: "));
//   Serial.println(status);
//   while (status != 0) { } // stop everything if could not connect to MPU6050
//   Serial.println(F("Calculating offsets, do not move MPU6050"));
//   delay(3000);
//   mpu.calcOffsets(); // gyro and accelero
//   Serial.println("Done!\n");
// }

// void loop() 
// {
//   mpu.update();
//   roll = mpu.getAngleX();
//   pitch = mpu.getAngleY();
//   yaw = mpu.getAngleZ();
//   if ((millis() - timer) > 10) 
//   { // print data every 10ms
    
//     if (roll < 6.66)
//     {
//         if (roll < -7)
//         {
//             if (pitch < -17.01)
//             {
//                 Serial.println("pitch down"); // (21/0) [9/0]
//             }
//             else if (pitch >= -17.01)
//             {
//                 if (pitch < 17.71)
//                 {
//                     Serial.println("roll right"); // (166/15) [85/6]
//                 }
//                 else if (pitch >= 17.71)
//                 {
//                     Serial.println("pitch up"); // (17/0) [4/0]
//                 }
//             }
//         }
//         else if (roll >= -7)
//         {
//             if (pitch < 5.08)
//             {
//                 if (pitch < -12.39)
//                 {
//                     Serial.println("pitch down"); // (49/11) [39/15]
//                 }
//                 else if (pitch >= -12.39)
//                 {
//                     Serial.println("none"); // (277/59) [132/31]
//                 }
//             }
//             else if (pitch >= 5.08)
//             {
//                 Serial.println("pitch up"); // (111/7) [51/3]
//             }
//         }
//     }
//     else if (roll >= 6.66)
//     {
//         if (pitch < -15.81)
//         {
//             Serial.println("pitch down"); // (63/0) [23/0]
//         }
//         else if (pitch >= -15.81)
//         {
//             if (pitch < 9.06)
//             {
//                 Serial.println("roll left"); // (126/6) [67/4]
//             }
//             else if (pitch >= 9.06)
//             {
//                 Serial.println("pitch up"); // (21/0) [16/0]
//             }
//         }
//     }
//     timer = millis();
//   }
// }

//  // TESTS
//  // SIDES
//  // 1. Works ok, but the middle needs more range
// // if (yaw < -0.67)
// // {
// //     Serial.println("-1");
// // }
// // if (yaw >= -0.67)
// // {
// //     if (yaw < 0.81)
// //     {
// //         if (pitch < -1.1)
// //         {
// //             Serial.println("1");
// //         }
// //         if (pitch >= -1.1)
// //         {
// //             Serial.println("0");
// //         }
// //     }
// //     if (yaw >= 0.81)
// //     {
// //         Serial.println("1");
// //     }
// // }

// //ALL
// // if (yaw < -4.54)
// //     {
// //       if (roll < 4.11)
// //       {
// //         Serial.println("Serial.println("none"); //");
// //       }
// //       else if (roll >= 4.11)
// //       {
// //         Serial.println("pitch left");
// //       }
// //     }
// //     else if (yaw >= -4.54)
// //     {
// //       if (roll < -7.6)
// //       {
// //         Serial.println("pitch right");
// //       }
// //       else if (roll >= -7.6)
// //       {
// //         if (roll < 4.25)
// //         {
// //           if (pitch < 8.6)
// //           {
// //             if (yaw < 1.65)
// //             {
// //               if (pitch < 0.84)
// //               {
// //                 if (roll < -0.21)
// //                 {
// //                   if (yaw < -0.98)
// //                   {
// //                     Serial.println("pitch right");
// //                   }
// //                   else if (yaw >= -0.98)
// //                   {
// //                     if (roll < -1.19)
// //                     {
// //                       Serial.println("pitch right");
// //                     }
// //                     else if (roll >= -1.19)
// //                     {
// //                       if (pitch < -1.64)
// //                       {
// //                         Serial.println("pitch right");
// //                       }
// //                       else if (pitch >= -1.64)
// //                       {
// //                         Serial.println("yaw back");
// //                       }
// //                     }
// //                   }
// //                 }
// //                 else if (roll >= -0.21)
// //                 {
// //                   if (pitch < -1.36)
// //                   {
// //                     if (yaw < -1.03)
// //                     {
// //                       Serial.println("yaw forw");
// //                     }
// //                     else if (yaw >= -1.03)
// //                     {
// //                       Serial.println("yaw back");
// //                     }
// //                   }
// //                   else if (pitch >= -1.36)
// //                   {
// //                     Serial.println("yaw back");
// //                   }
// //                 }
// //               }
// //               else if (pitch >= 0.84)
// //               {
// //                 if (roll < -1.3)
// //                 {
// //                   Serial.println("yaw forw");
// //                 }
// //                 else if (roll >= -1.3)
// //                 {
// //                   if (roll < 1.56)
// //                   {
// //                     if (yaw < 0.23)
// //                     {
// //                       if (pitch < 4.93)
// //                       {
// //                         if (yaw < -1.52)
// //                         {
// //                           Serial.println("yaw back");
// //                         }
// //                         else if (yaw >= -1.52)
// //                         {
// //                           if (pitch < 1.19)
// //                           {
// //                             Serial.println("yaw back");
// //                           }
// //                           else if (pitch >= 1.19)
// //                           {
// //                             Serial.println("yaw forw");
// //                           }
// //                         }
// //                       }
// //                       else if (pitch >= 4.93)
// //                       {
// //                         Serial.println("yaw forw");
// //                       }
// //                     }
// //                     else if (yaw >= 0.23)
// //                     {
// //                       Serial.println("yaw back");
// //                     }
// //                   }
// //                   else if (roll >= 1.56)
// //                   {
// //                     Serial.println("yaw forw");
// //                   }
// //                 }
// //               }
// //             }
// //             else if (yaw >= 1.65)
// //             {
// //               if (roll < -3.5)
// //               {
// //                 if (pitch < 4.04)
// //                 {
// //                   Serial.println("pitch right");
// //                 }
// //                 else if (pitch >= 4.04)
// //                 {
// //                   Serial.println("yaw forw");
// //                 }
// //               }
// //               else if (roll >= -3.5)
// //               {
// //                 if (roll < -2.58)
// //                 {
// //                   Serial.println("yaw back");
// //                 }
// //                 else if (roll >= -2.58)
// //                 {
// //                   if (pitch < -1.02)
// //                   {
// //                     if (pitch < -1.44)
// //                     {
// //                       Serial.println("yaw back");
// //                     }
// //                     else if (pitch >= -1.44)
// //                     {
// //                       if (roll < -1.17)
// //                       {
// //                         Serial.println("pitch right");
// //                       }
// //                       else if (roll >= -1.17)
// //                       {
// //                         Serial.println("yaw back");
// //                       }
// //                     }
// //                   }
// //                   else if (pitch >= -1.02)
// //                   {
// //                     Serial.println("yaw back");
// //                   }
// //                 }
// //               }
// //             }
// //           }
// //           else if (pitch >= 8.6)
// //           {
// //             if (pitch < 9.51)
// //             {
// //               if (yaw < 1.77)
// //               {
// //                 Serial.println("yaw forw");
// //               }
// //               else if (yaw >= 1.77)
// //               {
// //                 Serial.println("yaw back");
// //               }
// //             }
// //             else if (pitch >= 9.51)
// //             {
// //               Serial.println("yaw forw");
// //             }
// //           }
// //         }
// //         else if (roll >= 4.25)
// //         {
// //           if (pitch < 4.47)
// //           {
// //             Serial.println("pitch left");
// //           }
// //           else if (pitch >= 4.47)
// //           {
// //             Serial.println("yaw forw");
// //           }
// //         }
// //       }
// //     }

// // ALL 2
// // if (roll < 0.77)
// //     {
// //         if (pitch < -1.93)
// //         {
// //             Serial.println("pitch down"); // (72/2) [31/2]
// //         }
// //         else if (pitch >= -1.93)
// //         {
// //             if (pitch < 6.19)
// //             {
// //                 if (roll < -3.34)
// //                 {
// //                     Serial.printl)
// //  Serial.println("roll right"); //"); // (58/0) [29/1] WRONG
// //                 }
// //                 else if (roll >= -3.34)
// //                 {
// //                     Serial.println("none"); // (152/23) [77/16]
// //                 }
// //             }
// //             else if (pitch >= 6.19)
// //             {
// //                 if (roll < -14.13)
// //                 {
// //                     Serial.printl)
// //  Serial.println("roll right"); //"); // (2/0) [3/0] WRONG
// //                 }
// //                 else if (roll >= -14.13)
// //                 {
// //                     Serial.println("pitch up"); // (51/0) [27/3]
// //                 }
// //             }
// //         }
// //     }
// //     else if (roll >= 0.77)
// //     {
// //         if (roll < 4.16)
// //         {
// //             if (pitch < -0.25)
// //             {
// //                 if (pitch < -20.5)
// //                 {
// //                     Serial.println("pitch down"); // (2/0) [3/0]
// //                 }
// //                 else if(pitch >= -20.5)
// //                 {
// //                     Serial.println("roll left"); // (9/0) [5/1]
// //                 }
// //             }
// //             else if (pitch >= -0.25)
// //             {
// //                 Serial.println("none"); // (13/0) [7/0]
// //             }
// //         }
// //         else if (roll >= 4.16)
// //         {
// //             Serial.println("roll left"); // (71/0) [34/0]
// //         }
// //     }

// // TEST 3
// // if (roll < 7.83)
// //         {
// //             if (pitch < -0.68)
// //             {
// //                 if (roll < -7.21)
// //                 {
// //                     Serial.println("roll right"); // (2/0) [1/0]
// //                 }
// //                 else if (roll >= -7.21)
// //                 {
// //                     Serial.println("none"); // (174/5) [84/7]
// //                 }
// //             }
// //             else if (pitch >= -0.68)
// //             {
// //                 if (yaw < 0.58)
// //                 {
// //                     Serial.println("yaw right"); // (166/64) [92/39]
// //                 }
// //                 else if (yaw >= 0.58)
// //                 {
// //                     Serial.println("roll right"); // (52/26) [29/15]
// //                 }
// //             }
// //         }
// //         else if (roll >= 7.83)
// //         {
// //             Serial.println("roll left"); // (100/3) [50/0]
// //         }
// //     }
// //     else if (yaw >= 2.26)
// //     {
// //         if (pitch < -3.75)
// //         {
// //             if (yaw < 6.42)
// //             {
// //                 if (roll < -5.63)
// //                 {
// //                     Serial.println("roll right"); // (41/0) [21/0]
// //                 }
// //                 else if (roll >= -5.63)
// //                 {
// //                     Serial.println("roll left"); // (31/10) [10/3]
// //                 }
// //             }
// //             else if (yaw >= 6.42)
// //             {
// //                 Serial.println("pitch down"); // (138/0) [67/2]
// //             }
// //         }
// //         else if (pitch >= -3.75)
// //         {
// //             if (roll < -8.01)
// //             {
// //                 if (yaw < 5.96)
// //                 {
// //                     Serial.println("roll right"); // (99/2) [48/4]
// //                 }
// //                 else if (yaw >= 5.96)
// //                 {
// //                     Serial.println("pitch up"); // (17/1) [9/1]
// //                 }
// //             }
// //             else if (roll >= -8.01)
// //             {
// //                 if (pitch < 2.63)
// //                 {
// //                     Serial.println("yaw left"); // (101/16) [46/11]
// //                 }
// //                 else if (pitch >= 2.63)
// //                 {
// //                     Serial.println("pitch up"); // (140/4) [74/6]
// //                 }
// //             }
// //         }