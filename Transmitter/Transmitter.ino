#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

#define LED 7

char result[500]={'1','0','1','0','1','0','1','1','1','1','1','1','1','1','1','1'};
const char original[500] = {'1','0','1','0','1','0','1','1','1','1','1','1','1','1','1','1'};
int counter=16;

void setup() {
  // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
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

void loop() {
  // put your main code here, to run repeatedly:
  // int pos = 0;
  // char* msg = {"ABCDEFG"};
  // int2bin(strlen(msg)*8, 16);

  // for (int k = 0; k < strlen(msg); k++) {
  //   chartobin(msg[k]);
  // }

  // int length = strlen(result);
  // while (pos != length) {
  //   delay(100);
  //   if (result[pos] == '1') {
  //     digitalWrite(LED, HIGH);
  //   }
  //   else if (result[pos] == '0') {
  //     digitalWrite(LED, LOW);
  //   }
  //   pos++;
  // }
  // counter = 16;
  // memcpy(result, original, sizeof(char)*16);

  pixels.clear()

}


