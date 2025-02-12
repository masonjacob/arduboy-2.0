#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
#include <SeeedTouchScreen.h>
#include <EEPROM.h>

#define hTiles 28
#define vTiles 29
#define sprSize 16 
#define tileSize 8 
#define w hTiles*tileSize
#define h vTiles*tileSize
#define tilesLen hTiles*vTiles
#define copySprite0 cSprite=number; cSpriteMod=modify; for(int i=0; i<256; i++) wSprite[i]=newSprite[i]
#define copySprite2 cSprite=number; cSpriteMod=modify; for(int i=0; i<16; i++) for(int j=0; j<16; j++) wSprite[i*sprSize + sprSize - 1 - j]=newSprite[i*sprSize + j]
#define copySprite3 cSprite=number; cSpriteMod=modify; for(int i=0; i<16; i++) for(int j=0; j<16; j++) wSprite[j*sprSize + i]=newSprite[i*sprSize + j]
#define copySprite1 cSprite=number; cSpriteMod=modify; for(int i=0; i<16; i++) for(int j=0; j<16; j++) wSprite[(sprSize -1 - j)*sprSize + i]=newSprite[i*sprSize + j]
#define copySprite switch(modify) { case 0: { copySprite0; break; } case 1: { copySprite1; break; } case 2: { copySprite2; break; } case 3: { copySprite3; break; } }
#define mul 4
//#define ALTCONTROLS true

  byte posX = 125, posY = 10; 
  int dPosX, dPosY;
  unsigned int score=0, hiscore;  //x and y start position
  float gPosX = 15, gPosY = 23.4;  //x and y start position
  float vX = 0, vY = 0, animTimeout=0, frightTimer=0;  //x and y start position
  float delTime;
  byte oldPosX = 20, oldPosY = 120;  //x and y start position
  byte jX=70, jY=120, foundCX=0, foundCY=0, dotCounter=0, level=1, lives=3;
  byte minMove=5;
  byte cSpriteMod=0;
  byte orientation=0, spriteNum=0, fpsC=5;
  byte oldOr=0, oldSprite;
  boolean hMove=true, /*drawJoy=true,*/ paused=false;
  byte offsetx, offsety;
  byte cSprite = 0;
  byte cTile = 0;
  byte aColor;
  int frames = 0;
  //byte tileX, tileY;
  boolean makingLine=false;
  byte lineColor;
  byte lineStart;
  unsigned long countTime, oldTime, actTime;
  byte tiles[tilesLen/mul];//= { 87, 85, 85, 125, 85, 85, 213, 247, 223, 255, 125, 255, 247, 223, 251, 223, 255, 125, 255, 247, 239, 247, 223, 255, 125, 255, 247, 223, 87, 85, 85, 85, 85, 85, 213, 247, 223, 247, 255, 223, 247, 223, 247, 223, 247, 255, 223, 247, 223, 87, 213, 87, 125, 213, 87, 213, 255, 223, 255, 125, 255, 247, 255, 255, 223, 255, 125, 255, 247, 255, 255, 223, 3, 0, 192, 247, 255, 255, 223, 243, 255, 207, 247, 255, 252, 223, 51, 0, 204, 247, 63, 0, 16, 48, 0, 12, 4, 0, 252, 223, 51, 0, 204, 247, 63, 255, 223, 243, 255, 207, 247, 255, 255, 223, 3, 0, 192, 247, 255, 255, 223, 243, 255, 207, 247, 255, 255, 223, 243, 255, 207, 247, 255, 87, 85, 85, 125, 85, 85, 213, 247, 223, 255, 125, 255, 247, 223, 247, 223, 255, 125, 255, 247, 223, 91, 95, 85, 65, 85, 245, 229, 127, 223, 247, 255, 223, 247, 253, 127, 223, 247, 255, 223, 247, 253, 87, 213, 87, 125, 213, 87, 213, 247, 255, 255, 125, 255, 255, 223, 247, 255, 255, 125, 255, 255, 223, 87, 85, 85, 85, 85, 85, 213 };
  boolean countFrames=true, drawDots=false;
  Point p;

  //GHOSTS
  byte ghPosX[4], ghPosY[4];  //x and y start position
  float ghGPosX[4] = {15,15,15,15}, ghGPosY[4] = {11.4,11.4,11.4,11.4};  //x and y start position
  float ghVX[4] = {1,1,1,1}, ghVY[4] = {0,0,0,0}, sleepTimer[4]={0,10,20,30}; //x and y start position
  byte ghOldPosX[4] = {20,20,20,20}, ghOldPosY[4] = {120,120,120,120};  //x and y start position
  byte ghFoundCX[4] = {15,15,15,15};
  byte ghFoundCY[4] = {10,10,10,10};
  bool ghEaten[4]={false,false,false,false};
  byte ghColors[4]={5,4,11,13};

unsigned int palette[16]={BLACK, WHITE, GRAY1, GRAY2, 0xFBEF, RED, 0x7800, 0x87F0, 0x0620, 0x0460, CYAN, BLUE, 0x000F, 0xFC80, YELLOW, BLACK};

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

byte wSprite[256];

