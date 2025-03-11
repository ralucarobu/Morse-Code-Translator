#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <avr/pgmspace.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char morseAlphabet[36][6] PROGMEM = {
  ".-",   "-...", "-.-.", "-..",  ".",    "..-.", "--.",  "....", "..",   ".---",
  "-.-",  ".-..", "--",   "-.",   "---",  ".--.", "--.-", ".-.",  "...",  "-",
  "..-",  "...-", ".--",  "-..-", "-.--", "--..",
  ".----","..---","...--","....-",".....","-....","--...","---..","----.","-----"
};

const char letters[36] PROGMEM = {
  'A','B','C','D','E','F','G','H','I','J',
  'K','L','M','N','O','P','Q','R','S','T',
  'U','V','W','X','Y','Z',
  '1','2','3','4','5','6','7','8','9','0'
};


const int morseButtonPin = 2;   
const int buzzerPin       = 6; 
const int gameButtonPin   = 4;  
const int ledPin          = 12; 


unsigned long morsePressStartTime = 0;
bool prevMorseButtonState         = HIGH;
unsigned long lastMorseInputTime  = 0;
const unsigned long decodeDelay   = 2000;


static char morseSequence[16] = "";
static char decodedText[50]   = "";


bool gameMode = false;

const char* wordList[] = {"HELLO", "HAPPY", "CAKE", "DAY", "NIGHT", "MAN", "PMP"};
const int    wordCount  = 5;

static char randomWord[10] = "";
static char typedWord[10]  = "";

void appendChar(char* buffer, size_t bufSize, char c) {
  size_t len = strlen(buffer);
  if (len + 1 < bufSize) {
    buffer[len]   = c;
    buffer[len+1] = '\0';
  }
}

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed!"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  pinMode(morseButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(gameButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  randomSeed(analogRead(A0));

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Morse Code Ready"));
  display.println(F("Btn4 => Game Mode"));
  display.display();
  delay(1000);

  updateDisplay();
}

void loop() {
  handleGameButton();

  bool currentMorseButtonState = digitalRead(morseButtonPin);

  if (currentMorseButtonState == LOW && prevMorseButtonState == HIGH) {
    morsePressStartTime = millis();
  }

  if (currentMorseButtonState == HIGH && prevMorseButtonState == LOW) {
    unsigned long pressDuration = millis() - morsePressStartTime;

    if (pressDuration < 500) {
      appendChar(morseSequence, sizeof(morseSequence), '.');
      playDotSound();
    } else {
      appendChar(morseSequence, sizeof(morseSequence), '-');
      playDashSound();
    }

    lastMorseInputTime = millis();
    updateDisplay();
  }
  prevMorseButtonState = currentMorseButtonState;

  if ((strlen(morseSequence) > 0) && (millis() - lastMorseInputTime >= decodeDelay)) {
    decodeMorseSequence();
  }
}

void handleGameButton() {
  static bool prevGameButtonState = HIGH;
  bool currentGameButtonState     = digitalRead(gameButtonPin);

  if (prevGameButtonState == HIGH && currentGameButtonState == LOW) {
    gameMode = !gameMode;

    if (gameMode) {
      pickRandomWord(randomWord, sizeof(randomWord));
      typedWord[0]    = '\0';
      decodedText[0]  = '\0';
      morseSequence[0]= '\0';
    } else {
      typedWord[0]    = '\0';
      randomWord[0]   = '\0';
      morseSequence[0]= '\0';
    }
    updateDisplay();
  }
  prevGameButtonState = currentGameButtonState;
}

void pickRandomWord(char* destBuffer, size_t bufSize) {
  int index = random(wordCount);
  strncpy(destBuffer, wordList[index], bufSize - 1);
  destBuffer[bufSize - 1] = '\0';
}

void decodeMorseSequence() {
  char decodedChar = decodeMorse(morseSequence);
  morseSequence[0] = '\0';

  if (decodedChar == '?') {
    updateDisplay();
    return;
  }

  if (!gameMode) {
    appendChar(decodedText, sizeof(decodedText), decodedChar);
  } else {
    appendChar(typedWord, sizeof(typedWord), decodedChar);

    if (strlen(typedWord) == strlen(randomWord)) {
      if (strcmp(typedWord, randomWord) == 0) {
        Serial.println("Correct word!");
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
      } else {
        Serial.println("Wrong word!");
      }
      typedWord[0] = '\0';
    }
  }
  updateDisplay();
}

char decodeMorse(const char* code) {
  for (int i = 0; i < 36; i++) {
    char storedMorse[6];
    strcpy_P(storedMorse, morseAlphabet[i]);
    if (strcmp(code, storedMorse) == 0) {
      return (char)pgm_read_byte(&(letters[i]));
    }
  }
  return '?';
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (!gameMode) {
    display.setCursor(0, 0);
    display.println(F("Normal Mode"));
    display.setCursor(0, 10);
    display.println(F("Morse Input:"));
    display.setCursor(0, 20);
    display.println(morseSequence);

    display.setCursor(0, 35);
    display.println(F("Decoded Text:"));
    display.setCursor(0, 45);
    display.println(decodedText);
  } else {
    display.setCursor(0, 0);
    display.println(F("GAME MODE"));
    display.setCursor(0, 10);
    display.print(F("Target: "));
    display.println(randomWord);

    display.setCursor(0, 25);
    display.print(F("Morse now: "));
    display.println(morseSequence);

    display.setCursor(0, 40);
    display.print(F("Typed so far: "));
    display.println(typedWord);
  }

  display.display();
}

void playDotSound() {
  tone(buzzerPin, 1000, 100);
  delay(100);
}

void playDashSound() {
  tone(buzzerPin, 500, 300);
  delay(300);
}
