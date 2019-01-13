#include <TFTLCD_GUI.h>




/***************************************************************************/
// code for the Elegoo TFT GUI Element BASE CLASS

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

// Little hack for bypassing setOnClic "must set" function. If not set, each 
// element with no onClic function will break the program when touched on the
// screen.
//-----------------------------------------
void voidFunction(int16_t x){  return; }
//-----------------------------------------

GUI_Element::GUI_Element(
  Elegoo_GFX *gfx,
  uint16_t x,
  uint16_t y,
  uint16_t w,
  uint16_t h
  )
{
  _gfx = gfx;
  _x = x;
  _y = y;
  _w = w;
  _h = h;
  _enabled = (boolean *)malloc(sizeof(boolean));
  setOnClic(voidFunction);
}

boolean GUI_Element::contains(int16_t x, int16_t y) {
  if ((x < (_x)) || (x > (_x+_w))) return false;
  if ((y < (_y)) || (y > (_y+_h))) return false;
  return true;
}
void GUI_Element::setMenu(GUI_Menu *menu) { _menu = menu; }
void GUI_Element::enable() { *_enabled = true; }
void GUI_Element::disable() { *_enabled = false; }
boolean GUI_Element::isEnabled() { return *_enabled; }
void GUI_Element::setOnClic(const void (*function)(int16_t x)) { _onClick = function; }
void GUI_Element::onClic(int16_t x) { _onClick(x); }


/***************************************************************************/
// code for the Elegoo TFT GUI Menu

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

GUI_Menu::GUI_Menu(
  Elegoo_GFX *gfx, 
  GUI_Menu *parentMenu
  )
{
  _gfx = gfx;
  _parentMenu = parentMenu;
  head = 0;
}



void GUI_Menu::display()
{
  elements* elems = head;
  while(elems)
  {
    elems->elem->display();
    elems = elems->next;
  }
}

void GUI_Menu::hide()
{
  elements* elems = head;
  while(elems)
  {
    elems->elem->hide();
    elems = elems->next;
  }
}

GUI_Menu *GUI_Menu::getParent() { return _parentMenu; }

void GUI_Menu::addElement(GUI_Element *element)
{
  if(!head)
  {
    head = new elements(element);
  }
  else
  {
    elements* elems = head;
    while(elems->next) elems = elems->next;
    elems->next = new elements(element);
  }
  element->setMenu(this);
}

void GUI_Menu::loopThroughElements(int16_t x, int16_t y)
{
  elements* elems = head;
  while(elems)
  {
    if ( elems->elem->isEnabled() && elems->elem->contains(x, y) )
      elems->elem->onClic(x);
    elems=elems->next;
  }
}



/***************************************************************************/
// code for the Elegoo TFT GUI MenuItem

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/
GUI_MenuItem::GUI_MenuItem(
  Elegoo_GFX *gfx,
  char  *label,
  uint16_t x,
  uint16_t y,
  uint16_t w,
  uint16_t h)
  : GUI_Element(gfx, x, y, w, h) 
{
  strncpy(_label, label, strlen(label));
  disable();
}


void GUI_MenuItem::display()
{
  enable();

  uint16_t textColor = BLUE;
  uint16_t outlineColor = CYAN;
  uint8_t textsize = 2;

  // Draw outline
  _gfx->drawRoundRect(_x, _y, _w, _h, 5, outlineColor);

  // Print Label
  _gfx->setCursor( _x + _w/2- strlen(_label)*3*textsize, _y + _h/2-4*textsize);
  _gfx->setTextColor(textColor);
  _gfx->setTextSize(textsize);
  _gfx->print(_label);

}

void GUI_MenuItem::hide()
{
  disable();
  _gfx->fillRect(_x, _y, _w, _h, BLACK);
}

/***************************************************************************/
// code for the GFX value BAR UI element

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/


