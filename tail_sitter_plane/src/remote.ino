// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
// CLK
#define SCLK_PIN 26
// DIN  
#define MOSI_PIN 27
#define DC_PIN   33
#define CS_PIN   32
#define RST_PIN  25

// Color definitions 
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Ps3Controller.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string>

RF24 radio(4, 5); // CE, CSN 
const byte address[6] = "00001";

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

struct dataStruct {
    int right_stick_x;
    int right_stick_y;
    int left_stick_x;
    int left_stick_y;
    byte connection;
    byte trimY;
  };

dataStruct d1;
int battery = 0;
// int prevVal = 0;
int battery_indicator = 0;
int onBoard = 2;
int thrust = 0;
int delayAmount = 10;
// bool showVals = false;

// void displayValues();
// void clearValues();

void notify()
{
    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
       Serial.print("Moved the left stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
    }

   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       Serial.print("Moved the right stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
       Serial.println();
   }

   //---------------------- Battery events ---------------------
    if( battery != Ps3.data.status.battery ){
        battery = Ps3.data.status.battery;
        Serial.print("The controller battery is ");
        if( battery == ps3_status_battery_charging )      battery_indicator = 1;
        else if( battery == ps3_status_battery_full )     battery_indicator = 2;
        else if( battery == ps3_status_battery_high )     battery_indicator = 3;
        else if( battery == ps3_status_battery_low)       battery_indicator = 4;
        else if( battery == ps3_status_battery_dying )    battery_indicator = 5;
        else if( battery == ps3_status_battery_shutdown ) battery_indicator = 6;
        else Serial.println("UNDEFINED");
    }

}

void onConnect(){
    Serial.println("Connected.");
}

void setup() 
{
  Serial.begin(115200);
  pinMode(onBoard, OUTPUT);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("00:1a:80:e6:3a:d2");

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(2);
  tft.print("waiting to pair...");

  d1.right_stick_x = 127;
  d1.right_stick_y = 127;
  d1.left_stick_x = 127;
  d1.left_stick_y = 127;
  d1.connection = 0;

  while(!Ps3.isConnected())
  {
    if (Ps3.isConnected())
    {
      break;
    }
  }

  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(CYAN);  
  tft.setTextSize(2);
  tft.print("battery:");

  tft.setCursor(20, 50);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);

  if(battery_indicator == 1)
  {
      tft.print("CHARGING");
  }      
  else if(battery_indicator == 2)
  {
      tft.print("full");
  }
  else if(battery_indicator == 3)
  {
      tft.print("high");
  }
  else if(battery_indicator == 4)
  {
      tft.print("low");
  }
  else if(battery_indicator == 5)
  {
      tft.print("dying");
  }
  else if(battery_indicator == 6)
  {
      tft.print("shutdown");
  }
  else
  {
      tft.print("N/A");
  }

  delay(1000);
  tft.fillScreen(BLACK);
}

void loop() 
{
  if(!Ps3.isConnected())
    return;

  if (d1.connection == 1)
  {
    digitalWrite(onBoard, HIGH); 
  }

  d1.left_stick_y = Ps3.data.analog.stick.ly * -1;
  d1.right_stick_x = Ps3.data.analog.stick.rx;
  d1.right_stick_y = Ps3.data.analog.stick.ry;

  if (d1.left_stick_y > 127)
    {
     d1.left_stick_y = 127;
    }

    // if( Ps3.event.button_down.cross )
    // {
    //     showVals = true;
    // }

    // if (showVals)
    // {
    //     displayValues();
    // }
    
    // if( Ps3.event.button_down.circle )
    // {
    //     clearValues();
    // }

    // if( Ps3.event.button_down.up )
    // {

    // }

  radio.write(&d1, sizeof(dataStruct));
  delay(delayAmount);
}

// void displayValues()
// {
//   if (d1.left_stick_y >= 0 && d1.left_stick_y <= 127 && d1.left_stick_y != prevVal)
//   {
//     tft.fillRect(40,40, 50, 40, BLACK);
//     tft.setCursor(50, 50);
//     tft.setTextColor(WHITE);  
//     tft.setTextSize(2);
//     thrust = map(d1.left_stick_y, 0, 127, 0, 100);
//     tft.print(thrust);

//     tft.fillRect(0,50, d1.left_stick_y, 20, WHITE);
//     tft.fillRect(d1.left_stick_y+1,50, 127, 20, BLACK);

//     tft.fillRect(0,80, 50, 40, BLACK);
//     tft.setCursor(0, 90);
//     tft.setTextColor(MAGENTA);
//     tft.print(d1.right_stick_x);

//     tft.fillRect(70,80, 50, 40, BLACK);
//     tft.setCursor(70, 90);
//     tft.setTextColor(YELLOW);
//     tft.print(d1.right_stick_y);
//   }

//   prevVal = d1.left_stick_y;
// }

// void clearValues()
// {
//     tft.fillScreen(BLACK);
// }