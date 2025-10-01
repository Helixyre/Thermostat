/* mbed TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MBED_TEXTLCD_H
#define MBED_TEXTLCD_H

#include "mbed.h"

/**  A TextLCD interface for driving 4-bit HD44780-based LCDs
 *
 * Currently supports 16x2, 20x2 and 20x4 panels
 *
 * @code
 * #include "mbed.h"
 * #include "TextLCD.h"
 * 
 * TextLCD lcd(p10, p12, p15, p16, p29, p30); // rs, e, d4-d7
 * 
 * int main() {
 *     lcd.printf("Hello World!\n");
 * }
 * @endcode
 */
class TextLCD
{
public:

    /** LCD panel format */
    enum LCDType 
    {
        LCD16x2     /**< 16x2 LCD panel (default) */
        , LCD16x2B  /**< 16x2 LCD panel alternate addressing */
        , LCD20x2   /**< 20x2 LCD panel */
        , LCD20x4   /**< 20x4 LCD panel */
        , LCD16x1B    /**< 16x1 LCD panel 2 x "8 Columns Half Lines" >*/
    };

    /** Create a TextLCD interface
     *
     * @param rs    Instruction/data control line
     * @param e     Enable line (clock)
     * @param d4-d7 Data lines for using as a 4-bit interface
     * @param type  Sets the panel size/addressing mode (default = LCD16x2)
     */
    TextLCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type = LCD16x2);


    /** Locate to a screen column and row
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void locate(int column, int row);

    /** Clear the screen and locate to 0,0 */
    void cls();

    /** Print an unique charracter on actual cursor location 
     * @param value  character to print
     */   
    void putc(char value);

    /**Print a string starting on actual cursor location 
     * @param txt[]  String to print
     */
    void puts(char txt[]);

protected:

    int rows();
    int columns();
    int address(int column, int row);
    void character(int column, int row, char c);
    void writeByte(char value);
    void writeCommand(char command);
    void writeData(char data);

    DigitalOut _rs, _e;
    BusOut _d;
    LCDType _type;

    int _column;
    int _row;
};

#endif
