/* mbed TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2007-2010, sford, http://mbed.org
 * Modif JMS 2020
 */

#include "TextLCD.h"
#include "mbed.h"

TextLCD::TextLCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type) : _rs(rs),_e(e), _d(d4, d5, d6, d7), _type(type) 
{

    _e  = 1;
    _rs = 0;            // command mode

    thread_sleep_for(15);        // Wait 15ms to ensure powered up

    // send "Display Settings" 3 times (Only top nibble of 0x30 as we've got 4-bit bus)
    for (int i=0; i<3; i++) 
    {
        writeByte(0x3);
        thread_sleep_for(2);  // this command takes 1.64ms, so wait for it
    }
    writeByte(0x2);     // 4-bit mode
    wait_us(40);    // most instructions take 40us

    writeCommand(0x28); // Function set 001 BW N F - -
    writeCommand(0x0C);
    writeCommand(0x6);  // Cursor Direction and Display Shift : 0000 01 CD S (CD 0-left, 1-right S(hift) 0-no, 1-yes
    cls();
}

void TextLCD::character(int column, int row, char c) 
{
    int a = address(column, row);
    writeCommand(a);
    writeData(c);
}

void TextLCD::cls() 
{
    writeCommand(0x01); // cls, and set cursor to 0
    thread_sleep_for(2);     // This command takes 1.64 ms
    locate(0, 0);
}

void TextLCD::locate(int column, int row) 
{
    _column = column;
    _row = row;
}

void TextLCD::putc(char value) 
{
    if (value == '\n') 
    {
        _column = 0;
        _row++;
        if (_row >= rows()) 
        {
            _row = 0;
        }
    } 
    else 
    {
        character(_column, _row, value);
        _column++;
        if (_column >= columns()) 
        {
            _column = 0;
            _row++;
            if (_row >= rows()) 
            {
                _row = 0;
            }
        }
    }
}

void TextLCD::puts(char txt[])
{
    int i = 0;
    while(txt[i] != 0x00)
    {
        putc(txt[i]);
        i++;
    }
}


void TextLCD::writeByte(char value) 
{
    _d = value >> 4;
    wait_us(40); // most instructions take 40us
    _e = 0;
    wait_us(40);
    _e = 1;
    _d = value >> 0;
    wait_us(40);
    _e = 0;
    wait_us(40);  // most instructions take 40us
    _e = 1;
}

void TextLCD::writeCommand(char command) 
{
    _rs = 0;
    writeByte(command);
}

void TextLCD::writeData(char data) 
{
    _rs = 1;
    writeByte(data);
}

int TextLCD::address(int column, int row) 
{
    switch (_type) 
    {
        case LCD16x1B:
             return 0x80 + ((column/8) * 0x40) + column%8;
        case LCD20x4:
            switch (row) 
            {
                case 0:
                    return 0x80 + column;
                case 1:
                    return 0xc0 + column;
                case 2:
                    return 0x94 + column;
                case 3:
                    return 0xd4 + column;
            }
        case LCD16x2B:
            return 0x80 + (row * 40) + column;
        case LCD16x2:
        case LCD20x2:
        default:
            return 0x80 + (row * 0x40) + column;
    }
}

int TextLCD::columns() 
{
    switch (_type) {
        case LCD20x4:
        case LCD20x2:
            return 20;
        case LCD16x2:
        case LCD16x2B:
        case LCD16x1B:
        default:
            return 16;
    }
}

int TextLCD::rows() 
{
    switch (_type) 
    {
        case LCD16x1B:
            return 8;
        case LCD20x4:
            return 4;
        case LCD16x2:
        case LCD16x2B:
        case LCD20x2:
        default:
            return 2;
    }
}
