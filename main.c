/**
 * @file    t8.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "fsl_i2c.h"
#include "semphr.h"
#include "freertos_i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "WM8731.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_sai_edma.h"
#include "fsl_uart.h"
#include "string.h"
#include "filters.h"
/* TODO: insert other definitions and declarations here. */

/* SAI and I2C instance and clock */
#define DEMO_SAI              I2S0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_CLKSRC       kCLOCK_CoreSysClk
#define DEMO_SAI_CLK_FREQ     CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_SAI_IRQ          I2S0_Tx_IRQn
#define DEMO_SAITxIRQHandler  I2S0_Tx_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MCLK_OUTPUT  true
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_DMA             DMA0
#define DEMO_EDMA_CHANNEL    (0)
#define DEMO_DMAMUX          DMAMUX
#define DEMO_TX_EDMA_CHANNEL (0U)
#define DEMO_RX_EDMA_CHANNEL (1U)
#define DEMO_SAI_TX_SOURCE   kDmaRequestMux0I2S0Tx
#define DEMO_SAI_RX_SOURCE   kDmaRequestMux0I2S0Rx

#define DEMO_AUDIO_DATA_CHANNEL (1U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK 12288000U

#define BOARD_MASTER_CLOCK_CONFIG BOARD_MasterClockConfig
#define BUFFER_SIZE   (1024U)
#define BUFFER_NUMBER (2U) // 2 for TX 2 for RX

#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(SYS_CLK)
#define DEMO_UART          UART0

char* str;
char filter_data[3];

const char* ENABLE_LP = "LPE";
const char* ENABLE_HP = "HPE";
const char* ENABLE_BP = "BPE";
const char* DISABLE_LP = "LPD";
const char* DISABLE_HP = "HPD";
const char* DISABLE_BP = "BPD";

uint32_t rxBuffer = 0;

void BOARD_MasterClockConfig(void);

sai_master_clock_t mclkConfig;

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle);
static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t emptyBlock = BUFFER_NUMBER;
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};

SemaphoreHandle_t modules_initialized;

void init_modules(void * params);
void audio_codec(void *params);
void init_uart(void);

void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}
static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
        /* Handle the error. */ // Should be used to ping and pong Make a task that when ping is filled empty pong
    }
    else
    {
        emptyBlock--;
    }
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyBlock++;
    }
}

sai_transfer_t xfer;

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
	BOARD_InitPins();
	BOARD_InitLEDsPins();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);

    GPIO_PortSet(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

    modules_initialized= xSemaphoreCreateBinary();


    xTaskCreate(init_modules, "Init", 300, NULL, 1, NULL);
    xTaskCreate(audio_codec, "Codec", 300, NULL, 2, NULL);

    init_uart();

    vTaskStartScheduler();

    while(1) {
        __asm volatile ("nop");
    }
    return 0 ;
}

void init_modules(void * params) {

    init_I2C0_with_default_config();
    vTaskDelay(pdMS_TO_TICKS(100));

    uint8_t init_success = init_codec_i2s();
    if (!init_success) {
        PRINTF("Init error\n\r");
        GPIO_PortClear(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    }
    else {
        PRINTF("Success\n\r");
        GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
    }

    edma_config_t dmaConfig = {0};
    sai_transceiver_t config;

    /* Init DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_TX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_TX_EDMA_CHANNEL);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_RX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_RX_EDMA_CHANNEL);

    PRINTF("SAI example started!\n\r");

    /* Init DMA and create handle for DMA */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, DEMO_DMA, DEMO_TX_EDMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, DEMO_DMA, DEMO_RX_EDMA_CHANNEL);

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &dmaRxHandle);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, 1U << DEMO_SAI_CHANNEL);
    config.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    config.masterSlave = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);
    config.syncMode = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &config);


    CLOCK_EnableClock(kCLOCK_PortE);
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinMux(PORTC, 6, kPORT_MuxAlt6); // MCLK
    PORT_SetPinMux(PORTC, 1, kPORT_MuxAlt6); // TXD0
    PORT_SetPinMux(PORTE, 11, kPORT_MuxAlt4); // TX_FS
    PORT_SetPinMux(PORTE, 12, kPORT_MuxAlt4); // TX_BCLK
    PORT_SetPinMux(PORTC, 5, kPORT_MuxAlt4); // RXD0

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
            DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
            DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    xSemaphoreGive(modules_initialized);
    vTaskSuspend(NULL);

    for(;;) {
        // Nothing
    }
}

void audio_codec(void *params) {
    xSemaphoreTake(modules_initialized, portMAX_DELAY);
    uint8_t data[1025] = {0};

    for(;;) {
        if (emptyBlock > 0)
        {
            xfer.data     = Buffer + rx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
            {
                rx_index++;
            }
            if (rx_index == BUFFER_NUMBER)
            {
                rx_index = 0U;
            }
        }
        if (emptyBlock < BUFFER_NUMBER)
        {
			xfer.data     = Buffer + tx_index * BUFFER_SIZE;
			xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
            if (tx_index == BUFFER_NUMBER)
            {
                tx_index = 0U;
            }
        }
    }
}

void init_uart(void)
{
	init_filters(Buffer);
	static uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx     = true;
	config.enableRx     = true;

	UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

	uint8_t txbuff[]   = "Filtros\r\n\r\nPasa Bajas\n\rPasa Altas\n\rPasa Bandas\r\n";
	UART_WriteBlocking(DEMO_UART, txbuff, sizeof(txbuff) - 1);

	/* Tomar datos de la uart*/
	uint8_t i = 0;
	uint8_t car;
	UART_ReadBlocking(DEMO_UART, &car, 1);
	str[0] = car;
	UART_WriteBlocking(DEMO_UART,&car, 1);


	if(strcmp("1", str)==0)
	{

	}
	else if(strcmp("2", str)==0)
	{

	}
	else if(strcmp("3", str)==0)
	{

	}
	else if(strcmp("4", str)==0)
	{

	}
	else if(strcmp("5", str)==0)
	{

	}
	else if(strcmp("6", str)==0)
	{

	}
}

