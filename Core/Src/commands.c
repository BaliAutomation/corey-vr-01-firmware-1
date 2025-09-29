//
// Created by niclas on 7/8/25.
//

#include <string.h>
#include "commands.h"

#include <stdbool.h>
#include <stdio.h>

#include "main.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef *uart;
extern SPI_HandleTypeDef *spi;

extern uint64_t hu_counter;
extern uint64_t hv_counter;
extern uint64_t hw_counter;

static char conversion[40];

static HAL_StatusTypeDef status;

static uint8_t spi_tx_buff[16];
static uint8_t spi_rx_buff[16];

// Forward declarations of command handlers
static void help_handler(char *cmd, char *response, uint32_t arg);

static void da_handler(char *cmd, char *response, uint32_t arg);

static void out_handler(char *cmd, char *response, uint32_t arg);

static void read_ds_handler(char *cmd, char *response, uint32_t arg);

static void read_hall_handler(char *cmd, char *response, uint32_t arg);

static void read_port_handler(char *cmd, char *response, uint32_t arg);

static void rcmp_handler(char *cmd, char *response, uint32_t arg);

static void spics_handler(char *cmd, char *response, uint32_t cs);


cmd_mapping commands[] = {
    {"help", help_handler},
    {"da1_on", da_handler, 1},
    {"da2_on", da_handler, 2},
    {"da3_on", da_handler, 3},
    {"da1_off", da_handler, 17},
    {"da2_off", da_handler, 18},
    {"da3_off", da_handler, 19},
    {"out1_on", out_handler, 1},
    {"out2_on", out_handler, 2},
    {"out3_on", out_handler, 3},
    {"out4_on", out_handler, 4},
    {"out1_off", out_handler, 17},
    {"out2_off", out_handler, 18},
    {"out3_off", out_handler, 19},
    {"out3_off", out_handler, 20},
    {"ds1", read_ds_handler, 1},
    {"ds2", read_ds_handler, 2},
    {"ds3", read_ds_handler, 3},
    {"hu", read_hall_handler, 1},
    {"hu_count", read_hall_handler, 17},
    {"hv", read_hall_handler, 2},
    {"hv_count", read_hall_handler, 18},
    {"hw", read_hall_handler, 3},
    {"hw_count", read_hall_handler, 19},
    {"porta", read_port_handler, 1},
    {"portb", read_port_handler, 2},
    {"portc", read_port_handler, 3},
    {"portd", read_port_handler, 4},
    {"rcmp_on", rcmp_handler, 0},
    {"rcmp_off", rcmp_handler, 1},
    {"spics1", spics_handler, 1},
    {"spics2", spics_handler, 2}
};

// Serial comms
void comm_format_append(char *output, char const *text, uint32_t const value, bool const wide) {
    char tmp[40];
    strcpy(tmp, text);
    strcat(tmp, "%");
    if (wide) {
        strcat(tmp, "4l");
    }
    strcat(tmp, " x\n");
    snprintf(conversion, sizeof(conversion), tmp, value);
    strcat(output, conversion);
}

int checksum(char *text) {
    int cks = 0;
    for (int i = 0; i < strlen(text); i++) {
        cks = cks << 2;
        cks = cks ^ text[i];
        cks = cks << 5;
        cks = cks + text[i];
    }
    return cks;
}

// SPI comms

void spi_select(uint8_t cs) {
    // 0 = deselect
    // 1 = SPICS1 active
    // 2 = SPICS2 active
    HAL_GPIO_WritePin(SPICS1_GPIO_Port,SPICS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPICS2_GPIO_Port,SPICS2_Pin, GPIO_PIN_SET);
    if (cs == 1) {
        HAL_GPIO_WritePin(SPICS1_GPIO_Port,SPICS1_Pin, GPIO_PIN_RESET);
    }
    if (cs == 2) {
        HAL_GPIO_WritePin(SPICS2_GPIO_Port,SPICS2_Pin, GPIO_PIN_RESET);
    }
    uint32_t until = HAL_GetTick() + 2;
    while (HAL_GetTick() < until) {
    }
}

