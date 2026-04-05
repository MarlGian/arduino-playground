const byte motionSensor = 2; //pin for motion sensor module
const byte ledPin = 3; // pin for led
const byte buzzerPin = 11; //pin for buzzer

void setup() {
  //initializing the pins
  pinMode(motionSensor, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Motion detection starting...");
}

void loop() {
  //if it detects a motion this will blink the ledpin and play the siren tone for 5secs
  if (digitalRead(motionSensor) == HIGH) {
    Serial.println("Motion detected!!!");
    blinkLED(250);
    //playSound(500);
    playSiren();
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }
}

//function for Blinking LED Pin
void blinkLED(int time) {
  digitalWrite(ledPin, HIGH);
  delay(time);
  digitalWrite(ledPin, LOW);
  delay(time);
}

//Some normal playing sound
void playSound(int duration) {
  tone(buzzerPin, 425, duration);
  delay(duration);

  tone(buzzerPin, 125, duration);
  delay(duration);

  tone(buzzerPin, 475, duration);
  delay(duration);

  noTone(buzzerPin);
}

//The siren
void playSiren() {
  for (int i = 0; i < 5; i++) {

    for (int freq = 1500; freq <= 4000; freq += 50) {
      tone(buzzerPin, freq);
      delay(10);
    }

    for (int freq = 4000; freq >= 1500; freq -= 50) {
      tone(buzzerPin, freq);
      delay(10);
    }
  }
  noTone(buzzerPin);
}
