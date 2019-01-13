

#ifndef _TFTLCD_GUI_H_
#define _TFTLCD_GUI_H_

#include <Elegoo_GFX.h>

#define TFTWIDTH   240
#define TFTHEIGHT  320


// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

class GUI_MenuItem;
class GUI_Menu;
class GUI_Button;
class GUI_Value_Box;
class GUI_Value_Bar;
class GUI_Label;
class GUI_Element;


/***************************************************************************/
// HEADER for the Elegoo TFT GUI Element BASE ABSTRACT CLASS

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/
class GUI_Element {
public:
	GUI_Element(
		Elegoo_GFX *gfx,
		uint16_t x,
		uint16_t y,
		uint16_t w,
		uint16_t h
	);

	// Need to override
	// 
	virtual void display() = 0;
	virtual void hide() = 0;


	// Defualt behaviour:
	void setOnClic(const void (*function)(int16_t x));
	void onClic(int16_t x);
	boolean contains(int16_t x, int16_t y);
	void setMenu(GUI_Menu *menu);
	void enable();
	void disable();
	boolean isEnabled();

protected:
	Elegoo_GFX *_gfx;
	uint16_t _x;
	uint16_t _y;
	uint16_t _w;
	uint16_t _h;
	GUI_Menu *_menu;
	boolean *_enabled;
	void (*_onClick)(int16_t x);
};



/***************************************************************************/
// HEADER for the Elegoo TFT GUI Menu

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/
class GUI_Menu {
public:
	GUI_Menu(Elegoo_GFX *gfx, GUI_Menu *parentMenu);

	void loopThroughElements(int16_t x, int16_t y);
	void display();
	void hide();
	GUI_Menu *getParent();
	void addElement(GUI_Element *element);

private:
	Elegoo_GFX *_gfx;
	GUI_Menu *_parentMenu;

	struct elements{
		GUI_Element* elem;
		elements* next;

		elements(GUI_Element* element){
			elem=element;
			next=0;
		}
	};

	elements* head;
};


/***************************************************************************/
// HEADER for the Elegoo TFT GUI Menu Item

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

class GUI_MenuItem : public GUI_Element {
public:
	GUI_MenuItem(
		Elegoo_GFX *gfx,
		char  *label,
		uint16_t x,
		uint16_t y,
		uint16_t w,
		uint16_t h
		);

	void display();
	void hide();

private:
	char _label[15];
	// void (*_onClick)(void);

};

/***************************************************************************/
// HEADER for the Elegoo TFT GUI Value Bar

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/
class GUI_Value_Bar : public GUI_Element {
public:
  GUI_Value_Bar(
    Elegoo_GFX *gfx,
    int16_t min, 
    int16_t max, 
	uint16_t x,
	uint16_t y,
	uint16_t w,
	uint16_t h,
    uint16_t barcolor,
    uint16_t outlinecolor
  );

  void display();
  void hide();

  void updateValue(float value);
  void displayValue();
  void displayFilledBar();
  void setValue(float value);

  float getValue();
  int16_t getX();
  int16_t getY();
  uint8_t getW();
  uint16_t getMin();
  uint16_t getMax();

  // void setOnClic(const void (*function)(int16_t x));
  // void onClic(int16_t x);

private:
	int16_t _min, _max;
	uint16_t _barcolor;
	float *_value;
	uint16_t _outlinecolor;

	// void (*_onClick)(int16_t x);


}; // VALUE_BAR


/***************************************************************************/
// HEADER for the GUI Label UI element

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

class GUI_Label : public GUI_Element {
public:
	GUI_Label(
	    Elegoo_GFX *gfx, 
	    char* label, 
	    uint16_t textcolor, 
	    uint8_t textsize, 
	    uint16_t x, 
	    uint16_t y,
	    uint16_t w, 
	    uint16_t h
    ) ;

	void display();
	void hide();

	void setLabel(char *label);

private:
	char _label[15];
	uint16_t _textcolor;
	uint8_t _textsize;

};

/***************************************************************************/
// HEADER for the GFX value BOX UI element

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/
class GUI_Value_Box : public GUI_Element
{
public:
	GUI_Value_Box(
	    Elegoo_GFX *gfx,
	    uint16_t x,
	    uint16_t y,
	    uint16_t w,
	    uint16_t h,
	    char *name,
	    uint16_t textcolor,
	    uint8_t textsize,
	    float min,
	    float max,
	    float step,
	    float value
    );

	void display();
	void updateDisplayValue();
	void hide();

	void setValue(float v);
	void inc(); 
	void dec(); 
	float getValue();

	boolean plusORminus(uint16_t x);

private:

    char _name[15];
    uint16_t _textcolor;
    uint8_t _textsize;
    float _min;
    float _max;
    float _step;
    float *_value;

};


/***************************************************************************/
// HEADER for the GUI button UI element
// based on Elegoo libraries

/***************************************************************************/

// EXTENDED by Ander Elosegi

/***************************************************************************/
class GUI_Button : public GUI_Element
{
public:
	GUI_Button(Elegoo_GFX *gfx,
		uint16_t x, uint16_t y, 
		uint16_t w, uint16_t h, 
		uint16_t outline, uint16_t fill, 
		uint16_t textcolor,
		char *label, uint8_t textsize,
		uint8_t roundness
    );

	void display();
	void hide(uint16_t backcolor);
	void hide();

	void toggle();
	void invert();
	void revert();

	boolean isInverted();



private:
	uint16_t _outlinecolor;
	uint16_t _fillcolor;
	uint16_t _textcolor;
	uint8_t _textsize;
	char _label[15];
	uint8_t _roundness;
	boolean *_inverted;
	// void (*_onClick)(void);
};



#endif //_TFTLCD_GUI_H