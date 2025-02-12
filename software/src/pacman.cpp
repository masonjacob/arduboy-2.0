// /**************************************************************************
//    *  
//  *  ARDUINO PAC-MAN 1.2
//  *  
//  *  Jan/2022  Giovanni Verrua
//  *
//    *   This program is free software: you can redistribute it and/or modify
//  *   it   under the terms of the GNU General Public License as published by
//  *   the Free   Software Foundation, either version 3 of the License, or
//  *   (at your option)   any later version.
//  *
//  *   This program is distributed in the hope that it   will be useful,
//  *   but WITHOUT ANY WARRANTY; without even the implied warranty   of
//  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  *   GNU   General Public License for more details.
//  *
//  *   You should have received   a copy of the GNU General Public License
//  *   along with this program.  If not,   see <https://www.gnu.org/licenses/>.
//  * 
//  * NOTE: The code isn't optimized.   I know that, it could be wrote 
//  *       in less lines and some choices I made   are quite awful. 
//  *       The main reason is that I wrote it learning Arduino,   so I 
//  *       preferred a code easy to read than a more compact code.
//  *        
//  *       Also, I faced many strange problems. In example, a simple
//    *       variable declaration, an IF sentence, etc. in some cases prevented
//  *        the Arduino Nano from boot. Specifically, I couldn't use the 
//  *       TV.print()   function (that's the reason to use bitmap for the score
//  *       and the lives)   and I also have had serious troubles where the 
//  *       ghosts slow the speed   when they are "haunted" (I mean: when you eat
//  *       the magic pill, also   called "happy hour" in the comments ;-) or to 
//  *       make them flashing   at the 3 last seconds of the happy hour.
//  *       
//  *       So, forgive the   sometimes strange code, but I've lost more time
//  *       trying to fix this kind   of problems than to wrote the whole code.
//  *       
//  *       -- I wrote the   game from scratch instead to port it from another 
//  *       platform. Since I'm   not a game designer and honestly I have  
//  *       no skill about that, I'm sure   there's a better way to 
//  *       write the ghosts AI. My ghosts are AI: Artificially   Idiots.
//  *       Sorry for that.
//  *           
//  *   Connections to Arduino   Uno/Nano:
//  *    
//  *   Connect buttons to: pin 2 and +5V (button up), 
//    *                       pin 3 and +5V (button down),
//  *                       pin   4 and +5V (button left),
//  *                       pin 5 and +5V (but.right),
//    *                       pin 6 and +5V (button Fire / Start)
//  *                       
//    *   (note: you can use an Atari compatible Joystick too)                     
//    *                       
//  *   Connect 100 kohm pulldown resistors between pin   2 and GND, 
//  *                                               pin 3 and GND,   
//    *                                               pin 4 and GND,
//  *                                               pin   5 and GND,
//  *                                               pin 6 and GND
//    *   
//  *   TV composite out: 
//  *   Connect  1k ohm resistor from pin 9 to TV   out (+)
//  *   Connect 470 ohm resistor from pin 7 to TV out (+)   
//  *   Connect   GND                         to TV out (-)
//  *   
//  *   Audio:
//  *   Connect   a speaker between pin 11 and GND
//  * 
//  **************************************************************************/

// #include "bitmap.h"
// #include "pitches.h"


// //You   can use buttons or a Joystick Atari compatible
// #define BUTTON_UP     2   
// #define   BUTTON_DOWN   3
// #define BUTTON_LEFT   4
// #define BUTTON_RIGHT  5
// #define   BUTTON_START  6  //digital - button 

// #define PAC_STARTX   48 //pacman X position   
// #define PAC_STARTY   61 //pacman Y position


// #define GAME_SPEED 30   //higher value, lower speed
// #define GHOST_RELEASE_DELAY  1000 //how many millisec   between 2 ghosts release

// #define HOWMANYGHOSTS 4  //USED FOR DEBUG. MAX 4   GHOSTS (UNLESS YOU CHANGE THE GHOSTS ARRAY DECLARATIONS AND RESET)
// #define EATGHOSTSECS   10 //TIME ALLOWED TO EAT GHOSTS (HAPPY HOUR)


// TVout TV;

// int Score   = 0;
// int lives = 3;

// int gamSt = 0;  //0 = menu, 1 = in game, 2 = game   over

// int FoodPosY[8]   =       {5,15,25,35,55,65,75,85};
    
// int FoodPosX[12]    =      {  5, 15, 25, 34, 44, 59, 70, 84, 94,103,113,122}; 

// //This declaration   is just for explaining how the food and pills are. 
// //if you need to modify something,   must change in reset_food() too.
// int FoodMatr[8][12] = {   {  1,  1,  1,  1,   1,  1,  1,  1,  1,  1,  1,  1},    //5
//                           {  2,  0,   1,  0,  1,  1,  1,  1,  0,  1,  0,  2},    //15
//                           {   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},    //25    
//                           {   1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1},    //35
//                           {   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},    //55
//                           {   1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  2,  1},    //65
//                           {   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},    //75
//                           {   1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1} };  //85


//           int   gstPosX [4] = {51,52,53,54};  //ghosts position
//           int gstPosY [4] = {41,41,41,41};   //ghosts position
//           int gstDire [4] = {1,1,1,1};  //ghosts direction   0=north, 1=east, 2=south, 3=west 


