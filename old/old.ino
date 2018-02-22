#include <DS3231.h>
#include <OLED_I2C.h>
/**
 * 
 * EDIT VERSION HERE
 * 
 */
const String VERSION = "JAWW v.0.9";
 /**
  * 
  * END EDITING VERSION HERE
  * 
  */

//init oled & rtc
DS3231  rtc(SDA, SCL);
OLED myOLED(SDA, SCL, 8);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

int BL = 2;
int BM = 3;
int BR = 4;
int PageNR = 0;
int MainMenuTile = 0;
int LED = 5;
String MainMenuText[] = {" Clock "," Settings "," FlashLight "," NightMode "};
boolean invertedMode = false;

Time  t;

int nBL,nBM,nBR,pBL,pBM,pBR;
boolean BLchange,BMchange,BRchange,Schange;

boolean FlashLight = false;

int clamp(int var, int mn, int mx){
  if(var < mn){
    var = mn;
  }else if(var > mx){
    var = mx;
  }
  return var;
}

void setup()
{
  // Setup Serial
  Serial.begin(115200);
  
  // Initialize objects
  rtc.begin();
  myOLED.begin();
  myOLED.setFont(SmallFont);

  pinMode(BL, INPUT);
  pinMode(BM, INPUT);
  pinMode(BR, INPUT);
  pinMode(LED,OUTPUT);

  myOLED.clrScr();
  myOLED.print("LOADING.",CENTER,32);
  myOLED.update();
  delay(1000);
  myOLED.clrScr();
  myOLED.print("LOADING..",CENTER,32);
  myOLED.update();
  delay(1000);
  myOLED.clrScr();
  myOLED.print("LOADING...",CENTER,32);
  myOLED.update();
  delay(1000);
  myOLED.clrScr();
  myOLED.print(VERSION,CENTER,32);
  myOLED.update();
  delay(1000);
  myOLED.clrScr();
  drawPageClock();
  myOLED.update();
  PageNR = 0;
}

void loop()
{
  
  nBL = digitalRead(BL);
  nBM = digitalRead(BM);
  nBR = digitalRead(BR);
  t = rtc.getTime();

  BLchange = false;
  BMchange = false;
  BRchange = false;

  if(nBL != pBL && nBL == HIGH){
    BLchange = true;
  }
  if(nBM != pBM && nBM == HIGH){
    BMchange = true;
  }
  if(nBR != pBR && nBR == HIGH){
    BRchange = true;
  }

  myOLED.clrScr();
  //START OF PAGING
  
  if(PageNR == 0 && (BLchange || BMchange || BRchange)){
    PageNR = 1;
    BLchange = false;
    BMchange = false;
    BRchange = false;
  }
  if(PageNR == 1 && BMchange){
    switch(MainMenuTile){
      case 0: PageNR = 0; break;
      case 1: PageNR = 2; break;
      case 2: FlashLight = !FlashLight; break;
      case 3: invertedMode = !invertedMode; break;
    }
    BMchange = false;
  }
  if(PageNR == 1 && BRchange){
    if(MainMenuTile == 0){
      MainMenuTile = 3;
    }else{
      MainMenuTile--;
    }
    BRchange = false;
  }
  
  if(PageNR == 1 && BLchange){
    MainMenuTile++;
    if(MainMenuTile == 4){
      MainMenuTile = 0;
    }
    BLchange = false;
  }
  
   if(PageNR == 0 && (!BLchange && !BMchange && !BRchange)){
    drawPageClock();
  }
  if(PageNR == 1 && (!BLchange && !BMchange && !BRchange)){
    drawPageMainMenu();
  }

  myOLED.invert(invertedMode);
  digitalWrite(LED,FlashLight);
  
  //END OF LOOP
  myOLED.update();
  pBL = nBL;
  pBM = nBM;
  pBR = nBR;
}

void drawPageClock(){
  myOLED.setFont(SmallFont);
  myOLED.print(VERSION,LEFT,0);
  myOLED.print(String(rtc.getTemp()) + "C",RIGHT,0);
  myOLED.print(rtc.getDateStr(), CENTER, 16);
  myOLED.print(rtc.getDOWStr(), CENTER, 24);
  myOLED.setFont(BigNumbers);
  myOLED.print(rtc.getTimeStr(), CENTER,40);
  delay(10);
}

void drawPageMainMenu(){
  myOLED.setFont(SmallFont);
  myOLED.print(rtc.getTimeStr() + String(" - ") + rtc.getDateStr(), CENTER,0);
  //myOLED.setFont(BigNumbers);
  for(int i = 0; i < 5; i++){
    if(i == MainMenuTile){
      myOLED.print("> " + MainMenuText[i],LEFT,8 + 8 * i);
    }else{
      myOLED.print(MainMenuText[i],LEFT,8 + 8 * i);
    }
  }
  delay(10);
}

