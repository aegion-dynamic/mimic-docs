/*
 * STM32F411 UART Communication Firmware
 * Receives data on RX pin, echoes back on TX pin
 * Uses hardware USART1 (PA9=TX, PA10=RX) at 9600 baud
 * 
 * Can be easily modified for different pins/baud rates
 */

#include <stdint.h>
#include <string.h>

// STM32F411 Register definitions
#define RCC_BASE        0x40023800
#define RCC_AHB1ENR     (RCC_BASE + 0x30)
#define RCC_APB2ENR     (RCC_BASE + 0x44)

#define GPIOA_BASE      0x40020000
#define GPIOA_MODER     (GPIOA_BASE + 0x00)
#define GPIOA_AFRL      (GPIOA_BASE + 0x20)
#define GPIOA_AFRH      (GPIOA_BASE + 0x24)

#define USART1_BASE     0x40011000
#define USART1_SR       (USART1_BASE + 0x00)
#define USART1_DR       (USART1_BASE + 0x04)
#define USART1_BRR      (USART1_BASE + 0x08)
#define USART1_CR1      (USART1_BASE + 0x0C)
#define USART1_CR2      (USART1_BASE + 0x10)
#define USART1_CR3      (USART1_BASE + 0x14)

// UART Configuration
#define BAUD_RATE       9600
#define APB2_CLOCK      84000000  // STM32F411 APB2 clock

// Buffer size
#define BUFFER_SIZE     128

volatile char rx_buffer[BUFFER_SIZE];
volatile int rx_index = 0;

// Initialize USART1 on PA9 (TX) and PA10 (RX)
void uart_init(void) {
    volatile uint32_t *rcc_ahb1 = (uint32_t *)RCC_AHB1ENR;
    volatile uint32_t *rcc_apb2 = (uint32_t *)RCC_APB2ENR;
    
    // Enable GPIOA clock
    *rcc_ahb1 |= (1 << 0);
    
    // Enable USART1 clock
    *rcc_apb2 |= (1 << 4);
    
    // Configure PA9 (TX) and PA10 (RX) as alternate function 7
    volatile uint32_t *gpioa_moder = (uint32_t *)GPIOA_MODER;
    *gpioa_moder &= ~(0xF << 18);  // Clear PA9 and PA10
    *gpioa_moder |= (0xA << 18);   // Set to alternate function (10)
    
    // Set alternate function to USART1 (AF7)
    volatile uint32_t *gpioa_afrh = (uint32_t *)GPIOA_AFRH;
    *gpioa_afrh &= ~(0xFF << 4);   // Clear AFR for PA9 and PA10
    *gpioa_afrh |= (0x77 << 4);    // Set to AF7 (0111)
    
    // Configure USART1
    volatile uint32_t *usart_brr = (uint32_t *)USART1_BRR;
    *usart_brr = APB2_CLOCK / (16 * BAUD_RATE);
    
    // Enable TX and RX
    volatile uint32_t *usart_cr1 = (uint32_t *)USART1_CR1;
    *usart_cr1 = (1 << 13) |  // UE: USART enable
                 (1 << 3)  |  // TE: Transmitter enable
                 (1 << 2);    // RE: Receiver enable
}

// Send a character
void uart_send_char(char c) {
    volatile uint32_t *usart_sr = (uint32_t *)USART1_SR;
    volatile uint32_t *usart_dr = (uint32_t *)USART1_DR;
    
    // Wait for TX empty
    while (!(*usart_sr & (1 << 7)));
    
    // Send character
    *usart_dr = c;
}

// Send string
void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

// Receive a character (non-blocking)
int uart_recv_char(char *c) {
    volatile uint32_t *usart_sr = (uint32_t *)USART1_SR;
    volatile uint32_t *usart_dr = (uint32_t *)USART1_DR;
    
    if (*usart_sr & (1 << 5)) {  // RX not empty
        *c = *usart_dr;
        return 1;
    }
    return 0;
}

// Main program
int main(void) {
    uart_init();
    
    // Send startup message
    uart_send_string("[STM32] Ready at 9600 baud\n");
    
    while (1) {
        char c;
        if (uart_recv_char(&c)) {
            // Echo character back
            uart_send_char(c);
            
            // Store in buffer
            if (rx_index < BUFFER_SIZE - 1) {
                rx_buffer[rx_index++] = c;
            }
            
            // Process on newline
            if (c == '\n' || c == '\r') {
                rx_buffer[rx_index] = '\0';
                
                // Send response based on command
                if (strstr((char *)rx_buffer, "PING")) {
                    uart_send_string("[PONG]\n");
                } else if (strstr((char *)rx_buffer, "ID")) {
                    uart_send_string("[STM32F411-Discovery]\n");
                } else if (strstr((char *)rx_buffer, "STATUS")) {
                    uart_send_string("[OK] STM32 running\n");
                }
                
                // Reset buffer
                rx_index = 0;
                memset((void *)rx_buffer, 0, BUFFER_SIZE);
            }
        }
    }
    
    return 0;
}

// Minimal startup code
extern void main(void);

void Reset_Handler(void) {
    main();
    while (1);
}

// Minimal exception handlers
void NMI_Handler(void) { while (1); }
void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void) { while (1); }
void UsageFault_Handler(void) { while (1); }
void SVC_Handler(void) { while (1); }
void DebugMon_Handler(void) { while (1); }
void PendSV_Handler(void) { while (1); }
void SysTick_Handler(void) { while (1); }
