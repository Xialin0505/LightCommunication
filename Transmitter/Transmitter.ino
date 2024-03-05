#define LED 7

char result[500]={'1','0','1','0','1','0','1','1','1','1','1','1','1','1','1','1'};
int counter=20;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
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
    
    result[36]='\0';
}

void loop() {
  // put your main code here, to run repeatedly:
  int pos = 0;
  char* msg = {"ABCDEFG"};
  int2bin(strlen(msg)*8, 16);

  for (int k = 0; k < strlen(msg); k++) {
    chartobin(msg[k]);
  }

  int length = strlen(result);
  while (pos != length) {
    delay(100);
    if (result[pos] == '1') {
      digitalWrite(LED, HIGH);
    }
    else if (result[pos] == '0') {
      digitalWrite(LED, LOW);
    }
    pos++;
  }
}
