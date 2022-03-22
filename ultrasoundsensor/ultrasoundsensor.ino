// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}

boolean lijnsensoren[5]; 

boolean startup = true;

void loop() {
  char a = 'A';
  for (int i = 0; i < 5; i++) {
    lijnsensoren[i] = (digitalRead(a + i) < 30);
  }

  if(sum(lijnsensoren, 5) == 1) {
    3 - getNumber(lijnsensoren, 1);
  }


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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Start driving, slow down when object is near
    digitalWrite(M1,LOW);
    digitalWrite(M2, HIGH);
    
  if(distance < 10) {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    delay(1000);
    startup=true;
    start();
    turn(1000);
    startup=false;
  } else {
    if(startup){
    start();
    startup=false;
    }
  }
}
void start(){
    analogWrite(E1,70);   //PWM Speed Control
    analogWrite(E2,70);   //PWM Speed Control
    delay(500);
    analogWrite(E1,45);   //PWM Speed Control
    analogWrite(E2,45);   //PWM Speed Control
}

void turn(int time) {
  digitalWrite(M1,HIGH);
  digitalWrite(M2, HIGH);
  start();
  delay(time);
}

int sum(bool arr[], int n)
{
    int sum = 0; // initialize sum
 
    // Iterate through all elements
    // and add them to sum
    for (int i = 0; i < n; i++)
    sum += arr[i];
 
    return sum;
}

int getNumber(bool arr[], int n) {
  for (int i = 0; i < 5; i++) {
    if(arr[i] == 1) {
      return i;
    }
  }
  return 0;
}