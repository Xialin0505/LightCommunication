#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// 1 is on off key
// 2 is color
// 3 is PWM
#define MODULATION 3

#define WS 0
#define CC 262 
#define DD 294 
#define EE 330 
#define FF 349 
#define GG 392 
#define AA 440 
#define BB 494 
#define END -1

// int melody[] = {
//     NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C3, END};
  
int newmelody[] = { CC, CC, GG,GG,AA,AA,GG, WS ,FF, FF, EE, EE, DD, DD, CC, WS, GG, GG, FF, FF, EE, EE, DD, WS, GG, GG, FF, FF, EE, EE, DD, WS, CC, CC, GG, GG, AA, AA, GG, WS, FF, FF, EE, EE, DD, DD, CC, END};

int newnoteDurations[] = {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

int noteDurations[] = {
    4, 4, 4, 4, 4, 4, 4, 4};

#define PIN 6
#if (MODULATION == 2)
#define NUMPIXELS 1
#define DELAYTIME 200
#elif (MODULATION == 1)
#define NUMPIXELS 14
#define DELAYTIME 50
#else
#define NUMPIXELS 1
#define DELAYTIME 40
#endif

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define AUDIOPIN A2
#define TEXT 0
#define CYCLE 50


char result[500] = {'1', '0', '1', '0', '0', '1', '0', '0', '1', '1', '1','1'};
const char original[500] = {'1', '0', '1', '0', '0', '1', '0', '0', '1', '1', '1','1'};
int counter = 12;
uint32_t colorcol[4] = {pixels.Color(0, 0, 0, 255), pixels.Color(255, 0, 0), pixels.Color(0, 255, 0), pixels.Color(0, 0, 255)};
int thisNote = 0;


void setup()
{
    #if (MODULATION == 1) 
      setupOnOff();
    #elif (MODULATION == 2) 
      setupColor();
    #elif (MODULATION == 3) 
      setupPWM();
    #endif
}

void setupOnOff() {
        // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    pixels.setBrightness(100);
    pixels.clear();
    pixels.setAllWhite();
    Serial.begin(9600);
    pinMode(AUDIOPIN, INPUT);
}

void setupColor() {
        // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    pixels.clear();
    // pixels.setAllBlue();
    // pixels.setAllGreen();
    pixels.setAllYellow();
    pixels.setAllPurple();
    pixels.setALLHalfYellow();
    pixels.setALLHalfPurple();
    Serial.begin(9600);
    pinMode(AUDIOPIN, INPUT);
}

void setupPWM() {
          // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin();
    pixels.clear();
    pixels.setAllWhite();
    Serial.begin(9600);
    pinMode(AUDIOPIN, INPUT);
}

void chartobin(char c)
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        result[counter] = (c & (1 << i)) ? '1' : '0';
        counter++;
    }
}

void int2bin(unsigned integer, int n)
{
    for (int i = 0; i < n; i++)
    {
        // binary[i] = (integer & (int)1<<(n-i-1)) ? '1' : '0';
        // binary[n]='\0';
        result[counter] = (integer & (int)1 << (n - i - 1)) ? '1' : '0';
        // preamble 20 + data length field 16 = 36
        counter++;
    }

    result[32] = '\0';
}

void melodytobin(int i)
{
    int note = newmelody[i];
    int noteDuration = newnoteDurations[i];
    if (note == CC)
    { // NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, END
        note = 0;
    }
    else if (note == DD)
    {
        note = 1;
    }
    else if (note == EE)
    {
        note = 2;
    }
    else if (note == FF)
    {
        note = 3;
    }
    else if (note == GG)
    {
        note = 4;
    }
    else if (note == AA)
    {
        note = 5;
    }
    else if (note == BB)
    {
        note = 6;
    }
    else if (note == WS){
        note = 7;
    }
    else if (note == END)
    {
        note = 8;
    }
    Serial.println((String) "Note: " + note + "; Dur: " + noteDuration);

    for (int i = 0; i < 3; i++)
    {
        result[counter] = (note & (int)1 << (3 - i - 1)) ? '1' : '0';
        counter++;
    }
    for (int i = 0; i < 3; i++)
    {
        result[counter] = (noteDuration & (int)1 << (3 - i - 1)) ? '1' : '0';
        counter++;
    }

    result[12 + 6] = '\0';
}

