#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void) {
    // MOSI, SCK və SS pinlərini çıxış (Output) edirik
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
    
    // SPI-ı aktivləşdiririk (SPE), Master rejimini seçirik (MSTR)
    // Sürəti Fosc/16 təyin edirik (SPR0)    SPE ile SPI donanimi aktif, MSTR master edir SPR0 16da bir saat hizi
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_MasterTransmit(uint8_t data) {
    // Məlumatı data registrinə yazırıq, ötürülmə avtomatik başlayır
    // Verinin hatta gönderilmek üzere yazıldığı kaydedicidir. İşlemci, 
    //siz bu kaydediciye veriyi yazdığınız anda otomatik olarak saat sinyali (SCK) üretmeye 
    //ve veriyi MOSI pininden bit bit göndermeye başlar.
    SPDR = data;
    
    // Ötürülmə bitənə qədər gözləyirik (SPIF bayrağı 1 olana qədər) 
    //status register kontrol edilir
    while(!(SPSR & (1 << SPIF)));
}

int main(void) {
    SPI_MasterInit();
    
    uint8_t values[] = {85, 170, 255};
    uint8_t i = 0;

    while(1) {
        // SS-i LOW edirik (Slave-i seçirik)
        PORTB &= ~(1 << PORTB2);
        
        SPI_MasterTransmit(values[i]);
        
        // SS-i HIGH edirik (Ötürülmə bitdi)
        PORTB |= (1 << PORTB2);
        
        i++;
        if(i > 2) i = 0; // Ardıcıllığı başa qaytar
        
        _delay_ms(1000); // 1 saniyə gözlə
    }
}