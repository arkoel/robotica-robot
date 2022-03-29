#define echoPin A5
#define trigPin 0 

long duration;
int distance; 

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

bool oplossingsRoute[100];
int bochtNr = 0;
int oplossingSom = 0;

byte segValue[14][7] = {
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
    {0, 0, 1, 0, 1, 1, 1}, // F
    {0, 0, 0, 0, 0, 0, 0}  // UIT
}; 

boolean lijnsensoren[5];
boolean startup = true;
static const uint8_t analog_pins[] = {A0, A1, A2, A3, A4};
int state = 1;
// state 1=straight, 0=turning
double difference;
int turns = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 


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
  
  if (state==1) {
    dispnum(turns);
    for (int i = 0; i < 5; i++) {
      if (analogRead(analog_pins[i]) < 200) {
        lijnsensoren[i] = 1;
      }
      else if (analogRead(analog_pins[i]) > 200) {
        lijnsensoren[i] = 0;
      }
    }
    distance = getdistance();
    // PING SENSOR
    if (distance < 8 && turns>0) {
      dispnum(10);
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      delay(750);
      turnRight(4, 0);
    }


    int sumlijnsensoren = sum(lijnsensoren, 5);
    difference = 2 - getNumber(lijnsensoren);

   //als alle sensoren branden
    if (sumlijnsensoren == 5) { // MISSCHIEN FINNISH
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      delay(1000);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      speed(100);
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
      if (sumlijnsensoren == 5) { // FINNISH
        for(int j = 9;j>=0;j--){
          dispnum(j);
          delay(1000);
        }
        delay(1000); //10de seconde
        state = 0;
        // Laat route zien (bijv. L, L, R, L, R)
        dispnum(13);
        for(int i=0; i<oplossingSom; i++) {
          delay(400);
          dispr(oplossingsRoute[i]);
          delay(1000);
        }
        dispnum(12);
        dispr(0);
        delay(10000);
        state=0;
        // delay(10000);
      } else {
      //omdraaien toch geen finish

      }
    }

    //als geen sensor brand
    if(sumlijnsensoren == 0) {
      delay(200);
      for (int i = 0; i < 5; i++) {
         if (analogRead(analog_pins[i]) < 200) {
           lijnsensoren[i] = 1;
         }
         else if (analogRead(analog_pins[i]) > 200) {
           lijnsensoren[i] = 0;
         }
      }
      sumlijnsensoren = sum(lijnsensoren, 5);
      if(sumlijnsensoren == 0){
        digitalWrite(M1, LOW);
        digitalWrite(M2, HIGH);
        dispnum(11);
        turnRight(4, 10);
      }
    }

    //als 1 tot 4 sensoren aan staan
    if (abs(difference) == 0||abs(difference) == 1){
      if (difference == -1) {
         turnLeft(1, -10);
      }else if (difference == 1){
        turnRight(1, -10);
      } else{
        //straight
        digitalWrite(M1, HIGH);
        digitalWrite(M2, LOW);
        speed(150);
      }
    }
    if(abs(difference)==2){
      if(difference==-2){
      if (turns < 9){turns++;}
        dispr(false);
        bochtNr++;
        oplossingSom++;
        oplossingsRoute[bochtNr] = 0;
        difference *= 1.2;
        turnLeft(difference, 0);

      }else if(difference==2){
        if (turns < 9){turns++;}
        dispr(true);
        bochtNr++;
        oplossingSom++;
        oplossingsRoute[bochtNr] = 1;
        difference *= 1.2;
        turnRight(difference, 0);      
      }
    }
  } 

  if(state==0){
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    for(int i = 10; i>=0; i--){
      dispnum(i);
    }
    state = 1;
  }
}

void turnRight(int difference, int faster) {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  turn(abs(difference) * 150 , faster);
}

void turnLeft(int difference, int faster) {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  turn(abs(difference) * 150 , faster);
}

void turn(int time, int faster) {
  analogWrite(E1, 70+faster);
  analogWrite(E2, 75+faster); 
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
  //
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
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