#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

const int HX711_dout_1 = 32; 
const int HX711_sck_1  = 33; 
const int HX711_dout_2 = 25; 
const int HX711_sck_2  = 26; 

HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); 

const int calVal_eepromAdress_1 = 0; 
const int calVal_eepromAdress_2 = 4; 
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  float calibrationValue_1; 
  float calibrationValue_2; 

  calibrationValue_1 = -412.10; 
  calibrationValue_2 = 719.47; 
#if defined(ESP8266) || defined(ESP32)
#endif
  
  LoadCell_1.begin();
  LoadCell_2.begin();
  
  unsigned long stabilizingtime = 2000; 
  boolean _tare = true; 
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  while ((loadcell_1_rdy + loadcell_2_rdy) < 2) {
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  LoadCell_1.setCalFactor(calibrationValue_1); 
  LoadCell_2.setCalFactor(calibrationValue_2); 
  Serial.println("Startup is complete");
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; 

  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();

  if ((newDataReady)) {
    if (millis() > t + serialPrintInterval) {
      float a = LoadCell_1.getData();
      float b = LoadCell_2.getData();
      Serial.print("Load_cell 1 output val: ");
      Serial.print(a);
      Serial.print("    Load_cell 2 output val: ");
      Serial.println(b);
      newDataReady = 0;
      t = millis();
    }
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
    }
  }

  if (LoadCell_1.getTareStatus() == true) {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_2.getTareStatus() == true) {
    Serial.println("Tare load cell 2 complete");
  }

}
