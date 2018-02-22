//OLED Library from https://github.com/greiman/SSD1306Ascii.git
//RTC  Library from http://www.rinkydinkelectronics.com/library.php?id=73
//
//HARDWARE
//I2C OLED Display  on A4 / A5 or dedicated SDA / SCL Pins
//DS3231 RTC Module on A4 / A5 or dedicated SDA / SCL Pins
//Left Button       on Pin 2
//Middle Button     on Pin 3
//Right Button      on Pin 4
//LED               on Pin 5

//including needed libraries
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "DS3231.h"

//defining RTC and OLED module
DS3231 rtc(SDA,SCL);
SSD1306AsciiAvrI2c oled;

/**
 * 
 * EDIT VERSION HERE
 * 
 */
const String VERSION = "JAWW v.0.9.2";
 /**
  * 
  * END EDITING VERSION HERE
  * 
  */

//defining variables
String temperatur;
String datum;
String DOW;
String zeit;
int nBL,nBM,nBR,pBL,pBM,pBR;
boolean BLchange,BMchange,BRchange,repaint;
int BL = 2;
int BM = 3;
int BR = 4;
int LED = 5;
boolean FlashLight = false;
int PageNR = 0;
int MainMenuTile = 0;

String MainMenuText[] = {" Clock "," Settings "," FlashLight "," NightMode "};
boolean invertedMode = false;

//setup begin
void setup() { 
  Serial.begin(115200);               
  oled.begin(&Adafruit128x64, 0x3C);
  rtc.begin();
  oled.setFont(System5x7);
  oled.clear();
  oled.print("TEST");

  pinMode(BL, INPUT);
  pinMode(BM, INPUT);
  pinMode(BR, INPUT);
  pinMode(LED,OUTPUT);
  repaint = true;
}

//loop
void loop()
{
  //Serial.println("loop");
  temperatur = String(rtc.getTemp()) + "C";
  //Serial.println("loop");
  datum = rtc.getDateStr();
  //Serial.println("loop");
  DOW = rtc.getDOWStr();
  //Serial.println("loop");
  zeit = rtc.getTimeStr();
  //Serial.println("rtc");
  nBL = digitalRead(BL);
  nBM = digitalRead(BM);
  nBR = digitalRead(BR);
  //Serial.println("read");
  
  //PAGING VARIABLES
  BLchange = false;//Left pressed
  BMchange = false;//middle pressed
  BRchange = false;//right pressed
  boolean btnPressed = false;//anything pressed

  if(nBL != pBL && nBL == HIGH){
    BLchange = true;
    btnPressed = true;
  }
  if(nBM != pBM && nBM == HIGH){
    BMchange = true;
    btnPressed = true;
  }
  if(nBR != pBR && nBR == HIGH){
    BRchange = true;
    btnPressed = true;
  }
  //PAGING VARIABLES END

   if(btnPressed){
    Serial.println("pressed");
    if(PageNR == 1 && BMchange){
      switch(MainMenuTile){
        case 0: PageNR = 0; break;
        case 1: PageNR = 2; break;
        case 2: FlashLight = !FlashLight; break;
        case 3: invertedMode = !invertedMode; break;
      }
      BMchange = false;
      repaint = true;
    }
    if(PageNR == 1 && BRchange){
      if(MainMenuTile == 0){
        MainMenuTile = 3;
      }else{
        MainMenuTile--;
      }
      BRchange = false;
      repaint = true;
    }
  
    if(PageNR == 1 && BLchange){
      MainMenuTile++;
      if(MainMenuTile == 4){
        MainMenuTile = 0;
      }
      BLchange = false;
      repaint = true;
    }
  
    
  }
  if(PageNR == 0 && repaint){
    drawPageClock();
  }
  if(PageNR == 1 && repaint){
    drawPageMainMenu();
  }
  repaint = false;
  
  //Serial.println(PageNR);
  //FINISHING UP
  pBL = nBL;
  pBM = nBM;
  pBR = nBR;
  digitalWrite(LED,FlashLight);
  delay(100);
}






void drawPageClock(){
  oled.clear();  
  oled.println("Hello world!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nTEST");
}

void drawPageMainMenu(){
  
}

