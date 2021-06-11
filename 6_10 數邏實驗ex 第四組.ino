/*
  Melody
 */
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

#define BUTTON 2
#define BUTTON2 3

//設定各接腳
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
// notes in the melody:
int melody[] = {
  NOTE_C3, NOTE_C3, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_G3,0
};
int melody2[] = {
  NOTE_C4, NOTE_G3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_E3, NOTE_C3,0
};
int noteDurations[] = {
 4,4,4,4,4,4,4,4
};

int thisNote=-1,noteDuration=0;
long previousTime=0,presentTime=0,pauseBetweenNotes=0;

byte colDataMatrix[8] = {
   B11111110,
   B11111101,
   B11111011,
   B11110111,
   B11101111,
   B11011111,
   B10111111,
   B01111111
}; //掃描
byte rowDataMatrix[8] = {
   B00010000,
   B00111000,
   B00111000,
   B00111000,
   B00111000,
   B00111000,
   B11111110,
   B01101100,
}; //圖案A

byte worDataMatrix[8] = {
   B00000000,
   B00000000,
   B01011010,
   B10011001,
   B01100110,
   B00000000,
   B00000000,
   B00000000,
}; //圖案

int previousState=1, presentState=1, patternNumber=0;
int mPreviousState=1, mPresentState=1, mPatternNumber=0;


void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
}

void checkToPlay(int n){

  if(n==0){
  		presentTime=millis();
        if(presentTime-previousTime>=pauseBetweenNotes)
        {
          thisNote+=1;
          if(thisNote>=8)
            {
              thisNote=-1;
              pauseBetweenNotes=100;
              previousTime=millis();
            }
          else 
          {
              noteDuration = (500 / noteDurations[thisNote]);
              tone(9,melody[thisNote],noteDuration);
              pauseBetweenNotes=noteDuration*1.2;
              previousTime=millis();
                
           }
        }
  }

    else{
  		presentTime=millis();
         if(presentTime-previousTime>=pauseBetweenNotes)
            {
               thisNote+=1;
               if(thisNote>=8)
                   {
                      thisNote=-1;
                      pauseBetweenNotes=100;
                       previousTime=millis();
                   }
                else 
                   {
                      noteDuration = (500 / noteDurations[thisNote]);
                      tone(9,melody2[thisNote],noteDuration);
                      pauseBetweenNotes=noteDuration*1.2;
                      previousTime=millis();
                    }
              }
    }

}



void loop() {

  presentState=digitalRead(BUTTON);
  if(previousState==1&&presentState==0)
  {
  	patternNumber++;
    if(patternNumber>1) patternNumber=0;
  }
      mPresentState=digitalRead(BUTTON2);
  if(mPreviousState==1&&mPresentState==0){
  mPatternNumber++;
    thisNote=-1;
    if(mPatternNumber>1) mPatternNumber=0;
  }
  for(int i = 0; i < 8 ; i++){

    digitalWrite(latchPin,LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, colDataMatrix[i]);
    if(patternNumber==0)
    	shiftOut(dataPin, clockPin, MSBFIRST, rowDataMatrix[i]);
    if(patternNumber==1)
    	shiftOut(dataPin, clockPin, MSBFIRST, worDataMatrix[i]);
    digitalWrite(latchPin,HIGH);
    checkToPlay(mPatternNumber);
    delay(1);
    presentState=presentState;
  }

}
