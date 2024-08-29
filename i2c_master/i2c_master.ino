#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.setWireTimeout(2000, true);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    char buffer[20];
    Serial.readStringUntil('\n').toCharArray(buffer, 20);
    Serial.print("Echo: ");
    Serial.println(buffer);

    char* command = strtok(buffer, " \n");

    uint8_t addr = parse(strtok(NULL, " \n"), 16);
    uint8_t reg = parse(strtok(NULL, " \n"), 10);
    uint8_t data = parse(strtok(NULL, " \n"), 16);

    if (strcmp(command, "r") == 0) {
      uint8_t val = read_register(addr, reg);
      if (val != NULL) {
        Serial.println(val, HEX);
      }
    }

    else if (strcmp(command, "w") == 0) {
      write_register(addr, reg, data);
    }
  }
}

//Returns the integer value represented by the string.
int parse(char* str, int base) {
  return (int)strtol(str, NULL, base);
}

uint8_t read_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr >> 1);
  Wire.write(reg);
  uint8_t error = Wire.endTransmission();
  
  // check if register address was written successfully
  if (error) {
    Serial.println("ERROR - I2C failed to read");
    return NULL;
  }

  uint8_t len = Wire.requestFrom(addr >> 1, 1);

  // check if register was read successfully
  if (len == 0) {
    Serial.println("ERROR - I2C failed to read");
    return NULL;
  }
  return Wire.read();
}

void write_register(uint8_t addr, uint8_t reg, uint8_t data) {
  Wire.beginTransmission(addr >> 1);
  Wire.write(reg);
  Wire.write(data);
  uint8_t error = Wire.endTransmission();

  // check if data was successfully 
  if (error) {
    Serial.println("ERROR - I2C failed to write");
  } else {
    Serial.println("I2C write successful");
  }
}