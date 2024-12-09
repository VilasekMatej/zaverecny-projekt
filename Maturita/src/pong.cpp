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

// Tlačítka na pinech 0, 1 a 2
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

// Druhá pálka (na pravé straně)
int paddle2X;
int paddle2Y;
int paddle2Speed = 3;

int ballX, ballY;
int ballSpeedX = 2;
int ballSpeedY = 2;

int screenWidth = 320;
int screenHeight = 240 ;

// Funkce pro resetování míčku
void resetBall() {
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;
    ballSpeedX = random(2, 4) * (random(0, 2) * 2 - 1);  // Náhodný směr a rychlost
    ballSpeedY = random(2, 4) * (random(0, 2) * 2 - 1);
}

void setup() {
    tft.begin();
    tft.setRotation(3); // Nastavení orientace displeje
    Serial.begin(9600);

    // Inicializace tlačítek
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);

    // Inicializace pálky a míčku
    paddleY = screenHeight / 2 - paddleHeight / 2;
    paddle2X = screenWidth - paddleWidth - 10; // Druhá pálka na pravé straně
    paddle2Y = screenHeight / 2 - paddleHeight / 2;

    resetBall();

    // Vymazání obrazovky
    tft.fillScreen(ILI9341_BLACK);

    ts.InitTouch();
    ts.setPrecision(PREC_EXTREME);
}

void drawPaddle(int x, int y) {
    // Vykreslení pálky
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_WHITE);
}

void clearPaddle(int x, int y) {
    // Smazání pálky (černá barva)
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_BLACK);
}

void drawBall() {
    // Vykreslení míčku
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_WHITE);
}

void clearBall() {
    // Smazání míčku (černá barva)
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);
}

void moveBall() {
    clearBall(); // Smaže předchozí pozici míčku
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Kontrola nárazů na horní a dolní stěnu
    if (ballY <= 0 || ballY >= screenHeight - ballSize) {
        ballSpeedY = -ballSpeedY; // Změna směru na ose Y
    }

    // **Kolize s levou pálkou**
    if (ballX <= paddleX + paddleWidth && ballX >= paddleX) {
        // Zkontroluj, zda je míček v rozsahu výšky pálky
        if (ballY + ballSize >= paddleY && ballY <= paddleY + paddleHeight) {
            ballSpeedX = -ballSpeedX; // Změna směru na ose X
            ballX = paddleX + paddleWidth; // Posun míčku mimo pálku, aby se vyhnul uvíznutí
            
            // Zrychlení míčku o 5 %
            ballSpeedX *= 1.4;
            ballSpeedY *= 1.4;
            paddle2Speed *= 1.4;
        }
    }

    // **Kolize s pravou pálkou**
    if (ballX + ballSize >= paddle2X && ballX <= paddle2X + paddleWidth) {
        // Zkontroluj, zda je míček v rozsahu výšky pálky
        if (ballY + ballSize >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
            ballSpeedX = -ballSpeedX; // Změna směru na ose X
            ballX = paddle2X - ballSize; // Posun míčku mimo pálku, aby se vyhnul uvíznutí

            // Zrychlení míčku o 5 %
            ballSpeedX *= 1.4;
            ballSpeedY *= 1.4;
            paddle2Speed *= 1.4;
        }
    }

    // Kontrola, zda míček opustil obrazovku (prohra)
    if (ballX <= 0 || ballX >= screenWidth - ballSize) {
        resetBall();
    }

    drawBall(); // Vykreslí míček na nové pozici
}


void movePaddle() {
    clearPaddle(paddleX, paddleY); // Smaže předchozí pozici pálky

    // Pohyb pálky podle stisknutých tlačítek
    if (digitalRead(BUTTON_LEFT) == LOW && paddleY < screenHeight - paddleHeight) { // Pohyb doleva (snižování hodnoty X)
        paddleY += paddleSpeed; 
    }
    
    if (digitalRead(BUTTON_RIGHT) == LOW && paddleY > 0) { // Pohyb doprava (zvyšování hodnoty X)
        paddleY -= paddleSpeed; 
    }

    // Pohyb pálky pomocí dotyku
    if (ts.dataAvailable()) {
        ts.read();
        int touchY = ts.getY();
        if (touchY != -1) {
            paddleY = screenHeight - touchY - paddleHeight / 2; // Inverze osy Y
        }
    }

    // Zajištění, že pálka zůstane v rámci obrazovky (na horní a dolní hranici)
    if (paddleY < 0) {
        paddleY = 0; // Pálka se nemůže dostat nad obrazovku
    }
    if (paddleY > screenHeight - paddleHeight) {
        paddleY = screenHeight - paddleHeight; // Pálka se nemůže dostat pod obrazovku
    }

    drawPaddle(paddleX, paddleY); // Vykreslí pálku na nové pozici
}



void movePaddle2() {
    clearPaddle(paddle2X, paddle2Y); // Smaže předchozí pozici pálky 2

    // Jednoduchá AI pro pohyb druhé pálky (automatický pohyb za míčkem)
    if (ballY < paddle2Y && paddle2Y > 0) {
        paddle2Y -= paddle2Speed;
    }
    if (ballY > paddle2Y + paddleHeight && paddle2Y < screenHeight - paddleHeight) {
        paddle2Y += paddle2Speed;
    }


    drawPaddle(paddle2X, paddle2Y); // Vykreslí druhou pálku na nové pozici
}

void loop() {
    movePaddle();   // Pohyb první pálky
    movePaddle2();  // Pohyb druhé pálky (AI)
    moveBall();     // Pohyb míčku

    delay(10); // Rychlejší vykreslování
}
