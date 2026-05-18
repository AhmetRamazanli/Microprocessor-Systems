// *Slave kodu*

#define SLAVE_ADDR 0x08

#define BUTTON_PIN PD2
#define LED_PIN PB5

void I2C_Slave_Init(uint8_t addr) {
  PORTC |= (1 << PORTC4) | (1 << PORTC5); // Daxili pull-up
  TWAR = (addr << 1); 
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | (1 << TWIE);
}

// TWI kəsilmə vektoru (Avtomatik olaraq jmp ilə idarə olunur)
ISR(TWI_vect) {
  uint8_t status = TWSR & 0xF8; 
  
  switch (status) {
    case 0x60: // SLA+W qəbul edildi
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
      
    case 0x80: // Master-dən data gəldi (Master Write -> Slave LED idarəetməsi)
      if (TWDR == 0x01) PORTB |= (1 << LED_PIN);
      else PORTB &= ~(1 << LED_PIN);
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
      
    case 0xA8: // SLA+R qəbul edildi (Master Read -> Slave məlumat göndərməlidir)
      // Slave-in düymə vəziyyətini oxu və Data Registerə yaz
      if (!(PIND & (1 << BUTTON_PIN))) TWDR = 0x01; 
      else TWDR = 0x00;
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;

    case 0xB8: // Data göndərildi, ACK alındı
    case 0xA0: // Stop/Repeated Start qəbul edildi
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
      
    default:
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
  }
}

void setup() {
  DDRD &= ~(1 << BUTTON_PIN); 
  PORTD |= (1 << BUTTON_PIN); 
  DDRB |= (1 << LED_PIN);     

  I2C_Slave_Init(SLAVE_ADDR);
}

void loop() {
  // Əsas dövrə boş qalır, hər şey TWI_vect interrupt-u ilə idarə olunur.
  // Bu yanaşma "non-blocking" arxitekturanın təməlidir.
}