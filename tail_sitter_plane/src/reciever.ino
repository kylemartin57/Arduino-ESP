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

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

//Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(4, 5); // CE, CSN 
const byte address[6] = "00001";
int green = 2;
float p = 3.1415926;
void setup() {
  Serial.begin(115200);
  pinMode(green, OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  tft.begin();
  tft.fillScreen(BLACK);
  tftPrintTest();
}

void tftPrintTest() 
{
  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(RED);  
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 5);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(WHITE);
  tft.print(" seconds.");
}
void loop() {
  if (radio.available())
  {
    // refresh
    char text[32] = "";

     struct dataStruct {
      String gyro;
      String acc;
      String temp;
    };

    dataStruct d1;
    
    tft.setCursor(0, 25);
    tft.fillRect(0,25,128,10,BLACK);
    
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 5);
    //tft.println("Gryoscope (x,y,z)");
    tft.println("A dumb ass message: ");
    radio.read(&text, sizeof(text));
    Serial.println(text);
    tft.setCursor(0, 25);
    tft.setTextColor(MAGENTA);  
    tft.setTextSize(1);
    tft.println(text);

    digitalWrite(green, HIGH); 
    delay(500);
    digitalWrite(green, LOW); 
  }
  delay(100);
}
