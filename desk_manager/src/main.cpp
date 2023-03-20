/****************************************************************
GestureTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
May 30, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor
Tests the gesture sensing abilities of the APDS-9960. Configures
APDS-9960 over I2C and waits for gesture events. Calculates the
direction of the swipe (up, down, left, right) and displays it
on a serial console. 
To perform a NEAR gesture, hold your hand
far above the sensor and move it close to the sensor (within 2
inches). Hold your hand there for at least 1 second and move it
away.
To perform a FAR gesture, hold your hand within 2 inches of the
sensor for at least 1 second and then move it above (out of
range) of the sensor.
Hardware Connections:
IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 wemos D1 mini Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 D2           SDA              I2C Data
 D1           SCL              I2C Clock
 D6           INT              Interrupt
Resources:
Include Wire.h and SparkFun_APDS-9960.h
Development environment specifics:
Written in Arduino 1.0.5
Tested with SparkFun Arduino Pro Mini 3.3V
This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!
Distributed as-is; no warranty is given.
Modified for ESP8266 by Jon Ulmer Nov 2016 then adapted by dgemily Dec 2020 for the new ESP8266 library
Modified:
- added wire.begin to configure pin
- Interrupt callback functions in IRAM (required for the new ESP8266 libary) by dgemily Dec 2020
- use digitalPinToInterrupt(GPIO) in attachInterrupt and detachInterrupt by dgemily Dec 2020
- use GPIO number by dgemily Dec 2020
****************************************************************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include "OpenWeatherMapOneCall.h"
#include <JsonListener.h>
#include <Arduino.h>
#include <Adafruit_I2CDevice.h>

// Pins on wemos D1 mini
#define APDS9960_SDA    4  //GPIO4 (D2)
#define APDS9960_SCL    5  //GPIO5 (D1)
// Constants
const byte APDS9960_INT  = 0; //GPIO12 (D3)

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile bool isr_flag = 0;

//Interrupt callback function in IRAM
void ICACHE_RAM_ATTR interruptRoutine ();

#define TFT_DC D4
#define TFT_CS D0

// Hardware SPI on Feather or other boards
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <time.h>
#include <U8x8lib.h>


char mySSID[]="Skywalker";
char myPASSWORD[]="vastship969";

const char* ssid = mySSID;              //from credentials.h file
const char* password = myPASSWORD;      //from credentials.h file

const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO    = "PST8PDT";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

// See https://docs.thingpulse.com/how-tos/openweathermap-key/
String OPEN_WEATHER_MAP_APP_ID = "78608bbedd88492678558bb0aeaae5f6";

float OPEN_WEATHER_MAP_LOCATTION_LAT = 37.3541; // Berlin, DE
float OPEN_WEATHER_MAP_LOCATTION_LON = -121.9552; // Berlin, DE

String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = false;

OpenWeatherMapOneCallData openWeatherMapOneCallData;

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;
bool weatherState = false;
bool clockState = false;
bool checkWeather = false;

int timezone = -7 * 3600;
int dst = 0;
int sensorPin = A0;

void updateServer();
void updateIndoorTemp();


void setup() {
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(50, 100);
  tft.setTextColor(GC9A01A_OLIVE);
  tft.setTextSize(5);
  tft.println("clock");
  delay(500);
   //Start I2C with pins defined above
  Wire.begin(APDS9960_SDA,APDS9960_SCL);

  // Set interrupt pin as input
  pinMode(digitalPinToInterrupt(APDS9960_INT), INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }

  WiFi.begin(ssid, password);
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(GC9A01A_GREENYELLOW);
  tft.setTextSize(2);
  tft.print("Connecting to network");
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);    
    if (++counter > 100) 
      ESP.restart();
    Serial.print( "." );
  }
  tft.setCursor(50, 80);
  tft.setTextColor(GC9A01A_GREENYELLOW);
  tft.setTextSize(2);
  tft.println("\nWiFi connected\n\n");

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  tft.setCursor(50, 110);
  tft.setTextColor(GC9A01A_GREENYELLOW);
  tft.setTextSize(2);
  tft.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");
     
     delay(1000);
  }
  tft.setCursor(50, 140);
  tft.setTextColor(GC9A01A_GREENYELLOW);
  tft.setTextSize(2);
  tft.println("\nTime response....OK"); 
    
  delay(1000);

  tft.fillScreen(GC9A01A_DARKCYAN);

   updateServer();
   updateIndoorTemp();
   delay(2000);

   tft.fillScreen(GC9A01A_BLACK);


}


void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        tft.fillCircle(120,120,120,GC9A01A_BLACK);
        // tft.setCursor(50, 100);
        // tft.setTextColor(GC9A01A_GREEN);
        // tft.setTextSize(4);
        // tft.println("clock");
        clockState = true;
        weatherState = false;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        // tft.fillCircle(120,120,120,GC9A01A_BLACK);
        // tft.setCursor(50, 100);
        // tft.setTextColor(GC9A01A_GREEN);
        // tft.setTextSize(4);
        // tft.println("weather");
        clockState = false;
        weatherState = true;
        checkWeather = true;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}

int curSeconds = 0, prevSeconds = 0;

void updateClock()
{
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  int r = 120;
  // Now draw the clock face 
  //
  //hour ticks
  if (curSeconds != p_tm->tm_sec)
  {
      // display second clear hand
    float angle = (p_tm->tm_sec-1)*6 ;
    angle=(angle/57.29577951) ; //Convert degrees to radians  
    int x3=(120+(sin(angle)*(r)));
    int y3=(120-(cos(angle)*(r)));
    tft.drawLine(120,120,x3,y3,GC9A01A_BLACK);
  
//      for( int z=0; z < 360;z= z + 30 ){
//    //Begin at 0° and stop at 360°
//      float angle = z ;
//      
//      angle=(angle/57.29577951) ; //Convert degrees to radians
//      int x2=(120+(sin(angle)*r));
//      int y2=(120-(cos(angle)*r));
//      int x3=(120+(sin(angle)*(r-20)));
//      int y3=(120-(cos(angle)*(r-20)));
//      tft.drawLine(x2,y2,x3,y3,GC9A01A_MAGENTA);
//    }
//    // display second hand
//    angle = p_tm->tm_sec*6 ;
//    angle=(angle/57.29577951) ; //Convert degrees to radians  
//    x3=(120+(sin(angle)*(r)));
//    y3=(120-(cos(angle)*(r)));
//    tft.drawLine(120,120,x3,y3,GC9A01A_CYAN);
    for( int z=0; z < 360;z= z + 6 ){
    //Begin at 0° and stop at 360°
      float angle = z ;
      
      angle=(angle/57.29577951) ; //Convert degrees to radians
      int x2=(120+(sin(angle)*r));
      int y2=(120-(cos(angle)*r));
      int x3=(120+(sin(angle)*(r-2)));
      int y3=(120-(cos(angle)*(r-2)));
      tft.drawLine(x2,y2,x3,y3,GC9A01A_MAROON);
    }
     for( int z=0; z < 360;z= z + 30 ){
    //Begin at 0° and stop at 360°
      float angle = z ;
      
      angle=(angle/57.29577951) ; //Convert degrees to radians
      int x2=(120+(sin(angle)*r));
      int y2=(120-(cos(angle)*r));
      int x3=(120+(sin(angle)*(r-20)));
      int y3=(120-(cos(angle)*(r-20)));
      tft.drawLine(x2,y2,x3,y3,GC9A01A_RED);
    }

    for( int z=0; z < 360;z= z + 90 ){
    //Begin at 0° and stop at 360°
      float angle = z ;
      
      angle=(angle/57.29577951) ; //Convert degrees to radians
      int x2=(120+(sin(angle)*r));
      int y2=(120-(cos(angle)*r));
      int x3=(120+(sin(angle)*(r-25)));
      int y3=(120-(cos(angle)*(r-25)));
      tft.drawLine(x2,y2,x3,y3,GC9A01A_RED);
    }

    
    
    //
    // display minute hand
    angle = p_tm->tm_min * 6 ;
    angle=(angle/57.29577951) ; //Convert degrees to radians  
    x3=(120+(sin(angle)*(r-25)));
    y3=(120-(cos(angle)*(r-25)));
    tft.drawLine(120,120,x3,y3,GC9A01A_DARKGREY);
  
    // display minute clear hand
    // angle = (p_tm->tm_min-1)*6 ;
    // angle=(angle/57.29577951) ; //Convert degrees to radians  
    // x3=(120+(sin(angle)*(r-20)));
    // y3=(120-(cos(angle)*(r-20)));
    // tft.drawLine(120,120,x3,y3,GC9A01A_BLACK);
  
    //
    // display hour hand
    angle = p_tm->tm_hour * 30 + int((p_tm->tm_min / 12) * 6 );
    angle=(angle/57.29577951) ; //Convert degrees to radians  
    x3=(120+(sin(angle)*(r-55)));
    y3=(120-(cos(angle)*(r-55)));
    tft.drawLine(120,120,x3,y3,GC9A01A_LIGHTGREY);
  
    // display hour clear hand
    // angle = p_tm->tm_hour * 30 + int(((p_tm->tm_min-20) / 12) * 6 );
    // angle=(angle/57.29577951) ; //Convert degrees to radians  
    // x3=(120+(sin(angle)*(r-55)));
    // y3=(120-(cos(angle)*(r-55)));
    // tft.drawLine(120,120,x3,y3,GC9A01A_BLACK);

        // display second hand
    angle = p_tm->tm_sec*6 ;
    angle=(angle/57.29577951) ; //Convert degrees to radians  
    x3=(120+(sin(angle)*(r)));
    y3=(120-(cos(angle)*(r)));
    tft.drawLine(120,120,x3,y3,GC9A01A_MAROON);
  //  
    tft.setTextSize(2);
    tft.setTextColor(GC9A01A_WHITE);
    tft.setCursor((tft.width()/2)+40,(tft.height()/2) + 5);
    tft.print(p_tm->tm_mday);
  
    tft.fillCircle(tft.width()/2, tft.height()/2, 3, GC9A01A_DARKGREY);

  }
//
//  if ((p_tm->tm_sec == 0) && (curSeconds != p_tm->tm_sec))
//  {
//    tft.fillScreen(GC9A01A_BLACK);
//  }
   // update display with all data

    if ((p_tm->tm_sec) == 0)
    {
      tft.fillCircle(120,120, 100, GC9A01A_BLACK);
    }
  
  curSeconds = p_tm->tm_sec;
  delay(100);
}

void updateServer()
{ 
  OpenWeatherMapOneCall *oneCallClient = new OpenWeatherMapOneCall();
  oneCallClient->setMetric(IS_METRIC);
  oneCallClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);

  long executionStart = millis();
  oneCallClient->update(&openWeatherMapOneCallData, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATTION_LAT, OPEN_WEATHER_MAP_LOCATTION_LON);
  delete oneCallClient;
  oneCallClient = nullptr;

  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(50, 50);
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);
  tft.println(String(openWeatherMapOneCallData.current.temp, 1) + (IS_METRIC ? " C" : " F"));

  tft.setTextSize(2);
  tft.setCursor(50, 100);
  Serial.println(openWeatherMapOneCallData.current.weatherDescription);
  tft.println(openWeatherMapOneCallData.current.weatherDescription);
}


void updateIndoorTemp()
{
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);
  tft.setCursor(50, 150);
  int reading = analogRead(sensorPin);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 3.3;
  voltage /= 1024.0; 
  
  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");
  
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                                //to degrees ((voltage - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degrees C");
  
  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 22.0;
  Serial.print(temperatureF); Serial.println(" degrees F");
  tft.println(temperatureF);
}


long timePassed;

void loop()
{
  updateClock();
  delay(10);
  // timePassed = millis() / 1000;
  // // if( isr_flag == 1 ) {
  // //   detachInterrupt(digitalPinToInterrupt(APDS9960_INT));
  // //   handleGesture();
  // //   isr_flag = 0;
  // //   attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);
  // // }
  // if (clockState)
  // {
  //   updateClock();
  //   delay(10);
  // }
  // if (weatherState)
  // {
  //   if (checkWeather)
  //   {
  //     tft.fillRect(50, 150, 200, 200, GC9A01A_BLACK);
  //     updateServer();
  //     updateIndoorTemp();
  //     delay(100);
  //     checkWeather = false;
  //   }
  //   if (timePassed % 1200 == 0)
  //   {
  //     tft.fillRect(50, 150, 200, 200, GC9A01A_BLACK);
  //     updateServer();
  //     updateIndoorTemp();
  //     delay(10);
  //   }
  // }
}

