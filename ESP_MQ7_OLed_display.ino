#include <Arduino.h> 
#include "Wire.h"
#include "SSD1306.h" // legacy include: `#include "SSD1306.h"`
 
 
// Initialize the OLED display using Wire library
SSD1306 display(0x3c, 5, 4);  //18=SDK  19=SCK  As per labeling on ESP32 DevKit

double avgSensorPercent = 0; 
uint loopCounter = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Initializing OLED Display");

  display.init();
  display.clear();
  display.flipScreenVertically();
  delay(2.5*1000);
}
/* merge uploadat cu wemo D1 R1 pt kitul fara oled*/
/* merge uploadat cu SparkFun ESP32pt kitul CU oled
    vezi si https://www.instructables.com/id/ESP32-With-Display-Oled-Progress-Bar/
   merge MANUAL cu ESP32 Thing, daca tin apasat "boot" la scriere
*/

void loop() 
{
  double sensorPercent; 
  int sensorValue;
 
  sensorValue = analogRead(0);
  sensorPercent = 100*(double)sensorValue/4095;

  if (loopCounter==0) {
    // avgSensorPercent = sensorPercent;
    avgSensorPercent = 0;
  }

  avgSensorPercent = (29*avgSensorPercent + sensorPercent)/30;


  Serial.print(avgSensorPercent);
  Serial.println("");


  display.clear();
  // create more fonts at http://oleddisplay.squix.ch/
  
  if (avgSensorPercent>49.9) {
    display.invertDisplay();
  }
  else if (avgSensorPercent>39.9) {
    display.normalDisplay();
    display.setBrightness(100);
  }
  else {
    display.normalDisplay();
    display.setBrightness(1);
  }


  Serial.print(loopCounter);
  Serial.println("");


  
  char s1[10];
  dtostrf(avgSensorPercent, 5, 1, s1);

  // display the large, main value
  char s2[20];
  sprintf(s2, "%s%%", s1);
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 16, s2);

  // display the upper small values
  char s3[10];
  dtostrf(sensorPercent, 5, 3, s3);
  char s4[30];
  sprintf(s4, "%s, %s%%", String(sensorValue), s3);
  
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, s4);
  
  // write the buffer to the display
  display.display();

  loopCounter++;

  if (abs(avgSensorPercent-sensorPercent)>3.5) {
    // adapt the values faster when there are big differences
    delay(0.1*1000);
  }
  else if (abs(avgSensorPercent-sensorPercent)>1.25) {
    // adapt the values faster when there are big differences
    delay(0.5*1000);
  }
  else {
    delay(1.0*1000);
  }
}
