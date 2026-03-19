#include <avr/io.h>
#include <EEPROM.h> // change ıt to manually!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

byte counter; // will use this variable for R16

void setup() {
  Serial.begin(9600);
  
  // Read the zeroth address of and EEPROM and transfer it to the counter
  counter = EEPROM.read(0); 
  
  Serial.println("System started with assembly logic...");
}

void loop() {
  // --- ASSEMBLY BLOGU ---
  asm volatile (
    "inc %[count] \n\t"    // increase the counter 1 (INC command) - 1 CPU cycle continuing
    : [count] "+r" (counter) // put the value of counter inside a register 
  );

  Serial.print("Counter value: ");
  Serial.println(counter);

  // wait a second
  // 16MHz hızında bu 16.000.000 döngü demektir
  delay(1000); 

  // Seri komut kontrolü ve EEPROM kaydı
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'S' || cmd == 's') {
      EEPROM.write(0, counter);
      Serial.println(">> saved to EEPROM. ");
    }
  }
}
