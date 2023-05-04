#pragma once

#include <stdint.h>
#include "esp_loader_io.h"
#include "furi_hal.h"
#include <furi_hal_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART_CH (FuriHalUartIdUSART1)

#define RX_BUF_SIZE (320)

typedef enum {
    WorkerEvtStop = (1 << 0),
    WorkerEvtRxDone = (1 << 1),
} WorkerEvtFlags;

#define WORKER_ALL_RX_EVENTS (WorkerEvtStop | WorkerEvtRxDone)

typedef struct {
    const char* name;
    const GpioPin* pin;
} GpioItem;

typedef struct {
    FuriStreamBuffer* rx_stream;
    uint8_t rx_buf[RX_BUF_SIZE + 1];
    void (*handle_rx_data_cb)(uint8_t* buf, size_t len);
} UARTHandle;

typedef struct {
    const GpioItem* gpio_io0;
    const GpioItem* gpio_rst;
    uint32_t baudrate;
} loader_fz_config_t;

void loader_port_fz_init(loader_fz_config_t* config);

#ifdef __cplusplus
}
#endif