// //=====================================================================================   JINGLE
// void  Jingle(){   //Playing pac-man jingle
  
// TV.tone(NOTE_B4 ,100);   delay(120);
// TV.tone(NOTE_B5 ,100); delay(120);
// TV.tone(NOTE_FS5,100); delay(120);
// TV.tone(NOTE_DS5,100);   delay(120);
// TV.tone(NOTE_FS5,120); delay(200);
// TV.tone(NOTE_DS5,150); delay(250);

// TV.tone(NOTE_C5   ,100); delay(120);
// TV.tone(NOTE_C6 ,100); delay(120);
// TV.tone(NOTE_G5 ,100);   delay(120);
// TV.tone(NOTE_E5 ,100); delay(120);
// TV.tone(NOTE_C6 ,120); delay(200);
// TV.tone(NOTE_G5   ,150); delay(250);

// TV.tone(NOTE_B4 ,100); delay(120);
// TV.tone(NOTE_B5   ,100); delay(120);
// TV.tone(NOTE_FS5,100); delay(120);
// TV.tone(NOTE_DS5,100);   delay(120);
// TV.tone(NOTE_B5 ,120); delay(200);
// TV.tone(NOTE_FS5,150); delay(250);

// TV.tone(NOTE_DS5,100);   delay(120);
// TV.tone(NOTE_DS5,100); delay(120);
// TV.tone(NOTE_E5 ,100); delay(120);
// TV.tone(NOTE_F5   ,100); delay(120);
// TV.tone(NOTE_F5 ,100); delay(120);
// TV.tone(NOTE_FS5,100);   delay(120);
// TV.tone(NOTE_G5 ,100); delay(120);
// TV.tone(NOTE_G5 ,100); delay(120);
// TV.tone(NOTE_GS5,100);   delay(120);
// TV.tone(NOTE_A5 ,100); delay(120);
// TV.tone(NOTE_B5 ,100); delay(120);
// }



// //=====================================================================================   RESET_GHOST
// void reset_ghosts() {

//      gstPosX[0] = 51; gstPosX[1] = 52;   gstPosX[2] = 53; gstPosX[3] = 54;  //ghosts position
//      gstPosY[0] = 41; gstPosY[1]   = 41; gstPosY[2] = 41; gstPosY[3] = 41;  //ghosts position
//      gstDire[0] =   1;  gstDire[1] = 1;  gstDire[2] = 1;  gstDire[3] = 1;  //ghosts direction 0=north,   1=east, 2=south, 3=west 
  
// }
          
// //=====================================================================================   DRAW_FOOD
// void draw_food() {
//      for (int i=0;i<12;i++){
//          for   (int j=0;j<8;j++){
          
//              if (FoodMatr[j][i] == 1) { //food
//                  TV.set_pixel(FoodPosX[i],FoodPosY[j],1);
//              }
             
//               if (FoodMatr[j][i] == 2) { //pill
//                 TV.set_pixel(FoodPosX[i]-1,FoodPosY[j]+1,1);
//                  TV.set_pixel(FoodPosX[i]-1,FoodPosY[j]-1,1);
//                 TV.set_pixel(FoodPosX[i]+1,FoodPosY[j]-1,1);
//                  TV.set_pixel(FoodPosX[i]+1,FoodPosY[j]+1,1);                
//               }
             
//           }          
//       }
// }      

// //=====================================================================================   RESET_FOOD
// void reset_food() {  

//   //I tried with a const array for initialization,   but i got strange behaviors. I'm sure it's my fault. This isn't elegant but at least   it works LOL
//   FoodMatr[0][0]=1; FoodMatr[0][1]=1; FoodMatr[0][2]=1; FoodMatr[0][3]=1;   FoodMatr[0][4]=1; FoodMatr[0][5]=1; FoodMatr[0][6]=1; FoodMatr[0][7]=1; FoodMatr[0][8]=1;   FoodMatr[0][9]=1; FoodMatr[0][10]=1; FoodMatr[0][11]=1 ;
//   FoodMatr[1][0]=2;   FoodMatr[1][1]=0; FoodMatr[1][2]=1; FoodMatr[1][3]=0; FoodMatr[1][4]=1; FoodMatr[1][5]=1;   FoodMatr[1][6]=1; FoodMatr[1][7]=1; FoodMatr[1][8]=0; FoodMatr[1][9]=1; FoodMatr[1][10]=0;   FoodMatr[1][11]=2 ;
//   FoodMatr[2][0]=1; FoodMatr[2][1]=1; FoodMatr[2][2]=1; FoodMatr[2][3]=1;   FoodMatr[2][4]=1; FoodMatr[2][5]=1; FoodMatr[2][6]=1; FoodMatr[2][7]=1; FoodMatr[2][8]=1;   FoodMatr[2][9]=1; FoodMatr[2][10]=1; FoodMatr[2][11]=1 ;
//   FoodMatr[3][0]=1;   FoodMatr[3][1]=1; FoodMatr[3][2]=1; FoodMatr[3][3]=1; FoodMatr[3][4]=0; FoodMatr[3][5]=0;   FoodMatr[3][6]=0; FoodMatr[3][7]=0; FoodMatr[3][8]=1; FoodMatr[3][9]=1; FoodMatr[3][10]=1;   FoodMatr[3][11]=1 ;
//   FoodMatr[4][0]=1; FoodMatr[4][1]=1; FoodMatr[4][2]=1; FoodMatr[4][3]=1;   FoodMatr[4][4]=1; FoodMatr[4][5]=1; FoodMatr[4][6]=1; FoodMatr[4][7]=1; FoodMatr[4][8]=1;   FoodMatr[4][9]=1; FoodMatr[4][10]=1; FoodMatr[4][11]=1 ;
//   FoodMatr[5][0]=1;   FoodMatr[5][1]=2; FoodMatr[5][2]=1; FoodMatr[5][3]=1; FoodMatr[5][4]=1; FoodMatr[5][5]=1;   FoodMatr[5][6]=1; FoodMatr[5][7]=1; FoodMatr[5][8]=1; FoodMatr[5][9]=1; FoodMatr[5][10]=2;   FoodMatr[5][11]=1 ;
//   FoodMatr[6][0]=1; FoodMatr[6][1]=1; FoodMatr[6][2]=1; FoodMatr[6][3]=1;   FoodMatr[6][4]=1; FoodMatr[6][5]=1; FoodMatr[6][6]=1; FoodMatr[6][7]=1; FoodMatr[6][8]=1;   FoodMatr[6][9]=1; FoodMatr[6][10]=1; FoodMatr[6][11]=1 ;
//   FoodMatr[7][0]=1;   FoodMatr[7][1]=1; FoodMatr[7][2]=1; FoodMatr[7][3]=1; FoodMatr[7][4]=1; FoodMatr[7][5]=1;   FoodMatr[7][6]=1; FoodMatr[7][7]=1; FoodMatr[7][8]=1; FoodMatr[7][9]=1; FoodMatr[7][10]=1;   FoodMatr[7][11]=1 ;

