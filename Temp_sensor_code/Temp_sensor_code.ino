/***************************************
Author:   Loana Grom
Email:    loana.grom@studium.uni-hamburg.de
Date:     2023-03-12

This skript uses an Arduino together with a DS18B20 temperature
sensor and pure OneWire based communication.
****************************************/

// Include libraries 
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include <OneWire.h>
#include "ds18b20.h"

// User-defined constants
const String logfile = "tsensor.log";
float temp_sum = 0;
int count = 1;
String header = "# timestamp, millis, sensor_id, temperature in °C with 2 digits (spot, avg)";

// Set RTC and 1-Wire bus to respecting components on Arduino
RTC_DS1307 rtc;
OneWire ow(4);


void setup(){

  // Set baud rate
  Serial.begin(9600);

  //initialize RTC and SD modules and check if they are running
  if (!rtc.begin()){  
    Serial.println("RTC is NOT running. Let's see the time now!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    }
    //When time needs to be set on a new device, or afer a power loss, the following
    //line sets the RTC to the date & time this sketch was completed:
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

  if (!SD.begin(10)) {
    Serial.println("SD module initialization failed or Card is not present");  
    return;
  }

  // Print header to terminal and SD card
  printOutputln(header);
 }


void loop(){

  // Create variables for ROM code and scratchpad data
  byte rom_code[8]; 
  byte sp_data[9];

  // Start sequence to read out the ROM code
  ow.reset();
  ow.write(READ_ROM);
  for (int i=0; i<8; i++) {
    rom_code[i] = ow.read();
  }
  //Check, if the sensor actually is a DS18B20 sensor
  if (rom_code[0] != IS_DS18B20_SENSOR) {
    Serial.println("Sensor is not a DS18B20 sensor!");
  }
  //Store the serial number
  String registration_number;
  for (int i=1; i<7; i++) {
    registration_number += String(rom_code[i], HEX);
  }

  
  // Start sequence for converting temperatures
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(CONVERT_T);


  // Start sequence for reading the data from the scratchpad:
  ow.reset();
  ow.write(SKIP_ROM);
  ow.write(READ_SCRATCHPAD);
  for (int i=0; i<9; i++){
    sp_data[i] = ow.read();
  }
  //Store the temperature information with 8 bit shift as 2-digit temperature information
  int16_t tempRead = sp_data[1] << 8 |sp_data[0];
  float tempCelsius = tempRead / 16.0;

  // Store and write average and spot measurements every 10s to terminal and SD card 
  temp_sum += tempCelsius; 
  if (count%10==0){
    // Calculate average temperature measurement over 10s
    float temp_avg = temp_sum/10;
    
    //Print timestamp, sensor ID, temperature (spot,avg)
    printOutput(String(getISOtime()));
    printOutput(String(", "));
    printOutput(String(millis()));
    printOutput(String(", "));
    printOutput(String(registration_number));
    printOutput(String(", ")); 
    printOutput(String(tempCelsius));
    printOutput(String(", "));
    printOutputln(String(temp_avg));
    temp_sum = 0;

  }
  //Set measurement time interval to 1 second
  delay(((int(millis()/1000.0 +1.0)*1000.0))-millis()-1);
  
  count+=1;

  
  
}
