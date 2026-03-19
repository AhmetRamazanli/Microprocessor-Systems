extern "C" {
  void modeA_entry() __attribute__((used));
  void modeB_entry() __attribute__((used));
  void modeC_entry() __attribute__((used));
  // Task 5C Handler'ları
  void action_normal() __attribute__((used));
  void action_inverted() __attribute__((used));
  void action_dimmed() __attribute__((used));
  void action_reset() __attribute__((used));
}
volatile uint8_t current_action = 0;
void setup() {
  pinMode(13, OUTPUT);      
  pinMode(8, INPUT_PULLUP); 
  Serial.begin(9600);
}

void loop() {
  // 1. İLK DÜYMƏNİ GÖZLƏ (Assembly - Bloklayıcı)
  asm volatile (
    "wait_btn:       \n\t"
    "sbic %[pinb], %[bit]    \n\t" 
    "rjmp wait_btn   \n\t"
    :
    : [pinb] "I" (_SFR_IO_ADDR(PINB)), [bit]  "I" (PINB0)
  );
    PORTB |= (1 << PORTB5);  delay(200); 
    PORTB &= ~(1 << PORTB5); 
  // * Vacib Redaktə: İlk basışdan sonra düymənin buraxılmasını gözləyirik *
  while (!(PINB & (1 << PINB0))); // düymə basılıbsa (0-cı pin 0-sa), while-da gözlə (50 msan)
  delay(50); // Mexaniki sıçrayış (bounce) üçün kiçik gözləmə

  int pressCount = 0; // İlk basış artıq tam başa çatdı
  unsigned long startTime = millis();
  
  Serial.println("Taymer basladi! 2 saniyə ərzində əlavə seçimləri edin...");

  // 2. 2 SANİYƏLİK PƏNCƏRƏ (Əlavə basışları saymaq üçün)
  while (millis() - startTime < 2000) {
    if (!(PINB & (1 << PINB0))) { // düymə basılıbsa (0-cı pin 0-sa), if-ə daxil ol
      pressCount++;
      Serial.print("Cari say: "); Serial.println(pressCount);
      
      delay(200); // Debounce
      while (!(PINB & (1 << PINB0))); // Düymə buraxılana qədər gözlə
    }
  }

  Serial.print("Yekun say: "); Serial.println(pressCount);

  // 3. JMP İLƏ REJİMƏ KEÇİD
  if (pressCount == 1) {
    asm volatile ("jmp modeA_entry");
  } else if (pressCount == 2) {
    asm volatile ("jmp modeB_entry");
  } else if (pressCount >= 3) {
    asm volatile ("jmp modeC_entry");
  }
}

// --- TASK 5C: IJMP Mekanizması ---
void dispatch_action() {
  // Aksiyon fonksiyonlarının adreslerini içeren tablo
  static void* jump_table[] = { (void*)action_normal, (void*)action_inverted, (void*)action_dimmed, (void*)action_reset };
  
  void* target = jump_table[current_action];

  asm volatile (
    "movw r30, %[adr] \n\t" // Adresi Z register'ına (r31:r30) yükle
    "ijmp             \n\t" // Z register'ındaki adrese dolaylı atla
    :
    : [adr] "r" (target)
    : "r30", "r31"
  );
}

void check_button_action() {
  if (!(PINB & (1 << PINB0))) {
    current_action = (current_action + 1) % 4;
    delay(200); // Debounce
    while (!(PINB & (1 << PINB0)));
  }
}

// --- MODLAR VE HANDLERLAR ---

void modeA_entry() {
  while(1) {
    check_button_action();
    dispatch_action();
    _delay_ms(500); // Örnek hız
  }
}

void modeB_entry() {
  while(1) {
    check_button_action();
    dispatch_action();
    _delay_ms(200);
  }
}

void modeC_entry() {
  while(1) {
    check_button_action();
    dispatch_action();
    _delay_ms(50);
  }
}

// IJMP ile gidilecek aksiyon fonksiyonları
void action_normal()   { PORTB ^= (1 << PORTB5); } 
void action_inverted() { PORTB ^= (1 << PORTB5); } // Mantıksal fark loop içinde kurulabilir
void action_dimmed()   { PORTB |= (1 << PORTB5); _delay_ms(1); PORTB &= ~(1 << PORTB5); }
void action_reset()    { current_action = 0; }