// }      

// //=====================================================================================   SETUP
// void setup() {  
    
//     TV.begin(_PAL); //128x96 default
        
//      pinMode(BUTTON_UP,INPUT);
//     pinMode(BUTTON_DOWN,INPUT);
//     pinMode(BUTTON_LEFT,INPUT);
//      pinMode(BUTTON_RIGHT,INPUT);
//     pinMode(BUTTON_START,INPUT);
    
//      gamSt = 0;        
// }

// //=====================================================================================   DRAW_SCORE
// //for some reason I can't use the tv.print() function: it hangs the   sketch 
// //(it compiles fine but after upload it doesn't start). So I must to   write the
// //score using bitmaps.

// void draw_score() {

//  int pos =   30;
//  int num = 0;
//  int zero = 0;

//   int a = Score /10;
//   int b =   Score /100;
//   int c = Score /1000;
//   int d = Score /10000;
  
//   num   = d;      if (num > 0 || zero !=0 ) { draw_numbers(num,pos); pos+=8; zero = 1; }
//    num = c-d*10; if (num > 0 || zero !=0 ) { draw_numbers(num,pos); pos+=8; zero   = 1; }
//   num = b-c*10; if (num > 0 || zero !=0 ) { draw_numbers(num,pos); pos+=8;   zero = 1; }
//   num = a-b*10; if (num > 0 || zero !=0 ) { draw_numbers(num,pos);   pos+=8; zero = 1; }
//   num = Score-a*10;                         draw_numbers(num,pos);   
  
// }  

// //=====================================================================================   DRAW_NUMBERS
// //for some reason I can't use the tv.print() function: it hangs   the sketch 
// //(it compiles fine but after upload it doesn't start). So I must   to write the
// //score using bitmaps.
// void draw_numbers(uint8_t num, uint8_t   pos ) {
  
//                if (num == 0) TV.bitmap(  pos,88,nu0);
//                if   (num == 1) TV.bitmap(  pos,88,nu1);
//                if (num == 2) TV.bitmap(  pos,88,nu2);
//                 if (num == 3) TV.bitmap(  pos,88,nu3);
//                if (num   == 4) TV.bitmap(  pos,88,nu4);
//                if (num == 5) TV.bitmap(  pos,88,nu5);
//                 if (num == 6) TV.bitmap(  pos,88,nu6);
//                if (num   == 7) TV.bitmap(  pos,88,nu7);
//                if (num == 8) TV.bitmap(  pos,88,nu8);
//                 if (num == 9) TV.bitmap(  pos,88,nu9);            

// }


// //=====================================================================================   COLLISION
// unsigned char collision(uint8_t x, uint8_t y, uint8_t dir) {

//    //-----------------------------------------------------------------------------------------------------north
//    if (dir == 0){
//     if ( TV.get_pixel(x,y-1)==1 || TV.get_pixel(x+7,y-1)==1   ) return 1;
//      for (int i=1;i<6;i++){
//         if ( TV.get_pixel(x+i,y-1)==1   && TV.get_pixel(x+i+1,y-1)==1 ) return 1; //must to have at least 2 pixel in front   of it to represent a wall (1 pixel represent food or pill)
//      }           
//    }
  
//   //-----------------------------------------------------------------------------------------------------east
//    if (dir == 1){
//      if ( TV.get_pixel(x+8,y)==1 || TV.get_pixel(x+8,y+7)==1   ) return 1; 
//      for (int i=1;i<6;i++){
//         if ( TV.get_pixel(x+8,y+i)==1   && TV.get_pixel(x+8,y+i+1)==1 ) return 1;  //must to have at least 2 pixel in front   of it to represent a wall (1 pixel represent food or pill)
//      }           
//    }