GUI_Value_Bar::GUI_Value_Bar(
  Elegoo_GFX *gfx,
  int16_t min, 
  int16_t max, 
  uint16_t x,
  uint16_t y,
  uint16_t w,
  uint16_t h,  
  uint16_t barcolor,
  uint16_t outlinecolor
) : GUI_Element(gfx, x, y, w, h) 
{
  _min = min,
  _max = max,
  _barcolor = barcolor;
  _outlinecolor = outlinecolor;
  disable();
  _value = (float *)malloc(sizeof(float));  
  *_value = (float)(_max+_min)/2;
}

int16_t GUI_Value_Bar::getX() {return _x;}
int16_t GUI_Value_Bar::getY() {return _y;}
uint8_t GUI_Value_Bar::getW() {return _w;}
uint16_t GUI_Value_Bar::getMin() {return _min;}
uint16_t GUI_Value_Bar::getMax() {return _max;}
float GUI_Value_Bar::getValue() { return *_value; }


void GUI_Value_Bar::display() {
  enable();
  displayFilledBar();
  _gfx->drawRoundRect(_x, _y, _w, _h, 0, _outlinecolor);
  displayValue();
}

void GUI_Value_Bar::hide() {
 disable();
 _gfx->fillRect(_x, _y, _w, _h, BLACK);
}

void GUI_Value_Bar::updateValue(float value)
{
  setValue(value);
  displayFilledBar();
  displayValue();
  _gfx->drawRoundRect(_x, _y, _w, _h, 0, _outlinecolor);
}

void GUI_Value_Bar::displayValue()
{
  uint8_t textsize = 2;
  _gfx->setCursor(_x + _w/2 - (String(*_value).length()*3*3), _y+_h/2-4* textsize);
  _gfx->setTextColor(WHITE); 
  //tft.fillRect(0,70,240,25,BLACK);
  _gfx->setTextSize(textsize);
  _gfx->println(*_value);
}

void GUI_Value_Bar::displayFilledBar()
{
  int x = map(*_value, _min,_max, 0, _w);
  _gfx->fillRoundRect(_x+1, _y+1, x-1, _h-2, 0, _barcolor);
  _gfx->fillRoundRect(_x+x, _y+1, _w-x-2, _h-2, 0, BLACK);
}

void GUI_Value_Bar::setValue(float value) 
{
    *_value = value;
}

/***************************************************************************/
// code for the GFX value BOX UI element

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

GUI_Value_Box::GUI_Value_Box(
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
  float value)
  : GUI_Element(gfx, x, y, w, h) 
{
  strncpy(_name, name, 15);
  _textcolor = textcolor;
  _textsize = textsize;
  _min = min;
  _max = max;
  _value = (float *)malloc(sizeof(float));
  if (value < min || value > max) *_value = 0.0;
  else                            *_value = value;
  _step = step;
  disable();
}

void GUI_Value_Box::display()
{
  // Print Label
  // first quart
  _gfx->setCursor(_x + 3*_textsize,_y + _h/2-4*_textsize);
  _gfx->setTextColor(_textcolor);
  _gfx->setTextSize(_textsize);
  _gfx->print(_name);

  // // Print Value
  // // second quart
  updateDisplayValue();

  // Display buttons
  // 3/4 and 4/4
  _gfx->setCursor(_x + 13*_w/16 - strlen("+")*3*_textsize,_y + _h/2-4*_textsize);
  _gfx->setTextColor(GREEN);
  _gfx->setTextSize(_textsize);
  _gfx->print("+");

  _gfx->setCursor(_x + 15*_w/16 - strlen("-")*3*_textsize,_y + _h/2-4*_textsize);
  _gfx->setTextColor(GREEN);
  _gfx->setTextSize(_textsize);
  _gfx->print("-");

  enable();
}

void GUI_Value_Box::updateDisplayValue()
{
  _gfx->fillRect(_x + _w/2, _y, _w/4, _h, BLACK);
  _gfx->setCursor(_x + 5*_w/8 - (String(*_value).length()*3*_textsize), _y + _h/2-4*_textsize);
  _gfx->setTextColor(BLUE);
  _gfx->setTextSize(_textsize);
  _gfx->print(*_value);
}

void GUI_Value_Box::hide()

{
  if(isEnabled())
  {
    disable();
    _gfx->fillRect(_x, _y, _w, _h, BLACK);
  }
}

