#define echoPin A5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 0  // attach pin D3 Arduino to pin Trig of HC-SR04

long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

bool oplossingsRoute[100];
int bochtNr = 0;
int oplossingSom = 0;

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
    {1, 0, 1, 0, 1, 0, 0}, // obstakel
    {1, 0, 1, 0, 0, 0, 1}, // DOODLOPEND PAD
    {0, 0, 1, 0, 1, 1, 1}  // F
}; 

boolean lijnsensoren[5];
boolean startup = true;
static const uint8_t analog_pins[] = {A0, A1, A2, A3, A4};
boolean state = 1;
// state 1=straight, 0=turning
int difference;
int turns = 0;

void setup() {
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
  for(int l = 1; l<14; l++) {
    if(l<4 || l>7) {
      pinMode(l, OUTPUT);
    }
  }
}

void loop() {
  dispnum(turns);
  if (state) {
    for (int i = 0; i < 5; i++) {
      if (analogRead(analog_pins[i]) < 200) {
        lijnsensoren[i] = 1;
      }
      else if (analogRead(analog_pins[i]) > 200) {
        lijnsensoren[i] = 0;
      }
      // Serial.println(analogRead(analog_pins[i]));
      // delay(1000);
    }
    distance = getdistance();
    // PING SENSOR
    if (distance < 4 && turns>0) {
      dispnum(10);
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      delay(750);
      turnRight(4);
      // delay(250);
    }

    // Serial.println(sum(lijnsensoren, 5));
    // delay(1000);

    int sumlijnsensoren = sum(lijnsensoren, 5);
    if (sumlijnsensoren < 3 && sumlijnsensoren > 0) {
      difference = 2 - getNumber(lijnsensoren);
      if (difference < 0) {
        if(abs(difference)==2){
          if (turns < 9){turns++;}
          dispr(false);
          bochtNr++;
          oplossingSom++;
          oplossingsRoute[bochtNr] = 0;
        }
        turnLeft(difference);
      } else if (difference > 0) {
        if(abs(difference)==2){
          if (turns < 9){turns++;}
          dispr(true);
          bochtNr++;
          oplossingSom++;
          oplossingsRoute[bochtNr] = 1;
        }
        turnRight(difference);
      }else{
        digitalWrite(M1, HIGH);
        digitalWrite(M2, LOW);
        speed(250);
      }
    }
    if (sumlijnsensoren == 5) { // FINSIH
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      speed(300);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      for (int i = 0; i < 5; i++) {
        if (analogRead(analog_pins[i]) < 200) {
          lijnsensoren[i] = 1;
        } else if (analogRead(analog_pins[i]) > 200) {
          lijnsensoren[i] = 0;
        }
      }
      int sumlijnsensoren = sum(lijnsensoren, 5);
      if (sumlijnsensoren == 5) { // FINSIH

     
      state = false;
      
      //Fi display
      dispnum(12);
      dispr(0);
      delay(1000);
      
      // Laat route zien (bijv. L, L, R, L, R)
      for(int i=0; i<oplossingSom; i++) {
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        delay(400);
        dispr(oplossingsRoute[i]);
        delay(1000);
      }
      dispnum(12);
      dispr(0);
      delay(9000);
      } else{
      //omdraaien toch geen finish

      }
    }
    if(sumlijnsensoren == 0) {
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      dispnum(11);
      // exit(0);
      //delay(1000);
      turnRight(4);
    }
  } 
}

void turnRight(int difference) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  turn(abs(difference) * 150);
}

void turnLeft(int difference) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  turn(abs(difference) * 150);
}

void turn(int time) {
  analogWrite(E1, 70);
  analogWrite(E2, 75); 
  delay(100);
  analogWrite(E1, 40); 
  analogWrite(E2, 40); 
  delay(time-100);
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

void speed(int time) {
  analogWrite(E1, 70); 
  analogWrite(E2, 70);
  delay(100);
  analogWrite(E1, 35);
  analogWrite(E2, 35);
  delay(time-100);
}

int sum(bool arr[], int n) {
  int sum = 0; // initialize sum

  // Iterate through all elements
  // and add them to sum
  for (int i = 0; i < n; i++) {
    sum += int(arr[i]);
  }
  return sum;
}

int getNumber(bool arr[]) {
  int max = 0;
  for (int i = 0; i < 5; i++) {
    if (arr[i] == 1) {
      if(max>abs(2-i)||i!=0){
        max = i;
      }
      //return i;
    }
  }
  return max;
}

int getdistance() {
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

void dispnum(int n) {
  int m = 0;
  for (int i = 3; i < 14; i++) {
    digitalWrite(i, segValue[n][m]);
    m++;
    if (i == 3) {
      i = i + 4;
    }
  }
}

void dispr(bool n) {
  if (n) {
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
  }
  else {
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
  }
}