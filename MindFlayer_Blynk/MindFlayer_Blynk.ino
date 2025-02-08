#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <ESP8266WiFi.h>
#include "pitches.h"
// #include <dht11.h>

// #include "DHT.h"

#define BLYNK_TEMPLATE_ID "TMPL3pGQh8qqp"
#define BLYNK_TEMPLATE_NAME "MindFlayer HomeAutomation"
#include "secrets.h"
#include "songs.h"

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>

#define SERVO_PIN D0
#define BUZZER_PIN D3
#define GREEN_PIN D5
#define BLUE_PIN D6
#define DHT11_PIN D8

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

Servo servo;
LiquidCrystal_I2C lcd(0x27,16,2);

  // dht11 DHT11;
double lastFrameTemperature;

bool partyMode = false;

bool isSongPlaying = false;
int currNote = 0;
int *melody, *durations;
int notesCount;
bool isDisplaying = false;
int displayCounter = 0;

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V0) { // LED
  if (param.asInt()) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED on");
    lcd.print("LED ON");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED off");
    lcd.print("LED OFF");
  }
  isDisplaying = true;
  displayCounter = 0;
}

BLYNK_WRITE(V1) { // Servo
  servo.write(param.asInt());
  Serial.printf("Servo to %d\n", param.asInt());
}

BLYNK_WRITE(V2) { // Music System
  lcd.print("SONG CHANGE");
  displayCounter = 0;
  isDisplaying = true;
  switch (param.asInt()) {
    case 0:
      Serial.println("Music Off");
      isSongPlaying = false;
      return;

    case 1:
      Serial.println("Music : Shape of you");
      melody = melodyShapeOfYou;
      durations = durationShapeOfYou;
      break;
    
    case 2:
      Serial.println("Music : Game of thrones");
      melody = melodyGameOfThrones;
      durations = durationGameOfThrones;
      break;

    case 3:
      Serial.println("Music : Harry Potter");
      melody = melodyHarryPotter;
      durations = durationHarryPotter;
      break;

    case 4:
      Serial.println("Music : Pink Panther");
      melody = melodyPinkPanther;
      durations = durationPinkPanther;
      break;
    
    case 5:
      Serial.println("Music : Pirates of the Caribbean");
      melody = melodyPiratesOfCaribbean;
      durations = durationPiratesOfCaribbean;
      break;
    
    case 6:
      Serial.println("Music : The Lion Sleeps Tonight");
      melody = melodyLionSleepsTonight;
      durations = durationLionSleepsTonight;
      break;
    
    case 7:
      Serial.println("Music : Mario Bros");
      melody = melodyMarioBros;
      durations = durationMarioBros;
      break;
    
    case 8:
      Serial.println("Music : Pacman");
      melody = melodyPacman;
      durations = durationPacman;
      break; 

    case 9:
      Serial.println("Music : Tetris");
      melody = melodyTetris;
      durations = durationTetris;
      break; 

    case 10:
      Serial.println("Music : Happy Birthday");
      melody = melodyHappyBirthday;
      durations = durationHappyBirthday;
      break;
    
    case 11:
      Serial.println("Music : Merry Christmas");
      melody = melodyMerryChristmas;
      durations = durationMerryChristmas;
      break;
  }
  
  isSongPlaying = true;
  currNote = 0;
  notesCount = sizeof(durations) / sizeof(int);

}

BLYNK_WRITE(V3) { // LCD Display
  Serial.printf("Display  Text : %s\n", param.asString());
  lcd.clear();
  lcd.print(param.asString());
  isDisplaying = true;
  displayCounter = 0;
}

BLYNK_WRITE(V4) { // Color LED
  // servo.write(param.asInt());
  lcd.print("RGB LED CHANGE");
  displayCounter = 0;
  isDisplaying = true;
  partyMode = false;
  switch (param.asInt()) {
    // case 1:
    //   Serial.println("RGB LED : red");
    //   digitalWrite(RED_PIN, HIGH);
    //   digitalWrite(GREEN_PIN, LOW);
    //   digitalWrite(BLUE_PIN, LOW);
    //   break;
    
    case 1:
      Serial.println("RGB LED : green");
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      break;
    
    case 2:
      Serial.println("RGB LED : blue");
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      break;
    
    case 3:
      Serial.println("RGB LED : party");
      partyMode = true;  
      break;
    
    case 0:
    default:
      Serial.println("RGB LED : off");
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      break;

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  pinMode(LED_BUILTIN, OUTPUT);
  servo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(DHT11_PIN, INPUT);

  // int chk = DHT11.read(DHT11_PIN);
  // lastFrameTemperature = (double)DHT11.temperature;
  // Blynk.virtualWrite(V5, lastFrameTemperature);
  
  lcd.setCursor(2, 0);
  lcd.print("Welcome");
  displayCounter = 0;
  isDisplaying = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

  // Music System
  if (isSongPlaying) {
    int duration = 1000 / durations[currNote];
    tone(BUZZER_PIN, melody[currNote], duration);
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    currNote++;
    if (currNote > 8 && currNote == notesCount - 1) {
      isSongPlaying = false;
    }
  }

  // Party Mode
  if (partyMode) {
    int randomColor = rand() % 3;
    switch (randomColor) {
      case 0:
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH);
        break;
      
      case 1:
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
        break;
      
      case 2:
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
        break;
    }
  }
  
  //Temperature
  // int chk = DHT11.read(DHT11_PIN);
  // if (((double)DHT11.temperature) != lastFrameTemperature) {
  //   lastFrameTemperature = (double)DHT11.temperature;
  //   Blynk.virtualWrite(V5, lastFrameTemperature);
  // }

  if (isDisplaying) {
    displayCounter++;
    // Serial.print(".");

    if (displayCounter > 20000) {
      Serial.printf("Clearing...\n");
      lcd.clear();
      displayCounter = 0;
      isDisplaying = false;
    }
  }

}

