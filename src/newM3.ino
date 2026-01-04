// A5入力を32kHzでサンプリングし、閾値判定してD4をHIGH/LOWに出力

const int threshold = 256; // ADC閾値
const uint8_t outBit = _BV(PORTD4); // ★ D4 = PD4 に変更

ISR(TIMER1_COMPA_vect) {
  ADCSRA |= _BV(ADSC);  // ADC開始
}

ISR(ADC_vect) {
  uint16_t val = ADC;
  if (val > threshold) {
    PORTD |= outBit;    // ★ D4 HIGH
  } else {
    PORTD &= ~outBit;   // ★ D4 LOW
  }
}

void setup() {
  // ★ 出力ピンを D4（PD4）に変更
  DDRD |= outBit;

  // ADC設定（A5）
  ADMUX  = _BV(REFS0) | (5 & 0x07);
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  ADCSRB = 0;

  // Timer1設定（32kHz）
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 61;
  TCCR1B |= _BV(WGM12);
  TCCR1B |= _BV(CS11);
  TIMSK1 |= _BV(OCIE1A);
  interrupts();
}

void loop() {
  // 何もしない
}