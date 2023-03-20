// Including Arduino, ESP32 WiFi and Web Server, OLED graphics, NetWork Time Protocol(NTP), and Stopwatch libaries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <StopWatch.h>

// To use a different network, change the credientials below
char* SSID = "Kyle’s iPhone";
char* PASSWORD = "12345678";

// Server name and constructors to obtain local time from the NTP server
const char* NTPServer = "pool.ntp.org";
WiFiUDP NTPUDP;
NTPClient tClient(NTPUDP);

// Constructor for stopwatch, used to refresh code at 1 minute intervals
StopWatch tElapsed(StopWatch::SECONDS);

// Global variables:
String code = ""; // The authentication code in a String type, in order to be checked by the web server
int elapsed = 0; // The number of 10 second intervals elapsed, used for ladder animation
int intTime = 0; // The time format in an int type without ":", used as the seed for srand()
int nCode = 1; // Keeps track of the number of codes displayed in the serial monitor
int sec = 0; // Keeps track of the number of seconds elapsed
unsigned long prevSec; // Stores the previous second to avoid overlaps in ladder animation

// Initializing display and web server
Adafruit_SSD1306 display(128, 32, &Wire, 4);
WebServer server(80);

// Generates the random 6 digit authentication code. Calls getTime() to get current local time, which srand() then uses
// as its seed. randArr[6] is an array in which each number is chosen using the rand() function. These digits are then
// added to the String code which is displayed on the OLED screen and serial monitor.
void refreshCode()
{
    int randArr[6];
    code = "";
    getTime();
    srand(intTime);
    for (int i = 0; i < 6; i++)
    {
        randArr[i] = rand() % 10;
        code += String(randArr[i]);
    }
    // Alternative option for getting random code
    // int seed = rand() % (999999 + 1);
    // code = String(seed);

    // send to serial monitor
    Serial.print("(");
    Serial.print(nCode);
    Serial.print(")  :  ");
    Serial.println(code);
    nCode++;

    // display code
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);      
    display.setCursor(18, 9);
    display.cp437(true);    
    display.println(code);
    display.display();
}

// Used for creating the ladder animation, as well as keeping track of the 1 minute intervals. Every 10 seconds 
// the next "-" in the ladder is added until 1 minute passes and it is refreshed. The display is then cleared
// and refreshCode() is called to generate a new code.
void cycleCode()
{
    sec = tElapsed.elapsed();
    if (((sec % 10) == 0) && (sec != prevSec))
    {
        display.setTextSize(2); 
        display.setCursor(0, 20);    
        display.write("-");
        display.display();
        prevSec = sec;
        elapsed++;
    }
    if (elapsed > 1)
    {
        display.setCursor(0, 15);    
        display.write("-");
        display.display();
    }
    if (elapsed > 2)
    {
        display.setCursor(0, 10);    
        display.write("-");
        display.display();
    }
    if (elapsed > 3)
    {
        display.setCursor(0, 5);    
        display.write("-");
        display.display();
    }
    if (elapsed > 4)
    {
        display.setCursor(0, 0);    
        display.write("-");
        display.display();
    }
    if (elapsed > 5)
    {
        display.clearDisplay();
        elapsed = 0;
        // update code
        refreshCode();
    }
}

// Obtains the current local time from the NTP server, in the format HR:MIN:SEC
// This is then displayed to the serial monitor.
void getTime()
{
    String formattedTime;
    tClient.forceUpdate();
    formattedTime = tClient.getFormattedTime();
    Serial.print("Current Time: ");
    Serial.println(formattedTime);
    formattedTime.replace(":", "");
    intTime = formattedTime.toInt();
}

// Handler for the "/home" page. Checks code, username, and password fields.
// If fields are correct, will navigate to the "/authen" page, otherwise the 
// "incorrect" string wil be displayed under the submit button.
void handleAuthen()
{
    String incorrect;
    String username = "user";
    String password = "123";
    if (server.hasArg("CODE") && server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
    {
        if ((server.arg("CODE") == code) && (server.arg("USERNAME") == username) && (server.arg("PASSWORD") == password))
        {
            Serial.println("PASS");
            server.sendHeader("Location", "/authen");
            server.send(301);
        }
        else
        {
            Serial.println("FAIL");
            incorrect = "Wrong credentials, try again.";
        }
    }
    // Formatting the web page
    String body = "<html><body><form action='/home' method='POST'><H2>SecureID Token Authenticator</H2><br>";
    body += "Username:  <input type='text' name='USERNAME' placeholder='user'><br>";
    body += "Password:  <input type='password' name='PASSWORD' placeholder='123'><br>";
    body += "Code:      <input type='text' name='CODE' placeholder='Ex. 123456'><br><br>";
    body += "<input type='submit' name='SUBMIT' value='Submit'><br><br>";
    body += "<style type='text/css'>h1{ color:red; font-size: 12pt; }></style><h1>" + incorrect + "</h1></form><br>";
    server.send(200, "text/html", body);
}

// Handler for the "/authen" page. Simply displays a message to let the user
// know that they have been authenticated.
void handleSuccess()
{
    String body = "<html><body><H2>Successfully authenticated!</H2><br>";
    server.send(200, "text/html", body);    
}

// Function calls and processes which are run once before the main loop()
void setup()
{
    // Starts serial communication at baud rate of 9600
    Serial.begin(9600);

    // Print welcome message to make sure display is working
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setTextSize(2);     
    display.setTextColor(SSD1306_WHITE); 
    display.setCursor(0, 8);    
    display.cp437(true);       
    display.write("Welcome.");
    display.display();
    Serial.println("SecureID Web Server");

    // Search Wifi networks
    int numNetworks = 0;
    String temp;
    bool found = false;
    while (!found)
    {
        Serial.print("Searching... ");
        numNetworks = WiFi.scanNetworks();
        for (int i = 0; i < numNetworks; i++)
        {
            temp = WiFi.SSID(i);
            if (temp == SSID)
            {
                Serial.println("Found!");
                Serial.print(WiFi.SSID(i));
                Serial.print(" : ");
                Serial.println(WiFi.RSSI(i));
                found = true;
                break;
            }
        }
        if (!found)
        {
            Serial.println("Could not find network, restarting search");
        }
    }
    
    // Connect to network using SSID and PASSWORD, then display IP address
    Serial.print("Attemping to connect to ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("...");
        delay(1000);
    }
    Serial.println("Connected. IP address: ");
    Serial.println(WiFi.localIP());
    display.clearDisplay();

    // Start the web server and display message in serial monitor
    server.on("/authen", handleSuccess);
    server.on("/home", handleAuthen);
    Serial.println("Server ON");
    server.begin();

    // Get local time from the NTP server
    tClient.begin();
    tClient.setTimeOffset(-25200); // UTC (-7) in seconds

    // Start the stopwatch which controls the 1 minute intervals
    tElapsed.start();

    // Display the first authentication code 
    refreshCode();
}

// Body which is run repeatedly for rest of program
void loop()
{
    server.handleClient();
    cycleCode();
    delay(2);
}