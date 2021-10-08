#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

#define DI          15  // PC1
#define DO           8  // PB0
#define CAS          9  // PB1
#define RAS         17  // PC3
#define WE          16  // PC2

#define XA0         10//18  // PC4
#define XA1          2  // PD2
#define XA2         11//19  // PC5
#define XA3          6  // PD6
#define XA4          5  // PD5
#define XA5          4  // PD4
#define XA6          7  // PD7
#define XA7          3  // PD3
#define XA8         14  // PC0

#define M_START     12  // Start/mode 
//#define M_CHIPSEL   12 // Selector 4116 / 4164
#define M_CHIPSEL   A6 // Selector 4116 / 4164
#define BUS_SIZE     9
/*
 * GENERAL PINOUT ARDUINO
 * D1 = SWITCH START TEST
 * A4 = LCD
 * A5 = LCD
 * D12 = SWITCH MODE (5V) 4116 - (GND) 4164/256
 *
 *menù
 *sw 4116                             -                         4164/256
 *press start (2 sec to start test)   - press stat to select 4164/256, 4532L 4532H, long press to start test
 *
 */


int mode;
int lastState = HIGH;
int currentState;
unsigned long pressedTime = 0;
byte selector = 0;
byte l_sel = 0;
bool isError = false;
byte ntest =0;

String  m_sym[2] = {"\\","/"};

volatile int bus_size;
const unsigned int a_bus[BUS_SIZE] = {
  XA0, XA1, XA2, XA3, XA4, XA5, XA6, XA7, XA8
};


/* -----  SETUP  ------ */
 void setup() {
  int i;
   u8g2.begin();
   Serial.begin(9600); 

for (i = 0; i < BUS_SIZE; i++)
    pinMode(a_bus[i], OUTPUT);

  pinMode(CAS, OUTPUT);
  pinMode(RAS, OUTPUT);
  pinMode(WE, OUTPUT);
  pinMode(DI, OUTPUT);
  //pinMode(M_CHIPSEL, INPUT);
  pinMode(M_START, INPUT);
  pinMode(DO, INPUT);
  digitalWrite(WE, HIGH);
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);

  mode =EEPROM.read(0);//memoria 4164-256-32l-32h
  if (mode>3){mode=0;}

/*
if (digitalRead(M_TYPE)) {
    /* jumper not set - 41256 */
    /*bus_size = BUS_SIZE;
    //Serial.print("256Kx1 ");
  } else {
    /* jumper set - 4164 */
    /*bus_size = BUS_SIZE - 1;
    //Serial.print("64Kx1 ");
  }*/
  selector=selchip();
  l_sel=selector;
  setDisp();\
}
/* -----------  LOOP  ---------- */
  void loop(void) {

    getserial();

    selector=selchip(); // 0= vuoto 1=4164 2=4116

    if (l_sel!=selector) {
      l_sel=selector;
      setDisp();
    }
      
   //change mode - start
   currentState = digitalRead(M_START);
  if (lastState == HIGH && currentState == LOW) { // button is pressed
    pressedTime = millis();
    lastState = LOW;
  }
  if(lastState == LOW && currentState == HIGH) { // button is released
    lastState = HIGH;
    long pressDuration = millis() - pressedTime;
    if (pressDuration < 1500 ) { //short press chenge mode
      if (selector==1 ){
        mode++;
        if (mode > 3) mode=0;
        EEPROM.write(0, mode);
        setDisp();
      }
    }
  }
    if(lastState == LOW && currentState == LOW &&  millis() - pressedTime > 1500) {
    //else { //long press start
      isError=false;
      ntest=0;
      if (selector>0) startTest();
  }
   
   delay(100);
}

   
void startTest() {
   initDram();
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_logisoso16_tr);
   u8g2.drawStr(8,16,"Test no.1");
   u8g2.setFont(u8g2_font_gb16st_t_2);
   switch (selector) {
    case 2:
    u8g2.drawStr(4,32,">16k");
    bus_size = BUS_SIZE - 2;
    break;
    case 1:
      if (mode == 0){
      u8g2.drawStr(4,32,">256k");
      bus_size = BUS_SIZE;
      } else {
      bus_size = BUS_SIZE - 1;
      }
      if (mode == 1) u8g2.drawStr(4,32,">64k");
      if (mode == 2) u8g2.drawStr(4,32,">32k Low");
      if (mode == 3) u8g2.drawStr(4,32,">32k High");
   }

  u8g2.sendBuffer();
  interrupts(); noInterrupts(); 
  
  fillx(0);
  if (!isError){
    resDisp("Test no.2");
    interrupts();u8g2.sendBuffer(); noInterrupts(); 
    fillx(1);
  }
  if (!isError){
    interrupts(); 
    resDisp("Full Write...");
    u8g2.sendBuffer();
    noInterrupts(); 
    fill(1,false);
  }
  if (!isError){
    interrupts(); 
    resDisp("Full Read...");
    u8g2.sendBuffer();
    noInterrupts(); 
    readonly(1);
  }
    if (!isError){
   resDisp("Finishing...");
    interrupts();u8g2.sendBuffer(); noInterrupts(); 
    fill(0,true);
  }

  interrupts();
  
  if (!isError){
   finish();
   /*
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_logisoso16_tr);
   u8g2.drawStr(6,16,"TEST PASSED");
   u8g2.setFont(u8g2_font_gb16st_t_2);
   u8g2.drawStr(12,30,"press start");
   u8g2.sendBuffer();*/
         while (digitalRead(M_START)==LOW) {
      }
          while (digitalRead(M_START)==HIGH) {
      }
      setDisp();
      delay(300);
  } else {
    setDisp();
  }
}
