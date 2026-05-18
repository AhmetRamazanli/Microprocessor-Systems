#include <avr/io.h>

void SPI_SlaveInit(void) {
    // Yalnız MISO pini çıxış (Output) olmalıdır
    DDRB |= (1 << DDB4);
    
    // SPI-ı aktivləşdiririk (SPE), Master biti 0 qalır (Slave rejimi)
    SPCR = (1 << SPE);
}

uint8_t SPI_SlaveReceive(void) {
    // Məlumat tam qəbul olunana qədər gözləyirik
    while(!(SPSR & (1 << SPIF)));
    
    // Qəbul edilmiş məlumatı qaytarırıq
    return SPDR;
}

int main(void) {
    Serial.begin(9600); // Nəticəni görmək üçün
    SPI_SlaveInit();
    
    while(1) {
        uint8_t receivedData = SPI_SlaveReceive();
        Serial.print("Qebul edilen: ");
        Serial.println(receivedData);
    }
}