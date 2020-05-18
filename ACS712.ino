#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
/*
Measuring Current Using ACS712
*///--------------------------------------------------------------
const int analogIn = A0;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;
double Walt  = 0;

double Voltage1= 0;
double VRMS = 0;
double AmpsRMS = 0;
//----------------------------------------------------------------
/*
YF‐ S201 Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
Adaptation Courtesy: www.hobbytronics.co.uk
*/
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float lit;
unsigned long currentTime;
unsigned long cloopTime; 
//-----------------------------------------------------------------
void flow () // Interrupt function
{
   flow_frequency++;
}
float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(analogIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the minimum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
void setup()
{ 
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   Serial2.begin(115200);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   
   lcd.begin();                    
   lcd.backlight();
   lcd.setCursor(2,0);
   lcd.print("Ket Thuc Mon");
}

void loop()
{
currentTime = millis();
// Every second, calculate and print litres/hour
if(currentTime >= (cloopTime + 1000))
{
  cloopTime = currentTime; // Updates cloopTime
  // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
  l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
  flowMilliLitres = (l_hour / 60) * 1000;
  totalMilliLitres += flowMilliLitres/60;
  lit = (float)totalMilliLitres/1000;
  flow_frequency = 0; // Reset Counter
  
  Serial.print("Water Value = " ); // shows pre-scaled value 
  Serial.print((float)lit,3); // Print litres/hour
  Serial.println("L");
}
 unsigned int x=0;
  float average = 0;
   for(int i = 0; i < 1000; i++) {
   average = average + (.049 * analogRead(A0) -25);// for 20A mode
   delay(1);
 }
 average = abs(average/1000);
 if(average > 0.1)
 {
  Voltage = 220;
 }
 else
 {
  Voltage = 0;
 }
 Serial.print("Voltage :");
 Serial.print(Voltage);
 Serial.println("V");
 Serial.print("Current :");
 Serial.print(average);
 Serial.println("A");
 delay(50);
 Walt = Voltage * average;
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Nuoc =");
 lcd.setCursor(7,0);
 lcd.print(lit);
 lcd.setCursor(12,0);
 lcd.print("L");

 lcd.setCursor(0,1);
 lcd.print("Dien =");
 lcd.setCursor(7,1);
 lcd.print(Walt);
 lcd.setCursor(12,1);
 lcd.print("W");

 Serial.print("Walt Value = " ); // shows pre-scaled value 
 Serial.print(Walt,3);
 Serial.println("W"); 

 Serial2.println(lit,3); // the '3' after voltage allows you to display 3 digits after decimal point
 Serial2.print("-"); // shows the voltage measured 
 Serial2.println(Walt,3); // the '3' after voltage allows you to display 3 digits after decimal point
 totalMilliLitres = 0;
 delay(10000); 
}
