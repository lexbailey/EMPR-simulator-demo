#ifndef LCD_INCLUDE
#define LCD_INCLUDE

#include <LPC17xx.h>
#include <lpc17xx_i2c.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "i2c_helper.h"


void init_lcd();
void reset_adr_lcd(int offset);
void lcd_set_cursor_pos(int pos);
void write_char_lcd(I2C_M_SETUP_Type *transfer_cfg, const char c);
void write_char_lcd_raw(I2C_M_SETUP_Type *transfer_cfg, const char c);
void write_str_lcd(const char *str, int pos);
void update_lcd_power_flags();
void lcd_cursor_on();
void lcd_cursor_off();
void lcd_off();
void lcd_on();
void write_str_lcd_instant(const char *str, int pos);
void clear_lcd();
int lcd_printf(int pos, char *fmt, ...);



#endif /* LCD_INCLUDE */
