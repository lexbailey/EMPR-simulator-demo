#include "lcd.h"

const int LCD_ADDR = 0x3B;

void init_lcd(){
    unsigned char init_display[] = {0x00, 0x34, 0x0c, 0x06, 0x35, 0x04, 0x10, 0x42, 0x9f, 0x34, 0x02};
    do_transfer(init_display, 11, LCD_ADDR);
}

int convert_ascii(int val){
    // General punctuation, digits, uppercase and lowercase alphabet.
    if ((val >= 32 && val <= 63) || (val >= 65 && val <= 90) || (val >= 97 && val <= 122)){
        return (val + 128);
    }
    // @ symbol
    if (val == 64){
        return 128;
    }
    // Anything else returns empty square.
    else {
        return 49;
    }
}

void reset_adr_lcd(int offset){
    unsigned char reset_display[] = {0x00, 0x80+offset};
    do_transfer(reset_display, 2, LCD_ADDR);
}

void lcd_set_cursor_pos(int pos){
    if (pos >= 16){ reset_adr_lcd(0x40 - 16 + pos); }
    else { reset_adr_lcd(pos); }
}

void write_char_lcd(I2C_M_SETUP_Type *transfer_cfg, const char c){
    // Writes single char to LCD screen.
    unsigned char buffer[2];
    buffer[0] = 0x40;
    buffer[1] = convert_ascii(c);
    do_transfer_with_cfg(transfer_cfg, buffer, 2, LCD_ADDR);
}

void write_char_lcd_raw(I2C_M_SETUP_Type *transfer_cfg, const char c){
    // Writes a raw value to LCD screen.
    unsigned char buffer[2];
    buffer[0] = 0x40;
    buffer[1] = c;
    do_transfer_with_cfg(transfer_cfg, buffer, 2, LCD_ADDR);
}

void write_str_lcd(const char *str, int pos){
    // Function to write ASCII strng to LCD screen.
    I2C_M_SETUP_Type transfer_cfg = setup_transfer(NULL, 0);
    str --;
    reset_adr_lcd(pos);
    int count = pos;
    if (count >= 16){
        reset_adr_lcd(0x40 + (count-16));
    }
    int is_wide_char = 0;
    while (*++str != '\0'){
        count += 1;
        if (count >= 33){
            return;
        }
        if (is_wide_char){
            switch ((unsigned char) *str) {
                case 163: write_char_lcd_raw(&transfer_cfg, 0x81); break;
                default: write_char_lcd(&transfer_cfg, '?');
            }
            is_wide_char = 0;
            continue;
        }
        if (*str == 194){
            is_wide_char = 1;
            continue;
        }
        // Need to reset lcd addr to be line 2
        int is_newline = *str == '\n';
        if (!is_newline){
            write_char_lcd(&transfer_cfg, *str);
        }
        if (is_newline || (count == 16)){
            if (str[1] == '\0'){
                reset_adr_lcd(0x3F);
                return;
            }
            reset_adr_lcd(0x40);
            count = 16;
        }
        if (count == 32){
            reset_adr_lcd(0x00);
        }
    }
}

const int LCD_CURSOR = 0x03;
const int LCD_POWER = 0x04;

int lcd_power_flags = 0;

void update_lcd_power_flags(){
    unsigned char display_data[2];
    display_data[0] = 0;
    display_data[1] = 0x08 | lcd_power_flags;
    do_transfer(display_data, 2, LCD_ADDR);
}

void lcd_cursor_on(){
    lcd_power_flags |= LCD_CURSOR;
    update_lcd_power_flags();
}

void lcd_cursor_off(){
    lcd_power_flags &= ~LCD_CURSOR;
    update_lcd_power_flags();
}

void lcd_off(){
    lcd_power_flags &= ~LCD_POWER;
    update_lcd_power_flags();
}

void lcd_on(){
    lcd_power_flags |= LCD_POWER;
    update_lcd_power_flags();
}

void write_str_lcd_instant(const char *str, int pos){
    lcd_off();
    write_str_lcd(str, pos);
    lcd_on();
}

void clear_lcd(){
    const char empty[33] = {[0 ... 31] = ' ', '\0'};
    write_str_lcd_instant(empty, 0);
}

int lcd_printf(int pos, char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    char buf[33];
    int len = vsnprintf(buf, 33, fmt, args);
    write_str_lcd(buf, pos);
    va_end(args);
    return len;
}
