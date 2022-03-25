#define echoPin A5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 0 // attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

bool oplossingsRoute[100];



byte segValue[13][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {1, 0, 0, 1, 0, 0, 0}, // 1
    {0, 1, 1, 1, 1, 0, 1}, // 2
    {1, 1, 0, 1, 1, 0, 1}, // 3
    {1, 0, 0, 1, 0, 1, 1}, // 4
    {1, 1, 0, 0, 1, 1, 1}, // 5
    {1, 1, 1, 0, 1, 1, 1}, // 6
    {1, 0, 0, 1, 1, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 0, 1, 1, 1, 1}, // 9
    {1, 0, 1, 0, 1, 0, 0}, // obstakel ({[([{({[({[({[({[({[{[[[[[(([{[{{({({({({([[[[[[([([{{3}}])])]]]]]])})})})})}}]}]))]]]]]}]})]})]})]})]})}])]})
    {1, 0, 1, 0, 0, 0, 1}, // DOODLOPEND PAD
    {0, 0, 1, 0, 1, 1, 1}  // F
}; 
boolean lijnsensoren[5];

boolean startup = true;
static const uint8_t analog_pins[] = {A0, A1, A2, A3, A4};
boolean state = 1;
// state 1=straight, 0=turning
int difference;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT
  // Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  // Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  // Serial.println("with Arduino UNO R3");

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  for(int i = 1; i<14;i++){
    if(i<4||i>7){
      pinMode(i,OUTPUT);
    }
  }
}

void loop()
{
dispr(false);
  if(state){
    dispnum(12); //display stars
    for (int i = 0; i < 5; i++)
    {
      if(analogRead(analog_pins[i]) < 200){
        lijnsensoren[i] = 1;
      }
      else if(analogRead(analog_pins[i]) > 200){
        lijnsensoren[i] = 0;
      }
        // Serial.println(analogRead(analog_pins[i]));
        // delay(1000);
    }

    distance = getdistance();
    // PING SENSOR
    if (distance < 6) {
      analogWrite(E1, 0);
      analogWrite(E2, 0);

      delay(200);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      turn(750);
    }

    // Serial.println(sum(lijnsensoren, 5));
    // delay(1000);

    int sumlijnsensoren = sum(lijnsensoren, 5);

    if (sumlijnsensoren < 3 && sumlijnsensoren > 0)
    {
      difference = 2 - getNumber(lijnsensoren, 1);
      if (difference < 0)
      {
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        turn(abs(difference) * 100);
      }
      else if (difference > 0)
      {
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        turn(abs(difference) * 100);
      }
      else
      {
        digitalWrite(M1, LOW);
        digitalWrite(M2, HIGH);
      }
  
    }else if(sumlijnsensoren==0){
      dispnum(11); //display void

    }
        if(sumlijnsensoren == 5){
        stop();
        state = false;
        
      }
  }
}

void start()
{
  analogWrite(E1, 70); // PWM Speed Control
  analogWrite(E2, 70); // PWM Speed Control
  delay(200);
  analogWrite(E1, 55); // PWM Speed Control
  analogWrite(E2, 55); // PWM Speed Control
}

void stop(){
  analogWrite(E1, 0); // PWM Speed Control
  analogWrite(E2, 0); // PWM Speed Control
}

void turn(int time)
{
  //digitalWrite(M1, HIGH);
  //digitalWrite(M2, HIGH);
  analogWrite(E1, 70); // PWM Speed Control
  analogWrite(E2, 70); // PWM Speed Control
  delay(time);
  analogWrite(E1, 50); // PWM Speed Control
  analogWrite(E2, 50); // PWM Speed Control
}

int sum(bool arr[], int n)
{
  int sum = 0; // initialize sum

  // Iterate through all elements
  // and add them to sum
  for (int i = 0; i < n; i++) {
    sum += int(arr[i]);
  }
  return sum;
}

int getNumber(bool arr[], int n)
{
  for (int i = 0; i < 5; i++)
  {
    if (arr[i] == 1)
    {
      return i;
    }
  }
  return 0;
}

int getdistance()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  return distance;
}
  void dispnum(int n)
  {
    int m = 0;
    for (int i = 3; i < 14; i++)
    {
      digitalWrite(i, segValue[n][m]);
      m++;
      if (i == 3)
      {
        i = i + 4;
      }
    }
  }

  void dispr(bool n)
  {
    if (n)
    {
      digitalWrite(1, HIGH);
      digitalWrite(2, HIGH);
    }
    else
    {
      digitalWrite(1, LOW);
      digitalWrite(2, HIGH);
    }
  }