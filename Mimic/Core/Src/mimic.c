/**
  ******************************************************************************
  * @file    mimic.c
  * @brief   Mimic Command Interface Implementation
  * @author  Mimic Project
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mimic.h"
#include "usart.h"
#include <stdarg.h>

/* ========================== PRIVATE VARIABLES ============================= */

Mimic_CmdState_t mimic_state;

/* Interrupt-based RX */
static volatile uint8_t rx_byte;
static volatile uint8_t rx_ready = 0;

/* Circular buffer for received data */
#define RX_BUFFER_SIZE 256
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;

/* Port mapping table */
static const Mimic_PortMap_t port_map[] = {
    {'A', GPIOA},
    {'B', GPIOB},
    {'C', GPIOC},
    {'D', GPIOD},
    {'E', GPIOE},
    {'H', GPIOH},
    {0, NULL}
};

/* UART handles for dynamic configuration */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* ========================== INTERRUPT HANDLERS ============================ */

/**
  * @brief  UART RX Complete Callback (called from interrupt)
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &MIMIC_HOST_UART)
    {
        /* Store in circular buffer */
        uint16_t next_head = (rx_head + 1) % RX_BUFFER_SIZE;
        if (next_head != rx_tail)  /* Buffer not full */
        {
            rx_buffer[rx_head] = rx_byte;
            rx_head = next_head;
        }
        
        /* Re-enable interrupt for next byte */
        HAL_UART_Receive_IT(&MIMIC_HOST_UART, (uint8_t*)&rx_byte, 1);
    }
}

/**
  * @brief  Check if data available in RX buffer
  */
static uint8_t Mimic_RxAvailable(void)
{
    return rx_head != rx_tail;
}

/**
  * @brief  Get one byte from RX buffer
  */
static uint8_t Mimic_RxGet(void)
{
    if (rx_head == rx_tail) return 0;
    uint8_t data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return data;
}

/* ========================== CORE FUNCTIONS ================================ */

/**
  * @brief  Initialize Mimic command interface
  */
void Mimic_Init(void)
{
    memset(&mimic_state, 0, sizeof(Mimic_CmdState_t));
    
    /* Start interrupt-based reception */
    HAL_UART_Receive_IT(&MIMIC_HOST_UART, (uint8_t*)&rx_byte, 1);
    
    /* Send welcome message */
    Mimic_SendResponse("\r\n");
    Mimic_SendResponse("========================================\r\n");
    Mimic_SendResponse("  MIMIC - Hardware Control Interface\r\n");
    Mimic_SendResponseF("  Version: %s\r\n", MIMIC_VERSION);
    Mimic_SendResponse("  Type 'HELP' for commands\r\n");
    Mimic_SendResponse("========================================\r\n");
    Mimic_SendResponse("READY\r\n> ");
}

/**
  * @brief  Process incoming commands (call in main loop)
  */
void Mimic_Process(void)
{
    /* Process all available bytes from interrupt buffer */
    while (Mimic_RxAvailable())
    {
        uint8_t rx = Mimic_RxGet();
        
        /* Echo back */
        HAL_UART_Transmit(&MIMIC_HOST_UART, &rx, 1, 10);
        
        /* Handle special characters */
        if (rx == '\r' || rx == '\n')
        {
            if (mimic_state.cmd_index > 0)
            {
                Mimic_SendResponse("\r\n");
                mimic_state.cmd_buffer[mimic_state.cmd_index] = '\0';
                Mimic_ProcessCommand(mimic_state.cmd_buffer);
                mimic_state.cmd_index = 0;
                Mimic_SendResponse("> ");
            }
        }
        else if (rx == '\b' || rx == 127)  /* Backspace */
        {
            if (mimic_state.cmd_index > 0)
            {
                mimic_state.cmd_index--;
                Mimic_SendResponse(" \b");  /* Erase character */
            }
        }
        else if (rx >= 32 && rx < 127)  /* Printable */
        {
            if (mimic_state.cmd_index < MIMIC_MAX_CMD_LEN - 1)
            {
                mimic_state.cmd_buffer[mimic_state.cmd_index++] = rx;
            }
        }
    }
}

/**
  * @brief  Send response string
  */
void Mimic_SendResponse(const char *response)
{
    HAL_UART_Transmit(&MIMIC_HOST_UART, (uint8_t*)response, strlen(response), 100);
}

/**
  * @brief  Send formatted response
  */
