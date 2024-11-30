#include "stm32l476xx.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

void delay_ms(int milliseconds);
void setup_gpio(void);
void sos_signal(void);

// User HSI (high-speed internal) as the processor clock
void enable_HSI(){
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  //01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
}

void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
}

void turn_on_LED(){
	GPIOA->ODR |= 1 << LED_PIN;
}

void turn_off_LED(){
	GPIOA->ODR &= ~(1 << LED_PIN);
}

void toggle_LED(){
	GPIOA->ODR ^= (1 << LED_PIN);
}

void setup_gpio(void) {
    // GPIO saatlerini etkinlestir
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // GPIOA'yi etkinlestir (LED için)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // GPIOC'yi etkinlestir (Buton için)

    // LED (PA5) için GPIO ayarlari
    GPIOA->MODER &= ~(3UL << (5 * 2)); // PA5 için moderi sifirla
    GPIOA->MODER |= (1UL << (5 * 2));  // PA5 çikis moduna ayarla
    GPIOA->OTYPER &= ~(1UL << 5);      // PA5 push-pull olarak ayarla
    GPIOA->PUPDR &= ~(3UL << (5 * 2)); // PA5 no-pull olarak ayarla

    // Buton (PC13) için GPIO ayarlari
    GPIOC->MODER &= ~(3UL << (13 * 2)); // PC13 giris moduna ayarla
    GPIOC->PUPDR &= ~(3UL << (13 * 2)); // PC13 no-pull olarak ayarla
}

void sos_signal(void) {
    // SOS sinyali: DOT, DOT, DOT, DASH, DASH, DASH, DOT, DOT, DOT
    for (int i = 0; i < 3; i++) { // DOT (kisa)
        GPIOA->ODR |= (1UL << 5); // LED'i yak
        delay_ms(250);            // 0.25 saniye bekle
        GPIOA->ODR &= ~(1UL << 5); // LED'i söndür
        delay_ms(250);            // 0.25 saniye bosluk
    }

    for (int i = 0; i < 3; i++) { // DASH (uzun)
        GPIOA->ODR |= (1UL << 5); // LED'i yak
        delay_ms(500);            // 0.5 saniye bekle
        GPIOA->ODR &= ~(1UL << 5); // LED'i söndür
        delay_ms(250);            // 0.25 saniye bosluk
    }

    for (int i = 0; i < 3; i++) { // DOT (kisa)
        GPIOA->ODR |= (1UL << 5); // LED'i yak
        delay_ms(250);            // 0.25 saniye bekle
        GPIOA->ODR &= ~(1UL << 5); // LED'i söndür
        delay_ms(250);            // 0.25 saniye bosluk
    }

    // SOS sinyali tamamlandiktan sonra 1 saniye bekle
    delay_ms(1000);
}	

void delay_ms(int milliseconds) {
    // Basit bir gecikme fonksiyonu
    for (int i = 0; i < milliseconds * 4000; i++) {
        __NOP(); // No Operation (islemciyi mesgul et)
    }
}

int main(void){
	
	setup_gpio();

    while (1) {
        // Butona basilinca SOS sinyalini gönder
        if ((GPIOC->IDR & GPIO_IDR_IDR_13) == 0) { // PC13 butona basili mi kontrol et
            sos_signal();
        }
    }
	
	/*int i;
	enable_HSI();
	configure_LED_pin();
	turn_on_LED();
	
  // Dead loop & program hangs here
	while(1){
		for(i=0; i<1000000; i++); // simple delay
		toggle_LED();
	}*/
}
