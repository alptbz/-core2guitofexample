//=====================================================================
// LVGL : How-to
//      : M5Core2 slow fps. Scrol slider breaks when moving horizontally
// 2 Dec,2020
// https://forum.lvgl.io
//  /t/m5core2-slow-fps-scrol-slider-breaks-when-moving-horizontally/3931
// Arduino IDE 1.8.15
// https://github.com/mhaberler/m5core2-lvgl-demo
// Check : 2021.06.13 : macsbug
// https://macsbug.wordpress.com/2021/06/18/how-to-run-lvgl-on-m5stack-esp32/
//=====================================================================

#include <Arduino.h>
#include "view.h"
#include "networking.h"
#include "sideled.h"
#include <Wire.h>
#include <SPI.h>
#include <VL53L0X.h>

lv_obj_t * labelDesc;
lv_obj_t * labelValue;

VL53L0X sensor;

void init_gui_elements() {
  labelDesc = add_label("Distanz: ", 20, 20);
  labelValue = add_label("...", 100, 20);
}

void init_sensor() {
  Wire.begin();
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor.startContinuous();
}

void setup() {
  init_m5();
  init_display();
  Serial.begin(115200);
  // Uncomment the following lines to enable WiFi and MQTT
  //lv_obj_t * wifiConnectingBox = show_message_box_no_buttons("Connecting to WiFi...");
  //lv_task_handler();
  //delay(5);
  //setup_wifi();
  //mqtt_init(mqtt_callback);
  //close_message_box(wifiConnectingBox);
  init_gui_elements();
  init_sideled();
  init_sensor();

  set_sideled_state(0,10, SIDELED_STATE_ON);
}

long nextSensorRead = 0;
unsigned long next_lv_task = 0;
//=====================================================================
void loop(){
  if(next_lv_task < millis()) {
    lv_task_handler();
    next_lv_task = millis() + 5;
  }
 if(nextSensorRead < millis()) {
  unsigned long millimeter_distance = (unsigned long)sensor.readRangeContinuousMillimeters();
  
  lv_label_set_text(labelValue, (String(millimeter_distance, 10)+ " mm").c_str());

  
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  if(millimeter_distance > 500) millimeter_distance = 500;
  uint8_t color = (uint8_t)((millimeter_distance) / 500.0f * 254);
  
  set_sideled_color(0,10, CRGB(color,254-color,0));


  Serial.println();
  nextSensorRead = millis() + 150;
 }

}
//=====================================================================