//   //-----------------------------------------------------------------------------------------------------south
//    if (dir == 2){    
//     if ( TV.get_pixel(x,y+8)==1 || TV.get_pixel(x+7,y+8)==1   ) return 1;
//      for (int i=1;i<6;i++){
//         if ( TV.get_pixel(x+i,y+8)==1   && TV.get_pixel(x+i+1,y+8)==1 ) return 1; //must to have at least 2 pixel in front   of it to represent a wall (1 pixel represent food or pill)
//      }           
//    }

//   //-----------------------------------------------------------------------------------------------------west
//    if (dir == 3){
//      if ( TV.get_pixel(x-1,y)==1 || TV.get_pixel(x-1,y+7)==1   ) return 1;
//      for (int i=1;i<6;i++){
//         if ( TV.get_pixel(x-1,y+i)==1   && TV.get_pixel(x-1,y+i+1)==1 ) return 1; //must to have at least 2 pixel in front   of it to represent a wall (1 pixel represent food or pill)
//      }           
//    }  

//   return 0;
// }  

// //=====================================================================================   LOOP
// void loop() {
        
//   bool every_2 = false;
//   //-----------------------------------------------------------------------------------------------------MENU               
//   if (gamSt == 0){   //MENU

//         TV.clear_screen();            
//         TV.bitmap(0,  0, logo);
//         TV.bitmap(0, 33, start);

//          delay(1000);     

//         while ( digitalRead(BUTTON_START) == 0){}                   
//          gamSt = 1; 
//          delay(300);
         
//    }
    
 
//  //-----------------------------------------------------------------------------------------------------IN   GAME
 
//   if (gamSt == 1){   // IN GAME

//      Score = 0;
//      lives   = 3;

//      //---------------------------------
//      //drawing once the   grid and the food before the jingle() - not needed, just for show
//      TV.clear_screen();      
//      TV.bitmap(00,00,grid); 
//      reset_food();                         
//       draw_food(); 
//      Jingle();
//      //---------------------------------
       

//      while (lives > 0){    

//           int eatGhosts = 0;
//            int oldDire = 1;  //old pacman direction 
        
//           unsigned   long mill = millis() ;  //delay - aliens
//           unsigned long relg = millis()   ;  //delay - release ghosts
//           unsigned long eatg = millis() ;  //delay   - eat ghosts
          

//           int pacPosX = PAC_STARTX; //pacman X   position 
//           int pacPosY = PAC_STARTY; //pacman Y position
//           int   pacDire = 1;  //pacman direction 0=north, 1=east, 2=south, 3=west > changing with   wheel or buttons                   

//           int ghChg[4] = {0,0,0,0};  //ghost   change direction  
//           int ghSts[4] = {0,0,0,0};  //ghost status (1=eaten,   0=alive) - when eatGhosts = 1
        

//           int endfood = 0; // 0   = all eaten, 1 = some food/pills left yet    
//           int gotcha = 0;   
//            int numofgh = 0;
           
//            endfood = 1;     

//             TV.clear_screen();
//            TV.bitmap(00,00,grid);
                    
//             reset_ghosts();           
//            reset_food();            
//             draw_food(); 
           
//            //-----------------------------------------------------------------------------------------------------Main   Cycle-begin-\\
                      
//            while ( endfood != 0 && lives   > 0 ) {              
                                      
//              if   (mill + GAME_SPEED < millis()){  //the game move all at the same speed                

//                  if (every_2 ) every_2 = false; else every_2 = true;
      
//                  if (  numofgh < HOWMANYGHOSTS && relg + GHOST_RELEASE_DELAY < millis()   ) { numofgh++; relg = millis(); }
      
                     
//                //----------------------------------------------------------------------------------------------\  
//                //DON'T MOVE THIS PART FROM HERE! IT MUST TO DELETE ALL THE GHOSTS   BEFORE TO CHECK THE OBSTACLES 
//                //OR THE GHOSTS WILL BE CONFUSED   BY THE PRESENCE OF OTHER GHOSTS!!!
//                for (int i=0;i<numofgh;i++){   
//                    TV.bitmap(gstPosX[i],gstPosY[i],blk);  //delete all ghosts   before to check obstacles.        
//                 } 
//                 TV.bitmap(pacPosX,pacPosY,blk);
//                  //----------------------------------------------------------------------------------------------/

       
//                 //----------------------------------------------------3   secs to the end of happy hour! Ghosts are flashing
//                 if (eatGhosts   == 1 && eatg + EATGHOSTSECS*1000 -3000 < millis()) {
//                      eatGhosts   = 2;     
//                 }

//                 //----------------------------------------------------end   of happy hour! Ghosts are hunting you again!
//                 if (eatGhosts >   0 && eatg + EATGHOSTSECS*1000 < millis()) {
//                      eatGhosts =   0;     
//                      ghSts[0] = 0; ghSts[1] = 0; ghSts[2] = 0; ghSts[3]   = 0;                     
//                 }   
        
//                 //----------------------------------------------------------------------------------------------\                                                   

//                 oldDire   = pacDire ;
//                 if ( digitalRead(BUTTON_UP   ) == 1) pacDire = 0;   //north
//                 if ( digitalRead(BUTTON_RIGHT) == 1) pacDire = 1; //east
//                  if ( digitalRead(BUTTON_DOWN ) == 1) pacDire = 2; //south
//                 if   ( digitalRead(BUTTON_LEFT ) == 1) pacDire = 3; //west
                                   
