#include <avr/sleep.h>
#include <avr/power.h>

#define N_PATTERNS 32
#define N_TRANSDUCTEUR 80
#define N_PORTS 10
#define N_DIVS 10
#define N_T 8

//ports A C L B K F H D G J

#define WAIT() __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")
#define OUTPUT_WAVE(pointer, d)  PORTA = pointer[d*N_PORTS + 0]; PORTC = pointer[d*N_PORTS + 1]; PORTL = pointer[d*N_PORTS + 2]; PORTB = pointer[d*N_PORTS + 3]; PORTK = pointer[d*N_PORTS + 4]; PORTF = pointer[d*N_PORTS + 5]; PORTH = pointer[d*N_PORTS + 6];  PORTD = pointer[d*N_PORTS + 7]; PORTG = pointer[d*N_PORTS + 8]; PORTJ = pointer[d*N_PORTS + 9]

static byte buffer[ N_DIVS * N_PORTS];
volatile uint8_t sync_cnt = 0;  // compteur de fronts sur pin 3 (INT5)

// Attente robuste de N ticks (gère le wrap 0..255 et les ticks manqués)
static inline void wait_ticks(uint8_t n, volatile uint8_t* last_seen) {
  uint8_t start = *last_seen;
  while ( (uint8_t)(sync_cnt - start) < n ) { /* spin, interruptions actives */ }
  *last_seen = sync_cnt;
}

static inline void wait_substep(volatile uint16_t* tref, uint16_t ticks)
{
  uint16_t start = *tref;
  while ((uint16_t)(TCNT4 - start) < ticks) {
    // attente active, base de temps = Timer4
  }
  *tref = start + ticks;
}

void setup()
{
  //set as output ports A C L B K F H D G J
  DDRA = DDRC = DDRL = DDRB = DDRK = DDRF = DDRH = DDRD = DDRG = DDRJ = 0xFF;
  //low signal on all of them
  PORTA = PORTC = PORTL = PORTB = PORTK = PORTF = PORTH = PORTD = PORTG = PORTJ = 0x00;

  //clear the buffers
  for (int i = 0; i < (N_PATTERNS * N_DIVS * N_PORTS); ++i) {
    buffer[i] = 0;
  }
  
  // INT5 (PE5 = pin 3 sur Mega) sur FRONT DESCENDANT
  EICRB |= (1 << ISC51);   // ISC51:50 = 10 => front descendant
  EICRB &= ~(1 << ISC50);
  EIFR   = (1 << INTF5);   // clear tout pending
  EIMSK |= (1 << INT5);    // enable INT5

  // generate a sync signal of 40khz in pin 2
  pinMode (2, OUTPUT);
  noInterrupts();           // disable all interrupts
  TCCR3A = bit (WGM10) | bit (WGM11) | bit (COM1B1); // fast PWM, clear OC1B on compare
  TCCR3B = bit (WGM12) | bit (WGM13) | bit (CS10);   // fast PWM, no prescaler
  OCR3A =  (F_CPU / 80000L) - 1; //should only be -1 but fine tunning with the scope determined that -5 gave 40kHz almost exactly
  OCR3B = (F_CPU / 80000L) / 2;
  TCCR4A=0;
  TCCR4B=(1<<CS40);
  TCNT4=0;

  interrupts();             // enable all interrupts

  //sync in signal at pin 3
  pinMode(3, INPUT_PULLUP); //please connect pin3 to pin 2


  // disable everything that we do not need
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  power_usart1_disable();
  power_usart2_disable();
  power_usart3_disable();
  //power_usart0_disable();
  

//creation d'une liste (de liste des valeur binaire) que chaque transducteur doit prendre 64*10
  byte buffer[N_DIVS * N_PORTS] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0x42, 0x32, 0xCB, 0x4D, 0x5A, 0xBD, 0x10, 0x82, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xBD, 0xCD, 0x34, 0xB2, 0xA5, 0x42, 0x6B, 0x0D, 0x07, 0x02
};


//initialisation des variables
  byte* emittingPointerH = & buffer[0]; 
  byte* emittingPointerL = & buffer[N_PORTS * N_DIVS / 2];
  
  Serial.begin(115200);
  while(!Serial.available());
  //Serial.read();
  Serial.write(0x55);


//------boucle------------------------

LOOP:
{
  uint8_t last = sync_cnt;   // point de départ calé sur l’horloge externe

  // Exemple : 1 front (= 25 µs si 40 kHz) entre chaque OUTPUT_WAVE
  wait_ticks(1,&last);
  OUTPUT_WAVE(emittingPointerH, 0); WAIT();
  OUTPUT_WAVE(emittingPointerH, 1); WAIT();
  OUTPUT_WAVE(emittingPointerH, 2); WAIT();
  OUTPUT_WAVE(emittingPointerH, 3); WAIT();
  OUTPUT_WAVE(emittingPointerH, 4); WAIT();
  OUTPUT_WAVE(emittingPointerL, 0); WAIT();
  OUTPUT_WAVE(emittingPointerL, 1); WAIT();
  OUTPUT_WAVE(emittingPointerL, 2); WAIT();
  OUTPUT_WAVE(emittingPointerL, 3); WAIT();
  OUTPUT_WAVE(emittingPointerL, 4); WAIT();


  goto LOOP;
}
}

void loop() {}

ISR(INT5_vect) {
  sync_cnt++;  // 1 tick = 1 front reçu depuis le maître
}
