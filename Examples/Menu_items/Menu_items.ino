
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TFTLCD_GUI.h>      // GUI
#include <TouchScreen.h>

//#if defined(__SAM3X8E__)
//#undef __FlashStringHelper::F(string_literal)
//#define F(string_literal) string_literal
//#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


// self calibration!
#define TS_MINX 120
#define TS_MAXX 910
#define TS_MINY 90
#define TS_MAXY 910

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define BTN_WIDTH 150
#define BTN_HEIGHT 70

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define GUI_HEIGHT 40
#define ITEM_HEIGHT 40

#define TOUCH_MARGIN 2


//-------------------------------------------------------------------
// TuchScreen and TSPoints
//-------------------------------------------------------------------

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TSPoint p;
TSPoint oldPoint;

//-------------------------------------------------------------------
// CREATE MENUS and ELEMENTS
//-------------------------------------------------------------------

//

// Main
GUI_Menu mainMenu( &tft, NULL);
GUI_Menu subMenu1( &tft, &mainMenu);  // parent menu
GUI_Menu subMenu2( &tft, &mainMenu);  // parent menu
GUI_Menu subMenu3( &tft, &mainMenu);  // parent menu


GUI_MenuItem menu1( &tft, "Menu1", 0, 0, TFTWIDTH, ITEM_HEIGHT);
GUI_MenuItem menu2( &tft, "Menu2", 0, 1 * ITEM_HEIGHT, TFTWIDTH/2, ITEM_HEIGHT);
GUI_MenuItem menu3( &tft, "Menu3", TFTWIDTH/2, 1 * ITEM_HEIGHT, TFTWIDTH/2, ITEM_HEIGHT);


GUI_Label menu1label(&tft,  "MENU1",  GREEN,  1,  0,  1 * ITEM_HEIGHT,  TFTWIDTH,  ITEM_HEIGHT);
GUI_Label menu2label(&tft,  "MENU2",  WHITE,  2,  0,  1 * ITEM_HEIGHT,  TFTWIDTH,  ITEM_HEIGHT);
GUI_Label menu3label(&tft,  "MENU3",  RED,  4,  0,  1 * ITEM_HEIGHT,  TFTWIDTH,  ITEM_HEIGHT);

// Main MENU
GUI_Button BACK (&tft, TFTWIDTH / 2 , TFTHEIGHT - ITEM_HEIGHT, TFTWIDTH / 2, ITEM_HEIGHT, BLUE, WHITE, BLUE, "BACK", 2, 15);

// Pointer to the active menu
GUI_Menu *activeMenu;


//-------------------------------------------------------------------
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//HELPER FUNCTIONS
//-------------------------------------------------------------------
//-------------------------------------------------------------------
boolean checkIfPressingSamePoint(TSPoint old, TSPoint p, uint8_t margin)
{
  return
    old.x > p.x - margin &&
    old.x < p.x + margin &&
    old.y > p.y - margin &&
    old.y < p.y + margin;
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
// BEHAVIOURS
//-------------------------------------------------------------------
//-------------------------------------------------------------------





void goToMainMenu(int16_t x)
{
  if(activeMenu != &mainMenu){
    activeMenu->hide();
    activeMenu = &mainMenu;
    activeMenu->display();
    Serial.println("Main Menu");
  }
}

void goToMenu1(int16_t x)
{
  activeMenu->hide();
  activeMenu = &subMenu1;
  activeMenu->display();
}

void goToMenu2(int16_t x)
{
  activeMenu->hide();
  activeMenu = &subMenu2;
  activeMenu->display();
}

void goToMenu3(int16_t x)
{
  activeMenu->hide();
  activeMenu = &subMenu3;
  activeMenu->display();
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// SETUP
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void setup(void) {

  Serial.begin(9600);

  // TFT setup
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(0x9341);
  tft.fillScreen(BLACK);

  // Menu setup
  mainMenu.addElement(&BACK);
  mainMenu.addElement(&menu1);
  mainMenu.addElement(&menu2);
  mainMenu.addElement(&menu3);
  subMenu1.addElement(&BACK);
  subMenu1.addElement(&menu1label);
  subMenu2.addElement(&BACK);
  subMenu2.addElement(&menu2label);
  subMenu3.addElement(&BACK);
  subMenu3.addElement(&menu3label);

  BACK.setOnClic(goToMainMenu);
  menu1.setOnClic(goToMenu1);
  menu2.setOnClic(goToMenu2);
  menu3.setOnClic(goToMenu3);

  // Final SETUP
  activeMenu = &mainMenu;
  mainMenu.display();

  Serial.println("SETUP COMPLETED.");
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
// LOOPS. 
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void loop(void) {

  p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    if (!checkIfPressingSamePoint(oldPoint, p, TOUCH_MARGIN))
    {
      oldPoint = p;
      activeMenu->loopThroughElements(p.x, p.y);
    }

  } // if p.z is OK

}// LOOP


