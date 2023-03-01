/***************************************
Author:   Loana Grom
Email:    loana.grom@gmx.de
Date:     2023-02-13

This skript uses an Arduino together with a DS18B20 temperature
sensor and pure OneWire based communication.
****************************************/


#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include <OneWire.h>

RTC_DS1307 rtc;    //communication with the clock: always use rtc in the following
OneWire ow(4);     //ow for one wire, in brackets: pin -> bus number of the pin the arduinos digital communication lays on

void setup(){
  Serial.begin(9600);

  //initialize modules and check if they are running, with if statement is checked if the RTC is not running
  if (!rtc.begin()){  //initializes the RTC
    Serial.println("RTC is NOT running. Let's see the time now!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));   //if the time is not running, take the time and date of the computer
    }

  if (!SD.begin(10)) {
    Serial.println("SD module initialization failed or Card is not present");  //if the SD card at bin 10 is not present
    return; //breaks the routine
  }
  }

void loop(){
}
