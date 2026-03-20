#include <avr/io.h>
#include <avr/interrupt.h>

volatile int countdown = 0;
volatile int debounce_timer = 0; // Yeni basışı engelleme süresi (200 ms)

void setup_system() {
    cli();

    // 1. Port Ayarları (DDR: Data Direction Register)
    // LED (PB5 - Pin 13) Çıkış:
    DDRB |= (1 << DDB5);
    // Buton (PD2 - Pin 2) Giriş:
    DDRD &= ~(1 << DDD2);
    // Pull-up direnci aktif:
    PORTD |= (1 << PORTD2);

    // 2. Timer1 Ayarları (1ms CTC Modu)
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A  = 1999; 
    TCCR1B |= (1 << WGM12);  // CTC Modu
    TCCR1B |= (1 << CS11);   // Prescaler 8
    TIMSK1 |= (1 << OCIE1A); // Timer Kesmesi Aktif

    // 3. Harici Kesme (External Interrupt - INT0)
    // EICRA: Kesme tipi (Falling Edge: 10)
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    // EIMSK: INT0 kesmesini etkinleştir
    EIMSK |= (1 << INT0);

    sei();
}

// Butona basıldığında tetiklenen kesme (INT0)
ISR(INT0_vect) {
    // Sadece debounce_timer sıfırlandığında yeni basış kabul et
    if (debounce_timer == 0) { 
        PORTB |= (1 << PORTB5); 
        countdown = 10;         // LED 10ms yansın
        debounce_timer = 200;   // 200ms boyunca yeni kesme kabul etme
    }
}

// Her 1ms'de çalışan Timer kesmesi
ISR(TIMER1_COMPA_vect) {
    if (countdown > 0) {
        countdown--;
        if (countdown == 0) {
            PORTB &= ~(1 << PORTB5); // LED LOW (10ms dolunca)
        }
    }
    // Debounce süresini yönet (Her 1ms'de bir azalır)
    if (debounce_timer > 0) {
        debounce_timer--;
    }
}

int main(void) {
    setup_system();
    while (1) {
        // Döngü tamamen boş
    }
}
