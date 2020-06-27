/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/mligh/OneDrive/Particle/ArgonClock/argonclock/src/argonclock.ino"
////////////////////////////
//// INCLUDED LIBRARIES ////
////////////////////////////

#include "neopixel.h"

/////////////////////////////////////
// NEOPIXEL MATRIX CHARACTERISTICS //
/////////////////////////////////////

void sound(int freq, int dur);
void setup();
void num(int num, int pix, int R, int G, int B);
void snum(int num, int pix, int R, int G, int B);
void letter(char ltr, int pix, int R, int G, int B);
void strdisp(const char *wrd, int pix, int R, int G, int B);
void strscrl(const char *wrd, int pix, int dly, int R, int G, int B);
void dimg(int cnum, int inpix);
void condition(int cnum, int inpix);
void canim(int cnum, int inpix, bool tmr);
void settings();
void loop();
void myHandler(const char *event, const char *data);
void myHandler2(const char *event, const char *data);
void mshHdlr(const char *event, const char *data);
void mshHdlr2(const char *event, const char *data);
void co2Handler(const char *event, const char *data);
void itHandler(const char *event, const char *data);
#line 11 "c:/Users/mligh/OneDrive/Particle/ArgonClock/argonclock/src/argonclock.ino"
#define PIXEL_COUNT 256         //Number of pixels in matrix

#define PIXEL_PIN A3            //Define Hardware pin used for data

#define PIXEL_TYPE WS2812       //Define LED Type

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);    //Initialize neopixel function


////////////////////////////////////
/////// NIGHT MODE CONFIG //////////
////////////////////////////////////

#define dnbound 60              //Photoresistor value to initiate Night Mode

#define upbound 250             //Photoresistor value to exit Night Mode once entered, must be greater than dnbound

//  *Special note: values can be experimentally found through the particle console or app


////////////////////////////////////
//// HARDWARE CONFIGURATION ////////
////////////////////////////////////

SYSTEM_MODE(AUTOMATIC);         //Tells device to use WiFi by default

#define upbtn D0                //Defines button configuration for navigation buttons

#define enbtn D1

#define dnbtn D2

#define spkpn D3                //Define Speaker pin (optional)

#define brtsns A4               //Define Photoresistor pin (use 10k pulldown resistor)

////////////////////////////////////
//////// EEPROM ADDRESSES //////////
////////////////////////////////////
// EEPROM 1: Time zone offset    ///
// EEPROM 2: Weather Data toggle ///
// EEPROM 3: CO2 Data toggle     ///
// EEPROM 4: Indoor temp toggle  ///
// EEPROM 5: Dark Mode Color     ///
// EEPROM 6: Mini Clock          ///
////////////////////////////////////


////////////////////////////////////
////// GLOBAL VARIABLE LIST ////////
////////////////////////////////////

int mprev;      //Previous Minute
int hprev;      //Previous Hour
int bclock;     //Blue Reference
int rclock;     //Red Reference
int gclock;     //Green Reference
int photo;      //variable for most recent photoresistor value
int bound;      //Boundary variable for night/day mode
int photoupd;
int scan;
int tmr;
int TFahr;
int TC1;
int TC2;
int HC1;
int HC2;
int wmode;
int dmode;
int cid;
int cprev;
char cltr[1];
int i;
int j;
bool indark;
int ihumid;
int itemp;
int ico2;
uint8_t eevalue;
int fdark;
void fillStrip(int start, int end, int R, int G, int B);
//uint8_t plugIP[4] = {192,168,1,36};

