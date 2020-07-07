#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <dht.h>
#define dataPin 8 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

File myFile;
// change this to match your SD shield or module;
const int chipSelect = 10;

const int LightSensorPin = A0;
int LightValue;
int l;

void setup () 
{
  Serial.begin(9600);
  delay(3000); // wait for console opening
  Serial.println("Hello");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }

  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  }

void loop () 

{

  LightValue = analogRead(LightSensorPin);
  l = map(LightValue, 0, 1023, 0, 100);
  
  int readData = DHT.read22(dataPin); // DHT22/AM2302

  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity
  
    DateTime now = rtc.now();
    
    Serial.println("Current Date & Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.print(t);
    Serial.print(',');
    Serial.print(h);
    Serial.print(',');
    Serial.println(l);
    
    delay(5000);


  myFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to datalog.txt...");
    
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(',');
    myFile.print(t);
    myFile.print(',');
    myFile.print(h);
    myFile.print(',');
    myFile.println(l);
    
    myFile.close();
   
    Serial.println("done.");

    delay(5000);
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datalog.txt");
  }

}
