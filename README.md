# Arduino I2C+LCD1602 library

Licensing: MIT

A library for using an I2C+LCD display.

Connect the I2C board to 5V, GND, SDA, SCL (check manuals). Arduino boards may have marked SDA,SCL- ports or be using certain numbered ports (check manuals). I2C bus is supposed to be able to have multiple slave devices connected using 8-bit addresses.

Add i2clcd1602 module to project and make sure to include the header file:
<pre>
#include &lt;i2clcd1602.h&gt;
</pre>

Normal address for the I2C modules is 0x27, some boards have 0x3F. The address can often be changed using some soldering jumpers on the I2C board. Example csonstruction of I2CLCD object:
<pre>
I2CLCD lcd{0x27};
</pre>

Call begin to start transmitting:
<pre>
lcd.begin();
</pre>

Then various operations are available:
<pre>
  lcd.clear();
  lcd.home(); // To first line, position 0
  lcd << "Cursor";
  lcd.cursor();
  delay(3000);
  lcd.setPosition(0, 0); // To first line, position 0
  lcd << "Blinking cursor";
  lcd.blink();
  delay(3000);
  lcd.noBlink();
  lcd.noCursor();
  lcd.setPosition(0, 0);
  lcd << "Backlight off in";
  lcd.setPosition(1, 0); // Second line, position 0
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
  lcd.oneLine(); // Clears display and returns to position 0
  lcd << "One line display";
  delay(3000);
  lcd.twoLine(); // Clears display and returns to first line, position 0
  lcd << "Two line display";
  lcd.setPosition(1, 0);
  lcd << "Second line";
  delay(3000);
</pre>