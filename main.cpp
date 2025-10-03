#include "mbed.h"
#include "TextLCD.h"

TextLCD LCD(D2,D3,D4,D5,D6,D7,TextLCD::LCD16x2);
DigitalIn S1(D9, PullUp);
DigitalIn S2(D10, PullUp);
DigitalIn S3(D11, PullUp);
AnalogIn ADC0(A0);

char txt[20];

float val;
int page = 0;
float count1 = 0.0;
float count2 = 0.0;
float count3 = 0.0;

void screen()
{

    float temp = ADC0.read();
    float voltage = temp * 3.3;
    float tempC = voltage * 100.0;

    sprintf(txt, "Temp: %05.2f C", tempC);
    LCD.locate(0,0);
    LCD.puts(txt);

    switch(page) {
        case 0:
            sprintf(txt, "Conf H: %05.2f", count1);
            break;
        case 1:
            sprintf(txt, "Conf L: %05.2f", count2);
            break;
        case 2:
            sprintf(txt, "Conf AI: %05.2f", count3);
            break;
    }
    LCD.locate(0,1);
    LCD.puts(txt);
}

void page0()
{
	screen();
	if(S1.read() == 0)
	{
		thread_sleep_for(100);
		while(S1.read() == 0)
		{
		}
		if(count1 < 30.0)
		{
			count1 += 0.5;
		}
	}
	if(S2.read() == 0)
	{
		thread_sleep_for(100);
		while(S2.read() == 0)
		{
		}
		if(count1 > -5.0)
		{
			count1 -= 0.5;
		}
	}
}

void page1()
{
	screen();
	if(S1.read() == 0)
	{
		thread_sleep_for(100);
		while(S1.read() == 0)
		{
		}
		if(count2 < 20.0)
		{
			count2 += 0.5;
		}
	}
	if(S2.read() == 0)
	{
		thread_sleep_for(100);
		while(S2.read() == 0)
		{
		}
		if(count2 > -10.0)
		{
			count2 -= 0.5;
		}
	}
}

void page2()
{
	screen();
	if(S1.read() == 0)
	{
		thread_sleep_for(100);
		while(S1.read() == 0)
		{
		}
		if(count3 < 30.0)
		{
			count3 += 0.5;
		}
	}
	if(S2.read() == 0)
	{
		thread_sleep_for(100);
		while(S2.read() == 0)
		{
		}
		if(count3 > -15.5)
		{
			count3 -= 0.5;
		}
	}
}

int main()
{
	thread_sleep_for(100);
	LCD.locate(2, 1);
	LCD.puts("TCAN");


	while(1)
	{
		if(S3.read() == 0)
		{
			thread_sleep_for(100);
			while(S3.read() == 0)
			{
			}
			page++;
			if(page > 2)
			{
				page = 0;
			}
			LCD.cls();
		}
		switch(page)
		{
			case 0:
				page0();
				break;
			case 1:
				page1();
				break;
			case 2:
				page2();
				break;
			default:
				page = 0;
				break;
		}
		thread_sleep_for(100);

	}
}
