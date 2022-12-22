#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
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
int LPG, Co, Smoke; // Variable values of lpg, CO-gas, and Smoke
float h, t, f; // Variables values for humidity, temperature in celcius and temperature in fahrenheit
float count = 0;
//int sensorValue; 
//MQ2 mq2(pinAout); //
String secureUrl;
String secureUrl2;
String secureUrl3;
String secureUrl4;

String user = "";
  #define S0 D0 
  #define SIG A0


#include "DHT.h"

#define DHTPIN 2    
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

  
 
 void setup(void) { 
    // Instructions to start the MQ2 sensor
   //pinMode(pinAout,INPUT); 
   Serial.begin (9600);
   Serial.println ();                                    // cut the gibberish first line Serial Monitor
   Serial.println ();    
   Serial.print ("Hello sonvisage");                         // test Serial Monitor
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

  dht.begin();
}

 void loop (){
   
    digitalWrite(S0,LOW); 
    MQ2 mq2(SIG);
     mq2.begin();
    float* values= mq2.read(true); //if set to "false" will not appear on serial monitor
   
     LPG = mq2.readLPG();
     Co = mq2.readCO();
     Smoke = mq2.readSmoke();
     Serial.println("printing each sensor value");
     Serial.println(sizeof(values));
     for (uint8_t i=0; i< 4; i++){
      Serial.println(values[i]);
     }
     Serial.println(Co);
     Serial.println(Smoke);
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


  // display results on oled screen
    displayResults(t,h);
    display.display();
    delay(2000);
    display.clearDisplay();
     delay(1000);   
 } 

 void displayResults(float t, float h){
  Serial.println("inside display");
  Serial.println(t);
  Serial.println(h);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.write("CO: ");
    display.print(Co);
    display.println();
    display.write("Smoke: ");
    display.print(Smoke);
    display.println();
    display.write("LPG: ");
    display.print(LPG);
    display.println();
    display.write("Temperature:");
    display.print(t);
//    display.print(260, OCT);
    display.print(F("C "));
    display.println();
    display.write("Humidity:");
    display.print(h);
    display.print(F("%"));
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

 
  
  
