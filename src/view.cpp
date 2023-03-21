#include "view.h"

uint32_t startTime, frame = 0; // For frames-per-second estimate
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

void init_m5() {
  M5.begin(true, true, true, true);
  tft.begin();
  tft.setRotation(1);
  M5.Axp.SetLcdVoltage(2800);
  M5.Axp.SetLcdVoltage(3300);
  M5.Axp.SetBusPowerMode(0);
  M5.Axp.SetCHGCurrent(AXP192::kCHG_190mA);
  M5.Axp.SetLDOEnable(3, true);
  delay(150);
  M5.Axp.SetLDOEnable(3, false);
  M5.Axp.SetLed(1);
  delay(100);
  M5.Axp.SetLed(0);
  M5.Axp.SetLDOVoltage(3, 3300);
  M5.Axp.SetLed(1);
  startTime = millis();
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver,
                      lv_indev_data_t * data){
  TouchPoint_t pos = M5.Touch.getPressPoint();
  bool touched = ( pos.x == -1 ) ? false : true;
  if(!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR; 
    /*Set the coordinates*/
    data->point.x = pos.x;
    data->point.y = pos.y;
  }
  return false; 
//Return `false` because we are not buffering and no more data to read
}


void my_disp_flush(lv_disp_drv_t *disp, 
     const lv_area_t *area, lv_color_t *color_p){
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

void init_display() {
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  lv_init();
  
  
  //-------------------------------------------------------------------
  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res  = 320;
  disp_drv.ver_res  = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer   = &disp_buf;
  lv_disp_drv_register(&disp_drv);
  
  //-------------------------------------------------------------------
  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

lv_obj_t * add_label(const char * text, lv_coord_t x_ofs, lv_coord_t y_ofs) {
  lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(label, text);
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
  lv_label_set_long_mode(label, LV_LABEL_LONG_EXPAND); 

  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, x_ofs, y_ofs);

  return label;
}


// https://docs.lvgl.io/7.11/widgets/btn.html#overview
lv_obj_t * add_button(const char * text, lv_event_cb_t event_cb, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h) {
    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_cb);

    lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_width(btn1, w);
    lv_obj_set_height(btn1, h);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, text);

    return btn1;
}


lv_obj_t * show_message_box(const char * text, const char * ok_button_text, const char * no_button_text, lv_event_cb_t event)
{
    static const char * btns[] ={ok_button_text, no_button_text, ""};

    lv_obj_t * mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, text);
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 200);
    lv_obj_set_event_cb(mbox1, event);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); 

    return mbox1;
}

void close_message_box(lv_obj_t * msgbox) {
  lv_msgbox_start_auto_close(msgbox, 10);
}


lv_obj_t * show_message_box_no_buttons(const char * text)
{
    lv_obj_t * mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, text);
    lv_obj_set_width(mbox1, 200);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); 

    return mbox1;
}

// https://docs.lvgl.io/7.11/widgets/led.html
lv_obj_t * add_led(lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h)
{
  lv_obj_t * led1  = lv_led_create(lv_scr_act(), NULL);
  lv_obj_align(led1, NULL, LV_ALIGN_IN_TOP_LEFT, x_ofs, y_ofs);
  lv_obj_set_width(led1, w);
  lv_obj_set_height(led1, h);
  lv_led_off(led1);

  

  return led1;
}

