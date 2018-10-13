#include <LPC17xx.h>
#include "lcd.h"

void init_system(){
// system init code, specific to target
#ifdef TESTING
    // For now this is just a hello world and so we only need
    // to do something specific if we are using the simulator.
    // This function initialises the connection to the sim.
    // Maybe some kind of LD_PRELOAD trick could remove the need for thsis?
    init_test_env();
#endif
}

int main(){
    init_system();
    init_i2c_pins();
    select_bus(LPC_I2C1);
    init_i2c();
    init_i2c();
    init_lcd();
    clear_lcd();
    lcd_printf(0, "Hello World");
    while(1){}
    return 0;
}
