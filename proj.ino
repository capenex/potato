#include <LiquidCrystal.h>
#include "DHT.h"
#include <alloca.h>
#include "character.h"
#include "header.h"

#define DHTPIN 3
#define DHTTYPE DHT11
#define BUZZERPIN 4
#define BUTTON 2

#define SEL 15
#define HEART 14



void playsound(int buzzerPin, int* melody, int* noteDurations, int length);
void playsound_with_waves(int buzzerPin, int* melody, int* noteDurations, int length);
// void showMenu(char);

// #define DISTANCE_SENSOR_1 2
// #define DISTANCE_SENSOR_1 1

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd = LiquidCrystal(5, 6, 7, 8, 9, 10, 11);
int iCounter = 0; // counter
byte chara[8] = {
  0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000
};

char line1[17] = {};
char line2[17] = {};
char des[17] = {};

#define ALL_MENU_OPTIONS 3
char menuStr[8*ALL_MENU_OPTIONS] = " photo   music   back    ";


int menuOptions = 0;



char* scroll(char* src, int len_src, int begin, int len_des=16) ;
void write(int n); // lcd.write(byte(n)) not neccessary
char* menu(int button_left_or_right/*0: left, 1: right*/);



void setup() {
  dht.begin();

  lcd.begin( 16, 2 ); // set up the LCD’s numbers of colomns and rows

  // set up the waves for the sound
  lcd.createChar(0, space);
  for (int i=1; i<9; i++){
    chara[8-i] = 31;
    lcd.createChar(i, chara);
  }

  lcd.createChar(SEL, select);
  lcd.clear(); // clear LCD
  lcd.setCursor( 0, 0 );
  lcd.print(scroll(menuStr, sizeof(menuStr), 0));
  lcd.setCursor(0,0);
  write(SEL);
  Serial.begin(9600);
  pinMode(BUTTON, OUTPUT);

  
}

void loop() {
  // delay(100);
  // int temperatureC = int(dht.readTemperature());
  // int temperatureF = int(dht.readTemperature(true));
  // int humidity = int(dht.readHumidity());
  // lcd.print(temperatureC);
  // lcd.write(byte(1));
  // lcd.print("C ");
  // lcd.print(temperatureF);
  // lcd.write(byte(1));
  // lcd.print("F ");
  // lcd.write(byte(0));
  // char src[] = "hello world! hello potatoes!";
  // for (int i=0; i<16; i++) {
  //   lcd.setCursor(0, 1);
  //   char *s = scroll(src, sizeof(src)/sizeof(char), i, 15);
  //   lcd.print(s);
  //   // free(s);
  //   delay(500);
  // }
  // playsound_with_waves(BUZZERPIN, melody, noteDurations, sizeof(noteDurations)/sizeof(int));
  
  // if (digitalRead(BUTTON) == HIGH) {
  //   while (digitalRead(BUTTON) == HIGH) {
  //     delay(40);
  //   }
  //   delay(40);
  // }
  // lcd.print(menu(0));
  delay(1000);
  menu(0);

}

char* menu(int button_left_or_right/*0: left, 1: right*/) {
  if (button_left_or_right == 0) {
    menuOptions ++;
    if (menuOptions == ALL_MENU_OPTIONS) {
      menuOptions = 0;
    }
    if (menuOptions == 0) {
      lcd.setCursor(0,0);
      lcd.print(scroll(menuStr, sizeof(menuStr)/sizeof(char), menuOptions * 8));
      lcd.setCursor(0,0);
      write(SEL);
    } 
    // else if (menuOptions == 1) {
    //   lcd.setCursor(0,0);
    //   lcd.print(scroll(menuStr, sizeof(menuStr)/sizeof(char), menuOptions * 8));
    //   lcd.setCursor(8,0);
    //   write(SEL);
    // }
    else {
      lcd.setCursor(0,0);
      lcd.print(scroll(menuStr, sizeof(menuStr)/sizeof(char), (menuOptions-1) * 8));
      lcd.setCursor(8,0);
      write(SEL);

    }
  }

  return menuStr;
}

char* scroll(char* src, int len_src, int begin, int len_des=16) {
  // char* des = alloca(len_des+1);
  for (int i=0; i<len_des; i++) {
    if (begin+i<len_src) {
      des[i] = src[i+begin];
    }else{
      int index = i + begin;
      while (index >= len_src) {
        index -= len_src;
      }
      des[i] = src[index];
    }
  }
  des[len_des] = 0;
  return des;
}

void write(int n){
  if (n!=0){
  lcd.write(byte(n));
  } else {
    lcd.print(" ");
  }
}



int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, // "Happy Birthday to You"
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, // "Happy Birthday to You"
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, // "Happy Birthday dear [Name]"
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4 // "Happy Birthday to You"
};
// Define the note durations
int noteDurations[] = {
  4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 2
};

int above_zero(int n) {
  if (n<0) return 0;
  return n;
}

void playsound_with_waves(int buzzerPin, int* melody, int* noteDurations, int length){
  for (int thisNote = 0; thisNote < length; thisNote++) {
    // To calculate the note duration, take one second divided by the note type.
    lcd.setCursor(0,0);
    for (int i=0; i<16; i++)lcd.print(" ");
    lcd.setCursor(0,0);
    // A2 110 A6 1760
    int n = ((double)melody[thisNote]-110) /(460-110)*15;
    if (n<0) n=0; if (n>15) n=15;
    for (int i=0; i<15; i++) {
      write(above_zero(8-3*abs(i-n)));
    }
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.2;
    delay(pauseBetweenNotes);

    // Stop the tone playing:
    noTone(buzzerPin);
  }
}

void playsound(int buzzerPin, int* melody, int* noteDurations, int length) {
  for (int thisNote = 0; thisNote < length; thisNote++) {
    // To calculate the note duration, take one second divided by the note type.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Stop the tone playing:
    noTone(buzzerPin);
  }

}
