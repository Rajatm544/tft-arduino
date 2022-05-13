#include <TFT.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 5
#define MAXPRESSURE 1000
int command=0;

const int XP = 6, XM = A1, YP = A2, YM = 7; //ID=0x9341
const int TS_LEFT=901,TS_RT=123,TS_TOP=119,TS_BOT=881;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button on_btn, off_btn, on_btn1, off_btn1;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); 
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410
#define ORANGE  0xFA60
#define LIME    0x07FF
#define AQUA    0x04FF
#define PINK    0xF8FF
#define WALL    0x4E72
#define MAROON  0xC845
#define GREE    0x5DC0

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);//left.up.size.size
    on_btn.initButton(&tft,  60, 180, 100, 80, GREE, BLACK, GREE, "ON", 2);
    off_btn.initButton(&tft, 180, 180, 100, 80, AQUA, BLACK, AQUA, "OFF", 1);
    on_btn1.initButton(&tft,  60, 270, 100, 80, WALL, BLACK, WALL, "ON", 2);
    off_btn1.initButton(&tft, 180, 270, 100, 80, MAROON, BLACK, MAROON, "OFF", 2);
    on_btn.drawButton(false);
    off_btn.drawButton(false);
    on_btn1.drawButton(false);
    off_btn1.drawButton(false);

}


Adafruit_GFX_Button *buttons[] = {&on_btn, &off_btn,&on_btn1, &off_btn1, NULL};


bool update_button(Adafruit_GFX_Button *b, bool down)
{
    b->press(down && b->contains(pixel_x, pixel_y));
    if (b->justReleased())
        b->drawButton(false);
    if (b->justPressed())
        b->drawButton(true);
    return down;
}


bool update_button_list(Adafruit_GFX_Button **pb)
{
    bool down = Touch_getXY();
    for (int i = 0 ; pb[i] != NULL; i++) {
        update_button(pb[i], down);
    }
    return down;
}

void loop(void)
{

command=Serial.read();

  
    update_button_list(buttons);  //use helper function
    if (on_btn.justPressed()or command== 'a') {
        
        Serial.println("on");
    }
    if (off_btn.justPressed()or command=='c') {
       
          Serial.println("on");
    }
    if (on_btn1.justPressed()or command=='d') {
      
         Serial.println("automode");
    }
    if (off_btn1.justPressed() or command=='e') {
      
              Serial.println("manual mode");
    }

    
}
