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

#include "ds18b20.h"


// User-defined constants
const String logfile = "tsensor.log";


RTC_DS1307 rtc;    //communication with the clock: always use rtc in the following
OneWire ow(4);     //ow for one wire, in brackets: pin -> bus number of the pin the arduinos digital communication lays on

void setup(){
  Serial.begin(9600);

  //initialize modules and check if they are running, with if statement is checked if the RTC is not running
  if (!rtc.begin()){  //initializes the RTC
    Serial.println("RTC is NOT running. Let's see the time now!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));   //if the time is not running, take the time and date of the computer
    }

    //sometimes clock will be off, then clock has be be adjusted:

    //WHen time needs to be sot on a new device, or ater a power loss, the following
    //line sets the RTC to the date & time this sketch was completed (!)

    //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

  if (!SD.begin(10)) {
    Serial.println("SD module initialization failed or Card is not present");  //if the SD card at bin 10 is not present
    return; //breaks the routine
  }
  }

void loop(){
  byte rom_code[8]; // create an array containing 8 elements of type byte for the rom code; bits in byte always sorted form LSB to MSB
  byte sp_data[9]; // scratchpad data of 9 bits

  // Start sequence to read out the rom code (sensor family on LSB, then 48-bit registration number):
  ow.reset();
  ow.write(READ_ROM);
  // now loop over the bytes: from i=0 as long as i is smaller than 8, i++ means that i always increases for one step
  for (int i=0; i<8; i++) {
    rom_code[i] = ow.read();
  }

  delay(1000);
  //Check, if the sensor actually is a DS18B20 sensor -> the first bit in the rom_code has to be 0x28!
  if (rom_code[0] != IS_DS18B20_SENSOR) {
    Serial.println("Sensor is not a DS18B20 sensor!");
    //return //quits the code, if the sensor is not a DS18B20 sensor buuut would be better to remove return if one sensor is not this sensor
  }
  //store next 6 bits into a string for the serial number
  String registration_number;  //declare a variable as registration number, start at second least significant bit (1. position, not 0.)
  for (int i=1; i<7; i++) {
    registration_number += String(rom_code[i], HEX);  //store the serial number of 48bits into hex code
  }

  
  // Start first sequence (for converting temperatures):
  ow.reset();  //resets the 1-wire bus
  ow.write(SKIP_ROM);
  ow.write(CONVERT_T); //tells the sensor to convert its detected value into temp in °C, it doesn't send us the data now, it is stored in the scratchpad
  //to do: make things more readable for us: at beginning of file define placeholder the same name
  // as in datasheet e.g. #define CONVERT_T 0x44, but with many variables it is useful to outsource
  // the variables into another file


  // start sequence for reading the data from the scratchpad:
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(READ_SCRATCHPAD);
  for (int i=0; i<9; i++){
    sp_data[i] = ow.read();   //now we have an array with all the information of the scratchpad in form of bytes
  }
  //Store the temperature information in a 16bit number of 0 and 1ns
  int16_t tempRead = sp_data[1] << 8 |sp_data[0];  //create the correct temperature value (8bit shift), store in 16bit integer, in 8bit: every bit is a integer

  //until now, the numbers after the comma are not included! 12bit resolution with 4 decimals behind the comma: devide by 2^4 (4 commas)
  //now convert the number into float temperature value

  float tempCelsius = tempRead / 16.0;  //divide by 2^4 = 16 for 4 digits after the comma 


  //Print timestamp, sensor ID, temperature (spot)
  printOutput(String(getISOtime()));
  printOutput(String(", "));
  printOutput(String(millis()));
  printOutput(String(", "));
  printOutput(String(registration_number));
  printOutput(String(", ")); 
  printOutputln(String(tempCelsius));
  

  
  
}
