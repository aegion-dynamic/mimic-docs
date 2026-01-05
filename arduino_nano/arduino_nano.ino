/*
 * Arduino Nano - UART Communication Test Sketch
 * This sketch receives data on RX, processes it, and sends responses on TX
 * 
 * Arduino Nano Pinout:
 * RX (D0) - Receives data from STM32 TX
 * TX (D1) - Sends data to STM32 RX
 * 
 * Baud Rate: 9600 (configurable)
 */

#define BAUD_RATE 9600
#define BUFFER_SIZE 128

char rxBuffer[BUFFER_SIZE];
int rxIndex = 0;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(100);
  
  // Send startup message
  Serial.println("[ARDUINO] Ready at 9600 baud");
  delay(100);
  
  // Send test messages continuously
  for(int i = 0; i < 5; i++) {
    Serial.print("[TEST] Message ");
    Serial.println(i);
    delay(500);
  }
}

void loop() {
  // Read incoming data
  if (Serial.available() > 0) {
    char byte = Serial.read();
    
    // Echo the character back immediately
    Serial.print(byte);
    
    // Store in buffer
    if (rxIndex < BUFFER_SIZE - 1) {
      rxBuffer[rxIndex++] = byte;
    }
    
    // Process complete message on newline
    if (byte == '\n' || byte == '\r') {
      rxBuffer[rxIndex] = '\0'; // Null terminate
      
      // Process command
      processCommand(rxBuffer);
      
      // Reset buffer
      rxIndex = 0;
      memset(rxBuffer, 0, BUFFER_SIZE);
    }
  }
  
  delay(10);
}

void processCommand(char* cmd) {
  // Remove trailing newline/carriage return
  int len = strlen(cmd);
  if (len > 0 && (cmd[len-1] == '\n' || cmd[len-1] == '\r')) {
    cmd[len-1] = '\0';
  }
  
  // Simple test responses
  if (strstr(cmd, "PING")) {
    Serial.println("[PONG]");
  } 
  else if (strstr(cmd, "ID")) {
    Serial.println("[ARDUINO-NANO-v1]");
  }
  else if (strstr(cmd, "STATUS")) {
    Serial.println("[OK] Arduino running normally");
  }
  else if (strstr(cmd, "LED")) {
    Serial.println("[LED] Command received");
    // Could control onboard LED here
  }
  else if (strstr(cmd, "DATA")) {
    Serial.print("[DATA] Received: ");
    Serial.println(cmd);
  }
  else {
    Serial.print("[ECHO] ");
    Serial.println(cmd);
  }
}