void sound(int freq, int dur){
    tone(D3, freq, dur);
    delay(dur);
}
void setup() {
    Mesh.selectAntenna(MeshAntennaType::EXTERNAL);
    Particle.subscribe("hook-response/Weather", myHandler, MY_DEVICES);             //Subscribes to Weather API event
    Particle.subscribe("hook-response/co2dat", co2Handler, MY_DEVICES);             //Subscribes to CO2 data api
    Particle.subscribe("hook-response/itempdat", itHandler, MY_DEVICES);            //Subscribes to inside temp data api
    Particle.subscribe("hook-response/getState", mshHdlr2, MY_DEVICES);
    Mesh.subscribe("toggleBtn", mshHdlr);
    Particle.subscribe("pclock999", myHandler2);                                    //Subscribes to Google Home IFTTT integration
    Particle.publish("Weather", "1", PRIVATE);                                      //Initial Publishes to get weather data
    if(EEPROM.read(3) == 1)
    {
        Particle.publish("co2dat", "1", PRIVATE);
    }
    if(EEPROM.read(4) == 1)
    {
        Particle.publish("itempdat", "1", PRIVATE);
    }
    pinMode(D0, INPUT_PULLDOWN);                                                    //Pin Mode setter, these inputs are buttons
    pinMode(D1, INPUT_PULLDOWN);
    pinMode(D2, INPUT_PULLDOWN);
    RGB.control(true);
    RGB.color(0, 0, 0);
    scan = 1;                                                                       //scans photoresistor for room brightness
    photoupd = 0;
    wmode = 1;
    dmode = 1;
    fdark = 2;
    tmr = 55;
    strip.begin();
    strip.show();
    //EEPROM.write(1,18);
    //strscrl(WiFi.SSID(),0,100,50,50,50);
    //strdisp("F00L", 0, 50,50,50);
    //strscrl(WiFi.localIP(),0,100,50,50,50);
    //while(digitalRead(D0) == LOW){
    //    delay(5);
    //}
    //lettest(1000,50,50,50);
    //Time.beginDST();
    strip.clear();
    Time.zone(12-EEPROM.read(1));
    mprev=(Time.minute()-1);
    hprev=(Time.hourFormat12()-1);
    bound = dnbound;
    delay(50);
}
void num(int num, int pix, int R, int G, int B) {       //Code block for displaying larger 4x7 numbers, pix arg is the top left pixel, num is the number
if(num == 1)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        
        strip.setPixelColor(pix,0,0,0);
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+16,0,0,0);
        strip.setPixelColor(pix-1,0,0,0);
        strip.setPixelColor(pix-2,0,0,0);
        strip.setPixelColor(pix-3,0,0,0);
        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
        strip.setPixelColor(pix-6,0,0,0);
        strip.setPixelColor(pix+7,0,0,0);
        strip.setPixelColor(pix+10,0,0,0);
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+13,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        
        strip.setPixelColor(pix,0,0,0);
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+2,0,0,0);
        strip.setPixelColor(pix+3,0,0,0);
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
        strip.setPixelColor(pix+6,0,0,0);
        strip.setPixelColor(pix+9,0,0,0);
        strip.setPixelColor(pix+22,0,0,0);
        strip.setPixelColor(pix+15,0,0,0);
        strip.setPixelColor(pix+16,0,0,0);
        strip.setPixelColor(pix+12,0,0,0);
        strip.setPixelColor(pix+19,0,0,0);
    }
}
if(num == 2)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-4,R,G,B);
        strip.setPixelColor(pix-5,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        
        strip.setPixelColor(pix-1,0,0,0);
        strip.setPixelColor(pix-2,0,0,0);
        strip.setPixelColor(pix+22,0,0,0);
        strip.setPixelColor(pix+21,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+5,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+2,0,0,0);
        strip.setPixelColor(pix+26,0,0,0);
        strip.setPixelColor(pix+27,0,0,0);
    }
}
if(num == 3)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        
        strip.setPixelColor(pix-1,0,0,0);
        strip.setPixelColor(pix-2,0,0,0);
        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+2,0,0,0);
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
    }
}
if(num == 4)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+16,0,0,0);
        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
        strip.setPixelColor(pix-6,0,0,0);
        strip.setPixelColor(pix+7,0,0,0);
        strip.setPixelColor(pix+10,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
        strip.setPixelColor(pix+6,0,0,0);
        strip.setPixelColor(pix+9,0,0,0);
        strip.setPixelColor(pix+22,0,0,0);
        strip.setPixelColor(pix+15,0,0,0);
        strip.setPixelColor(pix+16,0,0,0);
        
        
    }
}
if(num == 5)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);

        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
        strip.setPixelColor(pix+19,0,0,0);
        strip.setPixelColor(pix+18,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
        strip.setPixelColor(pix+29,0,0,0);
        strip.setPixelColor(pix+30,0,0,0);
    }
}
if(num == 6)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-4,R,G,B);
        strip.setPixelColor(pix-5,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        
        strip.setPixelColor(pix+19,0,0,0);
        strip.setPixelColor(pix+18,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+5,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        
        strip.setPixelColor(pix+29,0,0,0);
        strip.setPixelColor(pix+30,0,0,0);
    }
}
if(num == 7)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        
        strip.setPixelColor(pix-1,0,0,0);
        strip.setPixelColor(pix-2,0,0,0);
        strip.setPixelColor(pix-3,0,0,0);
        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
        strip.setPixelColor(pix-6,0,0,0);
        strip.setPixelColor(pix+7,0,0,0);
        strip.setPixelColor(pix+10,0,0,0);
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+13,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        
        strip.setPixelColor(pix+1,0,0,0);
        strip.setPixelColor(pix+2,0,0,0);
        strip.setPixelColor(pix+3,0,0,0);
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
        strip.setPixelColor(pix+6,0,0,0);
        strip.setPixelColor(pix+9,0,0,0);
        strip.setPixelColor(pix+22,0,0,0);
        strip.setPixelColor(pix+12,0,0,0);
        strip.setPixelColor(pix+19,0,0,0);
    }
}
if(num == 8)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-4,R,G,B);
        strip.setPixelColor(pix-5,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+5,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
    }
}
if(num == 9)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        
        strip.setPixelColor(pix-4,0,0,0);
        strip.setPixelColor(pix-5,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+5,0,0,0);
    }
}
if(num == 0)
{
    if((pix/8)%2 == 1)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix-1,R,G,B);
        strip.setPixelColor(pix-2,R,G,B);
        strip.setPixelColor(pix-3,R,G,B);
        strip.setPixelColor(pix-4,R,G,B);
        strip.setPixelColor(pix-5,R,G,B);
        strip.setPixelColor(pix-6,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+7,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+10,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+21,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+23,R,G,B);
        
        strip.setPixelColor(pix+4,0,0,0);
        strip.setPixelColor(pix+13,0,0,0);
    }
    if((pix/8)%2 == 0)
    {
        strip.setPixelColor(pix,R,G,B);
        strip.setPixelColor(pix+1,R,G,B);
        strip.setPixelColor(pix+2,R,G,B);
        strip.setPixelColor(pix+3,R,G,B);
        strip.setPixelColor(pix+4,R,G,B);
        strip.setPixelColor(pix+5,R,G,B);
        strip.setPixelColor(pix+6,R,G,B);
        strip.setPixelColor(pix+9,R,G,B);
        strip.setPixelColor(pix+22,R,G,B);
        strip.setPixelColor(pix+25,R,G,B);
        strip.setPixelColor(pix+26,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        
        strip.setPixelColor(pix+12,0,0,0);
        strip.setPixelColor(pix+19,0,0,0);
    }
}
}                           
void snum(int num, int pix, int R, int G, int B){           //Code block for displaying smaller 3x5 numbers, pix arg is the top left pixel, num is the number
    if(num == 1)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,0,0,0);
            strip.setPixelColor(pix-1,0,0,0);
            strip.setPixelColor(pix-2,0,0,0);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,0,0,0);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,0,0,0);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,0,0,0);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+11,0,0,0);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,0,0,0);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,0,0,0);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 2)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,0,0,0);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,R,G,B);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,0,0,0);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,0,0,0);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 3)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,0,0,0);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 4)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,0,0,0);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,0,0,0);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+11,0,0,0);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,0,0,0);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 5)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,0,0,0);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,0,0,0);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 6)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,R,G,B);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,0,0,0);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,0,0,0);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 7)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,0,0,0);
            strip.setPixelColor(pix-2,0,0,0);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,0,0,0);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,0,0,0);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,0,0,0);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+11,0,0,0);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,0,0,0);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 8)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,R,G,B);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 9)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,0,0,0);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
    if(num == 0)
    {
        if((pix/8)%2 == 1)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix-1,R,G,B);
            strip.setPixelColor(pix-2,R,G,B);
            strip.setPixelColor(pix-3,R,G,B);
            strip.setPixelColor(pix-4,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,0,0,0);
            strip.setPixelColor(pix+3,0,0,0);
            strip.setPixelColor(pix+4,0,0,0);
            strip.setPixelColor(pix+5,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+14,R,G,B);
            strip.setPixelColor(pix+13,R,G,B);
            strip.setPixelColor(pix+12,R,G,B);
        }
        if((pix/8)%2 == 0)
        {
            strip.setPixelColor(pix,R,G,B);
            strip.setPixelColor(pix+1,R,G,B);
            strip.setPixelColor(pix+2,R,G,B);
            strip.setPixelColor(pix+3,R,G,B);
            strip.setPixelColor(pix+4,R,G,B);
            strip.setPixelColor(pix+11,R,G,B);
            strip.setPixelColor(pix+12,0,0,0);
            strip.setPixelColor(pix+13,0,0,0);
            strip.setPixelColor(pix+14,0,0,0);
            strip.setPixelColor(pix+15,R,G,B);
            strip.setPixelColor(pix+16,R,G,B);
            strip.setPixelColor(pix+17,R,G,B);
            strip.setPixelColor(pix+18,R,G,B);
            strip.setPixelColor(pix+19,R,G,B);
            strip.setPixelColor(pix+20,R,G,B);
        }
    }
}                           
void letter(char ltr, int pix, int R, int G, int B){            //Code for displaying a single character, ltr is the letter (''), pix is the top left pixel in a standard letter which is 5x5 with a space
    if(ltr == 'a')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'b')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
    }
    if(ltr == 'c')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'd')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'e')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'f')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'g')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'h')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
    }
    if(ltr == 'i')
    {
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+11,R,G,B);
    }
    if(ltr == 'j')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'k')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'l')
    {
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+11,R,G,B);
    }
    if(ltr == 'm')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+34,R,G,B);
        strip.setPixelColor(pix+44,R,G,B);
        strip.setPixelColor(pix+43,R,G,B);
    }
    if(ltr == 'n')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'o')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'p')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'q')
    {
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'r')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 's')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 't')
    {
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'u')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'v')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'w')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+36,R,G,B);
        strip.setPixelColor(pix+44,R,G,B);
        strip.setPixelColor(pix+45,R,G,B);
    }
    if(ltr == 'x')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
    }
    if(ltr == 'y')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'z')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'A')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'B')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'C')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'D')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'E')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'F')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'G')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'H')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'I')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'J')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'K')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'L')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
    }
    if(ltr == 'M')
    {
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+33,R,G,B);
        strip.setPixelColor(pix+47,R,G,B);
        strip.setPixelColor(pix+46,R,G,B);
        strip.setPixelColor(pix+45,R,G,B);
        strip.setPixelColor(pix+44,R,G,B);
        strip.setPixelColor(pix+43,R,G,B);
    }
    if(ltr == 'N')
    {
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+35,R,G,B);
        strip.setPixelColor(pix+43,R,G,B);
        strip.setPixelColor(pix+44,R,G,B);
        strip.setPixelColor(pix+45,R,G,B);
        strip.setPixelColor(pix+46,R,G,B);
        strip.setPixelColor(pix+47,R,G,B);
    }
    if(ltr == 'O')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'P')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'Q')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'R')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
    }
    if(ltr == 'S')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'T')
    {
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'U')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'V')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'W')
    {
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+36,R,G,B);
        strip.setPixelColor(pix+44,R,G,B);
        strip.setPixelColor(pix+45,R,G,B);
        strip.setPixelColor(pix+46,R,G,B);
        strip.setPixelColor(pix+47,R,G,B);
    }
    if(ltr == 'X')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+28,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'Y')
    {
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == 'Z')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
        strip.setPixelColor(pix+16,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+20,R,G,B);
        strip.setPixelColor(pix+27,R,G,B);
        strip.setPixelColor(pix+30,R,G,B);
        strip.setPixelColor(pix+31,R,G,B);
    }
    if(ltr == '0')
    {
        snum(0,pix+15,R,G,B);
    }
    if(ltr == '1')
    {
        snum(1,pix+15,R,G,B);
    }
    if(ltr == '2')
    {
        snum(2,pix+15,R,G,B);
    }
    if(ltr == '3')
    {
        snum(3,pix+15,R,G,B);
    }
    if(ltr == '4')
    {
        snum(4,pix+15,R,G,B);
    }
    if(ltr == '5')
    {
        snum(5,pix+15,R,G,B);
    }
    if(ltr == '6')
    {
        snum(6,pix+15,R,G,B);
    }
    if(ltr == '7')
    {
        snum(7,pix+15,R,G,B);
    }
    if(ltr == '8')
    {
        snum(8,pix+15,R,G,B);
    }
    if(ltr == '9')
    {
        snum(9,pix+15,R,G,B);
    }
    if(ltr == '!')
    {
        strip.setPixelColor(pix+11,R,G,B);
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
        strip.setPixelColor(pix+15,R,G,B);
    }
    if(ltr == '.')
    {
        strip.setPixelColor(pix+11,R,G,B);
    }
    if(ltr == ':')
    {
        strip.setPixelColor(pix+12,R,G,B);
        strip.setPixelColor(pix+14,R,G,B);
    }
    if(ltr == '+')
    {
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+17,R,G,B);
        strip.setPixelColor(pix+19,R,G,B);
    }
    if(ltr == '-')
    {
        strip.setPixelColor(pix+13,R,G,B);
        strip.setPixelColor(pix+29,R,G,B);
        strip.setPixelColor(pix+18,R,G,B);
    }
    /*if(ltr == '')
    {
        strip.setPixelColor(pix,);
        strip.setPixelColor(pix+1,);
        strip.setPixelColor(pix+2,);
        strip.setPixelColor(pix+3,);
        strip.setPixelColor(pix+4,);
        strip.setPixelColor(pix+11,);
        strip.setPixelColor(pix+12,);
        strip.setPixelColor(pix+13,);
        strip.setPixelColor(pix+14,);
        strip.setPixelColor(pix+15,);
        strip.setPixelColor(pix+16,);
        strip.setPixelColor(pix+17,);
        strip.setPixelColor(pix+18,);
        strip.setPixelColor(pix+19,);
        strip.setPixelColor(pix+20,);
        strip.setPixelColor(pix+27,);
        strip.setPixelColor(pix+28,);
        strip.setPixelColor(pix+29,);
        strip.setPixelColor(pix+30,);
        strip.setPixelColor(pix+31,);
        strip.setPixelColor(pix+32,);
        strip.setPixelColor(pix+33,);
        strip.setPixelColor(pix+34,);
        strip.setPixelColor(pix+35,);
        strip.setPixelColor(pix+36,);
    }*/
}                        
void strdisp(const char *wrd, int pix, int R, int G, int B){            //Code block for displaying a small string of characters ("") that fits on the screen
    int pnum;
    pnum=pix;
    for(i=0;i<strlen(wrd);i++)
    {
        if(wrd[i] == 'i' || wrd[i] == 'l' || wrd[i] == '.' || wrd[i] == '!')
        {
            letter(wrd[i],pnum,R,G,B);
            pnum=pnum+16;
        }
        else if(wrd[i] == 'm' || wrd[i] == 'w' || wrd[i] == 'W' || wrd[i] == 'M' || wrd[i] == 'N')
        {
            letter(wrd[i],pnum,R,G,B);
            pnum=pnum+48;
        }
        else if(wrd[i] == ' ')
        {
            letter('l',pnum,0,0,0);
            pnum=pnum+16;
        }
        else
        {
            letter(wrd[i],pnum,R,G,B);
            pnum=pnum+32;
        }
    }
    strip.show();
}                
void strscrl(const char *wrd, int pix, int dly, int R, int G, int B){           //Code block for displaying a large string of characters ("") that does not fit on the screen, scrolls to show all of the characters, dly is the delay of the scroll
    strdisp(wrd,0,50,50,50);
    delay(1000);
    int lnum;
    lnum=pix;
    for(i=0;i<strlen(wrd);i++)
    {
        if(wrd[i] == 'i' || wrd[i] == 'l' || wrd[i] == '.' || wrd[i] == '!' || wrd[i] == ':')
        {
            lnum=lnum+16;
        }
        else if(wrd[i] == 'm' || wrd[i] == 'w' || wrd[i] == 'W' || wrd[i] == 'M' || wrd[i] == 'N')
        {
            lnum=lnum+48;
        }
        else if(wrd[i] == ' ')
        {
            lnum=lnum+16;
        }
        else
        {
            lnum=lnum+32;
        }
    }
    for(j=0;j>248-lnum;j=j-16)
    {
        strip.clear();
        strdisp(wrd,j,50,50,50);
        delay(dly);
    }
    /*for(i=0;i<strlen(wrd2);i++)
    {
        const char *wordy;
        for(j=i;j<strlen(wrd2);j++)
        {
            wordy[j-i] = wrd2[j];
        }
        strdisp(wordy,0,50,50,50);
        strip.show();
        delay(1000);
    }*/
    
}       
void dimg(int cnum, int inpix){                     //Code block for displaying a still weather animation when in dark mode
    if(cnum > 299 && cnum < 322)
    {
        strip.setPixelColor(inpix+10,rclock,gclock,bclock);
        strip.setPixelColor(inpix+25,rclock,gclock,bclock);
        strip.setPixelColor(inpix+43,rclock,gclock,bclock);
    }
    else if(cnum > 199 && cnum < 235)
    {
        strip.setPixelColor(inpix+28,rclock,gclock,bclock);
        strip.setPixelColor(inpix+20,rclock,gclock,bclock);
        strip.setPixelColor(inpix+26,rclock,gclock,bclock);
        strip.setPixelColor(inpix+22,rclock,gclock,bclock);
    }
    else if(cnum > 499 && cnum < 533)
    {
        strip.setPixelColor(inpix+12,rclock,gclock,bclock);
        strip.setPixelColor(inpix+22,rclock,gclock,bclock);
        strip.setPixelColor(inpix+27,rclock,gclock,bclock);
        strip.setPixelColor(inpix+37,rclock,gclock,bclock);
        strip.setPixelColor(inpix+44,rclock,gclock,bclock);
    }
    else if(cnum > 599 && cnum < 630)                                    //Snow
    {
        strip.setPixelColor(inpix+12,rclock,gclock,bclock);
        strip.setPixelColor(inpix+22,rclock,gclock,bclock);
        strip.setPixelColor(inpix+27,rclock,gclock,bclock);
        strip.setPixelColor(inpix+37,rclock,gclock,bclock);
        strip.setPixelColor(inpix+44,rclock,gclock,bclock);    
    }
}                                             
void condition(int cnum, int inpix){                    //Code block for displaying the current weather condition (eg. sunny, rainy, cloudy...)
    if(cltr[0] == 'd')
    {
        if(cnum == 800 || cnum == 801 || cnum == 721)                       //Clear or Haze
        {
            strip.setPixelColor(inpix+3,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+18,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+19,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+20,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+31,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+29,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+28,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+27,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+25,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+34,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+35,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+36,rclock/1.8,gclock*1.3,0);
            strip.setPixelColor(inpix+51,rclock/1.8,gclock*1.3,0);
        }
        if(cnum == 802 || cnum == 803)                                      //Partly cloudy
        {
            strip.setPixelColor(inpix+3,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+18,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+19,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+31,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+29,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+34,rclock,gclock*1.9,0);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+26,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+28,rclock,gclock,bclock);
            strip.setPixelColor(inpix+35,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+37,rclock,gclock,bclock);
            strip.setPixelColor(inpix+42,rclock,gclock,bclock);
            strip.setPixelColor(inpix+43,rclock,gclock,bclock);
            strip.setPixelColor(inpix+44,rclock,gclock,bclock);
            strip.setPixelColor(inpix+52,rclock,gclock,bclock);
            strip.setPixelColor(inpix+51,rclock,gclock,bclock);
            strip.setPixelColor(inpix+53,rclock,gclock,bclock);
            strip.setPixelColor(inpix+59,rclock,gclock,bclock);
        }
        if(cnum > 599 && cnum < 630)                                    //Snow
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
            
            /*strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+3,rclock,gclock,bclock);
            strip.setPixelColor(inpix+12,rclock,gclock,bclock);
            strip.setPixelColor(inpix+19,rclock,gclock,bclock);
            strip.setPixelColor(inpix+35,rclock,gclock,bclock);
            strip.setPixelColor(inpix+44,rclock,gclock,bclock);
            strip.setPixelColor(inpix+51,rclock,gclock,bclock);
            strip.setPixelColor(inpix+10,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+28,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+26,rclock,gclock,bclock);
            strip.setPixelColor(inpix+25,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+42,rclock,gclock,bclock);*/
        }
        if(cnum == 804 || (cnum > 199 && cnum < 233) || (cnum > 299 && cnum < 532))                       //Cloudy or Thunderstorm (base)
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
        }
        if(cnum == 701 || cnum == 761)                                      //Mist
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
            strip.setPixelColor(inpix+11,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+20,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+27,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+36,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+43,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+42,rclock/3,gclock/3,bclock/3);
            strip.setPixelColor(inpix+25,rclock/3,gclock/3,bclock/3);
            strip.setPixelColor(inpix+38,rclock/3,gclock/3,bclock/3);
            
        }
        if(cnum == 711 || cnum == 731 || cnum == 761 || cnum == 751 || cnum == 762)         //Fog
        {
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+11,rclock,gclock,bclock);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+22,rclock,gclock,bclock);
            strip.setPixelColor(inpix+25,rclock,gclock,bclock);
            strip.setPixelColor(inpix+38,rclock,gclock,bclock);
            strip.setPixelColor(inpix+41,rclock,gclock,bclock);
        }
        if(cnum == 781)
        {
            strip.setPixelColor(inpix+25,255,127,0);
            strip.setPixelColor(inpix+27,255,127,0);
            strip.setPixelColor(inpix+28,255,127,0);
            strip.setPixelColor(inpix+29,255,127,0);
            strip.setPixelColor(inpix+30,255,127,0);
            strip.setPixelColor(inpix+31,255,127,0);
        }
        if(cnum > 599 || cnum < 623)
        {
            
        }
    }
    else
    {
        if(cnum == 800 || cnum == 801 || cnum == 721)
        {
            strip.setPixelColor(inpix+16,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+31,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+32,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+30,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+33,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+46,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+34,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+45,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+35,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+44,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+36,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+43,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+26,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+37,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+42,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+22,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+25,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+38,rclock,gclock*1.2,bclock/3);
        }
        if(cnum > 599 && cnum < 630)                                    //Snow
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
            
            /*strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+3,rclock,gclock,bclock);
            strip.setPixelColor(inpix+12,rclock,gclock,bclock);
            strip.setPixelColor(inpix+19,rclock,gclock,bclock);
            strip.setPixelColor(inpix+35,rclock,gclock,bclock);
            strip.setPixelColor(inpix+44,rclock,gclock,bclock);
            strip.setPixelColor(inpix+51,rclock,gclock,bclock);
            strip.setPixelColor(inpix+10,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+28,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+26,rclock,gclock,bclock);
            strip.setPixelColor(inpix+25,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+42,rclock,gclock,bclock);*/
        }
        if(cnum == 802 || cnum == 803)
        {
            strip.setPixelColor(inpix+17,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+21,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+30,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+29,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+34,rclock,gclock*1.2,bclock/3);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+26,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+28,rclock,gclock,bclock);
            strip.setPixelColor(inpix+35,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+37,rclock,gclock,bclock);
            strip.setPixelColor(inpix+42,rclock,gclock,bclock);
            strip.setPixelColor(inpix+43,rclock,gclock,bclock);
            strip.setPixelColor(inpix+44,rclock,gclock,bclock);
            strip.setPixelColor(inpix+52,rclock,gclock,bclock);
            strip.setPixelColor(inpix+51,rclock,gclock,bclock);
            strip.setPixelColor(inpix+53,rclock,gclock,bclock);
            strip.setPixelColor(inpix+59,rclock,gclock,bclock);
        }
        if(cnum == 804 || (cnum > 199 && cnum < 233) || (cnum > 299 && cnum < 532))
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
        }
        if(cnum == 701 || cnum == 741)
        {
            strip.setPixelColor(inpix+1,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+14,rclock,gclock,bclock);
            strip.setPixelColor(inpix+13,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+17,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+30,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+33,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+46,rclock,gclock,bclock);
            strip.setPixelColor(inpix+45,rclock,gclock,bclock);
            strip.setPixelColor(inpix+49,rclock,gclock,bclock);
            strip.setPixelColor(inpix+11,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+20,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+27,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+36,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+43,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(inpix+25,rclock/3,gclock/3,bclock/3);
            strip.setPixelColor(inpix+38,rclock/3,gclock/3,bclock/3);
        }
        if(cnum == 711 || cnum == 731 || cnum == 761 || cnum == 751 || cnum == 762)
        {
            strip.setPixelColor(inpix+15,rclock,gclock,bclock);
            strip.setPixelColor(inpix+16,rclock,gclock,bclock);
            strip.setPixelColor(inpix+31,rclock,gclock,bclock);
            strip.setPixelColor(inpix+32,rclock,gclock,bclock);
            strip.setPixelColor(inpix+47,rclock,gclock,bclock);
            strip.setPixelColor(inpix+18,rclock,gclock,bclock);
            strip.setPixelColor(inpix+29,rclock,gclock,bclock);
            strip.setPixelColor(inpix+34,rclock,gclock,bclock);
            strip.setPixelColor(inpix+11,rclock,gclock,bclock);
            strip.setPixelColor(inpix+20,rclock,gclock,bclock);
            strip.setPixelColor(inpix+27,rclock,gclock,bclock);
            strip.setPixelColor(inpix+36,rclock,gclock,bclock);
            strip.setPixelColor(inpix+22,rclock,gclock,bclock);
            strip.setPixelColor(inpix+25,rclock,gclock,bclock);
            strip.setPixelColor(inpix+38,rclock,gclock,bclock);
            strip.setPixelColor(inpix+41,rclock,gclock,bclock);
        }
        if(cnum == 781)
        {
            strip.setPixelColor(inpix+25,255,127,0);
            strip.setPixelColor(inpix+27,255,127,0);
            strip.setPixelColor(inpix+28,255,127,0);
            strip.setPixelColor(inpix+29,255,127,0);
            strip.setPixelColor(inpix+30,255,127,0);
            strip.setPixelColor(inpix+31,255,127,0);
        }
        if(cnum > 599 || cnum < 623)
        {
            
        }
    }
}                                        
void canim(int cnum, int inpix, bool tmr){                      //Code block for displaying one animation of a weather condition such as rain
    if(cnum > 199 && cnum < 235)
    {
        strip.setPixelColor(inpix+28,0,0,0);
        strip.setPixelColor(inpix+26,0,0,0);
        strip.setPixelColor(inpix+22,0,0,0);
        strip.setPixelColor(inpix+20,0,0,0);
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
            strip.show();
            for(i=0; i<100; i++)
            {
                if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
                {
                    delay(5);
                }
            }
        }
        strip.setPixelColor(inpix+28,rclock/1.2,gclock*2,0);
        strip.show();
        delay(10);
        strip.setPixelColor(inpix+20,rclock/1.2,gclock*2,0);
        strip.show();
        delay(10);
        strip.setPixelColor(inpix+26,rclock/1.2,gclock*2,0);
        strip.show();
        delay(10);
        strip.setPixelColor(inpix+22,rclock/1.2,gclock*2,0);
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
            strip.show();
            for(i=0; i<100; i++)
            {
                if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
                {
                    delay(5);
                }
            }
        }
    }
    else if(cnum > 299 && cnum < 322)
    {
        
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
        }
        strip.setPixelColor(inpix+12,0,gclock,bclock);
        //strip.setPixelColor(inpix+22,0,gclock/3,bclock);
        strip.setPixelColor(inpix+27,0,gclock,bclock);
        //strip.setPixelColor(inpix+37,0,gclock/3,bclock);
        strip.setPixelColor(inpix+44,0,gclock,bclock);
        strip.setPixelColor(inpix+9,0,0,0);
        //strip.setPixelColor(inpix+21,0,0,0);
        strip.setPixelColor(inpix+28,0,0,0);
        //strip.setPixelColor(inpix+36,0,0,0);
        strip.setPixelColor(inpix+41,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+11,0,gclock,bclock);
        //strip.setPixelColor(inpix+19,0,gclock/3,bclock);
        strip.setPixelColor(inpix+26,0,gclock,bclock);
        //strip.setPixelColor(inpix+38,0,gclock/3,bclock);
        strip.setPixelColor(inpix+43,0,gclock,bclock);
        strip.setPixelColor(inpix+12,0,0,0);
        //strip.setPixelColor(inpix+22,0,0,0);
        strip.setPixelColor(inpix+27,0,0,0);
        //strip.setPixelColor(inpix+37,0,0,0);
        strip.setPixelColor(inpix+44,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
        }
        strip.setPixelColor(inpix+10,0,gclock,bclock);
        //strip.setPixelColor(inpix+20,0,gclock/3,bclock);
        strip.setPixelColor(inpix+25,0,gclock,bclock);
        //strip.setPixelColor(inpix+35,0,gclock/3,bclock);
        strip.setPixelColor(inpix+42,0,gclock,bclock);
        strip.setPixelColor(inpix+11,0,0,0);
        //strip.setPixelColor(inpix+19,0,0,0);
        strip.setPixelColor(inpix+26,0,0,0);
        //strip.setPixelColor(inpix+38,0,0,0);
        strip.setPixelColor(inpix+43,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+9,0,gclock,bclock);
        //strip.setPixelColor(inpix+21,0,gclock/3,bclock);
        strip.setPixelColor(inpix+28,0,gclock,bclock);
        //strip.setPixelColor(inpix+36,0,gclock/3,bclock);
        strip.setPixelColor(inpix+41,0,gclock,bclock);
        strip.setPixelColor(inpix+10,0,0,0);
        //strip.setPixelColor(inpix+20,0,0,0);
        strip.setPixelColor(inpix+25,0,0,0);
        //strip.setPixelColor(inpix+35,0,0,0);
        strip.setPixelColor(inpix+42,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
    }
    else if(cnum > 499 && cnum < 533)
    {
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
        }
        strip.setPixelColor(inpix+12,0,gclock/3,bclock);
        strip.setPixelColor(inpix+22,0,gclock/3,bclock);
        strip.setPixelColor(inpix+27,0,gclock/3,bclock);
        strip.setPixelColor(inpix+37,0,gclock/3,bclock);
        strip.setPixelColor(inpix+44,0,gclock/3,bclock);
        strip.setPixelColor(inpix+9,0,0,0);
        strip.setPixelColor(inpix+21,0,0,0);
        strip.setPixelColor(inpix+28,0,0,0);
        strip.setPixelColor(inpix+36,0,0,0);
        strip.setPixelColor(inpix+41,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+11,0,gclock/3,bclock);
        strip.setPixelColor(inpix+19,0,gclock/3,bclock);
        strip.setPixelColor(inpix+26,0,gclock/3,bclock);
        strip.setPixelColor(inpix+38,0,gclock/3,bclock);
        strip.setPixelColor(inpix+43,0,gclock/3,bclock);
        strip.setPixelColor(inpix+12,0,0,0);
        strip.setPixelColor(inpix+22,0,0,0);
        strip.setPixelColor(inpix+27,0,0,0);
        strip.setPixelColor(inpix+37,0,0,0);
        strip.setPixelColor(inpix+44,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
        }
        strip.setPixelColor(inpix+10,0,gclock/3,bclock);
        strip.setPixelColor(inpix+20,0,gclock/3,bclock);
        strip.setPixelColor(inpix+25,0,gclock/3,bclock);
        strip.setPixelColor(inpix+35,0,gclock/3,bclock);
        strip.setPixelColor(inpix+42,0,gclock/3,bclock);
        strip.setPixelColor(inpix+11,0,0,0);
        strip.setPixelColor(inpix+19,0,0,0);
        strip.setPixelColor(inpix+26,0,0,0);
        strip.setPixelColor(inpix+38,0,0,0);
        strip.setPixelColor(inpix+43,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+9,0,gclock/3,bclock);
        strip.setPixelColor(inpix+21,0,gclock/3,bclock);
        strip.setPixelColor(inpix+28,0,gclock/3,bclock);
        strip.setPixelColor(inpix+36,0,gclock/3,bclock);
        strip.setPixelColor(inpix+41,0,gclock/3,bclock);
        strip.setPixelColor(inpix+10,0,0,0);
        strip.setPixelColor(inpix+20,0,0,0);
        strip.setPixelColor(inpix+25,0,0,0);
        strip.setPixelColor(inpix+35,0,0,0);
        strip.setPixelColor(inpix+42,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
    }
    else if(cnum > 599 && cnum < 630)
    {
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
        }
        strip.setPixelColor(inpix+12,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+22,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+27,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+37,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+44,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+9,0,0,0);
        strip.setPixelColor(inpix+21,0,0,0);
        strip.setPixelColor(inpix+28,0,0,0);
        strip.setPixelColor(inpix+36,0,0,0);
        strip.setPixelColor(inpix+41,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+11,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+19,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+26,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+38,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+43,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+12,0,0,0);
        strip.setPixelColor(inpix+22,0,0,0);
        strip.setPixelColor(inpix+27,0,0,0);
        strip.setPixelColor(inpix+37,0,0,0);
        strip.setPixelColor(inpix+44,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
        }
        strip.setPixelColor(inpix+10,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+20,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+25,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+35,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+42,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+11,0,0,0);
        strip.setPixelColor(inpix+19,0,0,0);
        strip.setPixelColor(inpix+26,0,0,0);
        strip.setPixelColor(inpix+38,0,0,0);
        strip.setPixelColor(inpix+43,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
        strip.setPixelColor(inpix+9,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+21,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+28,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+36,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+41,rclock*1.5,gclock*1.5,bclock*1.8);
        strip.setPixelColor(inpix+10,0,0,0);
        strip.setPixelColor(inpix+20,0,0,0);
        strip.setPixelColor(inpix+25,0,0,0);
        strip.setPixelColor(inpix+35,0,0,0);
        strip.setPixelColor(inpix+42,0,0,0);
        strip.show();
        for(i=0; i<50; i++)
        {
            if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
            {
                delay(5);
            }
        }
    }
    else
    {
        if(tmr)
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
            for(i=0; i<100; i++)
            {
                if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
                {
                    delay(5);
                }
            }
            strip.show();
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
            for(i=0; i<100; i++)
            {
                if(digitalRead(D0)==LOW && digitalRead(D1)==LOW && digitalRead(D2)==LOW)
                {
                    delay(5);
                }
            }
            strip.show();
        }
    }
}                                  
void settings(){                                        //Code block for a settings menu, once initiated, it waits for the user to press the up (D0) and down (D2) to exit                                                              
    strip.clear();
    bool sett = true;
    int smode = 1;
    int dispm = 1;
    strdisp("Settings",0,rclock,gclock,bclock);
    while(digitalRead(D0) == HIGH || digitalRead(D2) == HIGH){
        delay(5);
    }
    strip.clear();
    while(sett == true)
    {
        if(smode == 1){
            strdisp("WiFi", 0, rclock, gclock, bclock);
        }
        else if(smode == 2){
            strdisp("Sig Strgth", 0, rclock, gclock, bclock);
        }
        else if(smode == 3){
            strdisp("Time zone", 0, rclock, gclock, bclock);
        }
        else if(smode == 4){
            strdisp("Sys vrsn", 0, rclock, gclock, bclock);
        }
        else if(smode == 5){
            strdisp("Dsp wthr", 0, rclock, gclock, bclock);
        }
        else if(smode == 6){
            strdisp("Get CO2", 0, rclock, gclock, bclock);
        }
        else if(smode == 7){
            strdisp("Get tmp", 0, rclock, gclock, bclock);
        }
        else if(smode == 8){
            strdisp("Drk clr", 0, rclock, gclock, bclock);
        }
        else if(smode == 9){
            strdisp("Mini clk", 0, rclock, gclock, bclock);
        }
        if(digitalRead(D1) == HIGH){
            if(dispm == 1){
                strip.clear();
                strdisp(WiFi.SSID(), 0, rclock/2, gclock, bclock/2);
                while(digitalRead(D1) == HIGH)
                {
                    delay(50);
                }
                while(digitalRead(D1) == LOW)
                {
                    delay(50);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 2){
                WiFiSignal sig = WiFi.RSSI();
                int strength = int(sig.getStrength());
                strip.clear();
                snum(strength/100,0,rclock,gclock,bclock);
                snum((strength/10)%10,32,rclock,gclock,bclock);
                snum(strength%10,64,rclock,gclock,bclock);
                strip.show();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 3){
                strip.clear();
                int tzoff = 12-EEPROM.read(1);
                if(tzoff < 0)
                {
                    letter('-', 0, rclock, gclock, bclock);
                    strip.show();
                }
                else
                {
                    letter('+', 0, rclock, gclock, bclock);
                    strip.show();
                }
                snum(abs(tzoff)/10,47,rclock,gclock,bclock);
                snum(abs(tzoff%10),79,rclock,gclock,bclock);
                strip.show();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D2) == HIGH && tzoff < 12){
                        tzoff = tzoff+1;
                        while(digitalRead(D2) == HIGH)
                        {
                            delay(5);
                        }
                    }
                    if(digitalRead(D0) == HIGH && tzoff > -12){
                        tzoff = tzoff-1;
                        while(digitalRead(D0) == HIGH)
                        {
                            delay(5);
                        }
                    }
                    if(tzoff < 0)
                    {
                        letter('+', 0, 0, 0, 0);
                        letter('-', 0, rclock, gclock, bclock);
                        strip.show();
                    }
                    else
                    {
                        letter('+', 0, rclock, gclock, bclock);
                        strip.show();
                    }
                    snum(abs(tzoff)/10,47,rclock,gclock,bclock);
                    snum(abs(tzoff%10),79,rclock,gclock,bclock);
                    strip.show();
                    delay(5);
                }
                strip.clear();
                EEPROM.write(1, 12-tzoff);
                Time.zone(12-EEPROM.read(1));
                Particle.syncTime();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 4){
                strip.clear();
                strdisp(System.version(),0,rclock,gclock,bclock);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 5){
                strip.clear();
                strdisp("YES",0,0,gclock,0);
                EEPROM.write(2,1);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D0) == HIGH){
                        strip.clear();
                        strdisp("YES",0,0,gclock,0);
                        EEPROM.write(2,1);
                    }
                    else if(digitalRead(D2) == HIGH){
                        strip.clear();
                        strdisp("NO",0,rclock,0,0);
                        EEPROM.write(2,0);
                    }
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 6){
                strip.clear();
                strdisp("YES",0,0,gclock,0);
                EEPROM.write(3,1);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D0) == HIGH){
                        strip.clear();
                        strdisp("YES",0,0,gclock,0);
                        EEPROM.write(3,1);
                    }
                    else if(digitalRead(D2) == HIGH){
                        strip.clear();
                        strdisp("NO",0,rclock,0,0);
                        EEPROM.write(3,0);
                    }
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 7){
                strip.clear();
                strdisp("YES",0,0,gclock,0);
                EEPROM.write(4,1);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D0) == HIGH){
                        strip.clear();
                        strdisp("YES",0,0,gclock,0);
                        EEPROM.write(4,1);
                    }
                    else if(digitalRead(D2) == HIGH){
                        strip.clear();
                        strdisp("NO",0,rclock,0,0);
                        EEPROM.write(4,0);
                    }
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 8){
                strip.clear();
                int color;
                color = 1;
                strdisp("RED",0,rclock,0,0);
                EEPROM.write(5,1);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D0) == HIGH && digitalRead(D2) == LOW){
                        strip.clear();
                        strdisp("RED",0,rclock,0,0);
                        EEPROM.write(5,1);
                    }
                    else if(digitalRead(D2) == HIGH && digitalRead(D0) == LOW){
                        strip.clear();
                        strdisp("GREEN",0,0,gclock,0);
                        EEPROM.write(5,2);
                    }
                    else if(digitalRead(D0) == HIGH && digitalRead(D2) == HIGH)
                    {
                        strip.clear();
                        strdisp("BLUE",0,0,0,bclock);
                        EEPROM.write(5,3);
                    }
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
            else if(dispm == 9){
                strip.clear();
                strdisp("YES",0,0,gclock,0);
                EEPROM.write(6,1);
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
                while(digitalRead(D1) == LOW)
                {
                    if(digitalRead(D0) == HIGH){
                        strip.clear();
                        strdisp("YES",0,0,gclock,0);
                        EEPROM.write(6,1);
                    }
                    else if(digitalRead(D2) == HIGH){
                        strip.clear();
                        strdisp("NO",0,rclock,0,0);
                        EEPROM.write(6,0);
                    }
                    delay(5);
                }
                strip.clear();
                while(digitalRead(D1) == HIGH)
                {
                    delay(5);
                }
            }
        }
        if(digitalRead(D0) == LOW && digitalRead(D1) == LOW && digitalRead(D2) == HIGH)
        {
            if(smode < 9){
                smode++;
                dispm++;
            }
            else{
                smode = 1;
                dispm = 1;
            }
            while(digitalRead(D2) == HIGH && digitalRead(D0) == LOW)
            {
                delay(5);
            }
            strip.clear();
        }
        if(digitalRead(D0) == HIGH && digitalRead(D1) == LOW && digitalRead(D2) == LOW)
        {
            if(smode > 1){
                smode--;
                dispm--;
            }
            else{
                smode = 9;
                dispm = 9;
            }
            while(digitalRead(D0) == HIGH && digitalRead(D2) == LOW)
            {
                delay(5);
            }
            strip.clear();
        }
        if(digitalRead(D0) == HIGH && digitalRead(D1) == LOW && digitalRead(D2) == HIGH)
        {
            strip.clear();
            sett = false;
        }
        delay(50);
    }
}                                                           
void loop() {                           //General operating loop of the program
    Particle.variable("Photo", analogRead(A4));
    Particle.variable("Co2", ico2);
    Particle.variable("Inside Temp", itemp);
    Particle.variable("EEPROM", eevalue);
    int hr = Time.hourFormat12();
    int min = Time.minute();
    
//////////////////////////////////////
///////////PHOTORESISTOR//////////////
//////////////////////////////////////    
    
    if(scan == 1){
        scan=2;
        if(analogRead(A4) < bound) {
            bound = upbound;
            if(fdark == 2){
                strip.clear();
                fdark = 1;
            }
            rclock = 0;
            gclock = 0;
            bclock = 0;
            if(EEPROM.read(5) == 2){
                gclock = 1;
            }
            else if(EEPROM.read(5) == 3){
                bclock = 1;
            }
            else{
                rclock = 1;
            }
            dmode = 1;
            RGB.control(true);
            RGB.color(0, 0, 0);
        }
        else {
            if(fdark == 1){
                strip.clear();
                fdark = 2;
            }
            rclock = analogRead(A4)/18;
            gclock = (analogRead(A4)/18)*0.8;
            bclock = (analogRead(A4)/18)*0.6;
            bound = dnbound;
            //RGB.control(false);
        }
    }
    
//////////////////////////////////////
///////////BUTTON PRESSES/////////////
//////////////////////////////////////
    
    if(digitalRead(D0) == HIGH && digitalRead(D1) == LOW && digitalRead(D2) == HIGH){            //Top & Bottom Button Press
        settings();
    }
    if(digitalRead(D2) == HIGH && digitalRead(D1) == LOW && digitalRead(D0) == LOW){            //Bottom Button Press
        strip.clear();
        strdisp("CO2:", 0, rclock/2,gclock/3, bclock*2);
        snum(ico2/1000, 143, rclock/2,gclock/3, bclock*2);
        snum((ico2/(100))%10, 175, rclock/2,gclock/3, bclock*2);
        snum((ico2/(10))%10, 207, rclock/2,gclock/3, bclock*2);
        snum((ico2%10), 239, rclock/2,gclock/3, bclock*2);
        strip.show();
        delay(5000);
        strip.clear();
    }
    else if(digitalRead(D2) == LOW && digitalRead(D1) == LOW && digitalRead(D0) == HIGH){            //Bottom Button Press
        strip.clear();
        if(dmode == 1){
            dmode = 3;
        }
        else{
            scan = 2;
            dmode = 1;
        }
        strip.show();
        while(digitalRead(D0) == HIGH){
            delay(10);
        }
    }
    if(digitalRead(D1) == HIGH && digitalRead(D0) == LOW && digitalRead(D2) == LOW){            //Middle Button Press
        if(dmode == 1){
            dmode = 2;
            photoupd = 1;
            strip.setPixelColor(7,0,gclock,bclock/2);
            strip.show();
            while(digitalRead(D1) == HIGH){
                delay(5);
            }
            strip.clear();
            strip.show();
        }
        else{
            strip.setPixelColor(7,0,gclock,bclock/2);
            strip.show();
            while(digitalRead(D1) == HIGH){
                delay(5);
            }
            strip.clear();
            strip.show();
            scan = 2;
            dmode = 1;
        }
        while(digitalRead(D1) == HIGH){
            delay(10);
        }
        strip.setPixelColor(7,0,0,0);
    }
    
//////////////////////////////////////
//////////////MODE 1//////////////////              Clock with cycling weather conditions
//////////////////////////////////////
    
    if(dmode == 1){                                                     //Mode 1


        /////////////
        ///CLOCK CODE
        if(mprev != min || scan == 2) {                         //Execute change in display if new time is found

            if(analogRead(A4)>bound || EEPROM.read(6) == 0){    //If the brightness is above the threshold, post large numbers by default
                mprev = Time.minute();
                if(min/10 == 0) {
                    num(0,79,rclock,gclock,bclock);
                    num(min,112,rclock,gclock,bclock);
                }
                else {
                    num((min/10),79,rclock,gclock,bclock);
                    num((min%10),112,rclock,gclock,bclock);
                }
                for(i=0;i<7;i++)                                 //Display 1 if present in the hour, 7 pixels tall
                {
                    strip.setPixelColor(i,rclock*(hr/10),gclock*(hr/10),bclock*(hr/10));
                }
                num(hr-(10*(hr/10)),16,rclock,gclock,bclock);    //Display other digit of hour
                strip.show();
            }

            else{                                               //Display small clock if set in EEPROM
                mprev = Time.minute();
                if(min/10 == 0) {
                    snum(0,63,rclock,gclock,bclock);
                    snum(min,95,rclock,gclock,bclock);
                }
                else {
                    snum((min/10),63,rclock,gclock,bclock);
                    snum((min%10),95,rclock,gclock,bclock);
                }
                for(i=0;i<5;i++)
                {
                    strip.setPixelColor(i,rclock*(hr/10),gclock*(hr/10),bclock*(hr/10));
                }
                snum(hr-(10*(hr/10)),16,rclock,gclock,bclock);
                strip.show();
            }
        }
        scan=1;


        /////////////////
        ///OTHER ELEMENTS
        if(analogRead(A4) >= bound)///////////////////////////////////////IF SENSOR IS BRIGHT
        {
            /*strip.setPixelColor(58,rclock,gclock,bclock);
            strip.setPixelColor(62,rclock,gclock,bclock);*/
            strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);
            for(i=0; i<100; i++)
            {
                if(digitalRead(D1)==LOW)
                {
                    delay(5);
                }
            }
            strip.show();
            /*strip.setPixelColor(58,rclock/2,gclock/2,bclock/2);
            strip.setPixelColor(62,rclock/2,gclock/2,bclock/2);*/
            for(i=0; i<100; i++)
            {
                if(digitalRead(D1)==LOW)
                {
                    delay(5);
                }
            }
            strip.show();
            ///////WEATHER DISPLAY MODES//////////
            if(EEPROM.read(2) == 1)
            {
                if(wmode == 1)                                      //Outdoor Temperature
                {
                    fillStrip(160,255,0,0,0);
                    for(i=0; i <= 25; i++){
                        num(TC1,160,0,(gclock*i)/25,0);
                        num(TC2,207,0,(gclock*i)/25,0);
                        strip.setPixelColor(240,0,(gclock*i)/25,0);
                        strip.show();
                        delay(2);
                    }
                    num(TC1,160,0,gclock/1,0);
                    num(TC2,207,0,gclock/1,0);
                    strip.setPixelColor(240,0,gclock/1,0);
                    for(i=0; i < 25; i++){
                        if(digitalRead(D0) == LOW){
                            delay(100);
                        }
                    }
                    for(i=25; i >= 0; i--){
                        num(TC1,160,0,(gclock*i)/25,0);
                        num(TC2,207,0,(gclock*i)/25,0);
                        strip.setPixelColor(240,0,(gclock*i)/25,0);
                        strip.show();
                        delay(2);
                    }
                    if(EEPROM.read(4) == 1){
                        wmode = 2;
                    }
                    else{
                        wmode = 3;
                    }
                    strip.show();
                }
                if(wmode == 2)                                      //Indoor Temp from Adafruit Sensor
                {
                    fillStrip(160,255,0,0,0);
                    for(i=0; i <= 25; i++){
                        num(itemp/10,160,(rclock*i)/50,0,(bclock*i)/25);
                        num(itemp%10,207,(rclock*i)/50,0,(bclock*i)/25);
                        strip.setPixelColor(240,(rclock*i)/50,0,(bclock*i)/25);
                        strip.show();
                        delay(2);
                    }
                    for(i=0; i < 25; i++){
                        if(digitalRead(D0) == LOW){
                            delay(100);
                        }
                    }
                    for(i=25; i <= 0; i--){
                        num(itemp/10,160,(rclock*i)/50,0,(bclock*i)/25);
                        num(itemp%10,207,(rclock*i)/50,0,(bclock*i)/25);
                        strip.setPixelColor(240,(rclock*i)/50,0,(bclock*i)/25);
                        strip.show();
                        delay(2);
                    }
                    wmode = 3;
                }
                else if (wmode == 3)
                {
                    fillStrip(160,255,0,0,0);
                    for(i=0; i <= 25; i++){
                        if(HC1 == 10)
                        {
                            strip.setPixelColor(160,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(161,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(162,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(163,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(164,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(165,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(166,0,(gclock*i)/25,(bclock*i)/25);
                            num(0,176,0,(gclock*i)/25,(bclock*i)/25);
                            num(0,223,0,(gclock*i)/25,(bclock*i)/25);
                        }
                        else{
                            num(HC1,160,0,(gclock*i)/25,(bclock*i)/25);
                            num(HC2,207,0,(gclock*i)/25,(bclock*i)/25);
                        }
                        strip.show();
                        delay(2);
                    }
                    for(i=0; i < 25; i++){
                        if(digitalRead(D0) == LOW){
                            delay(100);
                        }
                    }
                    for(i=25; i >= 0; i--){
                        if(HC1 == 10)
                        {
                            strip.setPixelColor(160,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(161,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(162,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(163,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(164,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(165,0,(gclock*i)/25,(bclock*i)/25);
                            strip.setPixelColor(166,0,(gclock*i)/25,(bclock*i)/25);
                            num(0,176,0,(gclock*i)/25,(bclock*i)/25);
                            num(0,223,0,(gclock*i)/25,(bclock*i)/25);
                        }
                        else{
                            num(HC1,160,0,(gclock*i)/25,(bclock*i)/25);
                            num(HC2,207,0,(gclock*i)/25,(bclock*i)/25);
                        }
                        strip.show();
                        delay(2);
                    }
                    wmode = 4;
                }
                else if(wmode == 4)
                {
                    fillStrip(160,255,0,0,0);
                    condition(cid,160);
                    strip.show();
                    delay(100);
                    canim(cid,160,true);
                    canim(cid,160,true);
                    canim(cid,160,true);
                    wmode = 1;
                }
            }
        }
        else//////////////////////////////////////////////////////////IF SENSOR IS DARK
        {
            delay(1000);
            if(photoupd == 1)
            {
                for(i=0;i<160;i++)
                {
                    strip.setPixelColor(i,0,0,0);
                }
                photoupd=0;
            }
            for(i=160;i<247;i++)
            {
                strip.setPixelColor(i,0,0,0);
            }
            if(EEPROM.read(2) == 1)
            {
                condition(cid,160);
                dimg(cid,160);
            }
            strip.show();
            strip.setPixelColor(62,0,0,0);
            strip.setPixelColor(58,0,0,0);
            strip.setPixelColor(240,0,0,0);
            strip.show();
        }
    }
    
//////////////////////////////////////
//////////////MODE 2//////////////////              Show all weather condition
//////////////////////////////////////
    
    else if(dmode == 2)                                 //Mode 2
    {
        
        if(cid == cprev)
        {
            condition(cid,0);
            strip.show();
        }
        else
        {
            for(i=0;i<49;i++)
            {
                strip.setPixelColor(i,0,0,0);
            }
            strip.show();
            condition(cid,0);
            strip.show();
            cprev = cid;
        }
        canim(cid,0,false);
        canim(cid,0,false);
        canim(cid,0,false);
        if(HC1 == 10)
        {
            strip.setPixelColor(175,0,gclock,bclock);
            strip.setPixelColor(174,0,gclock,bclock);
            strip.setPixelColor(173,0,gclock,bclock);
            strip.setPixelColor(172,0,gclock,bclock);
            strip.setPixelColor(171,0,gclock,bclock);
            strip.setPixelColor(170,0,gclock,bclock);
            strip.setPixelColor(169,0,gclock,bclock);
            num(0,191,0,gclock,bclock);
            num(0,224,0,gclock,bclock);
        }
        else
        {
            strip.setPixelColor(240,0,0,0);
            num(HC1,191,0,gclock,bclock);
            num(HC2,224,0,gclock,bclock);
        }
        num(TC1,79,0,gclock,0);
        num(TC2,112,0,gclock,0);
        strip.setPixelColor(159,0,gclock,0);
        strip.show();
        scan = 1;
        delay(10);
    }
    
        
//////////////////////////////////////
//////////////MODE 3//////////////////              Scrolling Clock
//////////////////////////////////////
    
    else if(dmode == 3)                                 //Mode 3
    {
        int numRows = 34;
        for(i = 0; i < numRows; i++){
            /*if(min/10 == 0) {
                num(0,(i*16)+79,rclock,gclock,bclock);
                num(min,(i*16)+112,rclock,gclock,bclock);
            }
            else {
                num((min/10),(i*16)+79,rclock,gclock,bclock);
                num((min%10),(i*16)+112,rclock,gclock,bclock);
            }
            for(j=0;j<7;j++)
            {
                strip.setPixelColor(j+(i*16),rclock*(hr/10),gclock*(hr/10),bclock*(hr/10));
            }
            num(hr-(10*(hr/10)),16+(i*16),rclock,gclock,bclock);
            
            strip.setPixelColor(58+(i*16),rclock,gclock,bclock);
            strip.setPixelColor(62+(i*16),rclock,gclock,bclock);
            
            //Clock
            num(TC1,(i*16)-384,0,gclock/1,0);
            num(TC2,(i*16)-337,0,gclock/1,0);
            strip.setPixelColor((i*16)-304,0,gclock/1,0);
            
            num(itemp/10,(i*16)-288,rclock/2,0,bclock);
            num(itemp%10,(i*16)-241,rclock/2,0,bclock);
            strip.setPixelColor((i*16)-208,rclock/2,0,bclock);
            
            num(HC1,(i*16)-192,0,gclock,bclock);
            num(HC2,399+(i*16)-145,0,gclock,bclock);
            
            condition(cid,(i*16)-96);
            dimg(cid,(i*16)-96);*/
            
            if(min/10 == 0) {
                num(0,(i*16)+79,rclock,gclock,bclock);
                num(min,(i*16)+112,rclock,gclock,bclock);
            }
            else {
                num((min/10),(i*16)+79,rclock,gclock,bclock);
                num((min%10),(i*16)+112,rclock,gclock,bclock);
            }
            for(j=0;j<7;j++)
            {
                strip.setPixelColor(j+(i*16),rclock*(hr/10),gclock*(hr/10),bclock*(hr/10));
            }
            num(hr-(10*(hr/10)),16+(i*16),rclock,gclock,bclock);
            
            /*strip.setPixelColor(58+(i*16),rclock,gclock,bclock);
            strip.setPixelColor(62+(i*16),rclock,gclock,bclock);*/
            
            num(TC1,160+(i*16),0,gclock/1,0);
            num(TC2,207+(i*16),0,gclock/1,0);
            strip.setPixelColor(240+(i*16),0,gclock/1,0);
            
            num(itemp/10,256+(i*16),rclock/2,0,bclock);
            num(itemp%10,303+(i*16),rclock/2,0,bclock);
            strip.setPixelColor(336+(i*16),rclock/2,0,bclock);
            
            num(HC1,352+(i*16),0,gclock,bclock);
            num(HC2,399+(i*16),0,gclock,bclock);
            
            condition(cid,448+(i*16));
            dimg(cid,448+(i*16));
            
            
            if(min/10 == 0) {
                num(0,((i-numRows)*16)+79,rclock,gclock,bclock);
                num(min,((i-numRows)*16)+112,rclock,gclock,bclock);
            }
            else {
                num((min/10),((i-numRows)*16)+79,rclock,gclock,bclock);
                num((min%10),((i-numRows)*16)+112,rclock,gclock,bclock);
            }
            for(j=0;j<7;j++)
            {
                strip.setPixelColor(j+((i-numRows)*16),rclock*(hr/10),gclock*(hr/10),bclock*(hr/10));
            }
            num(hr-(10*(hr/10)),16+((i-numRows)*16),rclock,gclock,bclock);
            
            /*strip.setPixelColor(58+((i-numRows)*16),rclock,gclock,bclock);
            strip.setPixelColor(62+((i-numRows)*16),rclock,gclock,bclock);*/
            
            num(TC1,160+((i-numRows)*16),0,gclock/1,0);
            num(TC2,207+((i-numRows)*16),0,gclock/1,0);
            strip.setPixelColor(240+((i-numRows)*16),0,gclock/1,0);
            
            num(itemp/10,256+((i-numRows)*16),rclock/2,0,bclock);
            num(itemp%10,303+((i-numRows)*16),rclock/2,0,bclock);
            strip.setPixelColor(336+((i-numRows)*16),rclock/2,0,bclock);
            
            num(HC1,352+((i-numRows)*16),0,gclock,bclock);
            num(HC2,399+((i-numRows)*16),0,gclock,bclock);
            
            condition(cid,448+((i-numRows)*16));
            dimg(cid,448+((i-numRows)*16));
            strip.show();
            //Delay - speed of scroll
            if(digitalRead(D0) == HIGH){
                break;
            }
            delay(200);
            
            //Erase for next shift
            strip.clear();
        }
    }
    
//////////////////////////////////////
///////GET WEATHER DATA///////////////
//////////////////////////////////////
    
    
    if(tmr >= 60) {
        Particle.publish("Weather", "1", PRIVATE);
        if(EEPROM.read(3) == 1)
        {
            Particle.publish("co2dat", "1", PRIVATE);
        }
        if(EEPROM.read(4) == 1)
        {
            Particle.publish("itempdat", "1", PRIVATE);
        }
        tmr = 0;
    }
    else if(tmr == 30){
        tmr = tmr+1;
        Particle.publish("Weather", "1", PRIVATE);
        if(EEPROM.read(3) == 1)
        {
            Particle.publish("co2dat", "1", PRIVATE);
        }
        if(EEPROM.read(4) == 1)
        {
            Particle.publish("itempdat", "1", PRIVATE);
        }
    }
    else{
        tmr = tmr+1;
    }

}
void fillStrip(int start, int end, int R, int G, int B){
    for(i = start; i <= end; i++){
        strip.setPixelColor(i, R, G, B);
    }
}
void myHandler(const char *event, const char *data) {
  // Handle the integration response
    int j;
    char tempdata[6];
    char humdata[3];
    char cdata[3];
    float tempK;
    float tempF;
    float tempC;
    int humidity;
    for(j=0;j<strlen(data);j++)
    {
        if(data[j] == 't' && data[j+1] == 'e' && data[j+2] == 'm' && data[j+3] == 'p' && data[j+4] == '"'){
            tempdata[0] = data[j+6];
            tempdata[1] = data[j+7];
            tempdata[2] = data[j+8];
            tempdata[3] = data[j+9];
            tempdata[4] = data[j+10];
            tempdata[5] = data[j+11];
        }
        if(data[j] == 'h' && data[j+1] == 'u' && data[j+2] == 'm' && data[j+3] == 'i'){
            humdata[0] = data[j+10];
            humdata[1] = data[j+11];
            if(data[j+12] == '0')
            {
                humdata[2] = data[j+12];
            }
        }
        if(data[j] == 'i' && data[j+1] == 'c' && data[j+2] == 'o' && data[j+3] == 'n'){
            cltr[0] = data[j+9];
        }
        if(data[j-1] == '[' && data[j] == '{' && data[j+1] == '"' && data[j+2] == 'i' && data[j+3] == 'd'){
            cdata[0] = data[j+6];
            cdata[1] = data[j+7];
            cdata[2] = data[j+8];
        }
    }
    humidity = atof(humdata);
    cid = atof(cdata);
    if(cid == 781)
    {
        for(i=160;i<247;i++)
        {
            strip.setPixelColor(i,0,0,0);
        }
        condition(cid,160);
        strip.show();
        for(i=0;i<100;i++)
        {
            sound(300, 250);
            delay(250);
        }
    }
    HC1 = humidity/10;
    HC2 = humidity%10;
    tempK = atof(tempdata);
    tempC = tempK-273.15;
    tempF = (tempC*1.8)+32;
    TFahr = (int)tempF;
    TC1 = TFahr/10;
    TC2 = TFahr%10;
    strip.show();
    delay(10);
}
void myHandler2(const char *event, const char *data) {
    if(strcmp(data,"mode0")==0)
    {
        if(analogRead(A4) > bound)
        {
            strip.clear();
            scan = 2;
            dmode = 1;
        }
    }
    if(strcmp(data,"mode1")==0)
    {
        if(analogRead(A4) > bound)
        {
            dmode = 2;
        }
    }
}
void mshHdlr(const char *event, const char *data) {
    Particle.publish("getState","1");
    Particle.publish("btnpress999","toggle");
}
void mshHdlr2(const char *event, const char *data) {
    int lmpmode;
    char dat[3];
    for(i=0;i<strlen(data);i++){
        if(data[i]=='u' && data[i+1]=='l' && data[i+2]=='t'){
            dat[0] = data[i+5];
        }
    }
    lmpmode = atof(dat);
    if(lmpmode == 0){
        //for plugs, call
        //TPLink_Plug(plugIP, 0);
        //where state = 0 for OFF, state = 1 for ON
    }
    else{
        //for plugs, call
        //TPLink_Plug(plugIP, 1);
        //where state = 0 for OFF, state = 1 for ON
    }
}
void co2Handler(const char *event, const char *data) {
    for(j=0;j<strlen(data);j++){
        float codata;
        char codat[4];
        if(data[j] == 'l' && data[j+1] == 'u' && data[j+2] == 'e' && data[j+3] == '"' && data[j+4] == ':' && data[j+5] == '"'){
            codat[0] = data[j+6];
            codat[1] = data[j+7];
            codat[2] = data[j+8];
            codat[3] = data[j+9];
        }
        codata = atof(codat);
        ico2 = (int)codata;
        if(analogRead(A4) < bound && EEPROM.read(3) == 1)// && dco2 == 1)
        {
            for(i=248;i<256;i++){
                strip.setPixelColor(i,0,0,0);
            }
            for(i=248;i<(ico2/750)+248;i++){
                if(i>248 && i<256){
                    strip.setPixelColor(i,rclock,gclock,bclock);
                }
                else if(i==248)
                {
                    if(TFahr < 33){
                        strip.setPixelColor(i,bclock,gclock,rclock);
                    }
                    else{
                        strip.setPixelColor(i,rclock,gclock,bclock);
                    }
                }
            }
            strip.show();
        }
    }
}
void itHandler(const char *event, const char *data) {
    for(j=0;j<strlen(data);j++){
        float tempF;
        float tempC;
        char tempdat[2];
        if(data[j] == 'l' && data[j+1] == 'u' && data[j+2] == 'e' && data[j+3] == '"' && data[j+4] == ':' && data[j+5] == '"'){
            tempdat[0] = data[j+6];
            tempdat[1] = data[j+7];
            //tempdat[2] = data[j+8];
            //tempdat[3] = data[j+9];
        }
        tempC = atof(tempdat)-2;                //Temperature offset for accuracy of thermometer
        tempF = (tempC*1.8)+32;
        itemp = (int)tempF;
    }
}
/*void numtest(){
    snum(0,0,50,50,50);
    strip.show();
    delay(1000);
    snum(1,0,50,50,50);
    strip.show();
    delay(1000);
    snum(2,0,50,50,50);
    strip.show();
    delay(1000);
    snum(3,0,50,50,50);
    strip.show();
    delay(1000);
    snum(4,0,50,50,50);
    strip.show();
    delay(1000);
    snum(5,0,50,50,50);
    strip.show();
    delay(1000);
    snum(6,0,50,50,50);
    strip.show();
    delay(1000);
    snum(7,0,50,50,50);
    strip.show();
    delay(1000);
    snum(8,0,50,50,50);
    strip.show();
    delay(1000);
    snum(9,0,50,50,50);
    strip.show();
    delay(1000);
    strip.clear();
    strip.show();
    delay(1000);
    snum(0,15,50,50,50);
    strip.show();
    delay(1000);
    snum(1,15,50,50,50);
    strip.show();
    delay(1000);
    snum(2,15,50,50,50);
    strip.show();
    delay(1000);
    snum(3,15,50,50,50);
    strip.show();
    delay(1000);
    snum(4,15,50,50,50);
    strip.show();
    delay(1000);
    snum(5,15,50,50,50);
    strip.show();
    delay(1000);
    snum(6,15,50,50,50);
    strip.show();
    delay(1000);
    snum(7,15,50,50,50);
    strip.show();
    delay(1000);
    snum(8,15,50,50,50);
    strip.show();
    delay(1000);
    snum(9,15,50,50,50);
    strip.show();
    delay(1000);
}
void lettest(int dly, int R, int G, int B){
    letter('a',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('b',0,R,G,B);
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('c',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('d',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('e',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('f',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('g',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('h',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('i',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('j',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('k',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('l',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('m',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('n',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('o',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('p',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('q',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('r',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('s',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('t',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('u',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('v',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('w',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('x',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('y',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    letter('z',0,R,G,B);
    strip.show();
    delay(dly);
    for(i=0; i<37; i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
}*/










