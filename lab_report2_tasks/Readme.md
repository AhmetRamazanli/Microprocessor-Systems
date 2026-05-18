TASK 6
Using register-level programming and parallel I/O, configure the microcontroller’s hardware timer to drive a single-digit 7-segment display. The display should count from 0 to 9, updating once every 0.5 seconds. A push button connected to an external interrupt should pause the counter when pressed and resume it when pressed again. After the display reaches 9, it resets.
Questions to think about:- How can you be confident that it is counting in precise 0.5-second increments? How did you calculate this?- Does the counter resume exactly where it left off?

TASK 7
Your goal is to use two ATmega328P Arduinos to demonstrate basic SPI communication at the register level. One Arduino must act as the Master and the other as the Slave. The Master must send a single decimal value (0–255) once every second through SPI, and the Slave must correctly receive that value and display it on the Serial Monitor. You must configure all SPI settings using the SPCR, SPSR, SPDR, DDRB, and PORTB registers only.
The Master must transmit a sequence of three specific decimal values every second in the following order:
85 → 170 → 255
repeat

Evidence how can you show what is happening with SS, SCK, and MOSIDraw a timing diagram for one of the decimal values showing how the digital signals (SS, SCLK, MOSI, MISO) change over time.

TASK 8
Set up two Arduino boards communicating via I2C (one master, one slave), each with a push button and LED; pressing Button A should send a message to Board B to light its LED, and pressing Button B should send its state back to Board A to light its LED.
Explain your message protocol design in your report. Then, explore how clock speed affects the I2C signals by modifying the TWI clock registers (TWBR and prescaler bits in TWSR) to test at least two different speeds. Use an oscilloscope to observe the SDA and SCL waveforms at each speed and document the timing characteristics (bit period, rise/fall times, signal quality, and any ringing or noise).

Discuss how speed changes affect the observed waveforms, any timing violations or signal degradation you encounter, and the practical limits of increasing clock speed on your specific hardware setup
→ include oscilloscope screenshots and explain what constraints (capacitance, trace length, pull-up resistors, or device slew rates) are likely responsible for any limitations you observe.
