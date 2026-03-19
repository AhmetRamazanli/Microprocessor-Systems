// --- Method 1: Standard Blink using digitalWrite ---
/*
void setup() {
  // Set Pin 13 as an output
  pinMode(13, OUTPUT);  
}

void loop() {
  // Turn on the LED (executes in ~50-60 cycles)
  digitalWrite(13, HIGH);
  // Add 500ms delay for 1 Hz frequency (remove for high-speed testing)
  delay(500);    
  
  // Turn off the LED (executes in ~50-60 cycles)      
  digitalWrite(13, LOW);  
  delay(500);
}
*/

// --- Method 2: Fast Blink using Register Level Programming (Direct Port Access) ---
void setup() {
  // DDRB (Data Direction Register B)
  // Set the 5th bit to 1 to configure Pin 13 as an output
  DDRB |= (1 << 5); 
}

// Version for 1 Hz blinking:
void loop() {
  // Set the 5th bit of PORTB to 1 (LED ON) - executes in 1-2 cycles
  PORTB |= (1 << 5);  
  delay(500); // 1 Hz delay (remove for testing maximum speed)
  
  // Set the 5th bit of PORTB to 0 (LED OFF) - executes in 1-2 cycles
  PORTB &= ~(1 << 5);
  delay(500);
}
/*
// --- High-Speed Testing (Oscilloscope Test) ---
// Use this version to measure maximum frequency by eliminating loop overhead
void loop() {
  while(1) {
    PORTB |= (1 << 5);  // LED ON
    PORTB &= ~(1 << 5); // LED OFF
  }
}
*/