//                  if (collision(pacPosX,pacPosY,pacDire) == 1 && pacDire != oldDire   ) pacDire = oldDire ;
                
//                 if (collision(pacPosX,pacPosY,pacDire)   == 0){                
          
//                       if (pacDire == 0)   pacPosY--; 
//                       if (pacDire == 1) pacPosX++; 
//                       if   (pacDire == 2) pacPosY++; 
//                       if (pacDire == 3) pacPosX--;   
          
//                       if (pacPosX > 120) pacPosX = 0;
//                       if   (pacPosX < 0)   pacPosX = 120; 
      
//                        //------------------------------------------------------------------------------------------------------check   food/pills
//                        endfood = 0;
//                        for   (int i=0;i<12;i++){
//                            for (int j=0;j<8;j++){
//                                if   (FoodMatr[j][i] == 1 || FoodMatr[j][i] == 2 ) {
//                                   endfood   = 1;  //still some food / pills to eat

//                                   //-------------------------------------------------------------------------------------------------------food
//                                    if (FoodPosX[i] >= pacPosX && FoodPosX[i] <= pacPosX+7   && FoodPosY[j] >= pacPosY && FoodPosY[j] <= pacPosY+7) {
//                                       if   (FoodMatr[j][i] == 1) {
//                                           FoodMatr[j][i]   = 0;
//                                           Score ++;
//                                           TV.tone(NOTE_G1,   50);
//                                       }
//                                       if   (FoodMatr[j][i] == 2) {
//                                           FoodMatr[j][i]   = 0;
//                                           Score += 10;
//                                           eatGhosts   = 1;
//                                           eatg = millis() ;
//                                           TV.tone(NOTE_B5,   50);
//                                       }
//                                   }     
//                                   //-------------------------------------------------------------------------------------------------------pill
//                                 }
//                             }          
//                          }

//                         if (endfood == 0) Jingle();
//                  } 
                     
                     
//                 //----------------------------------------------------------------------------------------------/                  

//                 //----------------------------------------------------------------------------------------------pac   vs ghost collision 
//                  gotcha = 0;   
                
//                  for   (int i=0;i<numofgh;i++){    

//                     if (eatGhosts == 0 || ghSts[i]   == 0) {  

//                         //ghost coming from north
//                         if   (  gstPosY[i] <=pacPosY    && gstPosY[i]+8 >=pacPosY    &&  ( (gstPosX[i] >=pacPosX   && gstPosX[i]+7 <=pacPosX) || (gstPosX[i] <=pacPosX && gstPosX[i]+7 >=pacPosX) )   ) { gotcha = i+1; i == numofgh; }
                        
//                         //ghost   coming from south
//                         if (  pacPosY    <=gstPosY[i] && pacPosY    +8 >=gstPosY[i] &&  ( (gstPosX[i] >=pacPosX && gstPosX[i]+7 <=pacPosX) || (gstPosX[i]   <=pacPosX && gstPosX[i]+7 >=pacPosX) ) ) { gotcha = i+1; i == numofgh; }
                        
//                          //ghost coming from west
//                         if (   gstPosX[i] <=pacPosX    && gstPosX[i]+8 >=pacPosX    &&  ( (gstPosY[i] >=pacPosY   && gstPosY[i]+7 <=pacPosY) || (gstPosY[i] <=pacPosY && gstPosY[i]+7 >=pacPosY) )   ) { gotcha = i+1; i == numofgh; }
                                           
//                          //ghost coming from south
//                         if   (  pacPosX    <=gstPosX[i] && pacPosX   +8 >=gstPosX[i] &&  ( (gstPosY[i] >=pacPosY   && gstPosY[i]+7 <=pacPosY) || (gstPosY[i] <=pacPosY && gstPosY[i]+7 >=pacPosY) )   ) { gotcha = i+1; i == numofgh; }                    
//                     }    
//                   }

//                  if (gotcha > 0 ) {

//                     if   (eatGhosts == 0) {
//                         //-------------------------------------------------------------   the got you! ----------
//                         lives --;                                          
                         
//                         for (int i=0;i<5;i++){                   
//                             TV.tone(NOTE_G1*(5-i)*10, 100);
//                            TV.bitmap(pacPosX,pacPosY,p1w);   delay(50) ; 
//                            TV.bitmap(pacPosX,pacPosY,p1s); delay(50)   ;
//                            TV.bitmap(pacPosX,pacPosY,p1e); delay(50) ;
//                             TV.bitmap(pacPosX,pacPosY,p1n); delay(50) ;
//                         }                    
//                         delay(1000);
//                         relg   = millis() ;
//                         pacPosX = PAC_STARTX; //pacman X position   
//                         pacPosY = PAC_STARTY; //pacman Y position
//                         pacDire   = 1;  //pacman direction 0=north, 1=east, 2=south, 3=west > changing with wheel   or buttons 
//                         oldDire = 1;  //old pacman direction 
                         
//                         ghChg[0] = 0; ghChg[1] = 0;   ghChg[2] = 0; ghChg[3] = 0;   //ghost change direction  
//                         gotcha   = 0;   
//                         numofgh = 0;
                                            
