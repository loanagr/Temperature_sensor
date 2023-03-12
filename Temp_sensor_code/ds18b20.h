/*
 * Author:   Loana Grom
 * Email:    loana.grom@studium.uni-hamburg.de
 * Date:     2023-03-12
 * 
 * This file holds all the required HEX commands for working with the DS18B20 sensor
 */

#define CONVERT_T 0x44
#define READ_ROM 0x33
#define SKIP_ROM 0xCC
#define READ_SCRATCHPAD 0xBE

#define IS_DS18B20_SENSOR 0x28
