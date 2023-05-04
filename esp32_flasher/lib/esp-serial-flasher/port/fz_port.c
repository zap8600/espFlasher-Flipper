#include "fz_port.h"

static UARTHandle* uart_handle;
static const GpioItem *gpio_io0, *gpio_rst;

esp_loader_error_t loader_port_write(const uint8_t* data, uint16_t size, uint32_t timeout) {
    uart_tx((uint8_t*)data, size);
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t loader_port_read(uint8_t* data, uint16_t size, uint32_t timeout) {
    UARTHandle* uart = uart_handle;

    uart->rx_stream = furi_stream_buffer_alloc(RX_BUF_SIZE, 1);

    while(1) {
        uint32_t events =
            furi_thread_flags_wait(WORKER_ALL_RX_EVENTS, FuriFlagWaitAny, FuriWaitForever);
        furi_check((events & FuriFlagError) == 0);
        if(events & WorkerEvtStop) break;
        if(events & WorkerEvtRxDone) {
            size_t len = furi_stream_buffer_receive(uart->rx_stream, uart->rx_buf, RX_BUF_SIZE, 0);
            if(len > 0) {
                if(uart->handle_rx_data_cb) uart->handle_rx_data_cb(uart->rx_buf, len);
            }
        }
    }

    furi_stream_buffer_free(uart->rx_stream);

    return ESP_LOADER_SUCCESS;
}

void uart_set_handle_rx_data_cb(
    UARTHandle* uart,
    void (*handle_rx_data_cb)(uint8_t* buf, size_t len)) {
    furi_assert(uart);
    uart->handle_rx_data_cb = handle_rx_data_cb;
}

void loader_port_fz_init(loader_fz_config_t* config) {
    uart_handle = (UARTHandle*)malloc(sizeof(UARTHandle));

    furi_hal_console_disable();
    if(config->baudrate == 0) {
        config->baudrate = 115200;
    }

    furi_hal_uart_init(UART_CH, config->baudrate);
    uart_set_handle_rx_data_cb
}

void uart_tx(uint8_t* data, size_t len) {
    furi_hal_uart_tx(UART_CH, data, len);
}