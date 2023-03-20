// #include <ESP32Servo.h>
// #include <Ps3Controller.h>

// Servo esc1;
// Servo esc2;
// Servo serv1;
// Servo serv2;

// int player = 0;
// int battery = 0;
// int potValue;
// int servValue;

// void notify()
// {
//     //--- Digital cross/square/triangle/circle button events ---
//     if( Ps3.event.button_down.cross )
//         Serial.println("Started pressing the cross button");
//     if( Ps3.event.button_up.cross )
//         Serial.println("Released the cross button");

//     if( Ps3.event.button_down.square )
//         Serial.println("Started pressing the square button");
//     if( Ps3.event.button_up.square )
//         Serial.println("Released the square button");

//     if( Ps3.event.button_down.triangle )
//         Serial.println("Started pressing the triangle button");
//     if( Ps3.event.button_up.triangle )
//         Serial.println("Released the triangle button");

//     if( Ps3.event.button_down.circle )
//         Serial.println("Started pressing the circle button");
//     if( Ps3.event.button_up.circle )
//         Serial.println("Released the circle button");

//     //--------------- Digital D-pad button events --------------
//     if( Ps3.event.button_down.up )
//         Serial.println("Started pressing the up button");
//     if( Ps3.event.button_up.up )
//         Serial.println("Released the up button");

//     if( Ps3.event.button_down.right )
//         Serial.println("Started pressing the right button");
//     if( Ps3.event.button_up.right )
//         Serial.println("Released the right button");

//     if( Ps3.event.button_down.down )
//         Serial.println("Started pressing the down button");
//     if( Ps3.event.button_up.down )
//         Serial.println("Released the down button");

//     if( Ps3.event.button_down.left )
//         Serial.println("Started pressing the left button");
//     if( Ps3.event.button_up.left )
//         Serial.println("Released the left button");

//     //------------- Digital shoulder button events -------------
//     if( Ps3.event.button_down.l1 )
//         Serial.println("Started pressing the left shoulder button");
//     if( Ps3.event.button_up.l1 )
//         Serial.println("Released the left shoulder button");

//     if( Ps3.event.button_down.r1 )
//         Serial.println("Started pressing the right shoulder button");
//     if( Ps3.event.button_up.r1 )
//         Serial.println("Released the right shoulder button");

//     //-------------- Digital trigger button events -------------
//     if( Ps3.event.button_down.l2 )
//         Serial.println("Started pressing the left trigger button");
//     if( Ps3.event.button_up.l2 )
//         Serial.println("Released the left trigger button");

//     if( Ps3.event.button_down.r2 )
//         Serial.println("Started pressing the right trigger button");
//     if( Ps3.event.button_up.r2 )
//         Serial.println("Released the right trigger button");

//     //--------------- Digital stick button events --------------
//     if( Ps3.event.button_down.l3 )
//         Serial.println("Started pressing the left stick button");
//     if( Ps3.event.button_up.l3 )
//         Serial.println("Released the left stick button");

//     if( Ps3.event.button_down.r3 )
//         Serial.println("Started pressing the right stick button");
//     if( Ps3.event.button_up.r3 )
//         Serial.println("Released the right stick button");

//     //---------- Digital select/start/ps button events ---------
//     if( Ps3.event.button_down.select )
//         Serial.println("Started pressing the select button");
//     if( Ps3.event.button_up.select )
//         Serial.println("Released the select button");

//     if( Ps3.event.button_down.start )
//         Serial.println("Started pressing the start button");
//     if( Ps3.event.button_up.start )
//         Serial.println("Released the start button");

//     if( Ps3.event.button_down.ps )
//         Serial.println("Started pressing the Playstation button");
//     if( Ps3.event.button_up.ps )
//         Serial.println("Released the Playstation button");


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

//    //--------------- Analog D-pad button events ----------------
//    if( abs(Ps3.event.analog_changed.button.up) ){
//        Serial.print("Pressing the up button: ");
//        Serial.println(Ps3.data.analog.button.up, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.right) ){
//        Serial.print("Pressing the right button: ");
//        Serial.println(Ps3.data.analog.button.right, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.down) ){
//        Serial.print("Pressing the down button: ");
//        Serial.println(Ps3.data.analog.button.down, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.left) ){
//        Serial.print("Pressing the left button: ");
//        Serial.println(Ps3.data.analog.button.left, DEC);
//    }

//    //---------- Analog shoulder/trigger button events ----------
//    if( abs(Ps3.event.analog_changed.button.l1)){
//        Serial.print("Pressing the left shoulder button: ");
//        Serial.println(Ps3.data.analog.button.l1, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.r1) ){
//        Serial.print("Pressing the right shoulder button: ");
//        Serial.println(Ps3.data.analog.button.r1, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.l2) ){
//        Serial.print("Pressing the left trigger button: ");
//        Serial.println(Ps3.data.analog.button.l2, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.r2) ){
//        //Serial.print("Pressing the right trigger button: ");
//        //Serial.println(Ps3.data.analog.button.r2, DEC);
//    }

//    //---- Analog cross/square/triangle/circle button events ----
//    if( abs(Ps3.event.analog_changed.button.triangle)){
//        Serial.print("Pressing the triangle button: ");
//        Serial.println(Ps3.data.analog.button.triangle, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.circle) ){
//        Serial.print("Pressing the circle button: ");
//        Serial.println(Ps3.data.analog.button.circle, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.cross) ){
//        Serial.print("Pressing the cross button: ");
//        Serial.println(Ps3.data.analog.button.cross, DEC);
//    }

//    if( abs(Ps3.event.analog_changed.button.square) ){
//        Serial.print("Pressing the square button: ");
//        Serial.println(Ps3.data.analog.button.square, DEC);
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
//   servValue = Ps3.data.analog.stick.rx;
// //   //potValue = map(potValue, 0, 255, 0, 180);
//   if (servValue > 0)
//   {
//       servValue = map(servValue, 0, 127, 0, 180);
//       Serial.println(servValue);
//     //   serv1.write(servValue);
//   }
//   if (servValue < 0)
//   {
//       servValue = servValue * -1;
//       servValue = map(servValue, 0, 127, 0, 180);
//       Serial.println(servValue);
//     //   serv2.write(servValue);
//   }
//   potValue = map(potValue, 0, 127, 0, 180);
//   //Serial.println(potValue);
//   esc1.write(potValue);
//   esc2.write(potValue);
// }
