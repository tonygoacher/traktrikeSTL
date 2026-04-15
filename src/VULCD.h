#ifndef __VULCD_H__
#define __VULCD_H__

#include "LiquidCrystal_I2C.h"

class VULCD
{
public:
    VULCD(LiquidCrystal_I2C* lcd);
    void ShowBar(int row, int level);

private:
void Setup();
    LiquidCrystal_I2C* m_lcd;
    void bar(int row, int vol);
    unsigned long m_lastT;

};
#endif // __VULCD_H__