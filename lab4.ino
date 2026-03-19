#include <avr/io.h>
#include <avr/interrupt.h>

// Değişkeni kesme içinde kullanacağımız için volatile tanımlıyoruz
volatile int milisecond = 0; 

const int ledPin = 13;   // Mavi kablo
const int buttonPin = 2;  // Kırmızı kablo

void setup() {
  pinMode(ledPin, OUTPUT);
  // INPUT_PULLUP buton boştayken sinyalin '1' kalmasını sağlar
  pinMode(buttonPin, INPUT_PULLUP); 

  // --- Timer1 Ayarları (Her 1ms'de bir kesme üretmek için) ---
  cli();                      // Ayar yaparken kesmeleri durdur
  TCCR1A = 0;                 // Kayıtçıları sıfırla
  TCCR1B = 0;
  TCNT1  = 0;                 
  
  // 16MHz işlemci hızıyla 1ms elde etmek için gereken değer
  OCR1A = 1999; 
  
  TCCR1B |= (1 << WGM12);      // CTC Modu aktif
  TCCR1B |= (1 << CS11);       // 8 bölücü (prescaler)
  TIMSK1 |= (1 << OCIE1A);     // Zamanlayıcı kesmesini etkinleştir

  // --- Dış Kesme Ayarı (D2 pini için) ---
  // FALLING: Butona basılıp sinyal 1'den 0'a düştüğü an tetiklenir
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonclicked, FALLING);
  
  sei();                      // Kesmeleri başlat
}

unsigned long last_click = 0; // Sıçramayı engellemek için zaman tutucu

void buttonclicked() {
  unsigned long now = millis();
  
  // Eğer son basıştan bu yana 200ms geçmediyse bu basışı görmezden gel
  if (now - last_click > 200) { 
    digitalWrite(ledPin, HIGH);
    milisecond = 10;
    last_click = now; // Zamanı güncelle
  }
}

// Zamanlayıcı (Timer1) her 1ms'de bir bu bloğu çalıştırır
ISR(TIMER1_COMPA_vect) {
  if (milisecond > 0) {
    milisecond--;       // Sayacı 1 azalt
    
    if (milisecond == 0) {
      digitalWrite(ledPin, LOW); // 10ms dolunca LED'i söndür
    }
  }
}

void loop() {
  // Ana döngü boş. Her şey arka planda (kesmelerle) halloluyor.
}
