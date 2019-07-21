#define FASTADC 1

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
#if FASTADC
    // set prescale to 16
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
#endif
}

void loop() {
    // put your main code here, to run repeatedly:
    int sensor0 = analogRead(A0);
    int sensor1 = analogRead(A1);
    char text[100];
    sprintf(text, "%d, %d\n", sensor0, sensor1);

    Serial.println("asda dsadas asdasda");
    delay(1);
}