void Mimic_SendResponseF(const char *format, ...)
{
    char buffer[MIMIC_MAX_CMD_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Mimic_SendResponse(buffer);
}

/* ========================== COMMAND PROCESSING ============================ */

/**
  * @brief  Parse command line into command structure
  */
uint8_t Mimic_ParseCommand(const char *cmd_line, Mimic_Command_t *cmd)
{
    char buffer[MIMIC_MAX_CMD_LEN];
    char *token;
    uint8_t idx = 0;
    
    memset(cmd, 0, sizeof(Mimic_Command_t));
    strncpy(buffer, cmd_line, MIMIC_MAX_CMD_LEN - 1);
    
    /* Get command */
    token = strtok(buffer, " \t");
    if (token == NULL) return 0;
    
    /* Convert to uppercase */
    for (int i = 0; token[i]; i++) {
        cmd->command[i] = (token[i] >= 'a' && token[i] <= 'z') ? token[i] - 32 : token[i];
    }
    
    /* Get arguments */
    while ((token = strtok(NULL, " \t")) != NULL && idx < MIMIC_MAX_ARGS)
    {
        strncpy(cmd->args[idx], token, MIMIC_MAX_ARG_LEN - 1);
        idx++;
    }
    cmd->argc = idx;
    
    return 1;
}

/* Forward declarations for command handlers */
void Mimic_CMD_UART_POLL(Mimic_Command_t *cmd);

/**
  * @brief  Process a command line
  */
void Mimic_ProcessCommand(const char *cmd_line)
{
    Mimic_Command_t cmd;
    
    if (!Mimic_ParseCommand(cmd_line, &cmd) || strlen(cmd.command) == 0)
    {
        return;
    }
    
    /* ===== GPIO Commands ===== */
    if (strcmp(cmd.command, "PIN_STATUS") == 0 || strcmp(cmd.command, "PS") == 0)
    {
        Mimic_CMD_PIN_STATUS(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_SET_OUT") == 0 || strcmp(cmd.command, "PSO") == 0)
    {
        Mimic_CMD_PIN_SET_OUT(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_SET_IN") == 0 || strcmp(cmd.command, "PSI") == 0)
    {
        Mimic_CMD_PIN_SET_IN(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_HIGH") == 0 || strcmp(cmd.command, "PH") == 0)
    {
        Mimic_CMD_PIN_HIGH(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_LOW") == 0 || strcmp(cmd.command, "PL") == 0)
    {
        Mimic_CMD_PIN_LOW(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_READ") == 0 || strcmp(cmd.command, "PR") == 0)
    {
        Mimic_CMD_PIN_READ(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_TOGGLE") == 0 || strcmp(cmd.command, "PT") == 0)
    {
        Mimic_CMD_PIN_TOGGLE(&cmd);
    }
    else if (strcmp(cmd.command, "PIN_MODE") == 0 || strcmp(cmd.command, "PM") == 0)
    {
        Mimic_CMD_PIN_MODE(&cmd);
    }
    /* ===== UART Commands ===== */
    else if (strcmp(cmd.command, "UART_INIT") == 0 || strcmp(cmd.command, "UI") == 0)
    {
        Mimic_CMD_UART_INIT(&cmd);
    }
    else if (strcmp(cmd.command, "UART_SEND") == 0 || strcmp(cmd.command, "US") == 0)
    {
        Mimic_CMD_UART_SEND(&cmd);
    }
    else if (strcmp(cmd.command, "UART_RECV") == 0 || strcmp(cmd.command, "UR") == 0)
    {
        Mimic_CMD_UART_RECV(&cmd);
    }
    else if (strcmp(cmd.command, "UART_STATUS") == 0)
    {
        Mimic_CMD_UART_STATUS(&cmd);
    }
    else if (strcmp(cmd.command, "UART_TEST") == 0)
    {
        Mimic_CMD_UART_TEST(&cmd);
    }
    else if (strcmp(cmd.command, "UART_POLL") == 0 || strcmp(cmd.command, "UP") == 0)
    {
        Mimic_CMD_UART_POLL(&cmd);
    }
    /* ===== System Commands ===== */
    else if (strcmp(cmd.command, "HELP") == 0 || strcmp(cmd.command, "?") == 0)
    {
        Mimic_CMD_HELP(&cmd);
    }
    else if (strcmp(cmd.command, "VERSION") == 0 || strcmp(cmd.command, "VER") == 0)
    {
        Mimic_CMD_VERSION(&cmd);
    }
    else if (strcmp(cmd.command, "STATUS") == 0)
    {
        Mimic_CMD_STATUS(&cmd);
    }
    else if (strcmp(cmd.command, "RESET") == 0)
    {
        Mimic_CMD_RESET(&cmd);
    }
    else
    {
        Mimic_SendResponseF("ERROR: Unknown command '%s'\r\n", cmd.command);
    }
}

/* ========================== HELPER FUNCTIONS ============================== */

/**
  * @brief  Get GPIO port from character
  */
GPIO_TypeDef* Mimic_GetPort(char port_char)
{
    char upper = (port_char >= 'a' && port_char <= 'z') ? port_char - 32 : port_char;
    
    for (int i = 0; port_map[i].port != NULL; i++)
    {
        if (port_map[i].name == upper)
        {
            return port_map[i].port;
        }
    }
    return NULL;
}

/**
  * @brief  Get GPIO pin mask from number
  */
uint16_t Mimic_GetPin(uint8_t pin_num)
{
    if (pin_num > 15) return 0;
    return (1 << pin_num);
}

/**
  * @brief  Parse pin string like "A5" or "D12"
  */
uint8_t Mimic_ParsePin(const char *pin_str, GPIO_TypeDef **port, uint16_t *pin)
{
    if (strlen(pin_str) < 2) return 0;
    
    *port = Mimic_GetPort(pin_str[0]);
    if (*port == NULL) return 0;
    
    uint8_t pin_num = atoi(&pin_str[1]);
    *pin = Mimic_GetPin(pin_num);
    if (*pin == 0 && pin_str[1] != '0') return 0;
    
    return 1;
}

/* ========================== GPIO COMMANDS ================================= */

/**
  * @brief  PIN_STATUS <PIN> - Show pin configuration
  */
void Mimic_CMD_PIN_STATUS(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_STATUS <PIN> (e.g., PIN_STATUS A5)\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    uint8_t pin_num = 0;
    for (int i = 0; i < 16; i++) {
        if (pin & (1 << i)) { pin_num = i; break; }
    }
    
    /* Read MODER */
    uint32_t moder = (port->MODER >> (pin_num * 2)) & 0x03;
    const char *mode_str[] = {"INPUT", "OUTPUT", "ALT_FUNC", "ANALOG"};
    
    /* Read OTYPER */
    uint32_t otyper = (port->OTYPER >> pin_num) & 0x01;
    const char *otype_str[] = {"PUSH_PULL", "OPEN_DRAIN"};
    
    /* Read OSPEEDR */
    uint32_t ospeedr = (port->OSPEEDR >> (pin_num * 2)) & 0x03;
    const char *speed_str[] = {"LOW", "MEDIUM", "HIGH", "VERY_HIGH"};
    
    /* Read PUPDR */
    uint32_t pupdr = (port->PUPDR >> (pin_num * 2)) & 0x03;
    const char *pupd_str[] = {"NONE", "PULL_UP", "PULL_DOWN", "RESERVED"};
    
    /* Read current state */
    uint8_t state = HAL_GPIO_ReadPin(port, pin);
    
    Mimic_SendResponseF("PIN %s:\r\n", cmd->args[0]);
    Mimic_SendResponseF("  Mode:   %s\r\n", mode_str[moder]);
    Mimic_SendResponseF("  Type:   %s\r\n", otype_str[otyper]);
    Mimic_SendResponseF("  Speed:  %s\r\n", speed_str[ospeedr]);
    Mimic_SendResponseF("  Pull:   %s\r\n", pupd_str[pupdr]);
    Mimic_SendResponseF("  State:  %s\r\n", state ? "HIGH" : "LOW");
}

/**
  * @brief  PIN_SET_OUT <PIN> - Configure pin as output
  */
void Mimic_CMD_PIN_SET_OUT(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_SET_OUT <PIN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    /* Enable GPIO clock */
    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    
    Mimic_SendResponseF("OK: %s configured as OUTPUT\r\n", cmd->args[0]);
}

/**
  * @brief  PIN_SET_IN <PIN> [PULL] - Configure pin as input
  */
void Mimic_CMD_PIN_SET_IN(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_SET_IN <PIN> [UP|DOWN|NONE]\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    /* Enable GPIO clock */
    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    
    uint32_t pull = GPIO_NOPULL;
    if (cmd->argc >= 2)
    {
        if (strcmp(cmd->args[1], "UP") == 0) pull = GPIO_PULLUP;
        else if (strcmp(cmd->args[1], "DOWN") == 0) pull = GPIO_PULLDOWN;
    }
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = pull;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    
    Mimic_SendResponseF("OK: %s configured as INPUT\r\n", cmd->args[0]);
}

/**
  * @brief  PIN_HIGH <PIN> - Set pin high
  */
void Mimic_CMD_PIN_HIGH(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_HIGH <PIN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    Mimic_SendResponseF("OK: %s = HIGH\r\n", cmd->args[0]);
}

/**
  * @brief  PIN_LOW <PIN> - Set pin low
  */
void Mimic_CMD_PIN_LOW(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_LOW <PIN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    Mimic_SendResponseF("OK: %s = LOW\r\n", cmd->args[0]);
}

/**
  * @brief  PIN_READ <PIN> - Read pin state
  */
void Mimic_CMD_PIN_READ(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_READ <PIN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    uint8_t state = HAL_GPIO_ReadPin(port, pin);
    Mimic_SendResponseF("%s = %s\r\n", cmd->args[0], state ? "HIGH" : "LOW");
}

/**
  * @brief  PIN_TOGGLE <PIN> - Toggle pin state
  */
void Mimic_CMD_PIN_TOGGLE(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: PIN_TOGGLE <PIN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    HAL_GPIO_TogglePin(port, pin);
    uint8_t state = HAL_GPIO_ReadPin(port, pin);
    Mimic_SendResponseF("OK: %s toggled to %s\r\n", cmd->args[0], state ? "HIGH" : "LOW");
}

/**
  * @brief  PIN_MODE <PIN> <MODE> - Set pin mode
  */
void Mimic_CMD_PIN_MODE(Mimic_Command_t *cmd)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    
    if (cmd->argc < 2)
    {
        Mimic_SendResponse("Usage: PIN_MODE <PIN> <IN|OUT|AF|AN>\r\n");
        return;
    }
    
    if (!Mimic_ParsePin(cmd->args[0], &port, &pin))
    {
        Mimic_SendResponseF("ERROR: Invalid pin '%s'\r\n", cmd->args[0]);
        return;
    }
    
    /* Enable GPIO clock */
    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    if (strcmp(cmd->args[1], "IN") == 0)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    else if (strcmp(cmd->args[1], "OUT") == 0)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else if (strcmp(cmd->args[1], "AF") == 0)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    }
    else if (strcmp(cmd->args[1], "AN") == 0)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    }
    else
    {
        Mimic_SendResponse("ERROR: Mode must be IN, OUT, AF, or AN\r\n");
        return;
    }
    
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    Mimic_SendResponseF("OK: %s mode set to %s\r\n", cmd->args[0], cmd->args[1]);
}

/* ========================== UART COMMANDS ================================= */

/**
  * @brief  Get UART handle from instance name
  */
static UART_HandleTypeDef* Mimic_GetUARTHandle(const char *instance)
{
    if (strcmp(instance, "UART1") == 0 || strcmp(instance, "USART1") == 0 || strcmp(instance, "1") == 0)
        return &huart1;
    else if (strcmp(instance, "UART6") == 0 || strcmp(instance, "USART6") == 0 || strcmp(instance, "6") == 0)
        return &huart6;
    return NULL;  /* UART2 is reserved for host */
}

/**
  * @brief  Configure UART GPIO pins
  */
static void Mimic_ConfigureUARTGPIO(USART_TypeDef *instance)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (instance == USART1)
    {
        /* USART1: PA9 (TX), PA10 (RX) */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if (instance == USART6)
    {
        /* USART6: PC6 (TX), PC7 (RX) */
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
}

/**
  * @brief  UART_INIT <INSTANCE> <BAUDRATE> [PARITY] [STOPBITS]
  */
void Mimic_CMD_UART_INIT(Mimic_Command_t *cmd)
{
    if (cmd->argc < 2)
    {
        Mimic_SendResponse("Usage: UART_INIT <1|6> <BAUDRATE> [N|E|O] [1|2]\r\n");
        Mimic_SendResponse("  Example: UART_INIT 1 9600 N 1\r\n");
        return;
    }
    
    UART_HandleTypeDef *huart = Mimic_GetUARTHandle(cmd->args[0]);
    if (huart == NULL)
    {
        Mimic_SendResponse("ERROR: Invalid UART (use 1 or 6, UART2 is reserved)\r\n");
        return;
    }
    
    uint32_t baudrate = atoi(cmd->args[1]);
    if (baudrate < 300 || baudrate > 3000000)
    {
        Mimic_SendResponse("ERROR: Invalid baudrate (300-3000000)\r\n");
        return;
    }
    
    /* Enable UART clock */
    if (huart == &huart1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        huart->Instance = USART1;
        Mimic_ConfigureUARTGPIO(USART1);
    }
    else if (huart == &huart6)
    {
        __HAL_RCC_USART6_CLK_ENABLE();
        huart->Instance = USART6;
        Mimic_ConfigureUARTGPIO(USART6);
    }
    
    huart->Init.BaudRate = baudrate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    
    /* Parse parity */
    if (cmd->argc >= 3)
    {
        if (cmd->args[2][0] == 'E' || cmd->args[2][0] == 'e')
        {
            huart->Init.Parity = UART_PARITY_EVEN;
            huart->Init.WordLength = UART_WORDLENGTH_9B;
        }
        else if (cmd->args[2][0] == 'O' || cmd->args[2][0] == 'o')
        {
            huart->Init.Parity = UART_PARITY_ODD;
            huart->Init.WordLength = UART_WORDLENGTH_9B;
        }
    }
    
    /* Parse stop bits */
    if (cmd->argc >= 4)
    {
        if (cmd->args[3][0] == '2')
        {
            huart->Init.StopBits = UART_STOPBITS_2;
        }
    }
    
    if (HAL_UART_Init(huart) != HAL_OK)
    {
        Mimic_SendResponse("ERROR: UART init failed\r\n");
        return;
    }
    
    /* Reset state to ready */
    huart->gState = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;
    
    /* Verify actual baud rate */
    uint32_t pclk;
    if (huart->Instance == USART1 || huart->Instance == USART6)
    {
        pclk = HAL_RCC_GetPCLK2Freq();
    }
    else
    {
        pclk = HAL_RCC_GetPCLK1Freq();
    }
    uint32_t actual_baud = pclk / huart->Instance->BRR;
    
    Mimic_SendResponseF("OK: UART%s initialized\r\n", cmd->args[0]);
    Mimic_SendResponseF("  Requested: %lu baud\r\n", baudrate);
    Mimic_SendResponseF("  PCLK: %lu Hz, BRR: %lu\r\n", pclk, huart->Instance->BRR);
}

/**
  * @brief  UART_SEND <INSTANCE> <DATA>
  */
void Mimic_CMD_UART_SEND(Mimic_Command_t *cmd)
{
    if (cmd->argc < 2)
    {
        Mimic_SendResponse("Usage: UART_SEND <1|6> <DATA>\r\n");
        return;
    }
    
    UART_HandleTypeDef *huart = Mimic_GetUARTHandle(cmd->args[0]);
    if (huart == NULL)
    {
        Mimic_SendResponse("ERROR: Invalid UART instance\r\n");
        return;
    }
    
    if (huart->Instance == NULL)
    {
        Mimic_SendResponse("ERROR: UART not initialized. Use UART_INIT first.\r\n");
        return;
    }
    
    /* Combine all remaining args as data */
    char data[MIMIC_MAX_CMD_LEN] = {0};
    for (int i = 1; i < cmd->argc; i++)
    {
        if (i > 1) strcat(data, " ");
        strcat(data, cmd->args[i]);
    }
    
    /* Handle escape sequences */
    char *ptr = data;
    char output[MIMIC_MAX_CMD_LEN];
    int out_idx = 0;
    
    while (*ptr && out_idx < MIMIC_MAX_CMD_LEN - 1)
    {
        if (*ptr == '\\' && *(ptr+1))
        {
            ptr++;
            switch (*ptr)
            {
                case 'n': output[out_idx++] = '\n'; break;
                case 'r': output[out_idx++] = '\r'; break;
                case 't': output[out_idx++] = '\t'; break;
                case '\\': output[out_idx++] = '\\'; break;
                default: output[out_idx++] = *ptr; break;
            }
        }
        else
        {
            output[out_idx++] = *ptr;
        }
        ptr++;
    }
    output[out_idx] = '\0';
    
    HAL_StatusTypeDef status = HAL_UART_Transmit(huart, (uint8_t*)output, out_idx, 1000);
    
    if (status == HAL_OK)
    {
        Mimic_SendResponseF("OK: Sent %d bytes\r\n", out_idx);
    }
    else
    {
        Mimic_SendResponse("ERROR: Transmit failed\r\n");
    }
}

/**
  * @brief  UART_RECV <INSTANCE> <LENGTH> [TIMEOUT_MS]
  */
void Mimic_CMD_UART_RECV(Mimic_Command_t *cmd)
{
    if (cmd->argc < 2)
    {
        Mimic_SendResponse("Usage: UART_RECV <1|6> <LENGTH> [TIMEOUT_MS]\r\n");
        return;
    }
    
    UART_HandleTypeDef *huart = Mimic_GetUARTHandle(cmd->args[0]);
    if (huart == NULL)
    {
        Mimic_SendResponse("ERROR: Invalid UART instance\r\n");
        return;
    }
    
    if (huart->Instance == NULL)
    {
        Mimic_SendResponse("ERROR: UART not initialized\r\n");
        return;
    }
    
    /* Check and clear any error state */
    if (huart->gState != HAL_UART_STATE_READY)
    {
        Mimic_SendResponseF("WARNING: UART state=0x%02X, resetting...\r\n", huart->gState);
        huart->gState = HAL_UART_STATE_READY;
        huart->RxState = HAL_UART_STATE_READY;
    }
    
    /* Clear any pending errors */
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
    
    uint16_t length = atoi(cmd->args[1]);
    if (length > 64) length = 64;
    
    uint32_t timeout = 1000;
    if (cmd->argc >= 3)
    {
        timeout = atoi(cmd->args[2]);
    }
    
    /* Send status and wait for TX to complete before blocking on RX */
    Mimic_SendResponseF("Waiting for %d bytes (timeout: %lu ms)...\r\n", length, timeout);
    
    /* Wait for UART2 TX to complete so message is sent before we block */
    while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET) {}
    
    /* Debug: Check UART state before receive */
    Mimic_SendResponseF("DEBUG: gState=0x%02X, RxState=0x%02X, ErrorCode=0x%02lX\r\n", 
                        huart->gState, huart->RxState, huart->ErrorCode);
    while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET) {}
    
    uint8_t buffer[65] = {0};
    uint32_t start_time = HAL_GetTick();
    HAL_StatusTypeDef status = HAL_UART_Receive(huart, buffer, length, timeout);
    uint32_t elapsed = HAL_GetTick() - start_time;
    
    Mimic_SendResponseF("(waited %lu ms, status=%d)\r\n", elapsed, status);
    
    if (status == HAL_OK)
    {
        buffer[length] = '\0';
        Mimic_SendResponse("DATA: ");
        Mimic_SendResponse((char*)buffer);
        Mimic_SendResponse("\r\n");
        
        /* Also show hex */
        Mimic_SendResponse("HEX: ");
        for (int i = 0; i < length; i++)
        {
            Mimic_SendResponseF("%02X ", buffer[i]);
        }
        Mimic_SendResponse("\r\n");
    }
    else if (status == HAL_TIMEOUT)
    {
        Mimic_SendResponse("TIMEOUT: No data received\r\n");
    }
    else if (status == HAL_BUSY)
    {
        Mimic_SendResponse("ERROR: UART busy\r\n");
    }
    else
    {
        Mimic_SendResponseF("ERROR: Receive failed (status=%d)\r\n", status);
    }
}

/**
  * @brief  UART_STATUS - Show UART status
  */
void Mimic_CMD_UART_STATUS(Mimic_Command_t *cmd)
{
    Mimic_SendResponse("UART Status:\r\n");
    Mimic_SendResponseF("  UART2 (Host): %lu baud (ACTIVE)\r\n", huart2.Init.BaudRate);
    
    if (huart1.Instance != NULL)
    {
        Mimic_SendResponseF("  UART1: %lu baud (PA9/PA10)\r\n", huart1.Init.BaudRate);
    }
    else
    {
        Mimic_SendResponse("  UART1: Not initialized\r\n");
    }
    
    if (huart6.Instance != NULL)
    {
        Mimic_SendResponseF("  UART6: %lu baud (PC6/PC7)\r\n", huart6.Init.BaudRate);
    }
    else
    {
        Mimic_SendResponse("  UART6: Not initialized\r\n");
    }
}

/**
  * @brief  UART_TEST <INSTANCE> - Loopback test (TX->RX wire required)
  */
void Mimic_CMD_UART_TEST(Mimic_Command_t *cmd)
{
    if (cmd->argc < 1)
    {
        Mimic_SendResponse("Usage: UART_TEST <1|6>\r\n");
        Mimic_SendResponse("  Requires TX->RX loopback wire!\r\n");
        return;
    }
    
    UART_HandleTypeDef *huart = Mimic_GetUARTHandle(cmd->args[0]);
    if (huart == NULL)
    {
        Mimic_SendResponse("ERROR: Invalid UART instance\r\n");
        return;
    }
    
    if (huart->Instance == NULL)
    {
        Mimic_SendResponse("ERROR: UART not initialized. Use UART_INIT first.\r\n");
        return;
    }
    
    const char *test_msg = "MIMIC_LOOPBACK_TEST";
    uint8_t rx_buf[32] = {0};
    uint8_t len = strlen(test_msg);
    
    Mimic_SendResponseF("Testing UART%s loopback...\r\n", cmd->args[0]);
    Mimic_SendResponseF("  TX: \"%s\" (%d bytes)\r\n", test_msg, len);
    
    /* Send test message */
    HAL_StatusTypeDef tx_status = HAL_UART_Transmit(huart, (uint8_t*)test_msg, len, 100);
    if (tx_status != HAL_OK)
    {
        Mimic_SendResponse("  TX FAILED!\r\n");
        return;
    }
    
    /* Receive with timeout */
    HAL_StatusTypeDef rx_status = HAL_UART_Receive(huart, rx_buf, len, 500);
    
    if (rx_status == HAL_OK)
    {
        rx_buf[len] = '\0';
        Mimic_SendResponseF("  RX: \"%s\" (%d bytes)\r\n", rx_buf, len);
        
        if (memcmp(test_msg, rx_buf, len) == 0)
        {
            Mimic_SendResponse("  RESULT: PASS - Loopback OK!\r\n");
        }
        else
        {
            Mimic_SendResponse("  RESULT: FAIL - Data mismatch!\r\n");
        }
    }
    else if (rx_status == HAL_TIMEOUT)
    {
        Mimic_SendResponse("  RX: TIMEOUT - No data received\r\n");
        Mimic_SendResponse("  RESULT: FAIL - Check TX->RX wire!\r\n");
    }
    else
    {
        Mimic_SendResponse("  RX: ERROR\r\n");
        Mimic_SendResponse("  RESULT: FAIL\r\n");
    }
}

/* ========================== SYSTEM COMMANDS =============================== */

/**
  * @brief  HELP - Show available commands
  */
void Mimic_CMD_HELP(Mimic_Command_t *cmd)
{
    Mimic_SendResponse("\r\n");
    Mimic_SendResponse("=== MIMIC Command Reference ===\r\n\r\n");
    
    Mimic_SendResponse("[GPIO Commands]\r\n");
    Mimic_SendResponse("  PIN_STATUS <PIN>         - Show pin config (e.g., PIN_STATUS D12)\r\n");
    Mimic_SendResponse("  PIN_SET_OUT <PIN>        - Set as output\r\n");
    Mimic_SendResponse("  PIN_SET_IN <PIN> [PULL]  - Set as input (PULL: UP|DOWN|NONE)\r\n");
    Mimic_SendResponse("  PIN_HIGH <PIN>           - Set pin HIGH\r\n");
    Mimic_SendResponse("  PIN_LOW <PIN>            - Set pin LOW\r\n");
    Mimic_SendResponse("  PIN_READ <PIN>           - Read pin state\r\n");
    Mimic_SendResponse("  PIN_TOGGLE <PIN>         - Toggle pin\r\n");
    Mimic_SendResponse("  PIN_MODE <PIN> <MODE>    - Set mode (IN|OUT|AF|AN)\r\n\r\n");
    
    Mimic_SendResponse("[UART Commands]\r\n");
    Mimic_SendResponse("  UART_INIT <1|6> <BAUD> [PARITY] [STOP]\r\n");
    Mimic_SendResponse("    Example: UART_INIT 1 9600 N 1\r\n");
    Mimic_SendResponse("  UART_SEND <1|6> <DATA>   - Send data\r\n");
    Mimic_SendResponse("  UART_RECV <1|6> <LEN> [TIMEOUT]\r\n");
    Mimic_SendResponse("  UART_STATUS              - Show UART status\r\n\r\n");
    
    Mimic_SendResponse("[System Commands]\r\n");
    Mimic_SendResponse("  HELP                     - Show this help\r\n");
    Mimic_SendResponse("  VERSION                  - Show version\r\n");
    Mimic_SendResponse("  STATUS                   - System status\r\n");
    Mimic_SendResponse("  RESET                    - Reset MCU\r\n\r\n");
    
    Mimic_SendResponse("[Short Commands]\r\n");
    Mimic_SendResponse("  PS=PIN_STATUS, PSO=PIN_SET_OUT, PSI=PIN_SET_IN\r\n");
    Mimic_SendResponse("  PH=PIN_HIGH, PL=PIN_LOW, PR=PIN_READ, PT=PIN_TOGGLE\r\n");
    Mimic_SendResponse("  UI=UART_INIT, US=UART_SEND, UR=UART_RECV\r\n\r\n");
}

/**
  * @brief  VERSION - Show firmware version
  */
void Mimic_CMD_VERSION(Mimic_Command_t *cmd)
{
    Mimic_SendResponseF("MIMIC Firmware v%s\r\n", MIMIC_VERSION);
    Mimic_SendResponse("Target: STM32F411VET6 Discovery\r\n");
    Mimic_SendResponseF("Built: %s %s\r\n", __DATE__, __TIME__);
}

/**
  * @brief  STATUS - Show system status
  */
void Mimic_CMD_STATUS(Mimic_Command_t *cmd)
{
    Mimic_SendResponse("\r\n=== System Status ===\r\n");
    Mimic_SendResponseF("Uptime: %lu ms\r\n", HAL_GetTick());
    Mimic_SendResponseF("Host UART: %lu baud\r\n", huart2.Init.BaudRate);
    
    /* GPIO clocks */
    Mimic_SendResponse("\r\nGPIO Clocks Enabled:\r\n");
    if (RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN) Mimic_SendResponse("  GPIOA ");
    if (RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN) Mimic_SendResponse("  GPIOB ");
    if (RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN) Mimic_SendResponse("  GPIOC ");
    if (RCC->AHB1ENR & RCC_AHB1ENR_GPIODEN) Mimic_SendResponse("  GPIOD ");
    if (RCC->AHB1ENR & RCC_AHB1ENR_GPIOEEN) Mimic_SendResponse("  GPIOE ");
    Mimic_SendResponse("\r\n");
}

/**
  * @brief  RESET - Software reset
  */
void Mimic_CMD_RESET(Mimic_Command_t *cmd)
{
    Mimic_SendResponse("Resetting...\r\n");
    HAL_Delay(100);
    NVIC_SystemReset();
}

/**
  * @brief  UART_POLL <INSTANCE> <MESSAGE> <COUNT> [DELAY_MS]
  *         Continuously send a message
  */
void Mimic_CMD_UART_POLL(Mimic_Command_t *cmd)
{
    if (cmd->argc < 3)
    {
        Mimic_SendResponse("Usage: UART_POLL <1|6> <MESSAGE> <COUNT> [DELAY_MS]\r\n");
        Mimic_SendResponse("  Example: UART_POLL 1 Hello 10 500\r\n");
        Mimic_SendResponse("  Sends 'Hello' 10 times with 500ms delay\r\n");
        Mimic_SendResponse("  Use COUNT=0 for infinite (until reset)\r\n");
        return;
    }
    
    UART_HandleTypeDef *huart = Mimic_GetUARTHandle(cmd->args[0]);
    if (huart == NULL)
    {
        Mimic_SendResponse("ERROR: Invalid UART instance\r\n");
        return;
    }
    
    if (huart->Instance == NULL)
    {
        Mimic_SendResponse("ERROR: UART not initialized. Use UART_INIT first.\r\n");
        return;
    }
    
    const char *message = cmd->args[1];
    uint32_t count = atoi(cmd->args[2]);
    uint32_t delay_ms = 500;  // default 500ms
    
    if (cmd->argc >= 4)
    {
        delay_ms = atoi(cmd->args[3]);
    }
    
    uint8_t len = strlen(message);
    uint8_t tx_buf[66];
    
    // Add newline to message
    snprintf((char*)tx_buf, sizeof(tx_buf), "%s\r\n", message);
    len = strlen((char*)tx_buf);
    
    Mimic_SendResponseF("Sending '%s' %lu times (delay: %lu ms)\r\n", 
                        message, count == 0 ? 0xFFFFFFFF : count, delay_ms);
    Mimic_SendResponse("(Reset MCU to stop if infinite)\r\n");
    
    uint32_t sent = 0;
    uint32_t infinite = (count == 0);
    
    while (infinite || sent < count)
    {
        HAL_UART_Transmit(huart, tx_buf, len, 100);
        sent++;
        
        if (!infinite && sent >= count)
            break;
            
        HAL_Delay(delay_ms);
    }
    
    Mimic_SendResponseF("OK: Sent %lu messages\r\n", sent);
}

/* ========================== END OF FILE =================================== */
