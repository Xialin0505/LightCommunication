#include <Wire.h>
#include <Thread.h>
#include "Adafruit_AS726x.h"

//create the object
Adafruit_AS726x ams;

//buffer to hold raw values
uint16_t sensorValues[AS726x_NUM_CHANNELS];

// 1 for On Off Key
// 2 for Color
#define MODULATION_MODE 1
#define delayIntervalOnOff 50
#define delayIntervalColor 200

#define PHOTO_RESISTOR A0
#define VCC 11
#define READY_PIN 13
#define FreqPIN A5
#define ReadyPIN 1
#define DurPIN A4

#define SYMBOL_LENGTH 10

#define ERROR_RATE_WINDOW 2

#define FREQUENCY_BIT 3
#define DURATION_BIT 3
#define PACKET_LENGTH 6

#define speed 90

#define nodePin1 2
#define nodePin2 3
#define nodePin3 4
#define durPin1 5
#define durPin2 6
#define durPin3 7

#define VIOLET 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4

//global variables
unsigned int state;
String sequence = "000000000000";
String preamble = "101001001111";
int preamebl_length = 12;

String dataBits="";
boolean synchro_Done =false;
boolean receiveData_Done =false;

int decimalValue = 800; //just for initialisiation (gets later dynamically calculated)
boolean crc_check_value = false; //Flag for CRC calculation

int accmuluatedLength = 0;
bool startPacket = false;

int baseline = 0;
int baselineBlue = 0;
int baselineViolet = 0;
int baselineGreen = 0;
int baselineYellow = 0;
float window = 0.2;

int prevVoltage = 0;

char messages[8];
int messageidx = 0;

void setup() {

  Serial.begin(9600);

  //Input Pin for the Solarplate
  pinMode(PHOTO_RESISTOR ,INPUT);
  pinMode(VCC, OUTPUT);
  pinMode(READY_PIN, OUTPUT);
  pinMode(ReadyPIN, OUTPUT);
  pinMode(DurPIN, OUTPUT);
  pinMode(FreqPIN, OUTPUT);

  pinMode(nodePin1, OUTPUT);
  pinMode(nodePin2, OUTPUT);
  pinMode(nodePin3, OUTPUT);
  pinMode(durPin1, OUTPUT);
  pinMode(durPin2, OUTPUT);
  pinMode(durPin3, OUTPUT);

  digitalWrite(VCC, HIGH);
  digitalWrite(ReadyPIN, LOW);
  
  if (MODULATION_MODE == 2) {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    //begin and make sure we can talk to the sensor
    if(!ams.begin()){
      Serial.println("could not connect to sensor! Please check your wiring.");
      while(1);
    }

    for (int i = 0; i < 100; i++) {
      measure();
      baselineBlue += ams.readBlue();
      baselineYellow += ams.readYellow();
      baselineViolet += ams.readViolet();
      baselineGreen += ams.readGreen();
    }

    baselineBlue = baselineBlue / 100;
    baselineYellow = baselineYellow / 100;
    baselineViolet = baselineViolet / 100;
    baselineGreen = baselineGreen / 100;

    Serial.print("blue");
    Serial.println(baselineBlue);
    Serial.print("green");
    Serial.println(baselineGreen);
    Serial.print("yellow");
    Serial.println(baselineYellow);
    Serial.print("violet");
    Serial.println(baselineViolet);
  }
  
  int sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(PHOTO_RESISTOR);
  }
  baseline = sum / 100;

  //initial State is looking for Synchronization sequence
  state = 0;

  // speaker_thread.onRun(speakerCallback);

  // attachInterrupt(digitalPinToInterrupt(READY_PIN), inputReady, RISING);

  Serial.println("set up done");
}

void loop() {
  if (MODULATION_MODE == 1) {
    receiverMainOnOff();
  } else {
    receiverMainColor();
  }
}

void measure() {
  ams.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    rdy = ams.dataReady();
  }
}

void readAMS() {
  //read the device temperature
  uint8_t temp = ams.readTemperature();
  
  //ams.drvOn(); //uncomment this if you want to use the driver LED for readings
  ams.startMeasurement(); //begin a measurement
  
  //wait till data is available
  bool rdy = false;
  while(!rdy){
    rdy = ams.dataReady();
  }

  //ams.drvOff(); //uncomment this if you want to use the driver LED for readings

  //read the values!
  ams.readRawValues(sensorValues);
  // ams.readCalibratedValues(calibratedValues);

  // Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" Violet: "); Serial.print(sensorValues[AS726x_VIOLET]);
  Serial.print(" Blue: "); Serial.print(sensorValues[AS726x_BLUE]);
  Serial.print(" Green: "); Serial.print(sensorValues[AS726x_GREEN]);
  Serial.print(" Yellow: "); Serial.print(sensorValues[AS726x_YELLOW]);
  Serial.print(" Orange: "); Serial.print(sensorValues[AS726x_ORANGE]);
  Serial.print(" Red: "); Serial.print(sensorValues[AS726x_RED]);
  Serial.println();
}