uint8_t spi_read(uint8_t reg) {
    memset(spi_tx_buff, 0, 16);
    spi_tx_buff[0] = reg << 1;

    status = HAL_SPI_TransmitReceive(spi, spi_tx_buff, spi_rx_buff, 2, 10);
    // status = HAL_SPI_Transmit(spi, spi_tx_buff, 1, 5);
    // status |= HAL_SPI_Receive(spi, spi_rx_buff, 1, 5);
    if (status != HAL_OK) {
        return 0xAA;
    }
    return spi_rx_buff[0];
}

void spi_write(uint8_t reg, uint8_t value) {
    memset(spi_tx_buff, 0, 16);
    spi_tx_buff[0] = reg & 0x0f;
    spi_tx_buff[1] = value;
    status = HAL_SPI_TransmitReceive(spi, spi_tx_buff, spi_rx_buff, 2, 5);
}

void cmd_execute(char *cmd, char *response) {
    response[0] = '\0';
    bool found = false;
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(cmd, commands[i].cmd_str) == 0) {
            uint32_t arg = commands[i].arg;
            commands[i].cmd_func(cmd, response, arg);
            found = true;
        }
    }
    // response
    if (!found)
        strcpy(response, "No such command.");
    uint16_t cks = checksum(response);
    sprintf(conversion, "\r\n .ok(%d) ", status);
    strcat(response, conversion);
    sprintf(conversion, "[%4x]\r\n", cks);
    strcat(response, conversion);
    HAL_UART_Transmit_DMA(uart, (uint8_t *) response, strlen(response));
}

// Command handler implementations
static void help_handler(char *cmd, char *response, uint32_t arg) {
    strcpy(response, "Here are the commands:\n");
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        strcat(response, "  ");
        strcat(response, commands[i].cmd_str);
        strcat(response, "\n");
    }
}

static void da_handler(char *cmd, char *response, uint32_t arg) {
    if (arg < 16)
        // SET
        HAL_GPIO_WritePin(DA1_OUT_GPIO_Port, arg - 1, GPIO_PIN_SET);
    else
        // RESET
        HAL_GPIO_WritePin(DA1_OUT_GPIO_Port, (arg & 0x0F) - 1, GPIO_PIN_RESET);
}

static void out_handler(char *cmd, char *response, uint32_t arg) {
    if (arg < 16)
        // SET
        switch (arg) {
            case 1:
                HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_SET);
                break;
            case 2:
                HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_SET);
                break;
            case 3:
                HAL_GPIO_WritePin(OUT3_GPIO_Port, OUT3_Pin, GPIO_PIN_SET);
                break;
            case 4:
                HAL_GPIO_WritePin(OUT4_GPIO_Port, OUT4_Pin, GPIO_PIN_SET);
                break;
            default:
                break;
        }
    else
        // RESET
        switch (arg & 0x0F) {
            case 1:
                HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_RESET);
                break;
            case 2:
                HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_RESET);
                break;
            case 3:
                HAL_GPIO_WritePin(OUT3_GPIO_Port, OUT3_Pin, GPIO_PIN_RESET);
                break;
            case 4:
                HAL_GPIO_WritePin(OUT4_GPIO_Port, OUT4_Pin, GPIO_PIN_RESET);
                break;
            default:
                break;
        }
}

static void read_ds_handler(char *cmd, char *response, uint32_t arg) {
    GPIO_PinState pin_state = GPIO_PIN_RESET;
    switch (arg) {
        case 1:
            pin_state = HAL_GPIO_ReadPin(DS1_IN_GPIO_Port, DS1_IN_Pin);
            break;
        case 2:
            pin_state = HAL_GPIO_ReadPin(DS2_IN_GPIO_Port, DS2_IN_Pin);
            break;
        case 3:
            pin_state = HAL_GPIO_ReadPin(DS3_IN_GPIO_Port, DS3_IN_Pin);
            break;
        default:
            break;
    }
    response[0] = pin_state == GPIO_PIN_SET ? '0' : '1';
    response[1] = '\0';
}

