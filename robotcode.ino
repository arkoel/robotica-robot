//Arduino PWM Speed Control：
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

const int l1 = analogRead(A0);
const int l2 = analogRead(A1);
const int l3 = analogRead(A2);
const int l4 = analogRead(A3);
const int l5 = analogRead(A4);

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);

    Serial.begin(9600);
}

void loop()
{
    // WIEL BESTURING
    digitalWrite(M1,LOW);
    digitalWrite(M2, HIGH);
    analogWrite(E1,70);   //PWM Speed Control
    analogWrite(E2,70);   //PWM Speed Control
    delay(50);
    analogWrite(E1,50);   //PWM Speed Control
    analogWrite(E2,50);   //PWM Speed Control
    delay(1000);
    analogWrite(E1,10);   //PWM Speed Control
    analogWrite(E2,10);   //PWM Speed Control
    delay(500);
    digitalWrite(M1,HIGH);
    digitalWrite(M2,LOW);
    analogWrite(E1,70);   //PWM Speed Control
    analogWrite(E2,70);   //PWM Speed Control
    delay(50);
    analogWrite(E1,50);   //PWM Speed Control
    analogWrite(E2,50);   //PWM Speed Control
    delay(1000);
    analogWrite(E1,10);   //PWM Speed Control
    analogWrite(E2,10);   //PWM Speed Control
    delay(500);

    // LIJN SENSOR
    analogRead(A0);
    Serial.println(l1);
    Serial.println(l2);
    Serial.println(l3);
    Serial.println(l4);
    Serial.println(l5);

    // PING SENSOR

}