int decodeColor() {
  int deltaViolet = (sensorValues[AS726x_VIOLET] - baselineViolet) / baselineViolet;
  int deltaBlue = (sensorValues[AS726x_BLUE] - baselineBlue) / baselineBlue;
  int deltaYellow = (sensorValues[AS726x_YELLOW] - baselineYellow) / baselineYellow;
  int deltaGreen = (sensorValues[AS726x_GREEN] - baselineGreen) / baselineGreen;
  if (deltaViolet > deltaBlue && deltaViolet > deltaYellow && deltaViolet > deltaGreen) {
    return VIOLET;
  }
  if (deltaBlue > deltaViolet && deltaBlue > deltaYellow && deltaBlue > deltaGreen) {
    return BLUE;
  }
  if (deltaYellow > deltaViolet && deltaYellow > deltaBlue && deltaYellow > deltaGreen) {
    return YELLOW;
  }
  if (deltaGreen > deltaViolet && deltaGreen > deltaYellow && deltaGreen > deltaBlue) {
    return GREEN;
  }

  return BLUE;
}

void receiverMainColor() {
  delay(delayIntervalColor);
  String data = "00";

  readAMS();
  int color = decodeColor();
  Serial.println(color);
  if (color == BLUE) {
    data = "00";
  } else if (color == YELLOW) {
    data = "01";
  } else if (color == VIOLET) {
    data = "10";
  } else {
    data = "11";
  }

  // if (state == 0) {
  //   synchro_Done = false;
  //   lookForSynchro(data, 2);

  //   if (synchro_Done == true)
  //   {
  //     state=1;
  //     digitalWrite(READY_PIN, HIGH);
  //     dataBits = "";
  //   }
  // } else if (state == 1) {
  //   receiveData_Done = false;
  //   receiveData(data);

  //   if (receiveData_Done == true)
  //   {
  //     state = 0; 
  //     dataBits = "";
  //   }
  // }
}

void receiverMainOnOff() {
  // put your main code here, to run repeatedly:
  delay(delayIntervalOnOff);
  String data = "0";
  int sensorValue = analogRead(PHOTO_RESISTOR);

  float voltage = sensorValue;

  if (voltage >= baseline * (1 + window)) 
  {
    data="1";
  }
  else
  {
    data="0";
  }

  if (state == 0) {
    synchro_Done = false;
    lookForSynchro(data, 1);

    if (synchro_Done == true)
    {
      state=1;
      digitalWrite(READY_PIN, HIGH);
      dataBits = "";
    }
  } else if (state == 1) {
    receiveData_Done = false;
    receiveData(data);

    if (receiveData_Done == true)
    {
      state = 0; 
      dataBits = "";
    }
  }
}

void lookForSynchro(String bit, int offset)
{
  sequence.concat(bit);
  sequence.remove(0,offset);

  // Serial.println("Sequence: "+sequence);
  digitalWrite(READY_PIN, LOW);

  if (isSync()) {
    synchro_Done = true;  
    // Serial.println("sync done");
    sequence = "000000000000";
  }
}

void receiveData(String bit)
{
  dataBits.concat(bit);

  // if (dataBits.length() == 16)
  // {
  //   // Serial.println("data Bits: " + dataBits);
  //   char char_array[17];  // Prepare the character array (the buffer)
  //   dataBits.toCharArray(char_array, 17);
  //   decimalValue= strtol(char_array, NULL, 2);//function for converting string into long data type integer
    
  //   // Serial.println(decimalValue);
    
  //   startPacket = true;
  //   if (decimalValue < 0 || decimalValue % SYMBOL_LENGTH != 0) {
  //     restartTransmit();
  //   }
  //   dataBits = "";

  //   return;
  // } 
  
  // if (dataBits.length() % SYMBOL_LENGTH == 0) {
  //   // char buf;
  //   // Serial.println(dataBits);
  
  //   int charIdx = binToChar(dataBits);
  //   analogWrite(SPEAKER_PIN, charIdx);
  //   // Serial.println(charIdx);

  //   int interpolate = (prevVoltage + charIdx) / 2;
  //   if (interpolate != 1023 && interpolate != 0) {
  //     analogWrite(SPEAKER_PIN, interpolate);
  //     // Serial.println(interpolate);
  //   } 

  //   prevVoltage = charIdx;
    
  //   // Serial.println(charIdx);
    
  //   // buf = (char) charIdx;
  //   // messages[messageidx] = buf;
  //   // messageidx ++;

  //   // if (messageidx >= 7) {
  //   //   messages[7] = '\0';
  //   //   Serial.println(messages);
  //   //   messageidx = 0;
  //   // }

  //   dataBits = "";
  //   accmuluatedLength += SYMBOL_LENGTH;
    if (dataBits.length() == PACKET_LENGTH) {
      Serial.println(dataBits);

      int freq = binToChar(dataBits, 0, FREQUENCY_BIT);
      int dur = binToChar(dataBits, FREQUENCY_BIT, PACKET_LENGTH);

      Serial.println(freq);
      // analogWrite(FreqPIN, freq);
      freqToBin(freq);
      Serial.println(dur);
      // analogWrite(DurPIN, dur);
      durToBin(dur);
      digitalWrite(ReadyPIN, HIGH);

      // int noteDuration = speed * dur;
      // tone(MICROPHONE_PIN, freq, noteDuration*.95);
      
      dataBits = "";
      restartTransmit();
    }


    // if (accmuluatedLength >= PACKET_LENGTH) {
    //   restartTransmit();
    // }

    return;

  if(dataBits.length()==decimalValue+16+8) //Stop dynamically at the end of the message; +16 because 2 bytes frame for data message length; +8 for last byte CRC
  {

    if (crc_check_value==false) //do the CRC check
    {
      checkCRC(dataBits);  
    }

    if (crc_check_value==true) //only show message when not corrupted
    {
      //Converting the Bits to Text here
      String output = "";
      for(int i = 2; i < (dataBits.length()/8)-1; i++) //first 2 bytes are for the data length and have no msg data; -1 because last Byte is for CRC and has no message data
      {
          String pl = "";
          for(int l = i*8; l < 8*(i+1); l++){ 
              pl += dataBits[l];
          }    
          
          int n = 0;
          for(int j = 0; j < 8; j++)
          {
              int x = (int)(pl[j])-(int)'0';
              for(int k = 0; k < 7-j; k++)  x *= 2;
              n += x;
          }
          output += (char)n;
      }
      Serial.println(output);
    
      dataBits="";
      receiveData_Done=true; 
      crc_check_value=false;
    }
    
  }
}

