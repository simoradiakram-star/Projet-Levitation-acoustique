#include <avr/sleep.h>
#include <avr/power.h>

#define N_PATTERNS 2
#define N_PORTS 10
#define N_DIVS 10
#define PATTERN_PERIODS 400000  // nombre de périodes 40 kHz avant de passer au pattern suivant donc là on fait 500000*1/40000 donc 12.5s

#define WAIT() __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")

#define OUTPUT_WAVE(pointer, d)  \
  PORTA = pointer[d*N_PORTS + 0]; \
  PORTC = pointer[d*N_PORTS + 1]; \
  PORTL = pointer[d*N_PORTS + 2]; \
  PORTB = pointer[d*N_PORTS + 3]; \
  PORTK = pointer[d*N_PORTS + 4]; \
  PORTF = pointer[d*N_PORTS + 5]; \
  PORTH = pointer[d*N_PORTS + 6]; \
  PORTD = pointer[d*N_PORTS + 7]; \
  PORTG = pointer[d*N_PORTS + 8]; \
  PORTJ = pointer[d*N_PORTS + 9];


static const byte patterns[N_PATTERNS * N_DIVS * N_PORTS] = {

  
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,


   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x8F, 0x07, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

    };

static byte bufferA[N_DIVS * N_PORTS];
static byte bufferB[N_DIVS * N_PORTS];

void setup() {
  // --- Configuration des ports en sortie ---
  DDRA = DDRC = DDRL = DDRB = DDRK = DDRF = DDRH = DDRD = DDRG = DDRJ = 0xFF;
  PORTA = PORTC = PORTL = PORTB = PORTK = PORTF = PORTH = PORTD = PORTG = PORTJ = 0x00;

  //clear the buffers
  for (int i = 0; i < (N_PATTERNS * N_DIVS * N_PORTS); ++i) {
    bufferA[i] = 0;
    bufferB[i] = 0;
  }

  //initial pattern ==> sinus avec toute les sorties qui sont en phases
  for (int i = 0; i < (N_PORTS*N_DIVS/2); ++i){
     bufferA[i] = 0xFF;
  }

  // --- PWM 40 kHz sur pin 2 ---
  pinMode(2, OUTPUT);
  noInterrupts();
  TCCR3A = bit(WGM10) | bit(WGM11) | bit(COM1B1);
  TCCR3B = bit(WGM12) | bit(WGM13) | bit(CS10);
  OCR3A = (F_CPU / 40000L) - 3;
  OCR3B = (F_CPU / 40000L) / 2;
  interrupts();

  // --- Sync in pin ---
  pinMode(3, INPUT_PULLUP); // connectée à pin 2 pour synchro

  // --- Désactivation des périphériques inutiles ---
  ADCSRA = 0;
  power_adc_disable();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  power_usart1_disable();
  power_usart2_disable();
  power_usart3_disable();

  // L'idée est la suivante : on parcourt "patterns" par pas de N_Div * N_ports avec deux buffers comme avant, un lecture et un écriture
  // La variable de ce code est donc le timing que l'on souhaite entre chaque chgmt de pattern = "pattern_periods"

  bool useA = true;
  unsigned long currentPeriods = 0;
  byte currentPatternIndex = 0;  // index du pattern courant dans patterns

  byte* activeBuffer = bufferA;
  byte* nextBuffer   = bufferB;

  // copie initiale du premier pattern dans bufferA
  for(int i=0; i<N_PORTS*N_DIVS; i++)
      bufferA[i] = patterns[i];

  // pré-remplissage du bufferB avec le pattern suivant
  for(int i=0; i<N_PORTS*N_DIVS; i++)
      bufferB[i] = patterns[(currentPatternIndex + 1) * N_PORTS * N_DIVS + i];

  byte* emittingPointerH = &activeBuffer[0];
  byte* emittingPointerL = &activeBuffer[N_PORTS * N_DIVS / 2];

  LOOP:
    while (PINE & 0b00100000); // attente synchro

    OUTPUT_WAVE(emittingPointerH, 0); WAIT();
    OUTPUT_WAVE(emittingPointerH, 1); WAIT();
    OUTPUT_WAVE(emittingPointerH, 2); WAIT();
    OUTPUT_WAVE(emittingPointerH, 3); WAIT();
    OUTPUT_WAVE(emittingPointerL, 0); WAIT();
    OUTPUT_WAVE(emittingPointerL, 1); WAIT();
    OUTPUT_WAVE(emittingPointerL, 2); WAIT();
    OUTPUT_WAVE(emittingPointerL, 3); WAIT();

    // incrémente compteur de périodes
    currentPeriods++;
    if(currentPeriods >= PATTERN_PERIODS) {
        currentPeriods = 0;

        // switch des buffers
        useA = !useA;
        activeBuffer = useA ? bufferA : bufferB;
        nextBuffer   = useA ? bufferB : bufferA;

        // avance pattern
        currentPatternIndex++;
        if(currentPatternIndex >= N_PATTERNS) currentPatternIndex = 0;

        // met à jour pointeurs pour le buffer actif
        emittingPointerH = &activeBuffer[0];
        emittingPointerL = &activeBuffer[N_PORTS * N_DIVS / 2];

        // prépare le prochain pattern dans le buffer inactif
        int nextPatternIndex = currentPatternIndex + 1;
        if(nextPatternIndex >= N_PATTERNS) nextPatternIndex = 0;

        for(int i=0; i<N_PORTS*N_DIVS; i++)
            nextBuffer[i] = patterns[nextPatternIndex * N_PORTS * N_DIVS + i];
    }

    goto LOOP;
}

// Boucle Arduino vide (jamais utilisée)
void loop() {}