#include <Arduino.h>
#include <Wire.h>
#include "i2clcd1602.h"

#define SET4BIT 0x20
#define SET8BIT 0x30

#define LCD_CMD_FUNC 0x20
#define LCD_CMD_TOGGLES 0x08
#define LCD_CMD_HOME 0x02
#define LCD_CMD_SETADDR 0x80

#define LCD_MODE_BACKLIGHT 1
#define LCD_MODE_BLINK 2
#define LCD_MODE_CURSOR 4
#define LCD_MODE_ENABLED 8
#define LCD_MODE_TWOLINE 16

I2CLCD::I2CLCD(uint8_t addr) : addr(addr), mode(LCD_MODE_BACKLIGHT | LCD_MODE_ENABLED | LCD_MODE_TWOLINE), ddaddr(0) {
}

void I2CLCD::begin() {
    Wire.begin();
    reset();
}

void I2CLCD::write8(uint8_t data, uint8_t mode) {
    write4((data & 0xF0) | mode);
    write4(((data & 0x0F) << 4) | mode);
}

void I2CLCD::write4(uint8_t data) {
    uint8_t backlight = (mode & LCD_MODE_BACKLIGHT) != 0 ? 8 : 0;
    Wire.beginTransmission(addr);
    Wire.write((int) (data | backlight));
    Wire.endTransmission();
    Wire.beginTransmission(addr);
    Wire.write((int) (data | 4 | backlight)); // Enable
    Wire.endTransmission();
    delayMicroseconds(1);
    Wire.beginTransmission(addr);
    Wire.write((int) ((data & (~4)) | backlight));
    Wire.endTransmission();
}

void I2CLCD::reset() {
    delay(50);

    Wire.beginTransmission(addr);
    Wire.write(0);
    Wire.endTransmission();
    delay(1000);

    write4(SET8BIT);
    delayMicroseconds(4600);
    write4(SET8BIT);
    delayMicroseconds(4600);
    write4(SET8BIT);
    delayMicroseconds(4600);
    write4(SET4BIT);
    updateFunctions();

    updateToggles();

    // Make sure it is in a predictive state
    clear();
    home();
}

void I2CLCD::updateToggles() {
    write8(LCD_CMD_TOGGLES | ((mode & LCD_MODE_ENABLED) ? 4 : 0) | ((mode & LCD_MODE_CURSOR) ? 2 : 0) | ((mode & LCD_MODE_BLINK) ? 1 : 0), 0);
    delayMicroseconds(40);
}

void I2CLCD::updateFunctions() {
    write8(LCD_CMD_FUNC | ((mode & LCD_MODE_TWOLINE) ? 8 : 0), 0);
    delayMicroseconds(40);
}

void I2CLCD::clear() {
    write8(1, 0);
    delayMicroseconds(2000);
}

void I2CLCD::home() {
    write8(LCD_CMD_HOME, 0);
    ddaddr = 0;
    delayMicroseconds(2000);
}

I2CLCD & I2CLCD::operator += (int diff) {
    if (diff > 0) {
        while (diff > 0x4F) {
            diff -= 0x50;
        }
        if ((mode & LCD_MODE_TWOLINE) == 0) {
            ddaddr += (uint8_t) diff;
            if (ddaddr >= 0x50) {
                ddaddr -= 0x50;
            }
            write8(LCD_CMD_SETADDR, 0);
            delayMicroseconds(40);
        } else {
            if (ddaddr < 0x28) {
                ddaddr += diff;
                if (ddaddr > 0x27) {
                    ddaddr += 0x18;
                    if (ddaddr > 0x67) {
                        ddaddr -= 0x68;
                    }
                }
            } else {
                ddaddr += diff;
                if (ddaddr > 0x67) {
                    ddaddr -= 0x68;
                    if (ddaddr > 0x27) {
                        ddaddr += 0x18;
                    }
                }
            }
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        }
    } else {
        while (diff < -0x4F) {
            diff += 0x50;
        }
        return *this += (0x50 + diff);
    }
    return *this;
}

size_t I2CLCD::write(uint8_t ch) {
    write8(ch, 1);
    ddaddr++;
    if ((mode & LCD_MODE_TWOLINE) == 0) {
        if (ddaddr >= 0x50) {
            while (ddaddr >= 0x50) {
                ddaddr -= 0x50;
            }
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        }
    } else {
        if (ddaddr > 0x27 && ddaddr < 0x40) {
            ddaddr += (0x40 - 0x28);
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        } else if (ddaddr > 0x67) {
            ddaddr -= 0x68;
            while (ddaddr > 0x27) {
                ddaddr -= 0x28;
            }
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        }
    }
    return 1;
}

void I2CLCD::setPosition(uint8_t line, uint8_t pos) {
    if (line == 0) {
        if (pos < 0x50 && ((mode & LCD_MODE_TWOLINE) == 0) || pos < 0x28) {
            ddaddr = pos;
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        }
    } else if (line == 1 && (mode & LCD_MODE_TWOLINE) != 0) {
        if (pos < 0x28) {
            ddaddr = pos + 0x40;
            write8(LCD_CMD_SETADDR | ddaddr, 0);
            delayMicroseconds(40);
        }
    }
}

void I2CLCD::set_mode_toggle(uint8_t modeflag, bool enable) {
    uint8_t mode = enable ? (this->mode | modeflag) : (this->mode & ~modeflag);
    if (mode != this->mode) {
        this->mode = mode;
        updateToggles();
    }
}

bool I2CLCD::set_mode_functions(uint8_t modeflag, bool enable, bool clearDisp) {
    uint8_t mode = enable ? (this->mode | modeflag) : (this->mode & ~modeflag);
    if (mode != this->mode) {
        if (clearDisp) {
            clear();
        }
        this->mode = mode;
        updateFunctions();
        if (clearDisp) {
            home();
        }
        return true;
    }
    return false;
}

void I2CLCD::cursor(bool enable) {
    set_mode_toggle(LCD_MODE_CURSOR, enable);
}

void I2CLCD::blink(bool enable) {
    set_mode_toggle(LCD_MODE_BLINK, enable);
}

void I2CLCD::backlight(bool enable) {
    set_mode_toggle(LCD_MODE_BACKLIGHT, enable);
}

void I2CLCD::twoLine(bool enable) {
    set_mode_functions(LCD_MODE_TWOLINE, enable, true);
}
