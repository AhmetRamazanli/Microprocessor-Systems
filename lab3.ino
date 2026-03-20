/**
 * Task 3: Fixed Synchronization Version
 * Ensures R16 is updated correctly after Reset and Power-up.
 */

void setup() {
  Serial.begin(9600);

  // --- MANUAL EEPROM READ ---
  while(EECR & (1 << EEPE)); 
  EEAR = 0;                  
  EECR |= (1 << EERE);       
  
  // CRITICAL STEP: Get the value from EEPROM and FORCE it into R16
  uint8_t start_val = EEDR; 
  asm volatile("mov r16, %0" : : "r" (start_val));

  Serial.println("--- System Restarted ---");
  Serial.print("Initial Value (Loaded from EEPROM): ");
  Serial.println(start_val);
}

void loop() {
  // 1. Wait 1 second first (to avoid skipping the first number)
  delay(1000); 

  // 2. Increment R16 directly
  asm volatile ("inc r16"); 

  // 3. Sync: Pull the ACTUAL value from R16 to a variable for printing
  uint8_t current_count;
  asm volatile ("mov %0, r16" : "=r" (current_count));

  Serial.print("Counter value: ");
  Serial.println(current_count);

  // --- SERIAL COMMAND HANDLING ---
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd == 'S' || cmd == 's') {
      // Save the REAL value currently in R16
      eeprom_write_manual(0, current_count); 
      Serial.println(">> Stored in EEPROM.");
    } 
    else if (cmd == 'R' || cmd == 'r') {
      // Reset R16 and EEPROM to 0
      asm volatile ("clr r16"); 
      eeprom_write_manual(0, 0); 
      Serial.println(">> Reset Complete.");
    }
  }
}

// Manual EEPROM Write Function
void eeprom_write_manual(unsigned int address, uint8_t data) {
  while(EECR & (1 << EEPE)); 
  EEAR = address; 
  EEDR = data;    
  
  // Atomic write sequence
  uint8_t sreg = SREG;
  asm volatile("cli"); 
  EECR |= (1 << EEMPE); 
  EECR |= (1 << EEPE); 
  SREG = sreg; 
}