void signon(int colornum)
{
    pixels.clear();
    pixels.resetColor(colornum);
    pixels.show();
    return;
}

void signoff()
{
    pixels.clear();
    pixels.show();
    return;
}

void transmissionColor() {
    int pos = 0;

#if TEXT
    char *msg = {"ABCDEFG"};
    int2bin(strlen(msg) * 8, 10);
    for (int k = 0; k < strlen(msg); k++)
    {
        chartobin(msg[k]);
    }
#else
    // audiotobin();
    if (newmelody[thisNote] == -1)
    {
        thisNote = 0;
    }
    melodytobin(thisNote);
    thisNote++;
    Serial.println(result);

#endif
//00 blue, 01 yellow, 10 purplr, 11 green


    int length = strlen(result);
#if (MODULATION == 2)
        // Serial.print("\n");
        // if (result[pos] == '0' && result[pos+1] =='0')
        // {
        //   Serial.print("00");
        //   signon(1);
        // }
        // if (result[pos] == '0' && result[pos+1] =='1')
        // {
        //   Serial.print("01");
        //   signon(6);
        // }
        // if (result[pos] == '1' && result[pos+1] =='0')
        // {
        //   Serial.print("10");
        //   signon(4);
        // }
        // else if (result[pos] == '1' && result[pos+1] =='1')
        // {
        //   Serial.print("11");
        //   signon(2);
        //   // signoff();
        // }
#elif (MODULATION == 3)
while (pos != length)
    {
        // signon(4);
        delay(DELAYTIME);
        if (result[pos] == '1')
        {
          // Serial.print("1");
          signon(1);
        }
        else if (result[pos] == '0')
        {
          // Serial.print("0");
        //   signon(1);
          signon(4);
        }
        pos++;
    }
#endif
        // delay(DELAYTIME/2);
        // pixels.clear();
        // pixels.show();
    // Serial.print("\n");
    counter = 12;
    memcpy(result, original, sizeof(char) * 12);
    // sign0();
    // delay(10);
    pixels.clear();
    pixels.show();
    // delay(1000);
}

void transmissionOnOff() {
      // put your main code here, to run repeatedly:
    int pos = 0;

#if TEXT
    char *msg = {"ABCDEFG"};
    int2bin(strlen(msg) * 8, 10);
    for (int k = 0; k < strlen(msg); k++)
    {
        chartobin(msg[k]);
    }
#else
    // audiotobin();
    if (newmelody[thisNote] == -1)
    {
        thisNote = 0;
    }
    melodytobin(thisNote);
    thisNote++;
    Serial.println(result);

#endif

    int length = strlen(result);
    while (pos != length)
    {
        // signon(4);
        delay(DELAYTIME);
        if (result[pos] == '1')
        {
          // Serial.print("1");
          signon(0);
        }
        else if (result[pos] == '0')
        {
          // Serial.print("0");
        //   signon(1);
          signoff();
        }
        pos++;
    }
    // Serial.print("\n");
    counter = 12;
    memcpy(result, original, sizeof(char) * 12);
    // sign0();
    // delay(10);
    pixels.clear();
    pixels.show();
    // delay(1000);
}

void transmissionPWM() {
  int pos = 0;

#if TEXT
    char *msg = {"ABCDEFG"};
    int2bin(strlen(msg) * 8, 10);
    for (int k = 0; k < strlen(msg); k++)
    {
        chartobin(msg[k]);
    }
#else // audio
    // audiotobin();
    if (newmelody[thisNote] == -1)
    {
        thisNote = 0;
    }
    melodytobin(thisNote);
    thisNote++;
    Serial.println(result);

#endif

    int length = strlen(result);
    while (pos != length)
    {
        delay(DELAYTIME*0.3);
        if (result[pos] == '1')
        {
          signon(0);
          delay(DELAYTIME*0.7);
          signoff();
        }
        else if (result[pos] == '0')
        {
          signon(0);
          delay(DELAYTIME*0.3);
          signoff();
        }
        pos++;
    }
    counter = 12;
    memcpy(result, original, sizeof(char) * 12);
    pixels.clear();
    pixels.show();
}

void loop()
{
    #if (MODULATION == 1) 
        transmissionOnOff();
    #elif (MODULATION == 2)
        transmissionColor();
    #else
        transmissionPWM();
    #endif
}