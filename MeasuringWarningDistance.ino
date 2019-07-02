#include "RTClib.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>

//about measuring distanc
const int TriggerPin = 8; //Trig pin
const int EchoPin = 9; //Echo pin

float Duration = 0; //Variable Duration declaration
float Distance_m = 0; //Variable Distance_m declaration 

//about displaying information(distance and time)
RTC_DS1307 rtc; //RTC declaration
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  //A day of week array declaration
LiquidCrystal lcd(A0, A1, 5, 4, 3, 2); // (rs, e, d4, d5, d6, d7)

//about indicating with LED
const int green = 6;  //green led pin variable declaration
const int red = 7;  //red led pin variable declaration

//warning distance
const float WD = 0.75;  //WD(warning distance) variable declaration

//SD
File logfile;  //logFile variable declaration
const int chipSelect = 10;  //chipSelect variable declaration

void setup() {
  pinMode(TriggerPin, OUTPUT); // Trigger is an output pin
  pinMode(EchoPin, INPUT); // Echo is an input pin

  pinMode(green, OUTPUT); //Set the green led Pin
  pinMode(red, OUTPUT);  //Set the red led Pin

  Serial.begin(9600); // Serial Output

  lcd.begin(16, 2); //lcd Set up

  if (! rtc.begin()){  //if RTC can't begin, LCD lOG.
    lcd.print("Couldn't find RTC");
    while (1);
  }
  
  if (! rtc.isrunning()){  //if RTC isn't running, LCD lOG.
    lcd.print("RTC is NOT running!");
  }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //auto update from computer time
  
    Serial.print("Initializing SD card..."); // initialize the SD card
    pinMode(chipSelect, OUTPUT);  // make sure that the default chip select pin is set to output, even if you don't use it:

    if (!SD.begin(chipSelect)) {  // see if the card is present and can be initialized:
      Serial.println("Card failed, or not present");
      return;  // don't do anything more:
      
    }
    Serial.println("card initialized.");

}

void loop() {
    digitalWrite(TriggerPin, LOW); // Trigger pin to LOW
    delayMicroseconds(2); // 2us high
    digitalWrite(TriggerPin, HIGH); // Trigger pin to HIGH
    delayMicroseconds(10); // 10us high
    digitalWrite(TriggerPin, LOW); // Trigger pin to HIGH
  
    Duration = pulseIn(EchoPin, HIGH); // Waits for the echo pin to get high, returns the Duration in microseconds
    Distance_m = Distance(Duration); // Use function to calculate the distance
    
    Serial.print("Distance = "); // Output to serial
    Serial.print(Distance_m);  // Output Distance to serial
    Serial.println(" m "); // Additional Output message

    DateTime now = rtc.now();  //For RTC Current time 

    lcd.setCursor(0, 1);  //Set LCD Monitor cursor
    lcd.print(now.hour());  //output the time(hour) to lcd
    lcd.print(':');
    lcd.print(now.minute());  //output the time(minute) to lcd
    lcd.print(':');
    lcd.print(now.second());  //output the time(second) to lcd
    lcd.print(" ");
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);  //output the time(Day of the week) to lcd
    lcd.print(",");
    lcd.print(now.day());  //output the time(day)to lcd
    lcd.print('/');
    lcd.print(now.month());  //output the time(month) to lcd
    //lcd.print('/');  //if year is need,
    //lcd.print(now.year());

    lcd.setCursor(0, 0);  //Set LCD Monitor cursor
    lcd.print("Distance = "); // Output to serial
    lcd.print(Distance_m);  // Output Distance to serial
    lcd.print(" m");
    
    Serial.print('"');  
    Serial.print(now.hour());  //output the time(hour)to setial
    Serial.print(':');
    Serial.print(now.minute());  //output the time(minute) to serial
    Serial.print(':');
    Serial.print(now.second());  //output the time(second) to serial
    Serial.print("  ");

    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);  //output the time(the day of week) to serial
    Serial.print(" ,");
    Serial.print(now.day());  //output the time(day)to serial
    Serial.print('/');
    Serial.print(now.month());  //output the time(month) to serial
    Serial.print('/');
    Serial.print(now.year());  //output the time(year) to serial
    Serial.println('"');     
 
  if(0 < Distance_m && Distance_m< WD){  //if the distance is closer than the warning distance, 
       Serial.println("green off, red on"); 
       digitalWrite(green,LOW);  //green LED off       
       digitalWrite(red,HIGH);   //red LED on
    }
  else if(WD <= Distance_m && Distance_m <= 1){  //if the distance is between warning distance an 1 Meter
       Serial.println("green on, red off"); 
       digitalWrite(green,HIGH);  //green LED on       
       digitalWrite(red,LOW);  //red LED off  
    
    char filename[] = "LOGGER00.txt";  // create a new file
    for (uint8_t i = 0; i < 100; i++) {  //check the file name
      filename[6] = i/10 + '0';
      filename[7] = i%10 + '0';
      if (! SD.exists(filename)) { // only open a new file if it doesn't exist
        logfile = SD.open(filename, FILE_WRITE);  //file open
        break;  // leave the loop!
      }
    }
   /* 
   if (! logfile) {
      error("couldnt create file");
   }*/

    logfile.print('"');
    logfile.print("Distance = "); 
    logfile.print(Distance_m);  //output distance to logfile
    logfile.print(" m / ");
    logfile.print(now.hour()); //output hour to logfile
    logfile.print(':');
    logfile.print(now.minute()); //output minute to logfile
    logfile.print(':');
    logfile.print(now.second());  //output minute to logfile
    logfile.print("  ");

    logfile.print(daysOfTheWeek[now.dayOfTheWeek()]);  //output the day of the week to logfile
    logfile.print(" ,");
    logfile.print(now.day()); //output the day to logfile
    logfile.print('/');
    logfile.print(now.month());  //output the month to logfile
    logfile.print('/');
    logfile.print(now.year());  //output the year to logfile
    logfile.println('"');   

    Serial.print("Logging to: ");  //output message to serial
    Serial.println(filename);

    logfile.close();  //close the logfile
    delay(10000);  //Wait 
      
 }
  else{       
       Serial.println("green off, red off"); 
       digitalWrite(red,LOW); 
       digitalWrite(green,LOW);
      } 
      
  delay(1000); // Wait to do next measurement
}

float Distance(float time){ // Calculates the Distance in m
  // ((time)*(Speed of sound))/ toward and backward of object) * 10
  float DistanceCalc; // Calculation variable
  DistanceCalc = ((time / 2.9) / 2)/1000; // Actual calculation in m
  return DistanceCalc; // return calculated value
}