//                          if (lives >0) {
//                            TV.clear_screen();
//                             TV.bitmap(00,00,grid);                    
//                            draw_food();   
//                            reset_ghosts();
//                         }      
//                       }  
//                      //-------------------------------------------------------------   you got a ghost!-------
//                      else {   // eatGhosts == 1   
//                            TV.tone(NOTE_B6, 200);                       
//                           gstPosX[gotcha-1]   = 51;
//                           gstPosY[gotcha-1] = 41;
//                           Score   += 20;                          
                                     
//                      }
//                   }   

               
//                //---------------------------------------------------------------------------ghost   moving-begin-\\
      
//                 for (int i=0;i<numofgh;i++){ //DEBUG   //<4 ma bisogna capire come fare per differenziare le direzioni dei fantasmi
       
//                    ghChg[i] = 0;
                   
//                    //direction   0=north, 1=east, 2=south, 3=west
             
//                    //----------------------------------------------------------------------------------------north                                          
//                    if (gstDire[i]==0   && ghChg[i]==0 && collision(gstPosX[i],gstPosY[i],gstDire[i]) == 1){  //
//                        if   (pacPosX < gstPosX[i] && eatGhosts == 0){           //if (random(1,3) == 1){
//                             if (collision(gstPosX[i],gstPosY[i],3) == 0) { gstDire[i]   = 3; ghChg[i] = 1; } else {  //n -> w
//                            if (collision(gstPosX[i],gstPosY[i],1)   == 0) { gstDire[i] = 1; ghChg[i] = 1; } }       //n -> e
//                        }
//                         else {
//                            if (collision(gstPosX[i],gstPosY[i],1)   == 0) { gstDire[i] = 1; ghChg[i] = 1; } else {  //n -> e
//                            if   (collision(gstPosX[i],gstPosY[i],3) == 0) { gstDire[i] = 3; ghChg[i] = 1; } }       //n   -> w
//                        }
//                        if ( ghChg[i]==0 )  {gstDire[i]=2;   ghChg[i] = 1; } //n -> s}  //means it can only go back   
//                    }
//                     //----------------------------------------------------------------------------------------east                 
//                    if (gstDire[i]==1 && ghChg[i]==0 && collision(gstPosX[i],gstPosY[i],gstDire[i])   == 1){
//                       if (pacPosY > gstPosY[i] && eatGhosts == 0){           //if   (random(1,3) == 1){
//                           if (collision(gstPosX[i],gstPosY[i],2)   == 0) { gstDire[i] = 2; ghChg[i] = 1; }  else {  //e -> s               
//                           if   (collision(gstPosX[i],gstPosY[i],0) == 0) { gstDire[i] = 0; ghChg[i] = 1; } }        //e   -> n 
//                         }
//                        else {
//                           if   (collision(gstPosX[i],gstPosY[i],0) == 0) { gstDire[i] = 0; ghChg[i] = 1; }  else   {  //e -> n               
//                           if (collision(gstPosX[i],gstPosY[i],2)   == 0) { gstDire[i] = 2; ghChg[i] = 1; } }        //e -> s 
//                        }                              
//                        if ( ghChg[i]==0 )  {gstDire[i]=3;   ghChg[i] = 1; } //e -> w}  //means it can only go back 
//                    }
//                     //----------------------------------------------------------------------------------------south                 
//                    if (gstDire[i]==2 && ghChg[i]==0 && collision(gstPosX[i],gstPosY[i],gstDire[i])   == 1){
//                        if (pacPosX > gstPosX[i] && eatGhosts == 0){           //if   (random(1,3) == 1){
//                            if (collision(gstPosX[i],gstPosY[i],1)   == 0) { gstDire[i] = 1; ghChg[i] = 1; } else {   //s -> e               
//                            if   (collision(gstPosX[i],gstPosY[i],3) == 0) { gstDire[i] = 3; ghChg[i] = 1; } }        //s   -> w 
//                        }  
//                        else {
//                            if   (collision(gstPosX[i],gstPosY[i],3) == 0) { gstDire[i] = 3; ghChg[i] = 1; } else   {   //s -> w               
//                            if (collision(gstPosX[i],gstPosY[i],1)   == 0) { gstDire[i] = 1; ghChg[i] = 1; } }        //s -> e 
//                        }   
//                        if ( ghChg[i]==0 )  {gstDire[i]=0; ghChg[i] = 1; }   //s -> n}  //means it can only go back 
//                    }
//                    //----------------------------------------------------------------------------------------west                 
//                    if (gstDire[i]==3 && ghChg[i]==0 && collision(gstPosX[i],gstPosY[i],gstDire[i])   == 1){
//                        if (pacPosY < gstPosY[i] && eatGhosts == 0){           //if   (random(1,3) == 1){
//                            if (collision(gstPosX[i],gstPosY[i],0)   == 0) { gstDire[i] = 0; ghChg[i] = 1; } else {   //w -> n               
//                            if   (collision(gstPosX[i],gstPosY[i],2) == 0) { gstDire[i] = 2; ghChg[i] = 1; } }        //w   -> s 
//                        }  
//                        else {
//                            if   (collision(gstPosX[i],gstPosY[i],2) == 0) { gstDire[i] = 2; ghChg[i] = 1; } else   {   //w -> s               
//                            if (collision(gstPosX[i],gstPosY[i],0)   == 0) { gstDire[i] = 0; ghChg[i] = 1; } }        //w -> n 
//                        }   
//                        if ( ghChg[i]==0 )  {gstDire[i]=1; ghChg[i] = 1; }   //w -> e}  //means it can only go back 
//                    }
               
                    
//                  }  //next i
      
                 
                
