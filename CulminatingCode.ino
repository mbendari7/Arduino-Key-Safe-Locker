#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Keypad.h>
#include <Servo.h>

// TFT wiring
#define TFT_CS     A4
#define TFT_RST    A2
#define TFT_DC     A3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {12, 4, 10, 7}; 
byte colPins[COLS] = {8, 9, 1, 0}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LED pins
const int redLED = 2;
const int greenLED = 17;

// Servo setup
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

String passwords[4] = {"1111", "2222", "3333", "4444"};  // Passwords for servos 1-4

void setup() {
  Serial.begin(9600);

  // Setup LED pins
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  // Attach servos
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(11);
  servo4.attach(6);

  // Initialize TFT display
  tft.initR(INITR_BLACKTAB); // Try GREENTAB or REDTAB if screen stays white
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Key Sorter");
  delay(1000);
}

void loop() {
   // Reset LEDs before every run
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 20);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Enter slot #");
  tft.setCursor(10, 50);

  char key = waitForKeypad();

  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 20);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Enter slot #");
  tft.setCursor(10, 50);

  key = waitForKeypad();

  if (key < '1' || key > '4') {
    // Invalid selection
    tft.setTextColor(ST77XX_RED);
    tft.println("Invalid!");
    digitalWrite(redLED, HIGH);
    delay(1500);
    return;
  }

  int servoIndex = key - '1'; // Convert char '1'–'4' to index 0–3

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 20);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Slot ");
  tft.print(key);
  tft.println(" selected");
  delay(1000);

  tft.setCursor(10, 50);
  tft.println("Enter password:");

  String enteredPassword = "";
  while (enteredPassword.length() < 4) {
    char passKey = keypad.getKey();
    if (passKey != NO_KEY && isDigit(passKey)) {
      enteredPassword += passKey;
      tft.print("*");
    }
  }

  // Validate password
  if (enteredPassword == passwords[servoIndex]) {
    // Correct password
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 30);
    tft.setTextColor(ST77XX_GREEN);
    tft.println("Correct!");
    digitalWrite(greenLED, HIGH);
    moveServo(servoIndex + 1); // 1-based servo number
  } else {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 30);
    tft.setTextColor(ST77XX_RED);
    tft.println("Incorrect!");
    digitalWrite(redLED, HIGH);
  }

  delay(3000); // Wait before restarting
}

// Wait for a valid key from keypad
char waitForKeypad() {
  char key = NO_KEY;
  while (key == NO_KEY) {
    key = keypad.getKey();
  }
  return key;
}

void moveServo(int num) {
  switch (num) {
    case 1:
      servo1.write(100);   // Move to 100 degrees
      break;
    case 2:
      servo2.write(100);
      break;
    case 3:
      servo3.write(100);
      break;
    case 4:
      servo4.write(100);
      break;
  }

  delay(1000); // Hold position for 1 second

  // Reset back to 0 degrees
  switch (num) {
    case 1:
      servo1.write(0);
      break;
    case 2:
      servo2.write(0);
      break;
    case 3:
      servo3.write(0);
      break;
    case 4:
      servo4.write(0);
      break;
  }
}
