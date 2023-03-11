# Temperature measuring device
***
This repository contains Arduino code for operating a temperature measuring device built in the block course "Practical Measurement Electronics and Interfaces in Ocean Sciences" of the Master's programme Ocean and Climate Physics by Niels Fuchs and Markus Ritschel at the Universität Hamburg. Measurements of the devices of all participants for calibration and time constant determination as well as a report that serves as a data sheet for the devices are included.

## General Info
***
The temperature measuring device measures temperatures every ten seconds as spot and average measurements in a 2-digit form. An Arduino is used for operating the device. The code has to be uploaded to the device via the Arduino IDE program. When uplodaed, the device starts measuring the temperatures automatically after power up. How to wire the Arduino and the temperature sensor is described in the report.
Calibration and time constant measurement time series were taken in the ice laboratory of the Max-Planck-Insitute for Meteorology in Hamburg. The files are  included in the repository in the folder temperature_measurements. Further, a jupyter notebook file is included for the evaluation of the calibration and time constant of the temperature measuring devices as well as interpolating all sensors to the same time axis.

## Set up
***
This repository has be be cloned on your local machine with the following command:
```bash
git clone https://
```
Connect Arduino and temperature sensor DS18B20 as decribed in the report and load the code onto the Arduino.


## Contact
***
Contact for questions: loana.grom@studium.uni-hamburg.de