//                 for (int i=0;i<numofgh;i++){ 
                     
                   
//                     if (ghChg[i]==0  && random(1,3) == 1){   
                        
//                         //----------------------------------------------------------------------------------------west                                  
//                         if (gstDire[i]==3 &&   ghChg[i]==0){  //west
//                            if (pacPosY < gstPosY[i] &&   eatGhosts == 0){           //if (random(1,3) == 1){                                                    
//                                 if ( collision(gstPosX[i],gstPosY[i],0) == 0 && ghChg[i]==0)   {gstDire[i]=0;ghChg[i] = 1; } //w -> n
//                                if (random(1,5)==2){   
//                                if ( collision(gstPosX[i],gstPosY[i],2) == 0   && ghChg[i]==0) {gstDire[i]=2;ghChg[i] = 1; } //w -> s                         
//                                 }
//                             }                        
//                             else {                           
//                                if   ( collision(gstPosX[i],gstPosY[i],2) == 0 && ghChg[i]==0) {gstDire[i]=2;ghChg[i]   = 1; } //w -> s
//                                if (random(1,5)==2){ 
//                                if   ( collision(gstPosX[i],gstPosY[i],0) == 0 && ghChg[i]==0) {gstDire[i]=0;ghChg[i]   = 1; } //w -> n                                              
//                                }
//                              }                       
//                         }
                                       
//                         //----------------------------------------------------------------------------------------north
//                          if (gstDire[i]==0 && ghChg[i]==0){  //north
//                               if   (pacPosX < gstPosX[i] && eatGhosts == 0){           //if (random(1,3) == 1){
//                                    if ( collision(gstPosX[i],gstPosY[i],1) == 0 &&   ghChg[i]==0) {gstDire[i]=1;ghChg[i] = 1; } //n -> e                       
//                                   if   (random(1,5)==2){ 
//                                   if ( collision(gstPosX[i],gstPosY[i],3)   == 0 && ghChg[i]==0) {gstDire[i]=3;ghChg[i] = 1; } //n -> w                            
//                                    }
//                               } 
                        
//                                else {                                                  
//                                    if ( collision(gstPosX[i],gstPosY[i],3) == 0 &&   ghChg[i]==0) {gstDire[i]=3;ghChg[i] = 1; } //n -> w
//                                   if   (random(1,5)==2){ 
//                                   if ( collision(gstPosX[i],gstPosY[i],1)   == 0 && ghChg[i]==0) {gstDire[i]=1;ghChg[i] = 1; } //n -> e                                         
//                                    }
//                               }     
//                          }                   
                        
//                         //----------------------------------------------------------------------------------------east                          
//                         if (gstDire[i]==1 && ghChg[i]==0){   //east      
//                              if (pacPosY > gstPosY[i] && eatGhosts   == 0){           //if (random(1,3) == 1){                                                    
//                                    if ( collision(gstPosX[i],gstPosY[i],2) == 0 &&   ghChg[i]==0) {gstDire[i]=2;ghChg[i] = 1; } //e -> s 
//                                   if   (random(1,5)==2){ 
//                                   if ( collision(gstPosX[i],gstPosY[i],0)   == 0 && ghChg[i]==0) {gstDire[i]=0;ghChg[i] = 1; } //e -> n                                                 
//                                    }
//                               }                        
//                               else {                              
//                                   if   ( collision(gstPosX[i],gstPosY[i],0) == 0 && ghChg[i]==0) {gstDire[i]=0;ghChg[i]   = 1; } //e -> n
//                                   if (random(1,5)==2){ 
//                                   if   ( collision(gstPosX[i],gstPosY[i],2) == 0 && ghChg[i]==0) {gstDire[i]=2;ghChg[i]   = 1; } //e -> s                                         
//                                   }
//                               }     
//                         }                    
           
//                         //----------------------------------------------------------------------------------------south
//                          if (gstDire[i]==2 && ghChg[i]==0){  //south 
//                            if   (pacPosX > gstPosX[i] && eatGhosts == 0){           //if (random(1,3) == 1){                                                                                    
//                                    if ( collision(gstPosX[i],gstPosY[i],3) == 0 &&   ghChg[i]==0) {gstDire[i]=3;ghChg[i] = 1; } //s -> w 
//                                   if   (random(1,5)==2){ 
//                                   if ( collision(gstPosX[i],gstPosY[i],1)   == 0 && ghChg[i]==0) {gstDire[i]=1;ghChg[i] = 1; } //s -> e 
//                                   }
//                              }                        
//                             else   {                             
//                                   if ( collision(gstPosX[i],gstPosY[i],1)   == 0 && ghChg[i]==0) {gstDire[i]=1;ghChg[i] = 1; } //s -> e                 
//                                    if (random(1,5)==2){ 
//                                   if   ( collision(gstPosX[i],gstPosY[i],3) == 0 && ghChg[i]==0) {gstDire[i]=3;ghChg[i]   = 1; } //s -> w 
//                                   }
//                             }      
//                         }                       
                        
                         
//                   }     


//                   //--------------------------------------------------------------------------------------don't   touch the code or will hang Arduino begin ---\\
//                   //I know that   the code isn't optimized and could be written in a more compact way; however if   you will touch
//                   //this part, even with replicated code, the   sketch will hang the Arduino - it drove me crazy and this was
//                   //the   only way I succeded. If you find a better and working way, or can explain me how   it hangs, please
//                   //advise me. Thank you!

