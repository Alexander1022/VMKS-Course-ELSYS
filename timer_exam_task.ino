#define BTN_PIN 2
volatile uint16_t count = 0;

void setup()
{
    cli();

    pinMode(BTN_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), button_isr, FALLING);

    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCNT1 = 0;

    TCCR1A |= (1 << COM1B1);
    TCCR1A |= (1 << COM1B0);

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << WGM13);

    // top = 16MHZ / (200Hz * 8) - 1
    ICR1 = 9999;

    // duty = top * 25%
    OCR1B = 2500;

    // Overflow прекъсване
    TIMSK1 |= (1 << TOIE1);

    // 1024 prescaler
    TCCR1B |= (1 << CS11);

    sei();

    Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
	count ++;
}

volatile uint8_t flag = 0;

void button_isr(void)
{
  if(flag == 0)
  {
    TCCR1B |= (0 << CS11);
    flag = 1;
  }

  else
  {
    TCCR1B |= (1 << CS11);
    flag = 0;
  }
}


void loop()
{
  Serial.println(count);
}
