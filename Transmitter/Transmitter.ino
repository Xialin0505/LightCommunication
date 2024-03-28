#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


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
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
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

// notes in the song 'Mukkathe Penne'
int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                 

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                       

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, 0,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, END

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,
};


#define PIN        6
#define NUMPIXELS 14
Adafruit_NeoPixel pixels(14, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYTIME 20
#define AUDIOPIN A2
#define TEXT 0
#define CYCLE 50

char result[500] = {'1','0','1','0','0','1','0','0','1','1','1'};
const char original[500] = {'1','0','1','0','0','1','0','0','1','1','1'};
int counter=11;
uint32_t colorcol[4] = {pixels.Color(0,   0,   0,255),pixels.Color(255,   0,   0),pixels.Color(0,   255,   0),pixels.Color(0,   0,   255)};
int thisNote = 0;

void setup() {
  // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(100);
  pixels.setAllWhite();
  Serial.begin(9600);
  pinMode(AUDIOPIN, INPUT);
}

void chartobin(char c)
{
    int i;
    for(i=7; i>=0;i--){
        result[counter]= (c&(1<<i))?'1':'0';  
        counter++;
    }
}

void int2bin(unsigned integer, int n)
{  
    for (int i=0;i<n;i++)   
    {
    // binary[i] = (integer & (int)1<<(n-i-1)) ? '1' : '0';
    // binary[n]='\0';
    result[counter]= (integer & (int)1<<(n-i-1)) ? '1' : '0';
    // preamble 20 + data length field 16 = 36
    counter++;
    }
    
    result[32]='\0';
}

uint32_t colortype(int colorindex){
  
}



// void audiotobin() {
//   int RUN = 0;  
//   while (RUN < CYCLE) {
//     delay(DELAYTIME/CYCLE);
//     int aud = analogRead(AUDIOPIN);
//     Serial.println(aud);
//     for (int i = 0; i < 10; i++) {
//       result[counter] = (aud & (int)1 << (10-i-1)) ? '1' : '0';
//       counter++;
//     }
//     RUN++;
//   }
  
//   result[(11 + CYCLE * 10)] = '\0';
//   // char buffer[25];
//   // sprintf(buffer,"%s", result[11]);
//   // Serial.println(buffer);
//   // Serial.println("%s", result[11]);
// }

void melodytobin(int i) {
  int note = melody[i];
  int noteDuration = noteDurations[i];
  Serial.println((String)"Note: " + note + "; Dur: " + noteDuration);
  for (int i = 0; i < 10; i++) {
    result[counter] = (note & (int)1 << (10-i-1)) ? '1' : '0';
    counter++;
  }
  for (int i = 0; i < 5; i++) {
    result[counter] = (noteDuration & (int)1 << (5-i-1)) ? '1' : '0';
    counter++;
  }
  
  result[11+15] = '\0';
}

void colorset(int colorindex){
  // for(int i=0; i<14;i++){
  //   pixels.setPixelColor(i,colorcol[colorindex]);
  // }

}

void signon(int colornum){
  // pixels.clear();
  // colorset(0);
  // for(int i=0; i<14; i++) { // For each pixel...
  //   pixels.setPixelColor(i,pixels.Color(255, 255, 255));
  // }
  pixels.resetColor();
  pixels.show();
  return;
}

void signoff(){
  pixels.clear();
  pixels.show();
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
  int pos = 0;
  
#if TEXT
  char* msg = {"ABCDEFG"};
  int2bin(strlen(msg)*8, 10);
  for (int k = 0; k < strlen(msg); k++) {
    chartobin(msg[k]);
  }
#else 
  // audiotobin();
  if (melody[thisNote] == -1) {
    thisNote = 0;
  }
  melodytobin(thisNote);
  thisNote++;
  Serial.println(result);
  
#endif

  int length = strlen(result);
  while (pos != length) {
    delay(DELAYTIME);
    if (result[pos] == '1') {
      signon(0);
    }
    else if (result[pos] == '0') {
      signoff();
    }
    pos++;
  }
  counter = 11;
  memcpy(result, original, sizeof(char)*11);
  // sign0();
  // delay(10);
  pixels.clear();
  pixels.show();
  // delay(1000);
}



