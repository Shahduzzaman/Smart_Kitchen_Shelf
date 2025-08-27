#define BLYNK_TEMPLATE_ID   "TMPL6XEybgOny"
#define BLYNK_TEMPLATE_NAME "Smart Kitchen Shelf"
#define BLYNK_AUTH_TOKEN    "5JwB2c5uMV_s0WmHNYrf91dN-_fukqah"

#include <HX711_ADC.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif


char ssid[] = "xenos";
char pass[] = "01912390015";

const int HX711_dout_1 = 32; 
const int HX711_sck_1  = 33; 
const int HX711_dout_2 = 25; 
const int HX711_sck_2  = 26; 
const int HX711_dout_3 = 27; 
const int HX711_sck_3  = 14; 


HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); 
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); 

float calibrationValue_1 = -412.10;
float calibrationValue_2 = 719.47;
float calibrationValue_3 = -389.26;

unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting Smart Kitchen Shelf...");


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);


  LoadCell_1.begin();
  LoadCell_2.begin();
  LoadCell_3.begin();
  
  unsigned long stabilizingtime = 2000; 
  boolean _tare = true; 

  byte lc1_rdy = 0, lc2_rdy = 0, lc3_rdy = 0;

  while ((lc1_rdy + lc2_rdy + lc3_rdy) < 3) {
    if (!lc1_rdy) lc1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!lc2_rdy) lc2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!lc3_rdy) lc3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
  }

  if (LoadCell_1.getTareTimeoutFlag()) Serial.println("Timeout HX711 #1");
  if (LoadCell_2.getTareTimeoutFlag()) Serial.println("Timeout HX711 #2");
  if (LoadCell_3.getTareTimeoutFlag()) Serial.println("Timeout HX711 #3");

  LoadCell_1.setCalFactor(calibrationValue_1); 
  LoadCell_2.setCalFactor(calibrationValue_2); 
  LoadCell_3.setCalFactor(calibrationValue_3); 

  Serial.println("Startup complete, all load cells ready.");
}

void loop() {
  Blynk.run();

  static boolean newDataReady = 0;
  const int sendInterval = 1000;


  if (LoadCell_1.update()) newDataReady = true;
  if (LoadCell_2.update()) newDataReady = true;
  if (LoadCell_3.update()) newDataReady = true;

  if (newDataReady) {
    if (millis() > t + sendInterval) {
      float val1 = LoadCell_1.getData();
      float val2 = LoadCell_2.getData();
      float val3 = LoadCell_3.getData();

      Serial.print("LC1: "); Serial.print(val1);
      Serial.print("   LC2: "); Serial.print(val2);
      Serial.print("   LC3: "); Serial.println(val3);
      Serial.println();

      Blynk.virtualWrite(V0, val1);
      Blynk.virtualWrite(V1, val2);
      Blynk.virtualWrite(V2, val3);

      newDataReady = 0;
      t = millis();
    }
  }


  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
      LoadCell_3.tareNoDelay();
    }
  }

  if (LoadCell_1.getTareStatus()) Serial.println("Tare LC1 complete");
  if (LoadCell_2.getTareStatus()) Serial.println("Tare LC2 complete");
  if (LoadCell_3.getTareStatus()) Serial.println("Tare LC3 complete");
}
