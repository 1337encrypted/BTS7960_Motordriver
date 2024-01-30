#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C menuList(U8G2_R0);                                                               //Create object for oled panel

void menuListBegin()
{
  menuList.setColorIndex(1);        // set the color to white
  menuList.begin();                 // display in the screen
  menuList.setBitmapMode(1);        // able to merge two images, draws only white pixels
  menuList.setFont(u8g2_font_Pixellari_tf);
}

void write(const char* message)
{
  menuList.clearBuffer();  // clear buffer for storing display content in RAM
  menuList.drawStr(40,25+15+2, message); 
  menuList.sendBuffer(); // send buffer from RAM to display controller 
}