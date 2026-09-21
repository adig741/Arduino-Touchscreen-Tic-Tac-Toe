     /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*        	Tic Tac Toe Game				          */
   /*                  Adi Goldstein                   */
  /*                  Faraj Kharbaoui                 */
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//Define pins
#define YP A2  // Y Plus
#define XM A3  // X Minus
#define YM 8   // Y Minus
#define XP 9   // X Plus

#define LCD_CS A3  // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0  // LCD Read goes to Analog 0
#define LCD_RESET A4

//Define colors
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x6B2C
#define LIGHTCYAN 0x87FF
#define DARKGREEN 0x0408
#define ORANGE 0xfb00
#define DARKBLUE 0x0006

#define BUZZER_PIN 51

#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
#include <stdint.h>
#include "TouchScreen.h"
#include "pitches.h"

int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
int tempo[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 360);

int button_values[9][4] =
 {
  { 150, 150, 450, 400 },
  { 450, 150, 630, 400 },
  { 630, 150, 960, 400 },
  { 150, 400, 450, 660 },
  { 450, 400, 630, 660 },
  { 630, 400, 960, 660 },
  { 150, 660, 450, 930 },
  { 450, 660, 630, 930 },
  { 630, 660, 960, 930 }
};

/*    'X' and 'O' locations on touchscreen    */
int XandO_locations[9][2] = 
{
  { 180, 5 },
  { 180, 115 },
  { 180, 225 },
  { 90, 5 },
  { 90, 115 },
  { 90, 225 },
  { 8, 5 },
  { 8, 115 },
  { 8, 225 }
};