void setSprite(byte number, byte modify = 0) {
  bool emergency=false;
  if(cSprite == number && cSpriteMod == modify)
    return;
  if(number==1) {
    number=0;
    emergency=true;
  }
  switch (number)
  {
  case 0: {
byte newSprite[256]= {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0,15,15,14,14,14,14,14,14,15,15, 0, 0, 0,
   0, 0,15,14,14,14,14,14,14,14,14,14,14,15, 0, 0,
   0,15,14,14,14,14,14,14,14,14,14,14,14,14,15, 0,
   0,15,14,14,14,14,14,14,14,14,14,14,14,14,15, 0,
   0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
   0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
   0,14,14,14,14,14,14,14,14,14,15,15,15,15,15, 0,
   0,14,14,14,14,15,15,15,15,15,15,15,15,15,15, 0,
   0,14,14,14,14,14,14,14,14,14,15,15,15,15,15, 0,
   0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
   0,15,14,14,14,14,14,14,14,14,14,14,14,14,15, 0,
   0,15,14,14,14,14,14,14,14,14,14,14,14,14,15, 0,
   0, 0,15,14,14,14,14,14,14,14,14,14,14,15, 0, 0,
   0, 0, 0,15,15,14,14,14,14,14,14,15,15, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
    if(emergency) {
      number=1;
      byte i;
      for(i=92; i<95; i++)
        newSprite[i]=15;
      for(i=106; i<111; i++)
        newSprite[i]=15;
      for(i=120; i<127; i++)
        newSprite[i]=15;
      for(i=152; i<159; i++)
        newSprite[i]=15;
      for(i=170; i<175; i++)
        newSprite[i]=15;
      for(i=188; i<191; i++)
        newSprite[i]=15;
        
    }
    copySprite; 
    break;
  }
  case 2: {
byte newSprite[256]= {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0,
   0, 0, 0, 5, 1, 1, 5, 5, 5, 5, 1, 1, 5, 0, 0, 0,
   0, 0, 5, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 5, 0, 0,
   0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
   0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0,
   0, 5, 5, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 5, 5, 0,
   0, 5, 5, 5, 1, 1, 5, 5, 5, 5, 1, 1, 5, 5, 5, 0,
   0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0,
   0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0,
   0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0,
   0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0,
   0, 0, 5, 5, 5, 0, 5, 5, 5, 0, 5, 5, 5, 0, 0, 0,
   0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
    copySprite;
    break;
  }
  }
}

byte setFlag(byte place, boolean val, byte flags) {
  if(val)
    return flags | (1<<(place-1)); 
  return ~(~flags | (1<<(place-1))); 
}

boolean  getFlag(byte place, byte flags) {
  if((1<<(place-1)) & flags)
    return true;
  return false;
}

void setTile(byte x, byte y, byte value) {
  byte ind=(y*hTiles + x)/mul;
  byte shift=((y*hTiles + x) - ind*mul);
  tiles[ind]=setFlag(shift*2 + 1, 1 & value, tiles[ind]);
  tiles[ind]=setFlag(shift*2 + 2, (1<<1) & value, tiles[ind]);
}

byte getTile(int x, int y) {
  if(y<0 || y>28)
    return 3;
  byte tile=0;
  byte ind=(y*hTiles + x)/mul;
  byte shift=((y*hTiles + x) - ind*mul);
  tile=setFlag(1, getFlag(shift*2 + 1, tiles[ind]), tile);
  return setFlag(2, getFlag(shift*2 + 2, tiles[ind]), tile);
}

void lineLeft(int amount) {
  Tft.drawVerticalLine(dPosY,dPosX,amount,palette[aColor]);
  dPosX+=amount;
}

void lineRight(int amount) {
  dPosX-=amount;
  Tft.drawVerticalLine(dPosY,dPosX,amount,palette[aColor]);
}

void lineUp(int amount) {
  dPosY-=amount;
  Tft.drawHorizontalLine(dPosY,dPosX,amount,palette[aColor]);
}

void lineDown(int amount) {
  Tft.drawHorizontalLine(dPosY,dPosX,amount,palette[aColor]);
  dPosY+=amount;
}

void roundedRD(int radius, boolean reverse=false) {
  if(reverse) {
    dPosY-=radius;
    dPosX-=radius;
  }
  drawQCircle(dPosY+radius,dPosX,radius,palette[aColor],false,true,false,false);
  if(!reverse) {
    dPosY+=radius;
    dPosX+=radius;
  }
}

void roundedUR(int radius, boolean reverse=false) {
  if(reverse) {
    dPosY-=radius;
    dPosX+=radius;
  }
  drawQCircle(dPosY,dPosX-radius,radius,palette[aColor],true,false,false,false);
  if(!reverse) {
    dPosY+=radius;
    dPosX-=radius;
  }
}

void roundedDL(int radius, boolean reverse=false) {
  if(reverse) {
    dPosY+=radius;
    dPosX-=radius;
  }
  drawQCircle(dPosY,dPosX+radius,radius,palette[aColor],false,false,true,false);
  if(!reverse) {
    dPosY-=radius;
    dPosX+=radius;
  }
}

void roundedLU(int radius, boolean reverse=false) {
  if(reverse) {
    dPosY+=radius;
    dPosX+=radius;
  }
  drawQCircle(dPosY-radius,dPosX,radius,palette[aColor],false,false,false,true);
  if(!reverse) {
    dPosY-=radius;
    dPosX-=radius;
  }
}

void drawQCircle(int poX, int poY, int r, unsigned int color, boolean q1, boolean q2, boolean q3, boolean q4) {
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        if(q1) Tft.setPixel(poX-x, poY+y,color);
        if(q2) Tft.setPixel(poX+x, poY+y,color);
        if(q3) Tft.setPixel(poX+x, poY-y,color);
        if(q4) Tft.setPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

void findCross(byte x, byte y, float velX, float velY, byte* foundX, byte* foundY) {
  int sX=((velX>0)-(velX<0)), sY=((velY>0)-(velY<0));
  while(true) {
    x+=sX; y+=sY;
    if(getTile(x+sY,y+sX)<3 || getTile(x-sY,y-sX)<3) {
      *foundX=x; *foundY=y; break;
    }
  }
}

bool crossReached(float x, float y, byte cX, byte cY, float velX, float velY) {
  int sX=((velX>0)-(velX<0)), sY=((velY>0)-(velY<0));
  if(sX>0 && cX+0.4<=x) {
    return true;
  }
  else if(sX<0 && cX+0.4>=x) {
    return true;
  }
  else if(sY>0 && cY+0.4<=y) {
    return true;
  }
  else if(sY<0 && cY+0.4>=y) {
    return true;
  }
  else
    return false;
}

void drawCircles(byte x, byte y, byte type) {
  for(int k=-1; k<=1; k++)
    for(int l=-1; l<=1; l++)
      if(!(k==0 && l==0))
        if(getTile(x+k, y+l)==type)
          Tft.fillCircle(offsety + (y+l)*8+4, offsetx + (x+k)*8+4, type, WHITE);
}

void chooseRandom(byte cX, byte cY, float* velX, float* velY) {
  byte rDir=random(4);
  //byte rDir=0;
  while(true) {
    if(rDir==0 && *velX>=0 && getTile(cX+1,cY)<3)
      { *velX=1; *velY=0; break; }
    else if(rDir==1 && *velY<=0 && getTile(cX,cY-1)<3)
      { *velX=0; *velY=-1; break; }
    else if(rDir==2 && *velX<=0 && getTile(cX-1,cY)<3)
      { *velX=-1; *velY=0; break; }
    else if(rDir==3 && *velY>=0 && getTile(cX,cY+1)<3)
      { *velX=0; *velY=1; break; }
    else
      rDir=(rDir+1)%4;
      //rDir+=1;
  }
}

void chooseToPlayer(byte cX, byte cY, float* velX, float* velY, bool against) {
  float vectorX=gPosX-1-cX, vectorY=gPosY-1-cY;
  if(against) {
    vectorX=3*cX-gPosX-1;
    vectorY=3*cY-gPosY-1;
  }
  int sVX=((vectorX>0)-(vectorX<0)), sVY=((vectorY>0)-(vectorY<0));
  if(sVX==0) sVX=1;
  if(sVY==0) sVY=1;
  if(abs(vectorX)>abs(vectorY)) {
    if(getTile(cX+sVX,cY)<3 && (*velX!=(sVX*(-1)) || against)) {
      *velX=sVX; *velY=0;
    }
    else if(getTile(cX,cY+sVY)<3 && (*velY!=(sVY*(-1)) || against)) {
      *velY=sVY; *velX=0;
    }
    else if(getTile(cX,cY-sVY)<3 && (*velY!=sVY || against)) {
      *velY=-sVY; *velX=0;
    }
    else {
      *velX=-sVX; *velY=0;
    }
  }
  else {
    if(getTile(cX,cY+sVY)<3 && (*velY!=(sVY*(-1)) || against)) {
      *velY=sVY; *velX=0;
    }
    else if(getTile(cX+sVX,cY)<3 && (*velX!=(sVX*(-1)) || against)) {
      *velX=sVX; *velY=0;
    }
    else if(getTile(cX-sVX,cY)<3 && (*velX!=sVX || against)) {
      *velX=-sVX; *velY=0;
    }
    else {
      *velY=-sVY; *velX=0;
    }
  }
}

void drawLab() {
  // U and D Border
  Tft.drawHorizontalLine(0,99,4,palette[aColor]);
  Tft.drawHorizontalLine(236,99,4,palette[aColor]);
  Tft.drawHorizontalLine(0,204,4,palette[aColor]);
  Tft.drawHorizontalLine(0,211,4,palette[aColor]);
  Tft.drawHorizontalLine(0,316,4,palette[aColor]);
  Tft.drawHorizontalLine(236,316,4,palette[aColor]);
  Tft.drawVerticalLine(239,99,217,palette[aColor]);
  Tft.drawVerticalLine(0,99,105,palette[aColor]);
  Tft.drawVerticalLine(0,211,105,palette[aColor]);
  //gate
  Tft.drawVerticalLine(96,201,15,palette[10]);
  // RD border
  dPosX=96; dPosY=120;
  lineLeft(40); roundedRD(3); lineDown(25); roundedUR(3); lineRight(34); roundedDL(3,true); lineDown(35); roundedLU(3,true); lineLeft(10); roundedRD(3); lineDown(1); roundedUR(3); lineRight(10); roundedDL(3,true); lineDown(34);
  // LD border
  dPosX=320; dPosY=120;
  lineRight(41); roundedDL(3,true); lineDown(25); roundedLU(3,true); lineLeft(34); roundedRD(3); lineDown(35); roundedUR(3); lineRight(10); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(10); roundedRD(3); lineDown(34);
  // RU border
  dPosX=96; dPosY=103;
  lineLeft(40); roundedUR(3,true); lineUp(25); roundedRD(3,true); lineRight(34); roundedLU(3); lineUp(65);
  // LU border
  dPosX=320; dPosY=103;
  lineRight(41); roundedLU(3); lineUp(25); roundedDL(3); lineLeft(34); roundedUR(3,true); lineUp(65);
  // UM border
  dPosX=211; dPosY=4;
  lineDown(24); roundedUR(3); lineRight(1); roundedLU(3); lineUp(24);
  // Rectangles
  dPosX=299; dPosY=29; lineUp(10); roundedRD(3,true); lineRight(17); roundedDL(3,true); lineDown(9); roundedLU(3,true); lineLeft(17); roundedUR(3,true);
  dPosX=299; dPosY=53; lineUp(2); roundedRD(3,true); lineRight(17); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(17); roundedUR(3,true);
  dPosX=139; dPosY=29; lineUp(10); roundedRD(3,true); lineRight(17); roundedDL(3,true); lineDown(9); roundedLU(3,true); lineLeft(17); roundedUR(3,true);
  dPosX=139; dPosY=53; lineUp(2); roundedRD(3,true); lineRight(17); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(17); roundedUR(3,true);
  dPosX=259; dPosY=29; lineUp(10); roundedRD(3,true); lineRight(25); roundedDL(3,true); lineDown(9); roundedLU(3,true); lineLeft(25); roundedUR(3,true);
  dPosX=187; dPosY=29; lineUp(10); roundedRD(3,true); lineRight(25); roundedDL(3,true); lineDown(9); roundedLU(3,true); lineLeft(25); roundedUR(3,true);
  dPosX=259; dPosY=149; lineUp(26); roundedRD(3,true); lineRight(1); roundedDL(3,true); lineDown(25); roundedLU(3,true); lineLeft(1); roundedUR(3,true);
  dPosX=163; dPosY=149; lineUp(26); roundedRD(3,true); lineRight(1); roundedDL(3,true); lineDown(25); roundedLU(3,true); lineLeft(1); roundedUR(3,true);
  dPosX=259; dPosY=173; lineUp(2); roundedRD(3,true); lineRight(25); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(25); roundedUR(3,true);
  dPosX=187; dPosY=173; lineUp(2); roundedRD(3,true); lineRight(25); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(25); roundedUR(3,true);
  dPosX=235; dPosY=125; lineUp(26); roundedRD(3,true); lineRight(16); dPosX-=16; lineRight(17); roundedDL(3,true); lineDown(25); roundedLU(3,true); lineLeft(49); roundedUR(3,true);
  // T's
  dPosX=235; dPosY=53; lineUp(2); roundedRD(3,true); lineRight(49); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(18); roundedRD(3); lineDown(18); roundedLU(3,true); lineLeft(1); roundedUR(3,true); lineUp(18); roundedDL(3); lineLeft(18); roundedUR(3,true);
  dPosX=235; dPosY=149; lineUp(2); roundedRD(3,true); lineRight(49); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(18); roundedRD(3); lineDown(18); roundedLU(3,true); lineLeft(1); roundedUR(3,true); lineUp(18); roundedDL(3); lineLeft(18); roundedUR(3,true);
  dPosX=235; dPosY=197; lineUp(2); roundedRD(3,true); lineRight(49); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(18); roundedRD(3); lineDown(18); roundedLU(3,true); lineLeft(1); roundedUR(3,true); lineUp(18); roundedDL(3); lineLeft(18); roundedUR(3,true);
  // L's
  dPosX=139; dPosY=197; lineUp(26); roundedRD(3,true); lineRight(17); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(10); roundedRD(3); lineDown(18); roundedLU(3,true); lineLeft(1); roundedUR(3,true);
  dPosX=276; dPosY=197; lineUp(26); roundedDL(3); lineLeft(17); roundedRD(3); lineDown(1); roundedUR(3); lineRight(10); roundedDL(3,true); lineDown(18); roundedUR(3); lineRight(1); roundedLU(3);
  // Reversed T's
  dPosX=228; dPosY=218; lineDown(2); roundedLU(3,true); lineLeft(65); roundedUR(3,true); lineUp(1); roundedRD(3,true); lineRight(34); roundedLU(3); lineUp(18); roundedRD(3,true); lineRight(1); roundedDL(3,true); lineDown(18); roundedUR(3); lineRight(18); roundedDL(3,true);
  dPosX=116; dPosY=218; lineDown(2); roundedLU(3,true); lineLeft(65); roundedUR(3,true); lineUp(1); roundedRD(3,true); lineRight(18); roundedLU(3); lineUp(18); roundedRD(3,true); lineRight(1); roundedDL(3,true); lineDown(18); roundedUR(3); lineRight(34); roundedDL(3,true);
  // sideways T's
  dPosX=254; dPosY=103; lineLeft(2); roundedUR(3,true); lineUp(49); roundedRD(3,true); lineRight(1); roundedDL(3,true); lineDown(18); roundedUR(3); lineRight(18); roundedDL(3,true); lineDown(1); roundedLU(3,true); lineLeft(18); roundedRD(3); lineDown(18); roundedLU(3,true);
  dPosX=161; dPosY=103; lineRight(2); roundedLU(3); lineUp(49); roundedDL(3); lineLeft(1); roundedRD(3); lineDown(18); roundedLU(3,true); lineLeft(18); roundedRD(3); lineDown(1); roundedUR(3); lineRight(18); roundedDL(3,true); lineDown(18); roundedUR(3);
  
}

void drawBigJoy() {
  Tft.drawVerticalLine(jY-10,jX+10,12,palette[aColor]);
  Tft.drawVerticalLine(jY+10,jX+10,12,palette[aColor]);
  Tft.drawHorizontalLine(jY-10,jX+22,20,palette[aColor]);
  Tft.drawVerticalLine(jY-10,jX-22,12,palette[aColor]);
  Tft.drawVerticalLine(jY+10,jX-22,12,palette[aColor]);
  Tft.drawHorizontalLine(jY-10,jX-22,20,palette[aColor]);
  Tft.drawHorizontalLine(jY-22,jX-10,12,palette[aColor]);
  Tft.drawHorizontalLine(jY-22,jX+10,12,palette[aColor]);
  Tft.drawVerticalLine(jY-22,jX-10,20,palette[aColor]);
  Tft.drawHorizontalLine(jY+10,jX-10,12,palette[aColor]);
  Tft.drawHorizontalLine(jY+10,jX+10,12,palette[aColor]);
  Tft.drawVerticalLine(jY+22,jX-10,20,palette[aColor]);
#if defined(ALTCONTROLS)
  Tft.drawHorizontalLine(78,300,20,palette[aColor]);
  Tft.drawLine(78,300,88,290,palette[aColor]);
  //Tft.drawLine(78,300,88,310,palette[aColor]);
  Tft.drawHorizontalLine(126,300,20,palette[aColor]);
  //Tft.drawLine(146,300,136,290,palette[aColor]);
  Tft.drawLine(146,300,136,310,palette[aColor]);
#endif
}

void changeScore(int add) {
  score+=add;
  Tft.fillScreen(0,15,0,95,BLACK); 
  Tft.drawNumber(score,90,0,1,WHITE);
}

void changeLives(int add) {
  lives+=add;
  Tft.fillScreen(20,35,0,95,BLACK); 
  Tft.drawString("LIVES:",90,20,1,WHITE);
  Tft.drawNumber(lives,40,20,1,WHITE);
}

void changeLevel(int add) {
  level+=add;
  Tft.fillScreen(40,55,0,95,BLACK); 
  Tft.drawString("LEVEL:",90,40,1,WHITE);
  Tft.drawNumber(level,40,40,1,WHITE);
}

void drawHiScore() {
  EEPROM.get(3,hiscore);
  Tft.fillScreen(60,95,0,95,BLACK); 
  Tft.drawString("HI-SCORE:",90,60,1,WHITE);
  Tft.drawChar(EEPROM.read(0),35,60,1,WHITE);
  Tft.drawChar(EEPROM.read(1),25,60,1,WHITE);
  Tft.drawChar(EEPROM.read(2),15,60,1,WHITE);
  Tft.drawNumber(hiscore,90,80,1,WHITE);
}

void drawPause(bool state) {
  Tft.fillScreen(225,239,0,15,BLACK);
  if(state) {
    Tft.drawTriangle(225,15,239,15,232,1,WHITE);
  }
  else {
    Tft.fillScreen(225,239,0,5,WHITE);
    Tft.fillScreen(225,239,10,15,WHITE);
  }
}

void restartMap() {
  tiles[0]=87; tiles[1]=85; tiles[2]=85; tiles[3]=125; tiles[4]=85; tiles[5]=85; tiles[6]=213; tiles[7]=247; tiles[8]=223; tiles[9]=255; tiles[10]=125; tiles[11]=255; tiles[12]=247; tiles[13]=223; tiles[14]=251; tiles[15]=223; tiles[16]=255; tiles[17]=125; tiles[18]=255; tiles[19]=247; tiles[20]=239; tiles[21]=247; tiles[22]=223; tiles[23]=255; tiles[24]=125; tiles[25]=255; tiles[26]=247; tiles[27]=223; tiles[28]=87; tiles[29]=85; tiles[30]=85; tiles[31]=85; tiles[32]=85; tiles[33]=85; tiles[34]=213; tiles[35]=247; tiles[36]=223; tiles[37]=247; tiles[38]=255; tiles[39]=223; tiles[40]=247; tiles[41]=223; tiles[42]=247; tiles[43]=223; tiles[44]=247; tiles[45]=255; tiles[46]=223; tiles[47]=247; tiles[48]=223; tiles[49]=87; tiles[50]=213; tiles[51]=87; tiles[52]=125; tiles[53]=213; tiles[54]=87; tiles[55]=213; tiles[56]=255; tiles[57]=223; tiles[58]=255; tiles[59]=125; tiles[60]=255; tiles[61]=247; tiles[62]=255; tiles[63]=255; tiles[64]=223; tiles[65]=255; tiles[66]=125; tiles[67]=255; tiles[68]=247; tiles[69]=255; tiles[70]=255; tiles[71]=223; tiles[72]=3; tiles[73]=0; tiles[74]=192; tiles[75]=247; tiles[76]=255; tiles[77]=255; tiles[78]=223; tiles[79]=243; tiles[80]=255; tiles[81]=207; tiles[82]=247; tiles[83]=255; tiles[84]=252; tiles[85]=223; tiles[86]=51; tiles[87]=0; tiles[88]=204; tiles[89]=247; tiles[90]=63; tiles[91]=0; tiles[92]=16; tiles[93]=48; tiles[94]=0; tiles[95]=12; tiles[96]=4; tiles[97]=0; tiles[98]=252; tiles[99]=223; tiles[100]=51; tiles[101]=0; tiles[102]=204; tiles[103]=247; tiles[104]=63; tiles[105]=255; tiles[106]=223; tiles[107]=243; tiles[108]=255; tiles[109]=207; tiles[110]=247; tiles[111]=255; tiles[112]=255; tiles[113]=223; tiles[114]=3; tiles[115]=0; tiles[116]=192; tiles[117]=247; tiles[118]=255; tiles[119]=255; tiles[120]=223; tiles[121]=243; tiles[122]=255; tiles[123]=207; tiles[124]=247; tiles[125]=255; tiles[126]=255; tiles[127]=223; tiles[128]=243; tiles[129]=255; tiles[130]=207; tiles[131]=247; tiles[132]=255; tiles[133]=87; tiles[134]=85; tiles[135]=85; tiles[136]=125; tiles[137]=85; tiles[138]=85; tiles[139]=213; tiles[140]=247; tiles[141]=223; tiles[142]=255; tiles[143]=125; tiles[144]=255; tiles[145]=247; tiles[146]=223; tiles[147]=247; tiles[148]=223; tiles[149]=255; tiles[150]=125; tiles[151]=255; tiles[152]=247; tiles[153]=223; tiles[154]=91; tiles[155]=95; tiles[156]=85; tiles[157]=65; tiles[158]=85; tiles[159]=245; tiles[160]=229; tiles[161]=127; tiles[162]=223; tiles[163]=247; tiles[164]=255; tiles[165]=223; tiles[166]=247; tiles[167]=253; tiles[168]=127; tiles[169]=223; tiles[170]=247; tiles[171]=255; tiles[172]=223; tiles[173]=247; tiles[174]=253; tiles[175]=87; tiles[176]=213; tiles[177]=87; tiles[178]=125; tiles[179]=213; tiles[180]=87; tiles[181]=213; tiles[182]=247; tiles[183]=255; tiles[184]=255; tiles[185]=125; tiles[186]=255; tiles[187]=255; tiles[188]=223; tiles[189]=247; tiles[190]=255; tiles[191]=255; tiles[192]=125; tiles[193]=255; tiles[194]=255; tiles[195]=223; tiles[196]=87; tiles[197]=85; tiles[198]=85; tiles[199]=85; tiles[200]=85; tiles[201]=85; tiles[202]=213;
}

void setup()
{
  TFT_BL_ON;                                          // turn on the background light
  Tft.TFTinit();    // init TFT library

  offsetx=320-w;
  offsety=(240-h)/2;

  Tft.drawString("PA  -INO",274,80,5,YELLOW);
  Tft.fillCircle(98,188,18,YELLOW);
  setSprite(0);
  for(int l=0; l<7; l++)
    Tft.drawLine(98,188,95+l,168,BLACK);
  Tft.drawString("TAP THE SCREEN",245,150,2,RED);
  Tft.drawString("RESET SCORE",319,230,1,RED);
  do {
    p=ts.getPoint();
  } while(p.z < 4*__PRESURE);
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
  if((p.y > 240) && (p.x>225)) {
    hiscore=0;
    EEPROM.put(3,hiscore);
    EEPROM.write(0,'P');
    EEPROM.write(1,'A');
    EEPROM.write(2,'C');
  }
  restartMap();
  Tft.fillScreen();
  randomSeed(p.x+p.y+p.z);
  for(int i=0; i<hTiles; i++)
    for(int j=0; j<vTiles; j++)
      if(getTile(i, j)==1)
        Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 1, WHITE);
      else if(getTile(i, j)==2)
        Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 2, WHITE);
  //HiScore reset
  /*
  hiscore=0;
  EEPROM.put(3,hiscore);
  EEPROM.write(0,'P');
  EEPROM.write(1,'A');
  EEPROM.write(2,'C');
  */
  
  aColor=12;
  drawBigJoy();
  drawLab();

  posX=(gPosX-1)*tileSize;
  posY=(gPosY-1)*tileSize;
  oldPosX=posX;
  oldPosY=posY;
  for(byte g=0; g<4; g++) {
    ghPosX[g]=(ghGPosX[g]-1)*tileSize;
    ghPosY[g]=(ghGPosY[g]-1)*tileSize;
    ghOldPosX[g]=ghPosX[g];
    ghOldPosY[g]=ghPosY[g];
  } 
  changeScore(0);
  changeLives(0);
  changeLevel(0);
  drawHiScore();
  drawPause(false);
  countTime=actTime=millis();
}

void loop() {
    oldTime=actTime;
    actTime=millis();
    delTime=(float)(actTime-oldTime)/1000;
    p = ts.getPoint();

    //map the ADC value read to into pixel co-ordinates

    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
      if (p.z > __PRESURE) {
        if(abs(p.y-7)<7 && abs(p.x-232)<7) {
          paused=true;
          delay(500);
          drawPause(true);
          p.y=jX;
          p.x=jY;
        }
        #if defined(ALTCONTROLS)
        if(p.y>240)
          p.y=jX;
        #endif
        int moveX=p.y-jX, moveY=p.x-jY;
        if(abs(moveX)>abs(moveY) && abs(moveX)>=minMove) {
          if(moveX>0) {
            Tft.drawVerticalLine(jY,jX,20,RED);
            if(getTile(int(gPosX+1-1), int(gPosY-1) )<3) {
              vX=8;
              vY=0;
              if(!hMove) {
                hMove=true;
                gPosY=floor(gPosY)+0.4;
              }
              findCross(gPosX-1,gPosY-1,vX,vY,&foundCX,&foundCY);
            }
          }
          else {
            Tft.drawVerticalLine(jY,jX-20,20,RED);
            if(getTile(int(gPosX-1-1), int(gPosY-1))<3) {
              vX=-8;
              vY=0;
              if(!hMove) {
                hMove=true;
                gPosY=floor(gPosY)+0.4;
              }
              findCross(gPosX-1,gPosY-1,vX,vY,&foundCX,&foundCY);
            }
          }
        }
        else if(abs(moveY)>=minMove) {
          if(moveY>0) {
            Tft.drawHorizontalLine(jY,jX,20,RED);
            if(getTile(int(gPosX-1), int(gPosY+1-1))<3) {
              vY=8;
              vX=0;
              if(hMove) {
                hMove=false;
                gPosX=floor(gPosX)+0.4;
              }
              findCross(gPosX-1,gPosY-1,vX,vY,&foundCX,&foundCY);
            }
          }
          else {
            Tft.drawHorizontalLine(jY-20,jX,20,RED);
            if(getTile(int(gPosX-1), int(gPosY-1-1))<3 && gPosY>=1.9) {
              vY=-8;
              vX=0;
              if(hMove) {
                hMove=false;
                gPosX=floor(gPosX)+0.4;
              }
              findCross(gPosX-1,gPosY-1,vX,vY,&foundCX,&foundCY);
            }
          }
        }
      }
      else {
        Tft.drawVerticalLine(jY,jX-20,41,BLACK);
        Tft.drawHorizontalLine(jY-20,jX,41,BLACK);
        Tft.drawVerticalLine(jY,jX-4,9,0x000F);
        Tft.drawHorizontalLine(jY-4,jX,9,0x000F);   
      }
      while(paused) {
        p = ts.getPoint();
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
        p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
        if(abs(p.y-7)<7 && abs(p.x-232)<7) {
          paused=false;
          delay(500);
          drawPause(false);
          p.y=jX;
          p.x=jY;
          actTime=millis();
          oldTime=actTime;
          delTime=0;
        }
      }
      if(abs(vX+vY)>0) {
        gPosX+=vX*delTime;
        gPosY+=vY*delTime;
        if(gPosY<1.5)
          vY=0;
        if(crossReached(gPosX-1, gPosY-1,foundCX,foundCY,vX,vY)) {
          if(getTile(int(gPosX-1+((vX>0)-(vX<0))), int(gPosY-1+((vY>0)-(vY<0))))<3) {
            findCross(gPosX-1,gPosY-1,vX,vY,&foundCX,&foundCY);
          }
          else {
            gPosX=foundCX+1.4;
            gPosY=foundCY+1.4;
            vX=0; vY=0;
          }
        }
      }
      oldOr=orientation;
      oldSprite=spriteNum;
      if(vX>0) {
        orientation=0;
      }
      else if(vX<0) {
        orientation=2;
      }
      else if(vY>0) {
        orientation=3;
      }
      else if(vY<0) {
        orientation=1;
      }
      if(int(gPosX-1)==0 && int(gPosY-1)==13) { gPosX=26.4; gPosY=14.4; foundCX=21; foundCY=13; }
      if(int(gPosX-1)==27 && int(gPosY-1)==13) { gPosX=2.4; gPosY=14.4; foundCX=6; foundCY=13; }
      if(animTimeout>0.1) {
        spriteNum=!spriteNum;
        animTimeout=0;
      }
      else if(abs(vX+vY)>0)
        animTimeout+=delTime;
      gPosY=constrain(gPosY,1.5,29.4);
      if(getTile(int(gPosX-1), int(gPosY-1))==1) {
        setTile(int(gPosX-1), int(gPosY-1),0);
        dotCounter++;
        changeScore(10);
      }
      if(getTile(int(gPosX-1), int(gPosY-1))==2) {
        setTile(int(gPosX-1), int(gPosY-1),0);
        dotCounter++;
        for(byte g=0; g<4; g++) {
          ghEaten[g]=false;
        }
        frightTimer=4;
        changeScore(50);
      }
      if(dotCounter>=248) {
        changeScore(1000);
        sleepTimer[0]=0; sleepTimer[1]=10; sleepTimer[2]=20; sleepTimer[3]=30;
        gPosX = 15; gPosY = 23.4;
        vX=0; vY=0; changeLevel(1);
        frightTimer=0;
        dotCounter=0; drawDots=true;
        aColor=10; drawLab(); delay(250);
        aColor=12; drawLab(); delay(250);
        aColor=10; drawLab(); delay(250);
        aColor=12; drawLab(); delay(250);
        restartMap();
        for(int i=0; i<hTiles; i++)
          for(int j=0; j<vTiles; j++)
            if(getTile(i, j)==1)
              Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 1, WHITE);
            else if(getTile(i, j)==2)
              Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 2, WHITE);
  
      }
      for(byte g=0; g<4; g++) {
        if(abs(ghGPosX[g]-gPosX)<1 && abs(ghGPosY[g]-gPosY)<1) {
          if(frightTimer>0 && !ghEaten[g]) {
            delay(500); actTime=millis();
            sleepTimer[g]=(g>0)*3;
            ghEaten[g]=true;
            changeScore(100);
          }
          else {
            Tft.fillRectangle(offsety + (gPosY-2)*tileSize + 1, offsetx + (gPosX-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            Tft.fillRectangle(offsety + posY - 7, offsetx + posX - 7, sprSize-2, sprSize-1, BLACK);
            Tft.fillRectangle(offsety + (ghGPosY[g]-2)*tileSize + 1, offsetx + (ghGPosX[g]-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            Tft.fillCircle(offsety + (gPosY-1)*tileSize, offsetx + (gPosX-1)*tileSize, 6, YELLOW);
            delay(500);
            Tft.fillRectangle(offsety + (gPosY-2)*tileSize + 1, offsetx + (gPosX-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            Tft.fillCircle(offsety + (gPosY-1)*tileSize, offsetx + (gPosX-1)*tileSize, 4, YELLOW);
            delay(500);
            Tft.fillRectangle(offsety + (gPosY-2)*tileSize + 1, offsetx + (gPosX-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            Tft.fillCircle(offsety + (gPosY-1)*tileSize, offsetx + (gPosX-1)*tileSize, 2, YELLOW);
            delay(500);
            Tft.fillRectangle(offsety + (gPosY-2)*tileSize + 1, offsetx + (gPosX-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            Tft.drawVerticalLine(offsety + (gPosY-1)*tileSize, offsetx + (gPosX-1)*tileSize - 6, 3, YELLOW);
            Tft.drawVerticalLine(offsety + (gPosY-1)*tileSize, offsetx + (gPosX-1)*tileSize + 3, 3, YELLOW);
            Tft.drawHorizontalLine(offsety + (gPosY-1)*tileSize - 6, offsetx + (gPosX-1)*tileSize, 3, YELLOW);
            Tft.drawHorizontalLine(offsety + (gPosY-1)*tileSize + 3, offsetx + (gPosX-1)*tileSize, 3, YELLOW);
            delay(500);
            Tft.fillRectangle(offsety + (gPosY-2)*tileSize + 1, offsetx + (gPosX-2)*tileSize + 1, sprSize-2, sprSize-1, BLACK);
            changeLives(-1);
            if(lives==0) {
              Tft.drawString("GAME OVER",240,100,3,RED);
              Tft.drawString("TAP THE SCREEN",245,150,2,RED);
              do {
                p=ts.getPoint();
              } while(p.z < 4*__PRESURE);
              Tft.fillScreen();
              if(score>hiscore) {
                int curPos=0;
                bool pressed=false;
                char initials[3];
                initials[0]=' '; initials[1]=' '; initials[2]=' '; 
                Tft.drawString("HI SCORE!",240,0,3,RED);
                Tft.drawString("ENTER INITIALS",240,80,2,RED);
                Tft.drawString("A B C D E F G H I J",290,120,2,YELLOW);
                Tft.drawString("K L M N O P Q R S <-",290,160,2,YELLOW);
                Tft.drawString("T U V W X Y Z   Enter",290,200,2,YELLOW);
                do {
                  p=ts.getPoint();
                  if(p.z > __PRESURE && !pressed) {
                    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
                    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
                    Tft.fillScreen(30,60,30,210,BLACK); 
                    for(byte c=0; c<10; c++)
                      if(abs(p.y-5-(290-c*28)) < 15 && abs(p.x-8-120) < 20){
                        initials[curPos]=65+c;
                        curPos++;
                        pressed=true;
                      }
                    for(byte c=0; c<9; c++)
                      if(abs(p.y-5-(290-c*28)) < 15 && abs(p.x-8-160) < 20){
                        initials[curPos]=75+c;
                        curPos++;
                        pressed=true;
                      }
                    for(byte c=0; c<7; c++)
                      if(abs(p.y-5-(290-c*28)) < 15 && abs(p.x-8-200) < 20){
                        initials[curPos]=84+c;
                        curPos++;
                        pressed=true;
                      }
                    if(abs(p.y-10-38) < 20 && abs(p.x-8-160) < 20){
                      initials[curPos]=' ';
                      curPos--;
                      //initials[curPos]=' ';
                      pressed=true;
                    }
                    curPos=constrain(curPos,0,2);
                    if(abs(p.y-15-66) < 30 && abs(p.x-8-200) < 20){
                      curPos=11;
                      pressed=true;
                      EEPROM.put(3,score);
                      EEPROM.write(0,initials[0]);
                      EEPROM.write(1,initials[1]);
                      EEPROM.write(2,initials[2]);
                    }
                  }
                  if(pressed) {
                    delay(500);
                    pressed=false;
                  }
                  for (byte c=0; c<3; c++) {
                    Tft.drawChar('_',210-c*36,40,3,(c==curPos)?CYAN:RED);
                    Tft.drawChar(initials[c],210-c*36,30,3,RED);
                  }
                } while(curPos < 10);
              }
              Tft.fillScreen();
              aColor=12;
              drawBigJoy();
              score=0; changeScore(0);
              level=1; changeLevel(0);
              lives=3; changeLives(0);
              drawHiScore(); drawPause(false);
              sleepTimer[0]=0; sleepTimer[1]=10; sleepTimer[2]=20; sleepTimer[3]=30;
              gPosX = 15; gPosY = 23.4;
              vX=0; vY=0;
              frightTimer=0;
              dotCounter=0; drawDots=true;
              restartMap();
              for(int i=0; i<hTiles; i++)
                for(int j=0; j<vTiles; j++)
                  if(getTile(i, j)==1)
                    Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 1, WHITE);
                  else if(getTile(i, j)==2)
                    Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 2, WHITE);
  
            }
            frightTimer=0;
            drawLab();
            drawDots=true;
            sleepTimer[0]=0; sleepTimer[1]=10; sleepTimer[2]=20; sleepTimer[3]=30; 
            gPosX = 15, gPosY = 23.4;
            vX=0; vY=0;
            actTime=millis();
            break;
          }
        }
      }
      if(frightTimer>0)
        frightTimer-=delTime;
      oldPosX=posX;
      oldPosY=posY;
      posX=(gPosX-1)*tileSize;
      posY=(gPosY-1)*tileSize;
      
    setSprite(oldSprite,oldOr);
    for(int spriteX = 0; spriteX <  sprSize; spriteX++)
      for(int spriteY = 0; spriteY <  sprSize; spriteY++) {
        if(wSprite[spriteY * sprSize + spriteX] == 0)
            continue;
        int tempSpriteX=spriteX-(posX-oldPosX);
        int tempSpriteY=spriteY-(posY-oldPosY);
        if(tempSpriteX >= 0 && tempSpriteY >=0)
          if(tempSpriteX < sprSize && tempSpriteY < sprSize)
            if(wSprite[tempSpriteY * sprSize + tempSpriteX] != 0)
              continue;
        Tft.setPixel(offsety + spriteY + 1 + oldPosY - 8, offsetx + spriteX + 1 + oldPosX - 8, palette[0]);
      }
    setSprite(spriteNum,orientation);
    for(int spriteX = 0; spriteX <  sprSize; spriteX++)
      for(int spriteY = 0; spriteY <  sprSize; spriteY++) {
        if(!makingLine) {
          makingLine=true;
          lineColor=wSprite[spriteY * sprSize + spriteX];
          lineStart=spriteY;
        } 
        else if(lineColor != wSprite[spriteY * sprSize + spriteX] || spriteY == sprSize - 1)
          makingLine=false;
        if(!makingLine) {
          if(lineColor!=0)
            Tft.drawHorizontalLine(offsety + lineStart + 1 + posY - 8, offsetx + spriteX + 1 + posX - 8, constrain(spriteY - lineStart, 1, sprSize), palette[lineColor]);
          if(spriteY != sprSize - 1) {
            makingLine=true;
            lineColor=wSprite[spriteY * sprSize + spriteX];
            lineStart=spriteY;
          }
        }
      }
    drawCircles(gPosX-1, gPosY-1, 1);
    drawCircles(gPosX-1, gPosY-1, 2);

    //GHOSTS
    setSprite(2);
    for(byte g=0; g<4; g++) {
      if(sleepTimer[g]>0) {
        sleepTimer[g]-=delTime;
        ghGPosX[g] = 19-2*g; ghGPosY[g] = 14.9 - constrain(1-sleepTimer[g],0,255);
        
      }
      else if(sleepTimer[g]>-10) {
        byte bDir=random(2);
        ghGPosX[g] = 15; ghGPosY[g] = 11.4;
        ghVX[g] = 1*pow((-1),bDir); ghVY[g] = 0;  //x and y start position
        ghFoundCX[g] = 13.5 + 1.5*pow((-1),bDir); ghFoundCY[g] = 10;
        sleepTimer[g]=-15;
      }
      else if(abs(ghVX[g]+ghVY[g])>0) {
        float velMul=1;
        if(frightTimer>0 && !ghEaten[g]) 
          velMul=0.5;
        ghGPosX[g]+=4*(1 + 0.03*level)*ghVX[g]*delTime*velMul;
        ghGPosY[g]+=4*(1 + 0.03*level)*ghVY[g]*delTime*velMul;
        if(crossReached(ghGPosX[g]-1,ghGPosY[g]-1,ghFoundCX[g],ghFoundCY[g],ghVX[g],ghVY[g]) || (ghGPosY[g]<1.5 && ghVY[g]<0)) {
          ghGPosX[g]=ghFoundCX[g]+1.4;
          ghGPosY[g]=ghFoundCY[g]+1.4;
          if(random(10)<(1+g*3)) 
            chooseRandom(ghGPosX[g]-1,ghGPosY[g]-1,&ghVX[g],&ghVY[g]);
          else if(frightTimer>0 && !ghEaten[g]) 
            chooseToPlayer(ghGPosX[g]-1,ghGPosY[g]-1,&ghVX[g],&ghVY[g], true);
          else 
            chooseToPlayer(ghGPosX[g]-1,ghGPosY[g]-1,&ghVX[g],&ghVY[g], false);
          findCross(ghGPosX[g]-1,ghGPosY[g]-1,ghVX[g],ghVY[g],&ghFoundCX[g],&ghFoundCY[g]);
        }
      }
      if(int(ghGPosX[g]-1)==0 && int(ghGPosY[g]-1)==13) { ghGPosX[g]=26.4; ghGPosY[g]=14.4; ghFoundCX[g]=21; ghFoundCY[g]=13; }
      if(int(ghGPosX[g]-1)==27 && int(ghGPosY[g]-1)==13) { ghGPosX[g]=2.4; ghGPosY[g]=14.4; ghFoundCX[g]=6; ghFoundCY[g]=13; }
      
      ghOldPosX[g]=ghPosX[g];
      ghOldPosY[g]=ghPosY[g];
      ghPosX[g]=(ghGPosX[g]-1)*tileSize;
      ghPosY[g]=(ghGPosY[g]-1)*tileSize;
  
    
    for(int spriteX = 0; spriteX <  sprSize; spriteX++)
      for(int spriteY = 1; spriteY <  sprSize; spriteY++) {
        if(wSprite[spriteY * sprSize + spriteX] == 0)
            continue;
        int tempSpriteX=spriteX-(ghPosX[g]-ghOldPosX[g]);
        int tempSpriteY=spriteY-(ghPosY[g]-ghOldPosY[g]);
        if(tempSpriteX >= 0 && tempSpriteY >=0)
          if(tempSpriteX < sprSize && tempSpriteY < sprSize)
            if(wSprite[tempSpriteY * sprSize + tempSpriteX] != 0)
              continue;
        Tft.setPixel(offsety + spriteY + 1 + ghOldPosY[g] - 8, offsetx + spriteX + 1 + ghOldPosX[g] - 8, palette[0]);
      }

    for(int spriteX = 0; spriteX <  sprSize; spriteX++)
      for(int spriteY = 1; spriteY <  sprSize; spriteY++) {
        if(!makingLine) {
          makingLine=true;
          lineColor=wSprite[spriteY * sprSize + spriteX];
          lineStart=spriteY;
        } 
        else if(lineColor != wSprite[spriteY * sprSize + spriteX] || spriteY == sprSize - 1)
          makingLine=false;
        if(!makingLine) {
          if(lineColor==5)
            if(frightTimer>1 && !ghEaten[g])
              lineColor=12;
            else if(frightTimer>0 && !ghEaten[g])
              lineColor=10;
            else
              lineColor=ghColors[g];
          if(lineColor!=0)
            Tft.drawHorizontalLine(offsety + lineStart + 1 + ghPosY[g] - 8, offsetx + spriteX + 1 + ghPosX[g] - 8, constrain(spriteY - lineStart, 1, sprSize), palette[lineColor]);
          if(spriteY != sprSize - 1) {
            makingLine=true;
            lineColor=wSprite[spriteY * sprSize + spriteX];
            lineStart=spriteY;
          }
        }
      }
    drawCircles(ghGPosX[g]-1, ghGPosY[g]-1, 1);
    drawCircles(ghGPosX[g]-1, ghGPosY[g]-1, 2);

    Tft.fillCircle(offsety + (ghGPosY[g]-1)*8-1+ghVY[g], offsetx + (ghGPosX[g]-1)*8-2+ghVX[g], 1, BLACK);
    Tft.fillCircle(offsety + (ghGPosY[g]-1)*8-1+ghVY[g], offsetx + (ghGPosX[g]-1)*8+4+ghVX[g], 1, BLACK);
    }
    if(drawDots) {
      for(int i=0; i<hTiles; i++)
        for(int j=0; j<vTiles; j++)
          if(getTile(i, j)==1)
            Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 1, WHITE);
          else if(getTile(i, j)==2)
            Tft.fillCircle(offsety + j*8+4, offsetx + i*8+4, 2, WHITE);
      drawDots=false;
    }

    if(countFrames)
      frames++;
    if(millis()-countTime>=1000) {
      Tft.fillScreen(220,240,30,95,BLACK); 
      Tft.drawFloat((float)frames/((float)(millis()-countTime)/1000),90,220,1,palette[fpsC]);
      frames=0;
      countTime=millis();
    } 

    //delay(50);
}


