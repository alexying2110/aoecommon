#include <SPI.h>
#include <TFT_22_ILI9225.h>
#include <EEPROM.h>

#define CLF 13
#define SDA 12
#define RS 11
#define RST 10
#define CS 9
#define LED 3
#define BRIGHTNESS 200
#define BUTTON 1
#define YMAX 175
#define XMAX 219

TFT_22_ILI9225 tft = TFT_22_ILI9225(RST, RS, CS, LED, BRIGHTNESS);
/* tbh these variables are assigned in setup/initGame already but i'm too scared to fix this right now */
/* variables controlling pipe dimensions */
const int THICCNESS = 22;
const int GAP = 58;
/* x and y of pipe 1 and 2 respectively */
int xP1 = XMAX;
int yP1 = random(GAP / 2 + 4, 150 - GAP / 2);
int xP2 = XMAX * 3 / 2;
int yP2 = random(GAP / 2 + 4, 150 - GAP / 2);
/* temp variable for pipes about to be rendered */
int yP1Temp = yP1;
int yP2Temp = yP2;
int pipeRate = 1;
/* bird height */
int yB = 50;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
boolean held = false;
boolean gameStarted = false;
int pressed = 0;
char buffer[3];
int highScore;

void drawBird(int y) {
  /* erase old */
  int yOld = y + fallRateInt;
  if (fallRateInt < 0) {
    for (int r = yOld; r < y; r++) {
      for (int c = 25; c < 42; c++) {
        tft.drawPixel(c, r, COLOR_TURQUOISE);
      }
    }
  }  
  if (fallRateInt >= 0) {
    yOld = y + 12 + fallRateInt;
    for (int r = y + 12; r <= yOld; r++) {
      for (int c = 25; c < 42; c++) {
        tft.drawPixel(c, r, COLOR_TURQUOISE);
      }
    }
  }
  /* THE ACTUAL PIXEL ART */
  /* line 1 */
  tft.drawPixel(25, y, COLOR_TURQUOISE);
  tft.drawPixel(26, y, COLOR_TURQUOISE);
  tft.drawPixel(27, y, COLOR_TURQUOISE);
  tft.drawPixel(28, y, COLOR_TURQUOISE);
  tft.drawPixel(29, y, COLOR_TURQUOISE);
  tft.drawPixel(30, y, COLOR_TURQUOISE);
  tft.drawPixel(31, y, COLOR_BLACK);
  tft.drawPixel(32, y, COLOR_BLACK);
  tft.drawPixel(33, y, COLOR_BLACK);
  tft.drawPixel(34, y, COLOR_BLACK);
  tft.drawPixel(35, y, COLOR_BLACK);
  tft.drawPixel(36, y, COLOR_BLACK);
  tft.drawPixel(37, y, COLOR_TURQUOISE);
  tft.drawPixel(38, y, COLOR_TURQUOISE);
  tft.drawPixel(39, y, COLOR_TURQUOISE);
  tft.drawPixel(40, y, COLOR_TURQUOISE);
  tft.drawPixel(41, y, COLOR_TURQUOISE);
  /* line 3 */
  tft.drawPixel(25, y + 2, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 2, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 2, COLOR_TURQUOISE);
  tft.drawPixel(28, y + 2, COLOR_BLACK);
  tft.drawPixel(29, y + 2, COLOR_GOLD);
  tft.drawPixel(30, y + 2, COLOR_GOLD);
  tft.drawPixel(31, y + 2, COLOR_GOLD);
  tft.drawPixel(32, y + 2, COLOR_GOLD);
  tft.drawPixel(33, y + 2, COLOR_GOLD);
  tft.drawPixel(34, y + 2, COLOR_BLACK);
  tft.drawPixel(35, y + 2, COLOR_WHITE);
  tft.drawPixel(36, y + 2, COLOR_WHITE);
  tft.drawPixel(37, y + 2, COLOR_WHITE);
  tft.drawPixel(38, y + 2, COLOR_BLACK);
  tft.drawPixel(39, y + 2, COLOR_TURQUOISE);
  tft.drawPixel(40, y + 2, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 2, COLOR_TURQUOISE);
  /* line 5 */
  tft.drawPixel(25, y + 4, COLOR_BLACK);
  tft.drawPixel(26, y + 4, COLOR_WHITE);
  tft.drawPixel(27, y + 4, COLOR_WHITE);
  tft.drawPixel(28, y + 4, COLOR_WHITE);
  tft.drawPixel(29, y + 4, COLOR_WHITE);
  tft.drawPixel(30, y + 4, COLOR_BLACK);
  tft.drawPixel(31, y + 4, COLOR_GOLD);
  tft.drawPixel(32, y + 4, COLOR_GOLD);
  tft.drawPixel(33, y + 4, COLOR_GOLD);
  tft.drawPixel(34, y + 4, COLOR_BLACK);
  tft.drawPixel(35, y + 4, COLOR_WHITE);
  tft.drawPixel(36, y + 4, COLOR_WHITE);
  tft.drawPixel(37, y + 4, COLOR_BLACK);
  tft.drawPixel(38, y + 4, COLOR_WHITE);
  tft.drawPixel(39, y + 4, COLOR_BLACK);
  tft.drawPixel(40, y + 4, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 4, COLOR_TURQUOISE);
  /* line 7 */
  tft.drawPixel(25, y + 6, COLOR_BLACK);
  tft.drawPixel(26, y + 6, COLOR_GOLD);
  tft.drawPixel(27, y + 6, COLOR_WHITE);
  tft.drawPixel(28, y + 6, COLOR_WHITE);
  tft.drawPixel(29, y + 6, COLOR_WHITE);
  tft.drawPixel(30, y + 6, COLOR_GOLD);
  tft.drawPixel(31, y + 6, COLOR_BLACK);
  tft.drawPixel(32, y + 6, COLOR_GOLD);
  tft.drawPixel(33, y + 6, COLOR_GOLD);
  tft.drawPixel(34, y + 6, COLOR_GOLD);
  tft.drawPixel(35, y + 6, COLOR_GOLD);
  tft.drawPixel(36, y + 6, COLOR_BLACK);
  tft.drawPixel(37, y + 6, COLOR_BLACK);
  tft.drawPixel(38, y + 6, COLOR_BLACK);
  tft.drawPixel(39, y + 6, COLOR_BLACK);
  tft.drawPixel(40, y + 6, COLOR_BLACK);
  tft.drawPixel(41, y + 6, COLOR_TURQUOISE);
  /* line 9 */
  tft.drawPixel(25, y + 8, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 8, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 8, COLOR_BLACK);
  tft.drawPixel(28, y + 8, COLOR_BLACK);
  tft.drawPixel(29, y + 8, COLOR_BLACK);
  tft.drawPixel(30, y + 8, COLOR_GOLD);
  tft.drawPixel(31, y + 8, COLOR_GOLD);
  tft.drawPixel(32, y + 8, COLOR_GOLD);
  tft.drawPixel(33, y + 8, COLOR_GOLD);
  tft.drawPixel(34, y + 8, COLOR_BLACK);
  tft.drawPixel(35, y + 8, COLOR_ORANGE);
  tft.drawPixel(36, y + 8, COLOR_BLACK);
  tft.drawPixel(37, y + 8, COLOR_BLACK);
  tft.drawPixel(38, y + 8, COLOR_BLACK);
  tft.drawPixel(39, y + 8, COLOR_BLACK);
  tft.drawPixel(40, y + 8, COLOR_BLACK);
  tft.drawPixel(41, y + 8, COLOR_TURQUOISE);
  /* line 11 */
  tft.drawPixel(25, y + 10, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 10, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 10, COLOR_TURQUOISE);
  tft.drawPixel(28, y + 10, COLOR_TURQUOISE);
  tft.drawPixel(29, y + 10, COLOR_TURQUOISE);
  tft.drawPixel(30, y + 10, COLOR_BLACK);
  tft.drawPixel(31, y + 10, COLOR_BLACK);
  tft.drawPixel(32, y + 10, COLOR_GOLD);
  tft.drawPixel(33, y + 10, COLOR_GOLD);
  tft.drawPixel(34, y + 10, COLOR_GOLD);
  tft.drawPixel(35, y + 10, COLOR_GOLD);
  tft.drawPixel(36, y + 10, COLOR_BLACK);
  tft.drawPixel(37, y + 10, COLOR_BLACK);
  tft.drawPixel(38, y + 10, COLOR_BLACK);
  tft.drawPixel(39, y + 10, COLOR_BLACK);
  tft.drawPixel(40, y + 10, COLOR_BLACK);
  tft.drawPixel(41, y + 10, COLOR_TURQUOISE);
  /* line 2 */
  tft.drawPixel(25, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(28, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(29, y + 1, COLOR_BLACK);
  tft.drawPixel(30, y + 1, COLOR_BLACK);
  tft.drawPixel(31, y + 1, COLOR_GOLD);
  tft.drawPixel(32, y + 1, COLOR_GOLD);
  tft.drawPixel(33, y + 1, COLOR_GOLD);
  tft.drawPixel(34, y + 1, COLOR_GOLD);
  tft.drawPixel(35, y + 1, COLOR_BLACK);
  tft.drawPixel(36, y + 1, COLOR_WHITE);
  tft.drawPixel(37, y + 1, COLOR_BLACK);
  tft.drawPixel(38, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(39, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(40, y + 1, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 1, COLOR_TURQUOISE);
  /* line 4 */
  tft.drawPixel(25, y + 3, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 3, COLOR_BLACK);
  tft.drawPixel(27, y + 3, COLOR_BLACK);
  tft.drawPixel(28, y + 3, COLOR_BLACK);
  tft.drawPixel(29, y + 3, COLOR_BLACK);
  tft.drawPixel(30, y + 3, COLOR_GOLD);
  tft.drawPixel(31, y + 3, COLOR_GOLD);
  tft.drawPixel(32, y + 3, COLOR_GOLD);
  tft.drawPixel(33, y + 3, COLOR_GOLD);
  tft.drawPixel(34, y + 3, COLOR_BLACK);
  tft.drawPixel(35, y + 3, COLOR_WHITE);
  tft.drawPixel(36, y + 3, COLOR_WHITE);
  tft.drawPixel(37, y + 3, COLOR_BLACK);
  tft.drawPixel(38, y + 3, COLOR_WHITE);
  tft.drawPixel(39, y + 3, COLOR_BLACK);
  tft.drawPixel(40, y + 3, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 3, COLOR_TURQUOISE);
  /* line 6 */
  tft.drawPixel(25, y + 5, COLOR_BLACK);
  tft.drawPixel(26, y + 5, COLOR_WHITE);
  tft.drawPixel(27, y + 5, COLOR_WHITE);
  tft.drawPixel(28, y + 5, COLOR_WHITE);
  tft.drawPixel(29, y + 5, COLOR_WHITE);
  tft.drawPixel(30, y + 5, COLOR_WHITE);
  tft.drawPixel(31, y + 5, COLOR_BLACK);
  tft.drawPixel(32, y + 5, COLOR_GOLD);
  tft.drawPixel(33, y + 5, COLOR_GOLD);
  tft.drawPixel(34, y + 5, COLOR_GOLD);
  tft.drawPixel(35, y + 5, COLOR_BLACK);
  tft.drawPixel(36, y + 5, COLOR_WHITE);
  tft.drawPixel(37, y + 5, COLOR_WHITE);
  tft.drawPixel(38, y + 5, COLOR_WHITE);
  tft.drawPixel(39, y + 5, COLOR_BLACK);
  tft.drawPixel(40, y + 5, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 5, COLOR_TURQUOISE);
  /* line 8 */
  tft.drawPixel(25, y + 7, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 7, COLOR_BLACK);
  tft.drawPixel(27, y + 7, COLOR_GOLD);
  tft.drawPixel(28, y + 7, COLOR_GOLD);
  tft.drawPixel(29, y + 7, COLOR_GOLD);
  tft.drawPixel(30, y + 7, COLOR_BLACK);
  tft.drawPixel(31, y + 7, COLOR_GOLD);
  tft.drawPixel(32, y + 7, COLOR_GOLD);
  tft.drawPixel(33, y + 7, COLOR_GOLD);
  tft.drawPixel(34, y + 7, COLOR_GOLD);
  tft.drawPixel(35, y + 7, COLOR_BLACK);
  tft.drawPixel(36, y + 7, COLOR_ORANGE);
  tft.drawPixel(37, y + 7, COLOR_ORANGE);
  tft.drawPixel(38, y + 7, COLOR_ORANGE);
  tft.drawPixel(39, y + 7, COLOR_ORANGE);
  tft.drawPixel(40, y + 7, COLOR_ORANGE);
  tft.drawPixel(41, y + 7, COLOR_BLACK);
  /* line 10 */
  tft.drawPixel(25, y + 9, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 9, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 9, COLOR_TURQUOISE);
  tft.drawPixel(28, y + 9, COLOR_TURQUOISE);
  tft.drawPixel(29, y + 9, COLOR_BLACK);
  tft.drawPixel(30, y + 9, COLOR_GOLD);
  tft.drawPixel(31, y + 9, COLOR_GOLD);
  tft.drawPixel(32, y + 9, COLOR_GOLD);
  tft.drawPixel(33, y + 9, COLOR_GOLD);
  tft.drawPixel(34, y + 9, COLOR_GOLD);
  tft.drawPixel(35, y + 9, COLOR_BLACK);
  tft.drawPixel(36, y + 9, COLOR_ORANGE);
  tft.drawPixel(37, y + 9, COLOR_ORANGE);
  tft.drawPixel(38, y + 9, COLOR_ORANGE);
  tft.drawPixel(39, y + 9, COLOR_ORANGE);
  tft.drawPixel(40, y + 9, COLOR_BLACK);
  tft.drawPixel(41, y + 9, COLOR_TURQUOISE);
  /* line 12 */
  tft.drawPixel(25, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(26, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(27, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(28, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(29, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(30, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(31, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(32, y + 11, COLOR_BLACK);
  tft.drawPixel(33, y + 11, COLOR_BLACK);
  tft.drawPixel(34, y + 11, COLOR_BLACK);
  tft.drawPixel(35, y + 11, COLOR_BLACK);
  tft.drawPixel(36, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(37, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(38, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(39, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(40, y + 11, COLOR_TURQUOISE);
  tft.drawPixel(41, y + 11, COLOR_TURQUOISE);
}

void drawPipe(int x, bool pipe1) {
  int xEnd = x + pipeRate + 1;
  int y1End = yP2Temp - GAP / 2;
  int y2End = yP2Temp + GAP / 2;
  if (pipe1) {
    y1End = yP1Temp - GAP / 2;
    y2End = yP1Temp + GAP / 2;
  }
  /* this is ugly but works */
  for (int c1 = x; c1 < xEnd; c1++) {
    if (c1 < 0) {
      c1 += XMAX + 1;
      if (pipe1) {
        y1End = yP1 - GAP / 2;
        y2End = yP1 + GAP / 2;
      } else {
        y1End = yP2 - GAP / 2;
        y2End = yP2 + GAP / 2;
      }
    }
    /* slight optimizations to reduce number of pixels drawn and speed up game */
    for (int y1 = 0 + c1 % 4; y1 < y1End; y1 += 4) {
      tft.drawPixel(c1, y1, COLOR_DARKGREEN);
    }
    for (int y2 = y2End / 4 * 4 + c1 % 4; y2 < 150; y2 += 4) {
      tft.drawPixel(c1, y2, COLOR_DARKGREEN);
    }
  }
  /* erases old pipe line */
  int xErase = x + pipeRate + THICCNESS;
  for (int c2 = x + THICCNESS; c2 < xErase; c2++) {
    for (int y1 = 0 + c2 % 4; y1 < 150; y1 += 4) {
      tft.drawPixel(c2, y1, COLOR_TURQUOISE);
    }
  }
}

void initGame() {
  tft.setOrientation(3);
  tft.setFont(Terminal12x16);
  tft.fillRectangle(0, 0, XMAX, 149, COLOR_TURQUOISE);
  tft.fillRectangle(0, 150, XMAX, 160, COLOR_DARKGREEN);
  tft.fillRectangle(0, 161, XMAX, YMAX, COLOR_SIENNA);
  tft.setBackgroundColor(COLOR_TURQUOISE);
  tft.drawText(XMAX / 2 - 78, 50, "JUMP TO START", COLOR_WHITE);
  tft.fillTriangle(XMAX / 2 + 69, 50, XMAX / 2 + 69, 63, XMAX / 2 + 80, 57, COLOR_WHITE);
  tft.drawText(XMAX / 2 - 60, 88, "High Score:", COLOR_WHITE);
  tft.drawText(XMAX / 2 - 8, 118, itoa(highScore, buffer, 10), COLOR_WHITE); 
  while (!gameStarted) {
    pressed = digitalRead(BUTTON);
    if (pressed == LOW) {
      gameStarted = true;
      tft.fillRectangle(XMAX / 2 - 78, 80, XMAX / 2 + 80, 93, COLOR_TURQUOISE);
    }
  }
  tft.setBackgroundColor(COLOR_SIENNA);
  tft.drawText(XMAX / 2 - 8, 161, itoa(score, buffer, 10), COLOR_WHITE);
  loop();
}

void endScreen() {
  pressed = HIGH;
  tft.setBackgroundColor(COLOR_TURQUOISE);
  tft.fillRectangle(0, 0, XMAX, 149, COLOR_TURQUOISE);
  tft.fillRectangle(0, 161, XMAX, YMAX, COLOR_SIENNA);
  if (score > highScore) {
    tft.drawText(XMAX / 2 - 55, 50, "GAME OVER!", COLOR_WHITE);
    tft.drawText(XMAX / 2 - 60, 88, "Your Score:", COLOR_WHITE);
    tft.drawText(XMAX / 2 - 8, 118, itoa(score, buffer, 10), COLOR_WHITE);
  } else {
    EEPROM.write(0, score);
    tft.drawText(XMAX / 2 - 55, 30, "GAME OVER!", COLOR_WHITE);
    tft.drawText(XMAX / 2 - 60, 68, "Your Score:", COLOR_WHITE);
    tft.drawText(XMAX / 2 - 8, 98, itoa(score, buffer, 10), COLOR_WHITE);
    tft.drawText(XMAX / 2 - 75, 118, "NEW HIGH SCORE!", COLOR_WHITE);
  }
  while (true) {
    pressed = digitalRead(BUTTON);
    if (pressed == LOW) {
      break;
    }
  }
  xP1 = XMAX;
  yP1 = random(GAP / 2 + 4, 150 - GAP / 2);
  yP1Temp = yP1;
  xP2 = XMAX * 3 / 2;
  yP2 = random(GAP / 2 + 4, 150 - GAP / 2);
  yP2Temp = yP2;
  yB = 50;
  held = false;
  gameStarted = false;
  fallRateInt = 0;
  fallRate = 0;
  score = 0;
  initGame();
}

void setup() {
  Serial.begin(8000);
  pinMode(BUTTON, INPUT);
  tft.begin();
  tft.clear();
  highScore = EEPROM.read(0);
  initGame();
}

void loop() {
  xP1 -= pipeRate;
  xP2 -= pipeRate;
  drawBird(yB);
  drawPipe(xP1, true);
  drawPipe(xP2, false);
  if (xP1 == 0) {
    yP1 = random(GAP / 2 + 4, 150 - GAP / 2);
  }
  if (xP1 < -THICCNESS) {
    xP1 += XMAX + 1;
    yP1Temp = yP1;
  }
  if (xP2 == 0) {
    yP2 = random(GAP / 2 + 4, 150 - GAP / 2);
  }
  if (xP2 < -THICCNESS) {
    xP2 += XMAX + 1;
    yP2Temp = yP2;
  }
  yB -= fallRateInt;
  fallRate += 0.32;
  fallRateInt = -int(fallRate);
  pressed = digitalRead(BUTTON);
  /* draw the score as infrequently as possible because it's expensive */
  if (xP1 + THICCNESS == 25 || xP2 + THICCNESS == 25) {
    score++;
    /* if score > 100 it goes up by 10 for whatever reason probaby because of the char buffer 
     *  but idk how to fix that right now. I wonder where it will overflow
     */
    tft.drawText(XMAX / 2 - 8, 161, itoa(score, buffer, 10), COLOR_WHITE);
  }
  /* prevent holding */
  if (pressed == LOW && !held) {
    fallRate = -5;
    held = true;
  } else if (pressed == HIGH && held) {
    held = false;
  }
  /* basic collision detection */
  if (yB <= -2) {
    endScreen();
  }
  if (yB >= 138) {
    endScreen();
  }
  if (xP1 < 41 && xP1 + THICCNESS > 25) {
    if (yB < yP1 - GAP / 2 + 1 || yB + 12 > yP1 + GAP / 2 - 1) {
      endScreen();
    }
  }
  if (xP2 < 42 && xP2 + THICCNESS > 24) {
    if (yB < yP2 - GAP / 2 + 1 || yB + 12 > yP2 + GAP / 2 - 1) {
      endScreen();
    }
  }
 }
