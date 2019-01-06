#include <SoftwareSerial.h>

int const LDR = A0;                 //LDR input port, analong input A0
int Resistance = 0;                 //LDR analog value input
int const ThresholdResistance = 800;//Darkness level resistance limit
int const StreetLights = 6;         //Output port for street lights
int const LineTracker = 7;          //Input port for IR line tracker
int const YledT = 3;                //Output port for Yellow traffic light
int const RledT = 8;                //Output port for Red traffic light
int const GledT = 5;                //Output port for Green traffic light
int const PedLight = 2;             //Output port for pedestrian crossing light
int LineTrackerState = 0;           //IR line tracker digital input value 
int SignalFlag = 0;                 //Flag to switch traffic light when empty road
int SignalsCounter = 0;             //Counter for number of signals
int MissingCarTimer = 0;            //5 second threshold timer for empty road before signal switches
int seconds = 0;                    //Seconds counter
int MaxSeconds = 60;                //Default number of seconds till the signal remains open
int Rseconds = 0;                   //Remaining seconds from the last signal
int const Button = 9;               //Button input port, triggering the system
int ButtonValue = 1;                //Button digital input

void setup() 
{
  pinMode(StreetLights,OUTPUT);
  pinMode(LineTracker,INPUT);
  pinMode(YledT,OUTPUT);
  pinMode(RledT,OUTPUT);
  pinMode(GledT,OUTPUT);
  pinMode(PedLight,OUTPUT);
  pinMode(Button,INPUT);
  Serial.begin(9600);
}

void check_street_lights()
{
  Resistance = analogRead(LDR);
  Serial.println(Resistance);
  if (Resistance > ThresholdResistance)
    digitalWrite(StreetLights, HIGH);
  else
    digitalWrite(StreetLights, LOW);
}

void switch_off_signal()
{
  digitalWrite(GledT, LOW);
  digitalWrite(YledT,HIGH);
  delay(3000);
  check_street_lights();
  digitalWrite(YledT,LOW);
  digitalWrite(RledT,HIGH);
  digitalWrite(PedLight,HIGH);
  check_street_lights();
  delay(5000);
}

void switch_signal_on()
{
  digitalWrite(PedLight, LOW);
  delay(1000);
  digitalWrite(RledT,LOW);
  digitalWrite(GledT,HIGH);
}


void loop()
{
  Serial.println("Rseconds: ");
  Serial.println(Rseconds);

  //start program at button push
  while (ButtonValue != LOW)
    ButtonValue = digitalRead(Button);

  SignalsCounter = SignalsCounter + 1;
  if (SignalsCounter == 4){
    SignalsCounter = 0;
    Rseconds = 0;
  }
  
  MissingCarTimer = 0;
  seconds = MaxSeconds + Rseconds;    //MaxSeconds = 20
  SignalFlag = 0;
  
  switch_signal_on();
  while (seconds > 1)
  {
    check_street_lights();
    delay(1000);
    seconds = seconds - 1;
    LineTrackerState = digitalRead(LineTracker);
    Serial.println(LineTrackerState);             //LineTrackerState = 1 if no traffic, = 0 if passing traffic
    
    while (LineTrackerState && MissingCarTimer < 5)
    {
      check_street_lights();
      MissingCarTimer = MissingCarTimer + 1;
      //Serial.println(MissingCarTimer);
      delay(1000);
      seconds = seconds - 1;
      LineTrackerState = digitalRead(LineTracker);
      check_street_lights();
      if (MissingCarTimer == 5)
      {
        SignalFlag = 1;
        Rseconds = seconds;
      }
    }
    MissingCarTimer = 0;
    if (SignalFlag == 1)
      break;
  }
  switch_off_signal();
}

