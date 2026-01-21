/* To compile this file correctly you will need to:

*** add the following inline functions in the public interface (after "public:") of Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h
inline bool _dataAvailable() {return _rx_buffer_head != _rx_buffer_tail; }
inline byte _peekData() { return _rx_buffer[_rx_buffer_tail]; }
inline void _discardByte() { _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE; }

*** If you are using an IDE >1.6.X you will need to compile with -O3 (optimization speed), in some cases this does not work and you are forced to compile with 1.6. The binaries are also avaialable.
*** Comment the line power_usart3_disable(); if it gives you an error.

*/

#include <avr/sleep.h>
#include <avr/power.h>

#define N_PATTERNS 32
#define N_TRANSDUCTEUR 80
#define N_PORTS 10
#define N_DIVS 10
#define N_T 8

//ports A C L B K F H D G J

#define WAIT(a) __asm__ __volatile__ ("nop")
#define OUTPUT_WAVE(pointer, d)  PORTA = pointer[d*N_PORTS + 0]; PORTC = pointer[d*N_PORTS + 1]; PORTL = pointer[d*N_PORTS + 2]; PORTB = pointer[d*N_PORTS + 3]; PORTK = pointer[d*N_PORTS + 4]; PORTF = pointer[d*N_PORTS + 5]; PORTH = pointer[d*N_PORTS + 6];  PORTD = pointer[d*N_PORTS + 7]; PORTG = pointer[d*N_PORTS + 8]; PORTJ = pointer[d*N_PORTS + 9]

static byte buffer[ N_DIVS * N_PORTS];

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

//initial pattern ==> sinus avec toute les sorties qui sont en phases
  for (int i = 0; i < (N_PORTS*N_DIVS/2); ++i){
     buffer[i] = 0xFF;
  }
  
  
  // generate a sync signal of 40khz in pin 2
  pinMode (2, OUTPUT);
  noInterrupts();           // disable all interrupts
  TCCR3A = bit (WGM10) | bit (WGM11) | bit (COM1B1); // fast PWM, clear OC1B on compare
  TCCR3B = bit (WGM12) | bit (WGM13) | bit (CS10);   // fast PWM, no prescaler
  OCR3A =  (F_CPU / 40000L) - 5; //should only be -1 but fine tunning with the scope determined that -5 gave 40kHz almost exactly
  OCR3B = (F_CPU / 40000L) / 2;
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

  Serial.begin(115200);

//--------ecriture-du-buffer---------------------------

  static float Phi=[N_TRANSDUCTEUR];
  for (int i=0;i<N_TRANSDUCTEUR;i++){
    Phi[i]=0;
  }

//creation d'une liste (de liste des valeur binaire) que chaque transducteur doit prendre 64*10

  int oct_T[N_TRANSDUCTEUR][N_DIVS];
  for (int i=0; i<N_TRANSDUCTEUR, i++){
    for (int j=0;j<N_DIVS;j++){
      int v = 0;
      float angle = 2*PI*j/N_DIVS + Phi[i];
      float value = sin (angle);
      if (value<0){oct_T[i][j]=0}
      else{oct_T[i][j]=1}
    }
  }
  int oct_P[N_DIVS][N_PORTS][N_TRANSDUCTEUR/N_PORTS]
  for (int j=0;j<N_DIVS;j++){
    for (int i=0; i<N_PORTS, i++){
      for (int k=0; k<N_T ; k++){
        oct_P[j][i].append(oct_T[i*N_T+k][j]);
      }
    }
  }
  for (int i=0;i<N_DIVS;i++){
    for (int j=0;j<N_PORTS;j++){
      byte value =0;
      for (int k=0;k<8;k++){
        value = (value << 1) | (oct_P[i][j][k] & 1);
      }
      buffer[i*N_DIVS+j]=value;
    }
  }
  


//initialisation des variables
  bool byteReady = false;

  bool emittingA = true;
  byte* emittingPointerH = & buffer[0]; //code les instant O à T/2 (chaqu'un contient donc 5 instants)
  byte* emittingPointerL = & buffer[N_PORTS * N_DIVS / 2]; //code les instant T/2 à T


//------boucle------------------------

LOOP:
  while (PINE & 0b00100000); //wait for pin 3 (E5 Sync In) to go low

  OUTPUT_WAVE(emittingPointerH, 0); byteReady = Serial._dataAvailable(); 
  OUTPUT_WAVE(emittingPointerH, 1); bReceived = Serial._peekData(); 
  OUTPUT_WAVE(emittingPointerH, 2);
  OUTPUT_WAVE(emittingPointerH, 3); 
  OUTPUT_WAVE(emittingPointerH, 4); 
  OUTPUT_WAVE(emittingPointerL, 0);
  OUTPUT_WAVE(emittingPointerL, 1);
  OUTPUT_WAVE(emittingPointerL, 2);
  OUTPUT_WAVE(emittingPointerL, 3);
  OUTPUT_WAVE(emittingPointerL, 4); 
  
  goto LOOP;

}

void loop() {}

