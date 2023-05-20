#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>                             // Software Serial Library so we can use other Pins for communication with the GPS module
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

                      // Adafruit oled library for display
#include "DHT.h"
#define DHTPIN 2    
#define DHTTYPE DHT11   // DHT 11
                  // OLED display (SDA to Pin 4), (SCL to Pin 5)
DHT dht(DHTPIN, DHTTYPE);
static const int RXPin = 13, TXPin = 12;                // Ublox 6m GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;                   // Ublox GPS default Baud Rate is 9600

 #define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};


#include <Arduino.h>
#include <MQ2.h> // MQ2 Gas Sensor Library
  
//int pinAout = A0; // pin A0 of NodeMcu is connected to pin A0 of MQ-2
float LPG, Co, Smoke; // Variable values of lpg, CO-gas, and Smoke
float h, t, f; // Variables values for humidity, temperature in celcius and temperature in fahrenheit
float count = 0;
//int sensorValue; 
//MQ2 mq2(pinAout); //
String secureUrl;
String secureUrl2;
String secureUrl3;
String secureUrl4;
float latitude;
float longitude;

String user = "";
   #define S0 16
  #define  D0 
 #define SIG A0                  // Your Home Longitude

TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                        // The serial connection to the GPS device

#include <stdint.h>




void setup()
{  
                                      // Clear OLED display
     Serial.begin(9600);                              // Update display
  delay(1500);                                          // Pause 1.5 seconds  
  ss.begin(GPSBaud);  
  pinMode(S0,OUTPUT); 


  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
     // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Failure is always an option");

  Serial.println(F("DHTxx test!"));
  

  dht.begin();// Set Software Serial Comm Speed to 9600    
}

void loop()
{   
 
  Serial.print("Latitude  : ");
  latitude = gps.location.lat();
  longitude = gps.location.lng();
  Serial.println(latitude, 5);
  Serial.print("Longitude : ");
  Serial.println(longitude, 4);
  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());
  Serial.print("Elevation : ");
 Serial.print(gps.altitude.feet());
  Serial.println("ft"); 
  Serial.print("Time UTC  : ");
  Serial.print(gps.time.hour());                       // GPS time UTC 
  Serial.print(":");
  Serial.print(gps.time.minute());                     // Minutes
  Serial.print(":");
 Serial.println(gps.time.second());                   // Seconds
  Serial.print("Heading   : ");
  Serial.println(gps.course.deg());
  Serial.print("Speed     : ");
  Serial.println(gps.speed.mph());

    // Update display
  delay(200); 

  smartDelay(500);                                      // Run Procedure smartDelay

  if (millis() > 5000 && gps.charsProcessed() < 10)
   Serial.println(F("No GPS data received: check wiring"));

digitalWrite(S0,LOW); 
    MQ2 mq2(SIG);
     mq2.begin();
    float* values= mq2.read(true); //if set to "false" will not appear on serial monitor
    Serial.println("accesing the pointer values");
     LPG = mq2.readLPG();
     Co = mq2.readCO();
     Smoke = mq2.readSmoke();
//     float h= Co;
     Serial.println("printing each sensor value");
     Serial.println(sizeof(values));
    
     Serial.println("Co");
     Serial.println(Co, 5);
   delay(1000);
    
    digitalWrite(S0,HIGH);
    

      // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

   displayResults(t,h);
    display.display();
    delay(2000);
   display.clearDisplay();
     delay(1000); 
      delay(1000);  

}
 void displayResults(float t, float h){
  Serial.println("inside display");
  Serial.println(t);
  Serial.println(h);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.write("Air Monitor", 10);
    display.println();
    display.setCursor(0, 20);
    display.write("CO: ");
    display.print(Co, 8);
    display.println();
    display.write("Smoke: ");
    display.print(Smoke, 8);
    display.println();
    display.write("LPG: ");
    display.print(LPG, 8);
    display.println();
//    display.write("Temperature:");
//    display.print(t);
    display.write("Latitude:");
    display.print(latitude, 5);
//    display.print(260, OCT);
//    display.print(F("C "));
    display.println();
    display.write("Longitude:");
    display.print(longitude, 4);
//    display.write("Humidity:");
//    display.print(h);
//    display.print(F("%"));
    display.println();
    display.display();
    delay(1);
}
 void displayTitle(void){
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.write("sonvisage");
    display.display();
    delay(1);
}
static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
