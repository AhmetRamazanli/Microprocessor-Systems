// *Master kodu*

#define SLAVE_ADDR 0x08

// Pin təyinatları
#define BUTTON_PIN PD2
#define LED_PIN PB5

unsigned long lastCheck = 0;

void I2C_Init(bool fastMode = false) {
  PORTC |= (1 << PORTC4) | (1 << PORTC5); // Daxili pull-up (A4, A5)
  TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // TWPS = 0
  
  if(fastMode) TWBR = 12; // 400 kHz
  else TWBR = 72;         // 100 kHz
  
  TWCR = (1 << TWEN);
}

void I2C_Start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

void I2C_Stop() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2C_Write(uint8_t data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

uint8_t I2C_Read_NACK() {
  // TWEA bitini sıfır saxlayaraq (NACK) oxuyuruq, çünki yalnız 1 bayt lazımdır
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}

void setup() {
  // Düymə giriş (Pull-up ilə), LED çıxış
  DDRD &= ~(1 << BUTTON_PIN); 
  PORTD |= (1 << BUTTON_PIN); 
  DDRB |= (1 << LED_PIN);     

  I2C_Init(false); // fastMode = true edərək 400kHz test edə bilərsən
}

void loop() {
  // Hər 50 ms-dən bir xətti yoxlayırıq (Non-blocking)
  if (millis() - lastCheck >= 50) {
    lastCheck = millis();

    // 1. Master Write: Master-in düymə vəziyyətini Slave-ə göndər (Active Low)
    uint8_t masterBtnState = !(PIND & (1 << BUTTON_PIN)) ? 0x01 : 0x00;
    
    I2C_Start();
    I2C_Write((SLAVE_ADDR << 1) | 0); // SLA+W
    I2C_Write(masterBtnState);
    I2C_Stop();

    // Qısa fasilə (xəttin stabilləşməsi üçün mikro-fasilə)
    asm volatile("nop"); 

    // 2. Master Read: Slave-dən onun düymə vəziyyətini oxu
    I2C_Start();
    I2C_Write((SLAVE_ADDR << 1) | 1); // SLA+R
    uint8_t slaveBtnState = I2C_Read_NACK();
    I2C_Stop();

    // Slave-dən gələn dataya əsasən Master LED-i idarə et
    if (slaveBtnState == 0x01) {
      PORTB |= (1 << LED_PIN);  // LED ON
    } else {
      PORTB &= ~(1 << LED_PIN); // LED OFF
    }
  }
}