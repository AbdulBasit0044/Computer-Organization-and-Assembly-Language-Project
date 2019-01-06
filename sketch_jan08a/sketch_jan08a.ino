


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#define echoPin 4 // Echo Pin D2
#define trigPin 5 // Trigger Pin D1
 

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Blynk.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

// You should get Auth Token in the Blynk App.
//Go to the Project Settings (nut icon).



char auth[] = "cb675cb85ee64694921a0c3818be6a5b";

// Your WiFi credentials.

char ssid[] = "AndroidAp";
char pass[] = "dcje2203";


long duration, distance; // Duration used to calculate distance

void setup()
{
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth,ssid,pass);
 
 
}



void loop()
{
  
      
  delay(5);
  Blynk.run();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  delay(5);
  Serial.println(distance);
  Blynk.virtualWrite(V1,distance);
  //Blynk.virtualWrite(V2,distance);
  //Blynk.virtualWrite(V3,distance);
  if(distance<=20){
    delay(50);
    if(distance<=20){
       Blynk.notify("The Dust Bin is Full!!!");
    }
  }
 // Serial.println(distance);
  //Delay 50ms before next reading.
  delay(5000);
  
}

