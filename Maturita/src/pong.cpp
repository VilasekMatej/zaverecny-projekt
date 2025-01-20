#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "URTouch.h"
#include "Arduino.h"

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CLK 13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define t_SCK 3
#define t_CS 4
#define t_MOSI 5
#define t_MISO 6
#define t_IRQ 7

URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

// Tlačítka na pinech
#define BUTTON_UP A2
#define BUTTON_LEFT A3
#define BUTTON_DOWN A4
#define BUTTON_RIGHT A5

// Parametry hry
int paddleHeight = 40;
int paddleWidth = 5;
int ballSize = 5;

int paddleX = 10;
int paddleY;
int paddleSpeed = 5;

int paddle2X;
int paddle2Y;
int paddle2Speed = 5;

int ballX, ballY;
float ballSpeedX = 2;
float ballSpeedY = 2;

int screenWidth = 320;
int screenHeight = 240;

int scoreZoneHeight = 30;

int scorePlayer1 = 0;
int scorePlayer2 = 0;

// Parametry pro zrychlení míčku
float ballSpeedMultiplier = 1.34; 
float currentSpeedMultiplier = 1.0; 

// Funkce pro resetování míčku
void resetBall(bool towardsPlayer1) {
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;

    // Resetujeme rychlost míčku na základní hodnoty
    ballSpeedX = random(2, 4) * (towardsPlayer1 ? -1 : 1); // Směr závisí na parametru
    ballSpeedY = random(2, 4) * (random(0, 2) * 2 - 1);    // Náhodný směr nahoru/dol
    currentSpeedMultiplier = 1.0;
}

// Aktualizace skóre na obrazovce
void updateScore() {
    tft.fillRect(0, 0, screenWidth, scoreZoneHeight, ILI9341_BLACK);

    int scoreLineY = scoreZoneHeight - 2;
    tft.drawLine(0, scoreLineY, screenWidth, scoreLineY, ILI9341_WHITE);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    int player1ScoreX = 8;
    int player1ScoreY = 290;
    int player2ScoreY = 10;

    tft.setRotation(0);
    tft.setCursor(player1ScoreX, player2ScoreY);
    tft.print(scorePlayer2);

    tft.setCursor(player1ScoreX, player1ScoreY);
    tft.print(scorePlayer1);

    tft.setRotation(3);
}

void setup() {
    tft.begin();
    tft.setRotation(3);
    Serial.begin(9600);

    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);

    paddleY = (screenHeight + scoreZoneHeight) / 2 - paddleHeight / 2;
    paddle2X = screenWidth - paddleWidth - 10;
    paddle2Y = (screenHeight + scoreZoneHeight) / 2 - paddleHeight / 2;

    resetBall(true);

    tft.fillScreen(ILI9341_BLACK);
    updateScore();

    ts.InitTouch();
    ts.setPrecision(PREC_EXTREME);
}

void drawPaddle(int x, int y) {
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_WHITE);
}

void clearPaddle(int x, int y) {
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_BLACK);
}

void drawBall() {
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_WHITE);
}

void clearBall() {
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);
}

void moveBall() {
    clearBall();
    ballX += ballSpeedX * currentSpeedMultiplier;
    ballY += ballSpeedY * currentSpeedMultiplier;

    if (ballY <= scoreZoneHeight || ballY >= screenHeight - ballSize) {
        ballSpeedY = -ballSpeedY;
    }

    if (ballX <= paddleX + paddleWidth && ballX >= paddleX) {
        if (ballY + ballSize >= paddleY && ballY <= paddleY + paddleHeight) {
            ballSpeedX = -ballSpeedX;
            ballX = paddleX + paddleWidth;

            currentSpeedMultiplier *= ballSpeedMultiplier;
        }
    }

    if (ballX + ballSize >= paddle2X && ballX <= paddle2X + paddleWidth) {
        if (ballY + ballSize >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
            ballSpeedX = -ballSpeedX;
            ballX = paddle2X - ballSize;

            currentSpeedMultiplier *= ballSpeedMultiplier;
        }
    }

    if (ballX <= 0) {
        scorePlayer2++;
        updateScore();
        resetBall(false);
    } else if (ballX >= screenWidth - ballSize) {
        scorePlayer1++;
        updateScore();
        resetBall(true);
    }

    drawBall();
}

void movePaddle() {
    clearPaddle(paddleX, paddleY);

    if (digitalRead(BUTTON_LEFT) == LOW && paddleY < screenHeight - paddleHeight) { 
        paddleY += paddleSpeed; 
    }
    if (digitalRead(BUTTON_RIGHT) == LOW && paddleY > scoreZoneHeight) { 
        paddleY -= paddleSpeed; 
    }

    if (ts.dataAvailable()) {
        ts.read();
        int touchY = ts.getY();
        if (touchY != -1) {
            paddleY = screenHeight - touchY - paddleHeight / 2; 
        }
    }

    drawPaddle(paddleX, paddleY);
}

void movePaddle2() {
    clearPaddle(paddle2X, paddle2Y);

    int targetY = constrain(ballY - paddleHeight / 2, scoreZoneHeight, screenHeight - paddleHeight);

    if (paddle2Y < targetY) {
        paddle2Y += min(paddle2Speed, targetY - paddle2Y);
    } else if (paddle2Y > targetY) {
        paddle2Y -= min(paddle2Speed, paddle2Y - targetY);
    }

    drawPaddle(paddle2X, paddle2Y);
}

unsigned long previousMillis = 0;
const long interval = 10;

void displayMessage(String message, uint16_t color) {
    tft.fillScreen(color);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);

    int textWidth = message.length() * 6 * 3;
    int textHeight = 6 * 3;

    int xPos = (screenWidth - 80 - textWidth) / 2;
    int yPos = (screenHeight - textHeight) / 2;

    tft.setCursor(xPos, yPos);
    tft.print(message);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        movePaddle();
        movePaddle2();
        moveBall();
    }

    if (scorePlayer1 == 3) {
        tft.setRotation(0);
        displayMessage("You win", ILI9341_GREEN);
        while (true) {};
    }
    if (scorePlayer2 == 5) {
        tft.setRotation(0);
        displayMessage("You lose", ILI9341_RED);
        while (true) {};
    }
}