//                 //if   (eatGhosts == 0 && every_2 ) {  this should work fine and avoid to replicate the   IF; instead it hangs the sketch.
                  
//                   if (eatGhosts   == 0 ) {  //ghosts at normal speed (they're hunting you)                
//                      if   (gstDire[i] == 0) gstPosY[i]--; 
//                      if (gstDire[i] == 1) gstPosX[i]++;   
//                      if (gstDire[i] == 2) gstPosY[i]++; 
//                      if   (gstDire[i] == 3) gstPosX[i]--;     
                                      
//                       if (gstPosX[i] > 120) gstPosX[i] = 0; 
//                      if   (gstPosX[i] < 0)   gstPosX[i] = 120;       
//                      if (gstPosX[i]   > 120) gstPosX[i] = 0; else if (gstPosX[i] < 0)   gstPosX[i] = 120; 
//                   }
//                    //-------------------------------------------------------------------------------
//                    if ( eatGhosts > 0  && every_2 ) {  //happy hour, the ghosts are   haunted and move at half speed                
//                      if (gstDire[i]   == 0) gstPosY[i]--; 
//                      if (gstDire[i] == 1) gstPosX[i]++;   
//                      if (gstDire[i] == 2) gstPosY[i]++; 
//                      if   (gstDire[i] == 3) gstPosX[i]--;     
                                      
//                       if (gstPosX[i] > 120) gstPosX[i] = 0; 
//                      if   (gstPosX[i] < 0)   gstPosX[i] = 120;       
//                      if (gstPosX[i]   > 120) gstPosX[i] = 0; else if (gstPosX[i] < 0)   gstPosX[i] = 120; 
//                   }
                                                       
//                   //-----------------------------------------------------------------------draw   ghosts                                                         
//                  //Again,   don't touch this part or the sketch will hang the Arduino!
                 
//                   if (eatGhosts == 0) {                  
//                        if   ( (gstPosX[i] % 2) - (gstPosY[i] % 2) == 0) TV.bitmap(gstPosX[i],gstPosY[i],g1n);   
//                        else                                           TV.bitmap(gstPosX[i],gstPosY[i],g2n);
//                    }
                  
//                   if (eatGhosts ==   1) {                  
//                        if ( (gstPosX[i] % 2) - (gstPosY[i]   % 2) == 0) TV.bitmap(gstPosX[i],gstPosY[i],g1h); 
//                        else                                            TV.bitmap(gstPosX[i],gstPosY[i],g2h);   
//                   }     

//                   if (eatGhosts == 2) {                  
//                         if ( (gstPosX[i] % 2) - (gstPosY[i] % 2) == 0) TV.bitmap(gstPosX[i],gstPosY[i],g1h);   
//                        else                                           TV.bitmap(gstPosX[i],gstPosY[i],g2n);     
 
//                   }
//                   //--------------------------------------------------------------------------------------don't   touch the code or will hang Arduino end -----/


                  
                    
//                }  //next i
//                //---------------------------------------------------------------------------ghost   moving-end---/  


                
//                 //---------------------------------------------------------------------------------------------------   draw Pac Man
//                 if (pacDire == 0) {
//                     if (   (pacPosX % 2) - (pacPosY % 2) == 0) { TV.bitmap(pacPosX,pacPosY,p1n); }
//                     else                                      { TV.bitmap(pacPosX,pacPosY,p2n); }
//                   }
//                  if (pacDire == 1) {
//                     if ( (pacPosX % 2) -   (pacPosY % 2) == 0) { TV.bitmap(pacPosX,pacPosY,p1e); }
//                     else                                      { TV.bitmap(pacPosX,pacPosY,p2e); }
//                   }
//                  if (pacDire == 2) {
//                     if ( (pacPosX % 2) -   (pacPosY % 2) == 0) { TV.bitmap(pacPosX,pacPosY,p1s); }
//                     else                                      { TV.bitmap(pacPosX,pacPosY,p2s); }
//                   }
//                  if (pacDire == 3) {
//                     if ( (pacPosX % 2) -   (pacPosY % 2) == 0) { TV.bitmap(pacPosX,pacPosY,p1w); }
//                     else                                      { TV.bitmap(pacPosX,pacPosY,p2w); }
//                   }
                 
//                //---------------------------------------------------------------------------------------------------        
//                draw_food();   
//                draw_score(); 
//                draw_numbers(   lives, 118 );
      
//                 //------------------------------------------------------------
//                  mill = millis();
//               }
             
                
//                 //-----------------------------------------------------------------------------------------------------Main   Cycle-end--- /
               
//              } //while endfood != 0
              
//         } //while lives >0 
//        gamSt = 2 ;
//    } //if gamst == 1

//    //-----------------------------------------------------------------------------------------------------GAME   OVER 
//   if (gamSt == 2){  //GAME OVER
//     gamSt = 0;     
//     TV.clear_screen();            
//     TV.bitmap(0,  0, logo);
//     TV.bitmap(0, 33, over);     
      
//     draw_score();    
//     delay(1000);
    
//     //must to release   and press again the button before to continue
//     while ( digitalRead(BUTTON_START)   == 1){}
//     while ( digitalRead(BUTTON_START) == 0){}

//   }
  
// }
