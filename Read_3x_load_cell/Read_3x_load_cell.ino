#include <HX711_ADC.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Pins for 3 HX711 modules
const int HX711_dout_1 = 32; 
const int HX711_sck_1  = 33; 
const int HX711_dout_2 = 25; 
const int HX711_sck_2  = 26; 
const int HX711_dout_3 = 27; 
const int HX711_sck_3  = 14; 

// HX711 objects
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); 
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); 

// EEPROM addresses for calibration values
const int calVal_eepromAdress_1 = 0; 
const int calVal_eepromAdress_2 = 4; 
const int calVal_eepromAdress_3 = 8; 

unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  float calibrationValue_1; 
  float calibrationValue_2; 
  float calibrationValue_3; 

  // Set your calibration factors (replace with actual values after calibration)
  calibrationValue_1 = -412.10; 
  calibrationValue_2 = 719.47; 
  calibrationValue_3 = -389.26; 

  LoadCell_1.begin();
  LoadCell_2.begin();
  LoadCell_3.begin();
  
  unsigned long stabilizingtime = 2000; // ms
  boolean _tare = true; 

  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  byte loadcell_3_rdy = 0;

  // Run startup & tare for all 3 load cells
  while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy) < 3) {
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
  }

  if (LoadCell_1.getTareTimeoutFlag()) Serial.println("Timeout, check wiring HX711 #1");
  if (LoadCell_2.getTareTimeoutFlag()) Serial.println("Timeout, check wiring HX711 #2");
  if (LoadCell_3.getTareTimeoutFlag()) Serial.println("Timeout, check wiring HX711 #3");

  // Set calibration factors
  LoadCell_1.setCalFactor(calibrationValue_1); 
  LoadCell_2.setCalFactor(calibrationValue_2); 
  LoadCell_3.setCalFactor(calibrationValue_3); 

  Serial.println("Startup complete, all load cells ready.");
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; // ms (adjust to slow down printing)

  // Update load cells
  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();
  LoadCell_3.update();

  // Print values
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      float c = LoadCell_3.getData();

      Serial.print("Load_cell 1: "); Serial.print(a);
      Serial.print("    Load_cell 2: "); Serial.print(b);
      Serial.print("    Load_cell 3: "); Serial.println(c);

      newDataReady = 0;
      t = millis();
    }
  }

  // Tare all load cells if user sends 't'
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
      LoadCell_3.tareNoDelay();
    }
  }

  // Tare completion messages
  if (LoadCell_1.getTareStatus()) Serial.println("Tare load cell 1 complete");
  if (LoadCell_2.getTareStatus()) Serial.println("Tare load cell 2 complete");
  if (LoadCell_3.getTareStatus()) Serial.println("Tare load cell 3 complete");
}
