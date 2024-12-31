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
int paddle2Speed = 5;

int ballX, ballY;
int ballSpeedX = 2;
int ballSpeedY = 2;

int screenWidth = 320;
int screenHeight = 240 ;

// Parametry pro zrychlení míčku
float ballSpeedMultiplier = 1.1; // Faktor pro zrychlení míčku

// Funkce pro resetování míčku
void resetBall() {
    ballX = screenWidth / 2;
    ballY = screenHeight / 2;
    ballSpeedX = random(2, 4) * (random(0, 2) * 2 - 1);  // Náhodný směr a rychlost
    ballSpeedY = random(2, 4) * (random(0, 2) * 2 - 1);
}

// Nastavení počátečních hodnot zrychlení
void resetSpeed() {
    ballSpeedX = random(2, 4) * (random(0, 2) * 2 - 1); 
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

// Funkce pro vykreslení pálky
void drawPaddle(int x, int y) {
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_WHITE);
}

// Funkce pro smazání pálky
void clearPaddle(int x, int y) {
    tft.fillRect(x, y, paddleWidth, paddleHeight, ILI9341_BLACK);
}

// Funkce pro vykreslení míčku
void drawBall() {
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_WHITE);
}

// Funkce pro smazání míčku
void clearBall() {
    tft.fillRect(ballX, ballY, ballSize, ballSize, ILI9341_BLACK);
}

// Funkce pro pohyb míčku
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
        if (ballY + ballSize >= paddleY && ballY <= paddleY + paddleHeight) {
            ballSpeedX = -ballSpeedX; // Změna směru na ose X
            ballX = paddleX + paddleWidth; // Posun míčku mimo pálku
            
            // Zrychlení míčku o 40%
            ballSpeedX *= ballSpeedMultiplier; 
            ballSpeedY *= ballSpeedMultiplier;
        }
    }

    // **Kolize s pravou pálkou**
    if (ballX + ballSize >= paddle2X && ballX <= paddle2X + paddleWidth) {
        if (ballY + ballSize >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
            ballSpeedX = -ballSpeedX;
            ballX = paddle2X - ballSize; // Posun míčku mimo pálku

            // Zrychlení míčku o 40%
            ballSpeedX *= ballSpeedMultiplier; 
            ballSpeedY *= ballSpeedMultiplier;
        }
    }

    // Kontrola, zda míček opustil obrazovku (prohra)
    if (ballX <= 0 || ballX >= screenWidth - ballSize) {
        resetBall();
        resetSpeed();  // Resetujeme rychlost po resetu míčku
    }

    drawBall(); // Vykreslí míček na nové pozici
}

// Funkce pro pohyb pálky
void movePaddle() {
    clearPaddle(paddleX, paddleY); // Smaže předchozí pozici pálky

    // Pohyb pálky podle stisknutých tlačítek
    if (digitalRead(BUTTON_LEFT) == LOW && paddleY < screenHeight - paddleHeight) { 
        paddleY += paddleSpeed; 
    }
    
    if (digitalRead(BUTTON_RIGHT) == LOW && paddleY > 0) { 
        paddleY -= paddleSpeed; 
    }

    // Pohyb pálky pomocí dotyku
    if (ts.dataAvailable()) {
        ts.read();
        int touchY = ts.getY();
        if (touchY != -1) {
            paddleY = screenHeight - touchY - paddleHeight / 2; 
        }
    }

    // Zajištění, že pálka zůstane v rámci obrazovky
    if (paddleY < 0) {
        paddleY = 0; 
    }
    if (paddleY > screenHeight - paddleHeight) {
        paddleY = screenHeight - paddleHeight;
    }

    drawPaddle(paddleX, paddleY); // Vykreslí pálku na nové pozici
}

// Funkce pro pohyb druhé pálky
void movePaddle2() {
    clearPaddle(paddle2X, paddle2Y); 

    // Jednoduchá AI pro pohyb druhé pálky
    if (ballY < paddle2Y && paddle2Y > 0) {
        paddle2Y -= paddle2Speed;
    }
    if (ballY > paddle2Y + paddleHeight && paddle2Y < screenHeight - paddleHeight) {
        paddle2Y += paddle2Speed;
    }

    // Ujistíme se, že druhá pálka se vždy pohybuje hladce, aniž by zůstala "zaseknutá"
    if (paddle2Y < 0) {
        paddle2Y = 0;
    }
    if (paddle2Y > screenHeight - paddleHeight) {
        paddle2Y = screenHeight - paddleHeight;
    }

    drawPaddle(paddle2X, paddle2Y); // Vykreslí druhou pálku na nové pozici
}

// Funkce pro zajištění plynulosti bez použití delay
unsigned long previousMillis = 0;
const long interval = 10; // Interval pro aktualizaci (10 ms)

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        movePaddle();   // Pohyb první pálky
        movePaddle2();  // Pohyb druhé pálky (AI)
        moveBall();     // Pohyb míčku
    }
}
