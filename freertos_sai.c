/*
 * freertos_i2s.c
 *
 *  Created on: Nov 23, 2020
 *      Author: kevin
 */

#include "freertos_sai.h"

void i2s_init() {
    sai_config_t tx_default_config;
    SAI_TxGetDefaultConfig(&tx_default_config);
    SAI_TxInit(I2S0, tx_default_config);

    CLOCK_EnableClock(kCLOCK_PortE);
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinMux(PORTC, 1, kPORT_MuxAlt6); // TXD0
    PORT_SetPinMux(PORTE, 9, kPORT_MuxAlt2); // TXD1
    PORT_SetPinMux(PORTC, 6, kPORT_MuxAlt6); // MCLK
    PORT_SetPinMux(PORTE, 11, kPORT_MuxAlt4); // TX_FS
    PORT_SetPinMux(PORTE, 12, kPORT_MuxAlt4); // TX_BCLK

    SAI_Init(I2S0);
    SAI_TxEnable(I2S0, true);
    SAI_TxSetBitClockDirection(I2S0, kSAI_Master);
    // kCLOCK_Sai0 check mclk para ver la frec en la kinetis board_bootclockhsrun_core__clock 180M
    SAI_TxSetBitClockRate(I2S0, CLOCK_GetFreq(I2S0_CLK_SRC), 8000, 16, 1);

    // sai_tranceiver_t tiene la configuracion comun, a lo mejor no necesito configurar el clockrate

    sai_transceiver_t transact_config;
    SAI_TxSetConfig(I2C0, transact_config);

    /* sar dma ejemplo sai_emda_reconrd_playback el otro ejemplo es sai_interrupt.
     * Si usamos modo master en el bit 6 ms del audio format seleccionar el modo day y ads pin igual.
     * Yo quiero. Poner atencionen la parte dle synchronouse mode para ver como se configurta. Ver el caso del bit clk se transimte o recive.
     * */
}
