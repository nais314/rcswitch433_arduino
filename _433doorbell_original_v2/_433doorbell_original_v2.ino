/*
 * original http://www.theforce.dk/rf/
 * 
 * I had to rewrite this code to fit my devices
 *  
 * On ATmega based boards (UNO, Nano, Mini, Mega), 
 * it takes about 100 microseconds (0.0001 s) to read an analog input, 
 * so the maximum reading rate is about 10,000 times a second.
 * 
 * There will be alwys noise and misreading, 
 * the most times found code is maybe the good one ;)
 */

#define analogPin A0 // RF data pin = Analog pin 0

// set to true for more output:
#define EXPERIMENT false
 
byte dataBuffer[512];
int dataCounter = 0; // LOw or HIGH duration
int maxSignalLength = 255; // something too much to be true

uint32_t code = 0; // the parsed code
String codeStr; // the code in string format "10001110011..."

const int maxCodes = 12;
uint32_t foundCodes[maxCodes][1];
int numCodes;


// experiment with these values
const unsigned int upperThreshold = 80;
const unsigned int lowerThreshold = 2;
const unsigned int SIG_SYNC = 41; // the signal before sending
const unsigned int SIG_END = 40; // the signal between repeated signals

 
void setup() {
 
  Serial.begin(115200);
 
}


 
void loop() { 

  codeStr = "";
  
  Serial.println(" *");

  for (int i = 0; i < sizeof(dataBuffer); i++) dataBuffer[i] = 0;

  
  // wait for a SYNC - a very long signal
  do {
      // wait until a HIGH signal is received..............................
      while(analogRead(analogPin) < lowerThreshold) { }
  
      dataCounter = 0;
      while(analogRead(analogPin) < lowerThreshold && dataCounter < maxSignalLength)
        dataCounter++;

      if(EXPERIMENT) { 
          Serial.print("  ");
          Serial.println(dataCounter); // experiment with these values.
      }

      if (dataCounter > 50) dataCounter = 0; // too long, surely(?) not good
      
  # if EXPERIMENT == false
  } while (dataCounter != SIG_SYNC ); // experiment with these values. my values are 41/45
  # else
  } while (dataCounter < SIG_SYNC); 
  # endif

  if(EXPERIMENT) Serial.print("SYNC > "); Serial.println(dataCounter);
  //Serial.println("----------------------------");



 
  // got HIGH; read the rest of the data into dataBuffer................
  for (int i = 0; i < sizeof(dataBuffer); i = i+2) {
  //for (int i = 0; i < (512); i = i+2) {
   
    // LOW signals
    dataCounter = 0;
    while (analogRead(analogPin) > upperThreshold && dataCounter < maxSignalLength)
      dataCounter++;
   
    dataBuffer[i] = dataCounter;

   
    // HIGH signal
    dataCounter = 0;
    while(analogRead(analogPin) < lowerThreshold && dataCounter < maxSignalLength)
      dataCounter++;
     
    dataBuffer[i+1] = dataCounter;

  }




// ANALYSE AND PRINT..................................................

  //Serial.println("LOW,HIGH"); // for experiments
  delay(20);
  
  //setup
  bool sigBegin = false;
  int bits = 0;
  int first = 0;

  // i had better results, if i seeked until the second repetition of the signal:
  while (dataBuffer[first] < SIG_END) first++;

  first++;

  for (int i = first; i < sizeof(dataBuffer); i = i+2) {
  //for (int i = first; i <= (512); i = i+2) {

    if(EXPERIMENT) {
        Serial.print(dataBuffer[i]); // for experiments
        Serial.print(",");
        Serial.print(dataBuffer[i+1]);
        //delay(20);
    }


    if (dataBuffer[i+1] < SIG_END){
      
        if(dataBuffer[i] > dataBuffer[i+1]){
              code = code << 1;
              code ++;
              codeStr += '1';
              if(EXPERIMENT) Serial.println(" 1 ");
          } else {
              code = code << 1;
              codeStr += '0';
              if(EXPERIMENT) Serial.println(" 0 ");
          };
          
          bits++;
    }

     
    if (dataBuffer[i+1] > SIG_END - 3){

       if (bits == 24) {
          Serial.println("");
          Serial.println("CODE: "); 
          Serial.println(codeStr);
          Serial.println(code);
          Serial.println(bits);

          bool found = false;
          for (int c = 0; c < maxCodes; c++)
              if (foundCodes[c][0] == code){
                 foundCodes[c][1]++;
                 Serial.print(c);
                 Serial.print(": Found ");
                 Serial.print(foundCodes[c][1]);
                 Serial.println(" times.");
                 found = true;
                 break;
              }// end for
              
          if (!found) {
               numCodes = (numCodes == maxCodes - 1)? 0 : numCodes++;
               foundCodes[numCodes][0] = code;
               foundCodes[numCodes][1]++;
               Serial.println(numCodes);
            }
            
          Serial.println("......................\n");
          delay(2000);
        }
        //reset for new code
        code = 0;
        codeStr = "";
        bits = 0;

    }
    
  }  //end for
  
  //delay(1000); Serial.println(".");
}
