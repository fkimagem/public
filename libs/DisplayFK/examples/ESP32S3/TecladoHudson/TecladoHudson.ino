//Select Keypad
//#define keypadSerial_4pin
//#define keypadSerial_14pin
//#define keypadMatriz_16pin
#define keypadMatriz_20pin


// Constants and Essential Globals
const int button1 = 14;
bool isButtonPressed = false; //locks the key until released.
bool isBalancing = false; // when AD stabilizes
bool AD2 = false;
#define limit 100   // lowest limit AD
#define SAMPLES 30  // Define for the number of samples

// Key Definitions keypad 4 pins
#ifdef keypadSerial_4pin
#define keyCount 4
uint32_t keyVals[keyCount] = {1046529, 543169, 347544, 238144};
char keys[keyCount] =        {   '2' , '1'    , '4'   , '3'  }; 
#endif

// Key Definitions keypad 14 pins
#ifdef keypadSerial_14pin
#define keyCount 14
uint32_t keyVals[keyCount] = {1046529, 1018081, 753424, 586756, 483025, 404496, 337561, 262144, 208849, 144400, 105625, 70756, 38025, 14400};
char keys[keyCount] =        {   '1' , '2'    , '3'   , '4'   , '5'   , '6'   , '7'   , '8'   , '9'   , '0'   , 'a'   , 'b'  , 'c'  , 'd'  };
#endif
// Key Definitions keypad 16 pins ***CONNECT BUTTON2 GPIO27***
#ifdef keypadMatriz_16pin
#define keyCount 16
#define button2  13
uint32_t keyVals[keyCount] = {1046529, 933156, 753424, 616225, 547600, 431609, 360904, 295936, 1046529, 923531, 746496, 611524, 540225, 421201, 361201, 299209};
char keys[keyCount] =        { '1'   ,   '2' ,   '4' ,   '5' ,  '3'  ,   '6' ,  'A'  ,   'b' , '7'    ,   '8' ,  '*'  ,   '0' ,   '9' ,   '#' ,   'C' ,   'D' };

#endif
// Key Definitions keypad 20 pins ***CONNECT BUTTON2 GPIO27***
#ifdef keypadMatriz_20pin
#define keyCount 20
#define button2 13
uint32_t keyVals[keyCount] = {1046529, 933156, 753424, 616225, 547600, 431609, 360904, 295936, 243049, 206116, 1046529, 923531, 746496, 611524, 540225, 421201, 361201, 299209, 241081, 201601};
char keys[keyCount] =        { 'L'   ,   '7' ,   '0' ,   '8' ,  '4'  ,   '5' ,  '1'  ,   '2' ,   'F' ,   'f' , 'R'    ,   '9' ,  'E'  ,   'e' ,   '6' ,   'B' ,   '3' ,   'T' ,   '#' ,  '*'  };
#endif
// Function to display project information
void displayProjectInfo() {
  Serial.println("MULTI KEYBOARD PROJECT with ESP32");
  //Serial.println("path: C:->Users->David->Documents->hudson->##Work->Labmaker->Keyboard->NewCode_Frank");
  Serial.println(__FILE__);
}

// Function to determine the key pressed based on AD value
char getKey(uint32_t adValue) {
  uint32_t offset;
  int countBegin, countEnd;
  // Step through the keyVals array to compare values
  
 if( AD2){
  countBegin = keyCount / 2;
  countEnd = keyCount;
  }
 else{
  countBegin = 0;
 #ifdef button2
  countEnd = keyCount/2;
 #else
  countEnd = keyCount;
 #endif
 }
  for (int i = countBegin; i < countEnd - 1; i++) {
    if (adValue >= keyVals[i]) {
      // Calculate the offset as half the difference between the current and next key
         offset = (keyVals[i] - keyVals[i + 1]) / 2;
      if( i <= countBegin + 1)
         offset = (keyVals[countBegin] - keyVals[countBegin + 1]) / 2;
      
// Compare the adValue with the offset
      if (adValue <= (keyVals[i] + offset)) {
        return keys[i];  // The current key is the match
      } else {
        return keys[i - 1];  // The next key is the match
      }
    }
  }   // end for check array keyvals
  // Handle the case for the last key in the array
   offset = (keyVals[countEnd - 2] - keyVals[countEnd - 1]) / 2;
  if (adValue >= keyVals[countEnd - 1] + offset) {
    return keys[countEnd - 2];  // Second-to-last key
  } else if(adValue >= keyVals[countEnd - 1] - offset){
    return keys[countEnd - 1];  // Last key
  }
  else{
      return ' ';
    }
}
// Function to read buttons and return the analogValue squared
void readButtons(uint32_t &analogValue) {
  analogValue = 0;
  if (analogRead(button1) > limit) {
    for (int i = 1; i <= SAMPLES; i++) {
      analogValue += analogRead(button1);
    }
     AD2 = false;
  }
  #ifdef button2
  else if (analogRead(button2) > limit) {
    //isButtonPressed = false;  // Track button2 pressed
    for (int i = 1; i <= SAMPLES; i++) {
      analogValue += analogRead(button2);
    }
     AD2 = true;
  }
  #endif
  analogValue = analogValue / SAMPLES; // Calculate the average value
  analogValue = analogValue * analogValue; // Square the value
}

void setup() {
  analogReadResolution(10);
  Serial.begin(115200);
  displayProjectInfo();
  pinMode(button1, INPUT);  
}

void loop() {
  String keyLabel = "";
  uint32_t analogValue = 0; 
  
if (!isButtonPressed && (analogRead(button1) > limit 
  #ifdef button2
  || analogRead(button2) > limit
  #endif
)) {
    isButtonPressed = true;
    isBalancing = true;
    uint32_t value1 = 0, value2 = 0, startTime = millis();
    
    while (millis() < (startTime + 10)) {} // Short delay for stability
    
    while (isBalancing) {
      // Read button values and calculate the squared analog average
      readButtons(value2); 
      
      if (value1 >= (value2 - 2000) && value1 <= (value2 + 2000)) {
        isBalancing = false; // Stop balancing when values are stable
      } else {
        value1 = value2;
        value2 = 0;
        if (analogRead(button1) < limit 
        #ifdef button2
          && analogRead(button2) < limit
        #endif
        ) {
          Serial.println("Key not recognized");
          isBalancing = false;
        }
      }
    }

    // Determine the key based on the value read
    char key = getKey(value1);
    switch (key) {
      
          case 'A': keyLabel = "0"; break;
          case 'a': keyLabel = "11"; break;
          case 'b': keyLabel = AD2 ? "B": "12"; break;
          case 'c': keyLabel = "13"; break;
          case 'd': keyLabel = "14"; break;
          case 'E': keyLabel = "Enter"; break;
          case 'e': keyLabel = "Esc"; break;
          case 'F': keyLabel = "F1"; break;
          case 'f': keyLabel = "F2"; break;
          case 'L': keyLabel = "←"; break;
          case 'R': keyLabel = "→"; break;
          case 'B': keyLabel = "↓"; break;
          case 'T': keyLabel = "↑"; break;
          default: keyLabel = String(key); break;
        }
    if (keyLabel != " "){
//    Serial.print(" - AD Value: ");
//    Serial.print(value1);
    Serial.print(" Key: ");
    Serial.println(keyLabel);
    }
  }
  
if (isButtonPressed && (analogRead(button1) < limit 
  #ifdef button2
  && analogRead(button2) < limit
  #endif
)){
    isButtonPressed = false;
  }
}
