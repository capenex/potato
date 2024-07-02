#include <LiquidCrystal.h>

#define BUTTON 2

#define SEL 15

LiquidCrystal lcd = LiquidCrystal(5, 6, 7, 8, 9, 10, 11);

byte select[8] = {
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};
char des[17] = {};

#define ALL_MENU_OPTIONS 3

char menuStr[8*ALL_MENU_OPTIONS] = " photo   music   back    ";

int menuOptions = 0;

char* scroll(char* src, int len_src, int begin, int len_des=16) ;
void write(int n); // lcd.write(byte(n)) not neccessary
void menu(int button_left_or_right/*0: left, 1: right*/);


void menu(int button_left_or_right/*0: left, 1: right*/) {
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

  // if (button_left_or_right == 1) 
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

// example setup and loop
void setup() {
  lcd.begin( 16, 2 ); // set up the LCD’s numbers of colomns and rows

  lcd.createChar(SEL, select);
  lcd.clear(); // clear LCD
  lcd.setCursor( 0, 0 );

  lcd.print(scroll(menuStr, sizeof(menuStr), 0));
  lcd.setCursor(0,0);
  write(SEL);
  
}
void loop(){
  menu(0);
  delay(1000);
}