void GUI_Value_Box::setValue(float v) { *_value = v; }

void GUI_Value_Box::inc() 
{ 
  if ( *_value + _step <= _max )
    *_value = *_value + _step; 
}

void GUI_Value_Box::dec() 
{ 
  if ( *_value - _step >= _min )
    *_value = *_value - _step; 
}

float GUI_Value_Box::getValue() { return *_value; }

boolean GUI_Value_Box::plusORminus(uint16_t x)
{
  if ((x > (_x + 3*_w/4)) && (x < (_x + 7*_w/8))) 
    inc();
  else
    dec();
  updateDisplayValue();
  delay(50);
}

/***************************************************************************/
// code for the GUI button UI element
// based on Elegoo libraries

/***************************************************************************/

// EXTENDED by Ander Elosegi

/***************************************************************************/
GUI_Button::GUI_Button(Elegoo_GFX *gfx,
            uint16_t x, uint16_t y, 
            uint16_t w, uint16_t h, 
            uint16_t outline, uint16_t fill, 
            uint16_t textcolor,
            char *label, uint8_t textsize,
            uint8_t roundness)
            : GUI_Element(gfx, x, y, w, h) 
{
  _outlinecolor = outline;
  _fillcolor = fill;
  _textcolor = textcolor;
  _textsize = textsize;
  strncpy(_label, label, 15);
  _label[15] = 0;
  _roundness = roundness;
  disable();
  _inverted = (boolean *)malloc(sizeof(boolean));
  *_inverted = false;
}

 

void GUI_Button::display() {
  uint16_t fill, outline, text;

  if (!*_inverted) {
   fill = _fillcolor;
   outline = _outlinecolor;
   text = _textcolor;
  } else {
   fill =  _textcolor;
   outline = _outlinecolor;
   text = _fillcolor;
  }

  enable();

  _gfx->fillRoundRect(_x, _y, _w, _h, _roundness, fill);
  _gfx->drawRoundRect(_x, _y, _w, _h, _roundness, outline);
  _gfx->setCursor(_x+_w/2- strlen(_label)*3*_textsize, _y+_h/2-4*_textsize);
  _gfx->setTextColor(text);
  _gfx->setTextSize(_textsize);
  _gfx->print(_label);

}

void GUI_Button::toggle()
{
    *_inverted = !*_inverted ; 
}

void GUI_Button::invert()
{
  *_inverted = true ; 
}

void GUI_Button::revert()
{
  *_inverted = false ; 
}

void GUI_Button::hide(uint16_t backcolor) {
  if(isEnabled())
  {
    disable();
    _gfx->fillRect(_x, _y, _w, _h, backcolor);
  }
}
void GUI_Button::hide() {
  if(isEnabled())
  {
    disable();
    _gfx->fillRect(_x, _y, _w, _h, BLACK);
  }
}

boolean GUI_Button::isInverted() { return *_inverted; }

/***************************************************************************/
// code for the GUI Label UI element

/***************************************************************************/

// by Ander Elosegi

/***************************************************************************/

GUI_Label::GUI_Label(
    Elegoo_GFX *gfx, 
    char* label, 
    uint16_t textcolor, 
    uint8_t textsize, 
    uint16_t x, 
    uint16_t y,
    uint16_t w, 
    uint16_t h) : 
    GUI_Element(gfx, x, y, w, h) 
{
  _textcolor = textcolor;
  _textsize = textsize;
  
  strncpy(_label, label, 15);
  _label[15] = 0;
}

void GUI_Label::display()
{
  _gfx->setCursor(_x+_w/2- strlen(_label)*3*_textsize, _y+_h/2-4*_textsize);
  _gfx->setTextColor(_textcolor);
  _gfx->setTextSize(_textsize);
  _gfx->print(_label);
}

void GUI_Label::hide()
{
  _gfx->fillRect(_x, _y, _w, _h, BLACK);
}

void GUI_Label::setLabel(char *label)
{
  strncpy(_label, label, 15);
  _label[15] = 0;
}