void checkCRC(String dataFrame)
{
  int polynom[9]={1,0,0,1,0,1,1,1,1};
  dataFrame="10101010101111111111"+dataFrame;
  int k=dataFrame.length();  
  int p=9; //int p=strlen(polynom); // lenght of polynom (normaly fix, but it is better to use a variable if I want to change the polynom later
  int n=k+p-1; //add some zeros to the end of the data for the polynom division
  int frame[n]; //buffer frame with perfect size for CRC 
    
  //convert char array to int array
  for(int i=0;i<n;i++){
    if(i<k){
      frame[i]=dataFrame[i]-'0'; //converts an char number to corresponding int number
    }
    else{
      frame[i]=0;
    } 
  }
   
  //make the division
  int i=0;
  while (  i <  k  ){                     
    for( int j=0 ; j < p ; j++){
            if( frame[i+j] == polynom[j] )  {
                frame[i+j]=0;
            }
            else{
                frame[i+j]=1;
            }     
    }
    while( i< n && frame[i] != 1)
      i++; 
  }
  bool CRC_Done_false=false;  
  for(int j=k; j-k<p-1;j++)
  {
    //erst am Ende des Frames die CRC Sequenz deswegen j=k
    if (frame[j]==1){
      CRC_Done_false=true;
    }     
  }  

  if(CRC_Done_false==false)
  {
    Serial.println("Message has no errors!");
    Serial.println();
    Serial.print("Message: ");
    crc_check_value=true;  
  }

  if(CRC_Done_false==true)
  {
    Serial.println("Message had an error and was dropped!");
    crc_check_value=false;  
    dataBits="";
    receiveData_Done=true;
  }
   
}

int binToChar(String binary, int start, int end) {
  int result = 0;
  for (int i = start ; i < end; i++) {
    result *= 2;
    if (binary[i] == '1') {
      result += 1;
    }
  }
  return result;
}

void restartTransmit() {
  receiveData_Done = true;
  startPacket = false;
  accmuluatedLength = 0;
  messageidx = 0;
  digitalWrite(ReadyPIN, LOW);
}

int BitCount(unsigned int u)
{
    unsigned int uCount;

    uCount = u - ((u >> 1) & 033333333333) - ((u >> 2) & 011111111111);
    return ((uCount + (uCount >> 3)) & 030707070707) % 63;
}

bool isSync() {
  int numError = 0;
  for (int i = 0; i < preamebl_length; i++) {
    int s_ind = sequence[i];
    int p_ind = preamble[i];
    int result = s_ind ^ p_ind;
    numError += BitCount(result);
  }
  // Serial.println(numError);
  if (numError <= ERROR_RATE_WINDOW) {
    return true;
  }
  return false;
}

void durToBin(int num) {
  digitalWrite(durPin1, num & 0x1);
  digitalWrite(durPin2, (num & 0x2) >> 1);
  digitalWrite(durPin3, (num & 0x4) >> 2);
}

void freqToBin(int num) {
  digitalWrite(nodePin1, num & 0x1);
  digitalWrite(nodePin2, (num & 0x2) >> 1);
  digitalWrite(nodePin3, (num & 0x4) >> 2);
}
