#include <lpc17xx_pinsel.h>

void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg){
    test_socket_send("PINSEL.CONFIGPIN ");
    char n_buf[99];

    snprintf(n_buf, 99, "%d %d %d %d %d\n", PinCfg->Portnum, PinCfg->Pinnum, PinCfg->Pinmode, PinCfg->Funcnum, PinCfg->OpenDrain);
    test_socket_send(n_buf);
    free(test_socket_recv());
}
