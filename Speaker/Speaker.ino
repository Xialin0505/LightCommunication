#define FREQUENCY_PIN A5
#define DURATION_PIN A4
#define READY_PIN 2
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
// #define NOTE_CS4 277
#define NOTE_D4  294
// #define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
// #define NOTE_FS4 370
#define NOTE_G4  392
// #define NOTE_GS4 415
#define NOTE_A4  440
// #define NOTE_AS4 466
#define NOTE_B4  494

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
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