/* 
 * Simple 2-channel VU-meter with peak detect - by mira308sw - 2013
 * a crazy sketch for 16x2 lcd display
 */


#include "Arduino.h"
#include "VULCD.h"



/* Other minor configurable value
 */
#define T_REFRESH    100            // msec bar refresh rate
#define T_PEAKHOLD   2*T_REFRESH    // msec peak hold time before return

/* local variable
 */
byte  fill[6]={ 0x20,0x00,0x01,0x02,0x03,0xFF };      // character used to fill (0=empty  5=full)
byte  peak[7]={ 0x20,0x00,0x04,0x05,0x06,0x07,0x20 }; // character used to peak indicator
int   lmax[2];                                        // level max memory
int   dly[2];                                         // delay & speed for peak return

void  VULCD::bar( int row,int lev )
{
  m_lcd->setCursor( 0,row );
  m_lcd->write( 'V');
  for( int i=1 ; i<16 ; i++ )
  {
    int f=constrain( lev      -i*5,0,5 );
    int p=constrain( lmax[row]-i*5,0,6 );
    if( f )
      m_lcd->write( fill[ f ] );
    else
      m_lcd->write( ' ');//[ p ] );
  }
  if( lev>lmax[row] )
  {
    lmax[row] = lev;
    dly[row]  = -(T_PEAKHOLD)/T_REFRESH;                // Starting delay value. Negative=peak don't move
  }
  else
  {
    if( dly[row]>0 )
      lmax[row] -= dly[row]; 

    if( lmax[row]<0 )
      lmax[row]=0;
    else
      dly[row]++;
  }
}

byte block[8][8]=
{
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },  // define character for fill the bar
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },

  { 0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08 },  // define character for peak level
  { 0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04 },
  { 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02 },
  { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 },
};

void  VULCD::Setup  ( )
{
  m_lcd->begin( 16,2 );
  for( int i=0 ; i<8 ; i++ )
    m_lcd->createChar( i,block[i] );
}

VULCD::VULCD(LiquidCrystal_I2C* lcd)
{
    m_lcd = lcd;
    Setup();
    m_lastT = 0;
}


void  VULCD::ShowBar(int row, int vol)
{
    if( millis()<m_lastT )
        return;
    m_lastT += T_REFRESH;  
    int analog = map( sqrt( analogRead( vol  )*16 ),0,128,0,80 );    
    bar(row,analog);
}
