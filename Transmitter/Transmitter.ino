#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        6
#define NUMPIXELS 14
Adafruit_NeoPixel pixels(14, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYTIME 5
#define AUDIOPIN A2
#define TEXT 0
#define CYCLE 5

char result[500] = {'1','0','1','0','0','1','0','0','1','1','1'};
const char original[500] = {'1','0','1','0','0','1','0','0','1','1','1'};
int counter=11;
uint32_t colorcol[4] = {pixels.Color(0,   0,   0,255),pixels.Color(255,   0,   0),pixels.Color(0,   255,   0),pixels.Color(0,   0,   255)};

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



void audiotobin() {
  int RUN = 0;  
  while (RUN < CYCLE) {
    delay(DELAYTIME/CYCLE);
    int aud = analogRead(AUDIOPIN);
    Serial.println(aud);
    for (int i = 0; i < 10; i++) {
      result[counter] = (aud & (int)1 << (10-i-1)) ? '1' : '0';
      counter++;
    }
    RUN++;
  }
  
  result[(11 + CYCLE * 10)] = '\0';
  // char buffer[25];
  // sprintf(buffer,"%s", result[11]);
  // Serial.println(buffer);
  // Serial.println("%s", result[11]);
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
  audiotobin();
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



