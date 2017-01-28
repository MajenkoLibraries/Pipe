#include <Pipe.h>

Pipe txBuffer(1024, true);
Pipe rxBuffer(64);

volatile bool txRunning = false;

void __USER_ISR txIsr() {
    if (getIntFlag(_UART1_TX_IRQ)) {
        if (txBuffer.available()) {
            U1TXREG = txBuffer.read();
            txRunning = true;
        } else {
            txRunning = false;
        }
        clearIntFlag(_UART1_TX_IRQ);
    }
    if (getIntFlag(_UART1_RX_IRQ)) {
        rxBuffer.write(U1RXREG);
        clearIntFlag(_UART1_RX_IRQ);
    }
}

void kick() {
    if (txRunning == false) {
        U1TXREG = txBuffer.read();
        txRunning = true;
    }    
}

void setup() {
    Serial.begin(115200);
    txBuffer.onWrite(kick);
    setIntVector(_UART_1_VECTOR, txIsr);
    setIntPriority(_UART1_TX_IRQ, 4, 0);
    setIntPriority(_UART1_RX_IRQ, 4, 0);
    setIntEnable(_UART1_TX_IRQ);
    setIntEnable(_UART1_RX_IRQ);
}

void loop() {
    delay(1000);
    uint32_t start = micros();
    txBuffer.println("Buffered writing done through your own ISR. There is no reception here though, but you can add that yourself to the ISR.");
    uint32_t end = micros();
    uint32_t len = end - start;
    txBuffer.print("Time taken: ");
    txBuffer.print(len);
    txBuffer.println("us");

    while (rxBuffer.available()) {
        txBuffer.write(rxBuffer.read());
    }
}