int buttonUsed[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int turn_tracker = 0;

void setup() 
{
  //Elego touchscreen Setup
  tft.reset();
  uint16_t identifier = 0x9341;  //Elego touchscreen ID
  tft.begin(identifier);

  pinMode(BUZZER_PIN, OUTPUT);

  tft.setRotation(0);
  tft.fillScreen(ORANGE);

  tft.drawRect(0, 0, 240, 320, WHITE);  //Frmae draw
  tft.setRotation(1);                   //Rotate screen 90 degrees, so the text appears correctly.
  tft.setTextColor(WHITE);              //Set text color
  tft.setCursor(30, 30);                //position cursor correctly
  tft.setTextSize(4);                   //make text the right size
  tft.println("Tic-Tac-Toe");

  tft.setCursor(30, 120);        //position cursor correctly
  tft.setTextColor(GREEN);       //change text color
  tft.setTextSize(3);            //set text size
  tft.println("Adi Goldstein");  //that's me!

  tft.setTextColor(DARKBLUE);  //set text color
  tft.setCursor(30, 160);      //position cursor correctly
  tft.setTextSize(3);          //set text size
  tft.println("Faraj Kharbaoui");

  delay(3000);
}

void loop() 
{
  tft.setRotation(0);                       //set screen rotation
  tft.fillScreen(DARKBLUE);                 //set up background
  tft.drawRect(0, 0, 240, 320, WHITE);      //parameters: (x, y, length, width, color), this makes an outline of a rectangle
  tft.fillRect(100, 80, 60, 160, MAGENTA);  //parameters: (x, y, length, width, color), this makes the filled in part of a rectangle
  tft.drawRect(100, 80, 62, 162, WHITE);    //parameters: (x, y, length, width, color), this makes an outline of a rectangle
  tft.setRotation(1);                       //rotate screen 90 degrees, so the text appears correctly.
  tft.setTextColor(WHITE);                  //set text color
  tft.setCursor(30, 30);                    //position cursor correctly
  tft.setTextSize(4);                       //make text the right size
  tft.println("Tic-Tac-Toe");
  tft.setCursor(88, 90);         //start text at 80, 90.
  tft.setTextSize(5);            //set text size
  tft.println("Start");          //write "Start"
  tft.setTextColor(ORANGE);      //change text color
  tft.setCursor(30, 160);        //position cursor correctly
  tft.setTextSize(3);            //set text size
  tft.println("Adi Goldstein");  //that's me!
  tft.setTextColor(GREEN);       //change text color
  tft.setCursor(30, 195);        //position cursor correctly
  tft.setTextSize(3);            //set text size
  tft.println("Faraj Kharbaoui");
  tft.setTextColor(WHITE);  //set text color

  int i = 0;
  while (i == 0)
   {
    TSPoint p = ts.getPoint();
    if (p.z > 10 && p.y > 380 && p.y < 580 && p.x > 300 && p.x < 725)  //Check for button prss
    {
      /*		Buzzer tone		*/
      tone(BUZZER_PIN, 4699);
      delay(170);
      noTone(BUZZER_PIN);
      delay(170);
      i = 1;
    }
  }
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);


  tft.fillScreen(DARKBLUE);
  tft.setRotation(0);

  //Vertical bars
  tft.fillRect(0, 95, 250, 10, WHITE);
  tft.fillRect(0, 205, 250, 10, WHITE);

  //Horizontal bars
  tft.fillRect(65, 0, 10, 350, WHITE);
  tft.fillRect(150, 0, 10, 350, WHITE);
  delay(500);

  int j = 0;               //Counter the number of pressing buttons
  int button_pushed = 10;  //10 means that nothing is pressed

  for (int k = 0; k <= 8; k++)
   {
    buttonUsed[k] = 0;  //Clear the buttonUsed array
  }

  int turn_tracker = 0;
  while (j != 9) {
    while (button_pushed == 10) {
      button_pushed = checkButtons();
      delay(50);
    }

    if (turn_tracker == 0)  //if it's X's turn
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);

      //Draw the 'X'
      tft.setRotation(0);
      tft.setCursor(XandO_locations[button_pushed][0], XandO_locations[button_pushed][1]);
      tft.setTextColor(BLUE);
      tft.setTextSize(9);
      tft.print("x");
      turn_tracker = 1;  //now it's O's turn
      buttonUsed[button_pushed] = 1;
      j++;
      button_pushed = 10;

      /*		Buzzer tone		*/
      tone(BUZZER_PIN, 2200);
      delay(100);
      noTone(BUZZER_PIN);
      delay(100);
    }

    else if (turn_tracker == 1)  //if it's 'O's turn
    {
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);

      //Draw the O's
      tft.setRotation(0);
      tft.setCursor(XandO_locations[button_pushed][0], XandO_locations[button_pushed][1]);
      tft.setTextColor(RED);
      tft.setTextSize(9);
      tft.print("o");

      turn_tracker = 0;  //Now it's X's turn
      buttonUsed[button_pushed] = 2;
      j++;
      button_pushed = 10;

      /*		Buzzer tone		*/
      tone(BUZZER_PIN, 800);
      delay(120);
      noTone(BUZZER_PIN);
      delay(180);
    }

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    tft.setRotation(0);
    tft.setCursor(80, 90);
    tft.setTextColor(WHITE);
    tft.setTextSize(5);

    int winner = 0;
    winner = checkForWinner();
    if (winner == 1) //If X wins
    {  
      tft.fillScreen(BLACK);
      tft.setRotation(1);
      tft.setCursor(60, 100);
      tft.setTextColor(BLUE);
      tft.println("X wins!");  //Write "X wins!"

      /*		Buzzer tone		*/
      for (int i = 0; i < 8; i++)
       {
        int noteDuration = 1000 / tempo[i];
        tone(BUZZER_PIN, melody[i], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.70;
        delay(pauseBetweenNotes);
        noTone(BUZZER_PIN);
      }

      delay(2000);
      return;
    }

    else if (winner == 2) 
    {
      tft.fillScreen(BLACK);
      tft.setRotation(1);
      tft.setCursor(60, 100);
      tft.setTextColor(RED);
      tft.println("O wins!");  //Write "O wins!"

      /*		Buzzer tone		*/
      for (int i = 0; i < 8; i++) 
      {
        int noteDuration = 1000 / tempo[i];
        tone(BUZZER_PIN, melody[i], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.70;
        delay(pauseBetweenNotes);
        noTone(BUZZER_PIN);
      }

      delay(2000);
      return;
    }
  }
  delay(1000);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  tft.setRotation(1);
  tft.setCursor(40, 100);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.fillScreen(BLACK);
  tft.println("Its a Tie!");  //write "Its a Tie!"

  /*		Buzzer tone		*/
  for (int i = 0; i < 8; i++) 
  {
    int noteDuration = 1000 / tempo[i];
    tone(BUZZER_PIN, melody[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.70;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }

  delay(2000);
}

int checkButtons()
 {
  TSPoint p = ts.getPoint();    //Receive touchpad info
  for (int i = 0; i <= 8; i++)  //Loop for 9 times
  {
    int x_short = button_values[i][0];
    int y_short = button_values[i][1];
    int x_long = button_values[i][2];
    int y_long = button_values[i][3];

    //Check if the button press is inside those coordinants
    if (p.z > 10 && p.y > y_short && p.y < y_long && p.x > x_short && p.x < x_long && buttonUsed[i] == 0) 
    {
      return i;  //Skip the rest of the function, return the button number
    }
  }
  return 10;
}

/*    Check for winner function     */
int checkForWinner() 
{
  for (int i = 1; i <= 2; i++)  //for both 'X' and 'O'
  {
    if (buttonUsed[0] == i && buttonUsed[1] == i && buttonUsed[2] == i) 
    {
      return i;
    }

    if (buttonUsed[3] == i && buttonUsed[4] == i && buttonUsed[5] == i)
    {
      return i;
    }

    if (buttonUsed[6] == i && buttonUsed[7] == i && buttonUsed[8] == i) 
    {
      return i;
    }

    if (buttonUsed[0] == i && buttonUsed[3] == i && buttonUsed[6] == i) 
    {
      return i;
    }

    if (buttonUsed[1] == i && buttonUsed[4] == i && buttonUsed[7] == i) 
    {
      return i;
    }

    if (buttonUsed[2] == i && buttonUsed[5] == i && buttonUsed[8] == i) 
    {
      return i;
    }

    if (buttonUsed[0] == i && buttonUsed[4] == i && buttonUsed[8] == i) 
    {
      return i;
    }

    if (buttonUsed[2] == i && buttonUsed[4] == i && buttonUsed[6] == i) 
    {
      return i;
    }
  }

  return 0;
}
