#define FREQUENCY_PIN A5
#define DURATION_PIN A4
#define READY_PIN 13
#define SPEAKER_PIN 11
#define SPEED 130
#define MICROPHONE_PIN 3

#define nodePin1 6
#define nodePin2 7
#define nodePin3 8

#define durPin1 9
#define durPin2 10
#define durPin3 11


#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define END -1


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(FREQUENCY_PIN, INPUT);
  pinMode(DURATION_PIN, INPUT);
  pinMode(READY_PIN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);

  pinMode(nodePin1, INPUT);
  pinMode(nodePin2, INPUT);
  pinMode(nodePin3, INPUT);
  pinMode(durPin1, INPUT);
  pinMode(durPin2, INPUT);
  pinMode(durPin3, INPUT);


  attachInterrupt(digitalPinToInterrupt(READY_PIN), inputReady, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (digitalRead(READY_PIN)) {
  //   inputReady();
  // }
  // Serial.println(digitalRead(READY_PIN));
}

void inputReady() {
  // int frequency = analogRead(FREQUENCY_PIN);
  // int duration = analogRead(DURATION_PIN);
  int duration = readDur();
  int frequency = readFreq();

  Serial.println("ready");
  Serial.println(frequency);
  frequency = decode(frequency);
  Serial.println(duration);

  int noteDuration = SPEED * duration;
  tone(MICROPHONE_PIN, frequency, noteDuration*.95);
}

int readDur() {
  int num = (digitalRead(durPin3) << 2) + (digitalRead(durPin2) << 1) + digitalRead(durPin1);
  return num;
}

int readFreq() {
  int num = (digitalRead(nodePin3) << 2) + (digitalRead(nodePin2) << 1) + digitalRead(nodePin1);
  return num;
}

int decode(int num) {
  if (num == 0) {
    return NOTE_C4;
  } else if (num == 1) {
    return NOTE_D4;
  } else if (num == 2) {
    return NOTE_E4;
  } else if (num == 3) {
    return NOTE_F4;
  } else if (num == 4) {
    return NOTE_G4;
  } else if (num == 5) {
    return NOTE_A4;
  } else if (num == 6) {
    return NOTE_B4;
  } else if (num == 7) {
    return 0;
  }
}