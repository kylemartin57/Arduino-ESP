// #include <ESP32Servo.h>
// #include <Ps3Controller.h>

// Servo esc1;
// Servo esc2;
// Servo serv1;
// Servo serv2;

// int player = 0;
// int battery = 0;
// int potValue = 0;
// int servValHor = 0;
// int servValVert = 0;

// void notify()
// {
//     //---------------- Analog stick value events ---------------
//    if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
//        Serial.print("Moved the left stick:");
//        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
//        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
//        Serial.println();
//     }

//    if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
//        Serial.print("Moved the right stick:");
//        Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
//        Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
//        Serial.println();
//    }

//    //---------------------- Battery events ---------------------
//     if( battery != Ps3.data.status.battery ){
//         battery = Ps3.data.status.battery;
//         Serial.print("The controller battery is ");
//         if( battery == ps3_status_battery_charging )      Serial.println("charging");
//         else if( battery == ps3_status_battery_full )     Serial.println("FULL");
//         else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
//         else if( battery == ps3_status_battery_low)       Serial.println("LOW");
//         else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
//         else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
//         else Serial.println("UNDEFINED");
//     }

// }

// void onConnect(){
//     Serial.println("Connected.");
// }

// void setup() 
// {
//   // put your setup code here, to run once:
//   esc1.attach(17, 1000, 2000); // left brush
//   esc2.attach(16, 1000, 2000); // right brush
//   serv1.attach(14, 500, 2400); // left serv
//   serv2.attach(12, 500, 2400); // right serv
  
//   Ps3.attach(notify);
//   Ps3.attachOnConnect(onConnect);
//   Ps3.begin("00:1a:80:e6:3a:d2");

//   Serial.println("Ready.");
//   Serial.begin(9600);
// }

// void loop() 
// {
//   // put your main code here, to run repeatedly:
//   // esc.write(1000);
//   // delay(1000);
//   if(!Ps3.isConnected())
//     return;
//   //potValue = Ps3.data.analog.button.r2;
//   potValue = Ps3.data.analog.stick.ly * -1;
//   servValHor = Ps3.data.analog.stick.rx;
//   servValVert = Ps3.data.analog.stick.ry;
// //   //potValue = map(potValue, 0, 255, 0, 180);
// //   if (servValHor > 0)
// //   {
// //       servValHor = map(servValHor, 0, 127, 0, 180);
// //       Serial.println(servValHor);
// //       serv1.write(servValHor);
// //   }
// //   if (servValHor < 0)
// //   {
// //       servValHor = servValHor * -1;
// //       servValHor = map(servValHor, 0, 127, 0, 180);
// //       Serial.println(servValHor);
// //       serv2.write(servValHor);
// //   }

//   if (servValVert < 20 && servValVert > -20 && servValHor < 20 && servValHor > -20)
//   {
//       serv1.write(90);
//       serv2.write(90);
//   } 
//   else if (servValVert > 50 && servValVert < 100)
//   {
//        serv1.write(45);
//        serv2.write(135);
//   }
//   else if (servValVert < -50 && servValVert > -100)
//   {
//        serv1.write(135);
//        serv2.write(45);
//   }
//   else if (servValVert > 100)
//   {
//        serv1.write(0);
//        serv2.write(180);
//   }
//   else if (servValVert < -100)
//   {
//        serv1.write(180);
//        serv2.write(0);
//   }
//   else if (servValHor > 50 && servValHor < 100)
//   {
//        serv1.write(45);
//        serv2.write(45);
//   }
//   else if (servValHor < -50 && servValHor > -100)
//   {
//        serv1.write(135);
//        serv2.write(135);
//   }
//   else if (servValHor > 100)
//   {
//        serv1.write(0);
//        serv2.write(0);
//   }
//   else if (servValHor < -100)
//   {
//        serv1.write(180);
//        serv2.write(180);
//   }
  
// //   if (servValVert < 20 && servValVert > -20)
// //   {
// //       serv1.write(90);
// //       serv2.write(90);
// //   }

// //   if (servValVert > 0)
// //   {
// //       servValVert = map(servValVert, 0, 127, 90, 180);
// //       serv1.write(servValVert);
// //       servValVert = map(servValVert, 0, 127, 90, 0);
// //       serv2.write(servValVert);
// //   }
// //   if (servValVert < 0)
// //   {
// //       servValVert = servValVert * -1;
// //       servValVert = map(servValVert, 0, 127, 90, 180);
// //       serv2.write(servValVert);
// //       servValVert = map(servValVert, 0, 127, 90, 0);
// //       serv1.write(servValVert);
// //   }
//   potValue = map(potValue, 0, 127, 0, 180);
//   //Serial.println(potValue);
//   esc1.write(potValue);
//   esc2.write(potValue);
// }
