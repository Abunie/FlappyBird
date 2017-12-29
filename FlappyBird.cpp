#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
// TFT display and SD card will share the hardware SPI interface.
// For the Adafruit shield, these are the default.
// The display uses hardware SPI, plus #9 & #10
// Mega2560 Wiring: connect TFT_CLK to 52, TFT_MISO to 50, and TFT_MOSI to 51
#define TFT_DC  9
#define TFT_CS 10
#define SD_CS   6
#include <SPI.h>
#include <SD.h>
// touch screen pins, obtained from the documentaion
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin
// width/height of the display when rotated horizontally
#define TFT_WIDTH  320
#define TFT_HEIGHT 240
// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// a multimeter reading says there are 300 ohms of resistance across the plate,
// so initialize with this to get more accurate readings
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//variables used
int game=0;
int pressed=0;
int a=48; //var used for scoring

////////////////////////////////////SETUP///////////////////////////////////////
void setup()
{
	init();
	Serial.begin(9600);
	tft.begin();
	tft.setRotation(-1);
}

//player has to touch the screen to start playing the game
void unstartedgame()
{
	if(game==0)
	{
		tft.drawChar(20,50,84,BLACK,BLACK,3);
		tft.drawChar(40,50,97,BLACK,BLACK,3);
		tft.drawChar(60,50,112,BLACK,BLACK,3);
		tft.drawChar(100,50,116,BLACK,BLACK,3);
		tft.drawChar(120,50,111,BLACK,BLACK,3);
		tft.drawChar(160,50,83,BLACK,BLACK,3);
		tft.drawChar(180,50,116,BLACK,BLACK,3);
		tft.drawChar(200,50,97,BLACK,BLACK,3);
		tft.drawChar(220,50,114,BLACK,BLACK,3);
		tft.drawChar(240,50,116,BLACK,BLACK,3);
	}
	while(true)
	{
		int touchX, touchY;
		int X;
	 TSPoint touch = ts.getPoint();
	 if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE)
	 {
		 return;
	 }
	 touchY = map(touch.x, TS_MINX, TS_MAXX, 0, TFT_HEIGHT - 1);
	 touchX = map(touch.y, TS_MINY, TS_MAXY, TFT_WIDTH - 1, 0);
	 if ((X<=316)&&(X>=0))
	 {
		 game=1;
	 }
 }
}
///////////////////////////////CODE FOR CLEARING////////////////////////////////

