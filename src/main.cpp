#include <Arduino.h>
#include <Sensirion.h>
#include <ArduinoJson.h>

#define LED 13

// 19121 test

// Sensirion(dataPin, sclkPin, address, noholdmaster);
// SHT1x address = 0x00  SHT2x address = 0x40  SHT3x address = 0x44 or 0x45
Sensirion sht75 = Sensirion(9, 10);


float sht75_Temp;
float sht75_RH;
float dewpoint;
float humidity35oC;
int ret, mret=1234;
long mtime;

const int capacity = JSON_OBJECT_SIZE(9);

unsigned long delayTime;

void logError(int error) {
  if (error>=0) // No error
    return;
    
  Serial.println();
  Serial.print("Error: ");
  Serial.print(error);

  switch (error) 
  {
    case S_Err_Param:
      Serial.print(" - Parameter error in function call! ");
      break;
    case S_Err_NoACK:
      Serial.print(" - No response (ACK) from sensor! ");
      break;
    case S_Err_CRC:
      Serial.print(" - CRC mismatch! ");
      break;
    case S_Err_TO:
      Serial.print(" - Measurement timeout! ");
      break;
    default:
      Serial.print(" - Unknown error received! ");
      break;
  }
}

void setup() {
  
  pinMode(LED, OUTPUT);  

  Serial.begin(9600);
  delay(1000);
  Serial.println("Start SHT75 ");

  delayTime = 1000;
}

void loop() {
  StaticJsonDocument<capacity> jsonDoc;
  digitalWrite(LED,(((millis()>>6)&0x5)==0));   // double flash led rapidly

  // all parameters are optionals if user don't need it
  logError(ret = sht75.measure(&sht75_Temp, &sht75_RH, &dewpoint, 37, &humidity35oC)); // Measurement
  
  if (ret == S_Meas_Rdy) // A new measurement is available
  {    
    // Serial.println();
    // Serial.print("Temperature = ");   
    // Serial.print(sht75_Temp);
    // Serial.print(" oC, Humidity = ");  
    // Serial.print(sht75_RH);
    // Serial.print(" %, Dewpoint = ");  
    // Serial.print(dewpoint);
    // Serial.print(" oC, Humidity @37oC = ");
    // Serial.print(humidity35oC);
    // Serial.print(" % ");  

    jsonDoc["Temperature_SHT75"] = sht75_Temp;
    jsonDoc["RH_SHT75"] = sht75_RH;

    serializeJson(jsonDoc, Serial);
    Serial.println();
  }
  // Serial.println(ret);

  mret = ret;
}