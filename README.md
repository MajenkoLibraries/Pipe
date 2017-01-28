Pipe
====

A virtual Arduino `Stream` device.

The Pipe class implements the Arduino `Stream` class to provide a virtual
device through which you can transfer data. You "send" data to it through the
standard Arduino `print()`, `println()` etc methods, and you can read from it
in the same way you read from `Serial`.

It's like having a serial port that talks to itself (*loopback*) without having
an actual serial port there.

The included example sketch, aimed at running on a chipKIT UNO32, implements a
buffered UART interface to the Serial device. It takes over the normal interrupt
vector of UART1 (mapped, on the UNO32, to `Serial`) and implements its own buffering
strategy for transmission and reception. Each direction has its own `Pipe` for transferring
the data to/from the ISR. Writing to the UART has write "blocking" turned on (when the pipe
is full the program waits for data to be read from the other end - only useful for
interrupts) so you don't lose data. 