//if pipe is not in the bird area, it's gonna clear a bigger rectangular space
void clearflappy()
{
	tft.drawRect(40,33,44,203,BLUE);
 tft.fillRect(40,33,44,203,BLUE);
}
//code for clearing the pipes when it goes past the screen
void clearPipes(int dx)
{

	tft.drawRect(380+dx,33,31,203,BLUE);
	tft.fillRect(380+dx,33,31,203,BLUE);
}
void gameWon(){
	game=0;
	tft.fillScreen(ILI9341_MAGENTA);
	tft.drawChar(0,90,89,WHITE,WHITE,5);
	tft.drawChar(30,90,79,WHITE,WHITE,5);
	tft.drawChar(60,90,85,WHITE,WHITE,5);
	tft.drawChar(110,90,87,WHITE,WHITE,5);
	tft.drawChar(140,90,79,WHITE,WHITE,5);
	tft.drawChar(170,90,78,WHITE,WHITE,5);
	tft.drawChar(230,90,33,WHITE,WHITE,5);
	tft.drawChar(250,90,33,WHITE,WHITE,5);
	tft.drawChar(270,90,33,WHITE,WHITE,5);

}
////////////////////////////draws the bird//////////////////////////////////////
void drawBird(double dy)
{
	clearflappy();
	tft.drawCircle( 60,100+dy, 15, YELLOW); //body
	tft.fillCircle( 60,100+dy, 14, YELLOW);
	tft.drawRect(72,101+dy,12,5,RED); //beak
	tft.fillRect(72,101+dy,11,4,RED);
	tft.drawCircle( 72,93+dy, 4, WHITE); //eyes
	tft.fillCircle( 72, 93+dy,4, WHITE);
	tft.drawCircle(72,93+dy,  2, BLACK);
	tft.fillCircle( 72,93+dy,  2, BLACK);
	tft.drawTriangle(40,101+dy,40,90+dy,55,101+dy,WHITE); //wings
	tft.fillTriangle(40,101+dy,40,90+dy,55,101+dy,WHITE);
}
////////////////////////////draws the pipes/////////////////////////////////////
void Pipes(int dx,int y)
{
	clearPipes(dx);
	//upper pipes
	tft.drawRect(330+dx,31,30,y,GREEN);
	tft.fillRect(330+dx,31,30,y,GREEN);
	//lower pipes
	tft.drawRect(330+dx,y+150,30,00,GREEN);
  tft.fillRect(330+dx,y+150,30,200,GREEN);
}
/////////////////////////////////LIVES//////////////////////////////////////////
void scores(int num)
{
  tft.fillRect( 208,0, 45, 32, BLACK);
	tft.drawChar(210,0,num,WHITE,WHITE,3);
}
////////////////////////////////STARTING BACKGROUND/////////////////////////////
//used at the very beginning; shows the bird that is NOT in motion
void DrawBackground(int y)
{
	tft.fillScreen(ILI9341_BLUE);
	tft.drawRect( 0,0, 320, 32, BLACK);
	tft.fillRect( 0,0, 320, 32, BLACK);
	tft.drawChar(0,0,83,WHITE,WHITE,3);
	tft.drawChar(30,0,67,WHITE,WHITE,3);
	tft.drawChar(60,0,79,WHITE,WHITE,3);
	tft.drawChar(90,0,82,WHITE,WHITE,3);
	tft.drawChar(120,0,69,WHITE,WHITE,3);
	tft.drawChar(150,0,58,WHITE,WHITE,3);
	scores(48);
	drawBird(0);
}
//////////////////////////////////////GAME STOPS////////////////////////////////
void displaygameover(int high,int carry)
{
  tft.fillScreen(ILI9341_RED); //you failed
	tft.drawChar(30,30,89,WHITE,RED,6);
	tft.drawChar(70,30,79,WHITE,RED,6);
	tft.drawChar(110,30,85,WHITE,RED,6);
	tft.drawChar(90,90,70,WHITE,WHITE,6);
	tft.drawChar(120,90,65,WHITE,WHITE,6);
	tft.drawChar(150,90,73,WHITE,WHITE,6);
	tft.drawChar(180,90,76,WHITE,WHITE,6);
	tft.drawChar(220,90,69,WHITE,WHITE,6);
	tft.drawChar(260,90,68,WHITE,WHITE,6);
	tft.drawChar(290,90,33,WHITE,WHITE,6);
	tft.drawChar(0,200,83,WHITE,WHITE,3);
	tft.drawChar(30,200,67,WHITE,WHITE,3);
	tft.drawChar(60,200,79,WHITE,WHITE,3);
	tft.drawChar(90,200,82,WHITE,WHITE,3);
	tft.drawChar(120,200,69,WHITE,WHITE,3);
	tft.drawChar(150,200,58,WHITE,WHITE,3);
	tft.drawChar(210,200,high,WHITE,WHITE,3);
	tft.drawChar(190,200,carry,WHITE,WHITE,3);
	//touch
  tft.drawChar(0,140,84,BLACK,RED,2);
  tft.drawChar(20,140,79,BLACK,RED,2);
  tft.drawChar(40,140,85,BLACK,RED,2);
  tft.drawChar(60,140,67,BLACK,RED,2);
  tft.drawChar(80,140,72,BLACK,RED,2);
  //to
  tft.drawChar(120,140,84,BLACK,RED,2);
  tft.drawChar(140,140,79,BLACK,RED,2);
  //start
  tft.drawChar(180,140,82,BLACK,RED,2);
  tft.drawChar(200,140,69,BLACK,RED,2);
  tft.drawChar(220,140,83,BLACK,RED,2);
  tft.drawChar(240,140,84,BLACK,RED,2);
  tft.drawChar(260,140,65,BLACK,RED,2);
	tft.drawChar(280,140,82,BLACK,RED,2);
  tft.drawChar(300,140,84,BLACK,RED,2);
}
/////////////////////////////restarts the game//////////////////////////////////
void restart()
{
    int touchGO;
    while(true)
    {
     TSPoint touch = ts.getPoint();
     if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE)
     {
         continue;
     }
     touchGO = map(touch.y, TS_MINY, TS_MAXY, TFT_WIDTH - 1, 0);
     if ((touchGO<=316)&&(touchGO>=0))
     {
        break;
     }
 }
}
////////////////////////////////////////////////////////////////////////////////
//shows 'ouch it hurts' for only a few seconds
void birdsad(int high, int carry)
{
    tft.fillScreen(ILI9341_BLACK);
    int dy=80;
    tft.drawCircle( 60,100+dy, 15, YELLOW);
    tft.fillCircle( 60,100+dy, 14, YELLOW);
    tft.drawRect(72,101+dy,12,5,RED);
    tft.fillRect(72,101+dy,11,4,RED);
    tft.drawCircle( 72,93+dy, 4, WHITE);
    tft.fillCircle( 72, 93+dy,4, WHITE);
    tft.drawCircle(72,93+dy,  2, BLACK);
    tft.fillCircle( 72,93+dy,  2, BLACK);
    tft.drawTriangle(40,101+dy,40,90+dy,55,101+dy,WHITE);
    tft.fillTriangle(40,101+dy,40,90+dy,55,101+dy,WHITE);
    tft.drawCircle( 73,80+dy, 2, WHITE);
    tft.drawCircle( 79,77+dy, 4, WHITE);
    tft.drawCircle( 86,70+dy, 6, WHITE);
    tft.drawCircle( 98,63+dy, 10, WHITE);
    tft.drawCircle( 195,20+dy, 100, WHITE);
    tft.drawChar(140,30,79,WHITE,WHITE,3);
 		tft.drawChar(160,30,85,WHITE,WHITE,3);
  	tft.drawChar(180,30,67,WHITE,WHITE,3);
    tft.drawChar(200,30,72,WHITE,WHITE,3);
    tft.drawChar(220,30,33,WHITE,WHITE,3);
    tft.drawChar(120,70,84,WHITE,WHITE,3);
    tft.drawChar(140,70,104,WHITE,WHITE,3);
    tft.drawChar(160,70,97,WHITE,WHITE,3);
    tft.drawChar(180,70,116,WHITE,WHITE,3);
    tft.drawChar(160,120,72,WHITE,WHITE,3);
    tft.drawChar(180,120,117,WHITE,WHITE,3);
  	tft.drawChar(200,120,114,WHITE,WHITE,3);
    tft.drawChar(220,120,116,WHITE,WHITE,3);
    tft.drawChar(240,120,115,WHITE,WHITE,3);
    delay(1000);
    displaygameover(high,carry);
}
///////////////////////////////////MAIN/////////////////////////////////////////
int main()
{
	setup();
	DrawBackground(0);
	while (true) //while loop to enable the player to restart the game
	{
		while (game==0)
		{
			unstartedgame();
		}
		DrawBackground(0);
		double g = 1;
		double movePipe = 0;
		int i=48;
		//picks a random number for the height of upper pipe
		int y=rand();
		//if it's too long, then it continues to get a random number
		while(y>=100)
		{
			y=rand();
		}
		//enables to play the game
		while (true)
		{
			g+=2;
			drawBird(g);
			movePipe += -10;
			Pipes(movePipe,y);
			int touchX;
			while(true)
			{
			 TSPoint touch = ts.getPoint();
			 if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE)
			 {
				 break;
			 }
				touchX = map(touch.y, TS_MINY, TS_MAXY, TFT_WIDTH - 1, 0);
				if ((touchX<=316)&&(touchX>=0))
			  {
			 		//if touched, thefall bird would go up
			 		g += -5;
			 	 	drawBird(g);
			  }
			}
			//case 1:bird beak hitting the lower pipe
			if ((106+g>=y+150)&&(330+movePipe<=84))
			{
				break;
			}
			//case 2:bird beak hitting the upper pipe
			if ((y+31>=106+g)&&(106+g>=31)&&(330+movePipe<=84))
			{
				break;
			}
			//case 3: bird head hitting the upper pipe
			//bird is in between of the two pipes
			if ((70+g<=y)&&(60>=314+movePipe)&&(60<=360+movePipe))
			{
				break;
			}
			//case 4: bird lower body hitting the lower pipe
			if ((70+g>=y+150)&&(60>=314+movePipe)&&(360+movePipe>=60))
			{
				break;
			}
			//case 5: bird hitting the upper/lower edge of the screen
			if ((67+g<=31)||(120+g>=240))
			{
				break;
			}
			if(a==53){
       game=0;
			 break;

			}
			//if pipe has gone past the screen, then it creates another pipe
			if (movePipe == -380)
			{
				i++;
				if(i<58)
				{
				scores(i);
				}
				if(i==58)
				{
					i=48;
					a++;
					tft.fillRect( 180,0, 45, 32, BLACK);
					tft.drawChar(190,0,a,WHITE,WHITE,3);
					scores(i);

				}


				movePipe = 0;
				y=rand();
				while(y>=100)
				{
					y=rand();
				}
				Pipes(movePipe,y);
			}
		}
		if(game==0)
		{
			gameWon();
			break;

		}
		else{
	  birdsad(i,a);
	}
		restart();
	}
	restart();


	Serial.end();
	return 0;
}
