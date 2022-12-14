
#include <Arduino.h>
#include <MQ2.h> // MQ2 Gas Sensor Library
  
//int pinAout = A0; // pin A0 of NodeMcu is connected to pin A0 of MQ-2
int LPG, Co, Smoke; // Variable values of lpg, CO-gas, and Smoke
int sensorValue; 
//MQ2 mq2(pinAout); //
String secureUrl;
String secureUrl2;
String secureUrl3;
String secureUrl4;
int left = 0;
int right = 0;
int total = 0;
String user = "";
  #define S0 D0 
  #define SIG A0 
  
 
 void setup(void) { 
    // Instructions to start the MQ2 sensor
   //pinMode(pinAout,INPUT); 
   Serial.begin (9600);
   Serial.println ();                                    // cut the gibberish first line Serial Monitor
   Serial.println ();    
   Serial.print ("Hello sonvisage");                         // test Serial Monitor
   pinMode(S0,OUTPUT); 
}

 void loop (){
   
    digitalWrite(S0,LOW); 
    MQ2 mq2(SIG);
     mq2.begin();
    float* values= mq2.read(true); //if set to "false" will not appear on serial monitor
   
     LPG = mq2.readLPG();
     Co = mq2.readCO();
     Smoke = mq2.readSmoke();
   delay(1000);
    Serial.println(right);
    
    digitalWrite(S0,HIGH);
    
    sensorValue = analogRead(SIG);  
    Serial.println(sensorValue);  
    left = map(left, 0, 1023, 0, 60);
    Serial.println(left);
    total = left + right;
   delay(1000);   
 } 

 
  
  