static void read_hall_handler(char *cmd, char *response, uint32_t arg) {
    if (arg < 16) {
        // Read state
        GPIO_PinState pin_state = GPIO_PIN_RESET;
        switch (arg) {
            case 1:
                pin_state = HAL_GPIO_ReadPin(HU_IN_GPIO_Port, HU_IN_Pin);
                break;
            case 2:
                pin_state = HAL_GPIO_ReadPin(HV_IN_GPIO_Port, HV_IN_Pin);
                break;
            case 3:
                pin_state = HAL_GPIO_ReadPin(HW_IN_GPIO_Port, HW_IN_Pin);
                break;
            default:
                break;
        }
        response[0] = pin_state == GPIO_PIN_SET ? '0' : '1';
        response[1] = '\0';
    } else {
        uint64_t value = 0;
        switch (arg & 0x0F) {
            case 1:
                value = hu_counter;
                break;
            case 2:
                value = hv_counter;
                break;
            case 3:
                value = hw_counter;
                break;
            default:
                break;
        }
        snprintf(response, sizeof(response), "%llu", value);
    }
}

static void read_port_handler(char *cmd, char *response, uint32_t arg) {
    GPIO_TypeDef *port = NULL;
    switch (arg) {
        case 1:
            strcat(response, "\nPortA\n");
            port = GPIOA;
            break;
        case 2:
            strcat(response, "\nPortB\n");
            port = GPIOB;
            break;
        case 3:
            strcat(response, "\nPortC\n");
            port = GPIOC;
            break;
        case 4:
            strcat(response, "\nPortD\n");
            port = GPIOD;
            break;
        default:
            break;
    }
    if (port != NULL) {
        comm_format_append(response, "   IDR: ", port->IDR, true);
        comm_format_append(response, "   ODR: ", port->ODR, true);
        comm_format_append(response, "   CRL: ", port->CRL, true);
        comm_format_append(response, "   CRL: ", port->CRL, true);
        comm_format_append(response, "   CRH: ", port->CRH, true);
        comm_format_append(response, "  BSRR: ", port->BSRR, true);
        comm_format_append(response, "   BRR: ", port->BRR, true);
        comm_format_append(response, "  LCKR: ", port->LCKR, true);
    }
}

static void rcmp_handler(char *cmd, char *response, uint32_t arg) {
    if (arg == 0)
        HAL_GPIO_WritePin(RCMP_STM_GPIO_Port, RCMP_STM_Pin, GPIO_PIN_SET);
    if (arg == 1)
        HAL_GPIO_WritePin(RCMP_STM_GPIO_Port, RCMP_STM_Pin, GPIO_PIN_RESET);
}

static void spics_handler(char *cmd, char *response, uint32_t cs) {
    spi_select(cs);
    comm_format_append(response, "SPICS", cs, false);
    comm_format_append(response, "     SetOUT: ", spi_read(0x00), false);
    comm_format_append(response, "     SetLED: ", spi_read(0x01), false);
    comm_format_append(response, "   DoiLevel: ", spi_read(0x02), false);
    comm_format_append(response, "  Interrupt: ", spi_read(0x03), false);
    comm_format_append(response, "   OvrLdChF: ", spi_read(0x04), false);
    comm_format_append(response, "  OpnWirChF: ", spi_read(0x05), false);
    comm_format_append(response, "  ShtVDDChF: ", spi_read(0x06), false);
    comm_format_append(response, "  GlobalErr: ", spi_read(0x07), false);
    comm_format_append(response, "   OpenWrEn: ", spi_read(0x08), false);
    comm_format_append(response, "   ShtVDDEn: ", spi_read(0x09), false);
    comm_format_append(response, "    Config1: ", spi_read(0x0A), false);
    comm_format_append(response, "    Config2: ", spi_read(0x0B), false);
    comm_format_append(response, "   ConfigDI: ", spi_read(0x0C), false);
    comm_format_append(response, "   ConfigDO: ", spi_read(0x0D), false);
    comm_format_append(response, "    CurrLim: ", spi_read(0x0E), false);
    comm_format_append(response, "       Mask: ", spi_read(0x0F), false);
    spi_select(0);
}
