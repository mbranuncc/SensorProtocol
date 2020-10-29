/*********************************************************************************
 *  Arduino Serial Interrupt Example
 *  http://umeshdutta.blogspot.com/2018/07/serial-receive-interrupt-programming-in.html
 *  http://www.cplusplus.com/reference/vector/vector/vector/
 *  https://github.com/janelia-arduino/Vector <- Must be installed through Manage Libraries before compilation
 *  https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#include <Vector.h>
using namespace std;

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD - 1

#define ELEMENT_COUNT 127

#define LED 13

char temp;
char storage_array[ ELEMENT_COUNT ];
Vector<char> buf( storage_array );
bool Ready2Print = false;

void Timer1_Init( unsigned int hz )
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = FOSC/256/hz;

  TCCR1B |= ( 1 << WGM12 ); // CTC mode
  TCCR1B |= ( 1 << CS12 ); // 256 prescaler
  TIMSK1 |= ( 1 << OCIE1A ); // enable timer compare interrupt

  interrupts();
}

void USART_Init( unsigned int ubrr )
{
  noInterrupts();
  
  UBRR0 = ubrr;
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); 
  // Use 8-bit character sizes
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  
  // Turn on the transmission, reception, and Receive interrupt 
   
  interrupts();// enable global interrupt
}

void USART_Transmit( char data ) 
{
  while( !( UCSR0A & (1<<UDRE0)) );

  UDR0 = data;
}

void setup() {
  // put your setup code here, to run once:
  USART_Init( MYUBRR );
  
  pinMode( LED, OUTPUT );
  Timer1_Init( 1 );
}

void loop() {
  // put your main code here, to run repeatedly:
  if( Ready2Print ) {
    
    for (auto ir = buf.begin(); ir != buf.end(); ++ir)
      USART_Transmit( *ir );
      
    buf.clear();
    Ready2Print = false;
  }
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(LED, !digitalRead(LED));   // toggle LED pin
}

ISR(USART_RX_vect)
{ 
  temp=UDR0;// read the received data byte in temp
  buf.push_back( temp );
  if( strcmp( temp, '\n' ) == 0 ) Ready2Print = true;
}
