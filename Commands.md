[Project Idea]

The Mimic is all about to mimic different hardware peripherals in my fingertips.. 

I want to access the complete MCU by giving commands.. the stm32 contains firmware that parses all the commands and manuplates the hardware peripherals accordingly with some good advantages.

for example I want to send a particular data via uart with a particular frame format and baudrate.. rather than writing all the code for that I'll use this command interface to just do it.. and also get to know the status of each and every peripheral.. its speed, mode,otype, input and output.. 


[Commands Example]

[GPIO]
PIN_Status      PIN -> for showing the every status of the GPIO pin
PIN_Set_OUT     PIN -> for setting the pin as an output
PIN_Set_IN      PIN -> for setting the pin as an output
PIN_set_Mode    PIN -> setting the pin mode
PIN_set_Type    PIN -> setting the pin Type

[Commands for UART]
UART_INIT INSTANCE(UART1 | UART2 | UART6) BAUDRATE  -> for initializing the uart with Baudrate
UART_SEND INSTANCE DATA -> for sending the data via uart
UART_RECEIVE INSTANCE LENGTH -> for receiving the data via uart

[Commands for SPI]
SPI_INIT INSTANCE(1-5) MODE(MASTER|SLAVE) SPEED [CPOL] [CPHA] [DATASIZE] [BITORDER] -> Initialize SPI
  Example: SPI_INIT 1 MASTER 1000000
  Example: SPI_INIT 2 MASTER 500000 1 1 8 MSB
SPI_SEND INSTANCE HEX_DATA -> Send data via SPI (half-duplex)
  Example: SPI_SEND 1 A5 3C FF
SPI_RECV INSTANCE LENGTH [TIMEOUT] -> Receive data via SPI
  Example: SPI_RECV 1 4 500
SPI_TRANSFER INSTANCE HEX_DATA -> Full-duplex SPI transfer
  Example: SPI_TRANSFER 1 A5 3C FF
SPI_CS PIN STATE(HIGH|LOW) -> Control chip select pin
  Example: SPI_CS A4 LOW
SPI_STATUS -> Show status of all SPI peripherals

[Commands for I2C]



[Actual system] 
STM32F411VET6 Discovery board is used in the project as the main Controller.
Initially the UART2 (PA2, PA3) is used for communication with the host computer via USB to TTL converter. while others can be configured as per the user requirement.

[SPI Pin Mappings]
SPI1: PA5 (SCK), PA6 (MISO), PA7 (MOSI), PA4 (NSS)
SPI2: PB13 (SCK), PB14 (MISO), PB15 (MOSI), PB12 (NSS)
SPI3: PB3 (SCK), PB4 (MISO), PB5 (MOSI), PA15 (NSS)
SPI4: PE2 (SCK), PE5 (MISO), PE6 (MOSI), PE4 (NSS)
SPI5: PE12 (SCK), PE13 (MISO), PE14 (MOSI), PE11 (NSS)
