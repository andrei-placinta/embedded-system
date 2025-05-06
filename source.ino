#define UBRR 16000000/16/9600-1 
#define TEMPERATURA 35
#define HISTEREZIS 0.5
//UBRR=fOSC/16/BAUD_RATE-1

enum {FADE_IN, ON, FADE_OUT, OFF} PWM = FADE_IN;
float temp;

void setup()
{
    DDRB = 0x3B;
    DDRD = 0xFC;
    DDRC = 0x00;
    Timer1_Init();
    Timer2_Init();
    USART_Init(UBRR);
    ADC_Init();
    sei(); // allow interrupts
}

void loop()
{
    Citire_Seriala();
    PWM_Blink();
    delay(1);
}

void Timer1_Init()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 15624;
    TCCR1B |= (1 << WGM12); // Mode CTC
    TCCR1B |= (1 << CS12) | (1 << CS10); //Prescaler 1024
    TIMSK1 |= (1 << OCIE1A); // Compare A Match Interrupt Enable
}

void Timer2_Init()
{
    TCCR2A = 0;
    TCCR2A |= (1 << COM2B1);
  // Clear OC2B on compare match when up-counting. 
  // Set OC2B on compare match when down-counting.
    TCCR2A |= (1 << WGM21)|(1 << WGM20);  // Mode Fast PWM
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); //Prescaler 1024
    OCR2B = 0;
}

void USART_Init(unsigned int ubrr)
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);//| (1 << RXCIE0
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

unsigned char USART_Receive(void)
{
  /* Wait for data to be received */
  while (!(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}


void ADC_Init()
{
  ADMUX  |= (1 << REFS0); //Setam ca referinta 5V.
  ADCSRA |= ((1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN)|(1 << ADSC)); 
  // Factor de divizare128 , Enable & Start Conversion
}

void PWM_Blink()
{
 	if(PWM == FADE_IN && TCNT1 % 61 == 0) OCR2B += 10;
    else if(PWM == FADE_OUT && TCNT1 % 61 == 0) OCR2B -= 10;
    else if(PWM == ON) OCR2B = 255;
    else if(PWM == OFF)OCR2B = 0;
}

void Initiale()
{
 
  if(PORTD & 0x08) PORTD = 0x08;
  else PORTD = 0;
  if(PORTB & 0x01) PORTB &= ~ 0x01;
  if(PORTB & 0x02) PORTB &= ~ 0x02;
  if(PORTB & 0x20)
  {
    PORTD |= 0x94;
    PORTB |= 0x03;
  }
  else
  {
    PORTD |= 0xB4;
    PORTB |= 0x03;
  }
}

void Citire_Seriala()
{
    unsigned char c = UDR0;
    if(c == 'A' || c == 'a')
      PORTB |= 0x10;
    else if(c == 'S' || c == 's')
      PORTB &= ~(0x10);

}
uint16_t ReadADC(uint8_t channel)
{
    ADMUX &= 0xF0;
    ADMUX |= channel;
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return ADCW;
}
  
void Temp()
{
    temp=(((ReadADC(0)*5000.0)/1023.0)-500.0)/10.f;
    Serial.println(temp);
    if(temp < TEMPERATURA - HISTEREZIS)
      PORTB &= ~(0x08);
    else if(temp > TEMPERATURA + HISTEREZIS)
      PORTB |= 0x08;   
}

ISR(TIMER1_COMPA_vect)
{
  PORTB ^= 0x20;
  Initiale();
  Temp();
  	if(PWM == FADE_IN)
      PWM = ON;
  	else if(PWM == FADE_OUT)
      PWM = OFF;
  	else if(PWM == ON)
      PWM = FADE_OUT;
  	else if(PWM == OFF) 
      PWM = FADE_IN;
}