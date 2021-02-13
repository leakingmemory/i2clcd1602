#include <Arduino.h>
#include "../i2clcd1602.h"

I2CLCD lcd{0x27};

void setup() {
    lcd.begin();
    lcd << "Hello there!";
    lcd.setPosition(1, 0);
    lcd << "<<<<< #### >>>>>";
    lcd.setPosition(0, 14);
    lcd << "%%";
    lcd--;
    lcd << ")";
    lcd += 0x1D;
}

void loop() {
    delay(3000);
    lcd.clear();
    lcd.home();
    lcd << "Cursor";
    lcd.cursor();
    delay(3000);
    lcd.setPosition(0, 0);
    lcd << "Blinking cursor";
    lcd.blink();
    delay(3000);
    lcd.noBlink();
    lcd.noCursor();
    lcd.setPosition(0, 0);
    lcd << "Backlight off in";
    lcd.setPosition(1, 0);
    lcd << "3";
    delay(1000);
    lcd << " 2";
    delay(1000);
    lcd << " 1";
    delay(1000);
    lcd << " 0";
    lcd.noBacklight();
    delay(3000);
    lcd.backlight();
    lcd.oneLine();
    lcd << "One line display";
    delay(3000);
    lcd.twoLine();
    lcd << "Two line display";
    lcd.setPosition(1, 0);
    lcd << "Second line";
    delay(3000);
}
