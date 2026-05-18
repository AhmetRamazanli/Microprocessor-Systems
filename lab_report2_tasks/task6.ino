#include <avr/io.h>
#include <avr/interrupt.h>

// 7-seqment üçün rəqəm maskaları (Ümumi Katod)
// PD3-PD7 (a-e), PB0-PB1 (f-g)
uint8_t segment_PD[] = {0xF8, 0x30, 0xD8, 0x78, 0x30, 0x68, 0xE8, 0x38, 0xF8, 0x78};
uint8_t segment_PB[] = {0x01, 0x00, 0x02, 0x02, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03};
/* Qeyd: Yuxarıdakı bit maskaları qoşulma sırasına görədir:
Rəqəm 0: abcdef (g sönülü) -> PD3-7 yandır, PB0 yandır, PB1 söndür.
Rəqəm 1: bc (digərləri sönülü) -> PD4-5 yandır, PB0-1 söndür.
və s.
*/

volatile uint8_t counter = 0;
volatile bool isPaused = false;

void setup() {
  // Port konfiqurasiyası: D3-D7 çıxış (PORTD), D8-D9 çıxış (PORTB)
  DDRD |= 0xF8; // 11111000 -> PD3, PD4, PD5, PD6, PD7 çıxış
  DDRB |= 0x03; // 00000011 -> PB0, PB1 çıxış

  // Düymə konfiqurasiyası: PD2 giriş (D2)
  DDRD &= ~(1 << DDD2);   
  PORTD |= (1 << PORTD2); // Daxili pull-up aktivləşdirilir

  // Timer1 Konfiqurasiyası (CTC rejimi)
  TCCR1A = 0;             // Normal rejim
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC rejim və 1024 prescaler
  OCR1A = 7811;           // 0.5 saniyə üçün müqayisə dəyəri
  TIMSK1 |= (1 << OCIE1A); // Müqayisə kəsilməsini aktiv et

  // Xarici Kəsilmə (INT0) - Düymə üçün
  EICRA |= (1 << ISC01);  // Düşən kənar (Falling edge) kəsilməsi
  EIMSK |= (1 << INT0);   // INT0 kəsilməsini aktiv et

  sei(); // Qlobal kəsilmələri aktiv et
}

void loop() {
  // Əsas dövr boş qalır, hər şey kəsilmələrlə idarə olunur
}

// Timer1 kəsilmə funksiyası (0.5 saniyədən bir işləyir)
ISR(TIMER1_COMPA_vect) {
  if (!isPaused) {
    // Cari rəqəmi ekrana çıxar
    updateDisplay(counter);
    
    // Sayğacı artır
    counter++;
    if (counter > 9) counter = 0;
  }
}

// Düymə kəsilmə funksiyası (PD2-yə basıldıqda)
ISR(INT0_vect) {
  // Pauza və ya davam etmə vəziyyətini dəyiş
  isPaused = !isPaused;
  
  // Sadə debouncing üçün kiçik gecikmə (register səviyyəsində proqramda loop daxilində də edilə bilər)
}

// Portlara məlumatı yazan funksiya
void updateDisplay(uint8_t num) {
  // Əvvəlki seqmentləri söndür və yenilərini rəqəmə uyğun yandır
  PORTD = (PORTD & 0x07) | segment_PD[num];
  PORTB = (PORTB & 0xFC) | segment_PB[num];
}