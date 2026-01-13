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

[Commands for I2C]



[Actual system] 
STM32F411VET6 Dsicovery board is used in the project as the main Controller.
Initially the UART2 (PA2, PA3) is used for communication with the host computer via USB to TTL converter. while others can be configured as per the user requirement.
