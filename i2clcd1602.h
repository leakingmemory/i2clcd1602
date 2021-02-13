#ifndef _I2CLCD1602_H_
#define _I2CLCD1602_H_

class I2CLCD : public Print {
private:
    uint8_t addr;
    uint8_t mode;
    uint8_t ddaddr;
    void updateToggles();
    void updateFunctions();
    void write8(uint8_t data, uint8_t mode);
    void write4(uint8_t data);
    void set_mode_toggle(uint8_t modeflag, bool enable);
    bool set_mode_functions(uint8_t modeflag, bool enable, bool clear);
public:
    I2CLCD(uint8_t addr);
    void begin();
    void reset();
    void clear();
    void home();

    size_t write(uint8_t ch) override;
    void setPosition(uint8_t line, uint8_t position);
    void cursor(bool enable = true);
    void noCursor() {
        cursor(false);
    }
    void blink(bool enable = true);
    void noBlink() {
        blink(false);
    }
    void backlight(bool enable = true);
    void noBacklight() {
        backlight(false);
    }
    void twoLine(bool enable = true);
    void oneLine() {
        twoLine(false);
    }

    I2CLCD & operator << (const char *str) {
        print(str);
        return *this;
    }

    I2CLCD & operator += (int diff);
    I2CLCD & operator -= (int diff) {
        return *this += -diff;
    }
    I2CLCD & operator ++() {
        return *this += 1;
    }
    I2CLCD & operator ++(int) {
        return *this += 1;
    }
    I2CLCD & operator --() {
        return *this -= 1;
    }
    I2CLCD & operator --(int) {
        return *this -= 1;
    }
};

#endif //_I2CLCD1602_H_
