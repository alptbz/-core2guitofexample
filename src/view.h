#ifndef VIEW_H_   /* Include guard */
#define VIEW_H_

#include <M5Core2.h>
#include <Arduino.h>
#include <lvgl.h>

void init_m5();

void init_display();

lv_obj_t * add_label(const char * text, lv_coord_t x_ofs, lv_coord_t y_ofs);

lv_obj_t * add_button(const char * text, lv_event_cb_t event_cb, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h);

lv_obj_t * show_message_box(const char * text, const char * ok_button_text, const char * no_button_text, lv_event_cb_t event);

void close_message_box(lv_obj_t * msgbox);

lv_obj_t * show_message_box_no_buttons(const char * text);

lv_obj_t * add_led(lv_coord_t x_ofs, lv_coord_t y_ofs, lv_coord_t w, lv_coord_t h);

#endif // BASE_H