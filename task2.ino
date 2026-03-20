void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for the serial port to be ready

  // Scenario: 5 - 8 = -3 (253 in Unsigned world)
  uint8_t n1 = 5; 
  uint8_t n2 = 8;
  uint8_t res;
  uint8_t sreg_val;

  // --- ASM (Assembly) Block ---
  asm volatile (
    "mov r20, %2      \n\t" // n1 -> r20
    "mov r21, %3      \n\t" // n2 -> r21
    "sub r20, r21     \n\t" // r20 = r20 - r21
    "in  %1, 0x3F     \n\t" // Read SREG value (0x3F = SREG address)
    "mov %0, r20      \n\t" // Transfer result to 'res' variable
    : "=r" (res), "=r" (sreg_val)           // Outputs (%0, %1)
    : "r" (n1), "r" (n2)                    // Inputs (%2, %3)
    : "r20", "r21"                          // Clobbered registers
  );

  // --- Printing Outputs ---
  Serial.println("--- AVR SUBTRACTION ANALYSIS ---");
  Serial.print("Num1: "); Serial.println(n1);
  Serial.print("Num2: "); Serial.println(n2);
  Serial.print("Result (Hex): 0x"); Serial.println(res, HEX);
  Serial.print("SREG (Bin): "); Serial.println(sreg_val, BIN);

  // Extracting flags using Bit Mask
  bool C = (sreg_val & (1 << 0)); // Carry Flag (bit 0)
  bool Z = (sreg_val & (1 << 1)); // Zero Flag (bit 1)

  Serial.println("\n--- FLAG STATUS ---");
  Serial.print("Zero (Z): "); Serial.println(Z);
  Serial.print("Carry (C): "); Serial.println(C);
}

void loop() {}