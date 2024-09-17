#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_CLK 13

#define BTN_LEFT 2
#define BTN_RIGHT 3
#define BTN_ROTATE 4
#define BTN_DOWN 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Herní pole
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define BLOCK_SIZE 10

// Typy Tetris bloků
const uint16_t COLORS[] = {
  ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE, ILI9341_YELLOW,
  ILI9341_CYAN, ILI9341_MAGENTA, ILI9341_WHITE, ILI9341_ORANGE
};

// Tetris bloky (každý blok je reprezentován jako 4x4 bitová matice)
const uint16_t TETROMINOS[7][4] = {
  {0x0F00, 0x4444, 0x0F00, 0x4444},  // I
  {0x6660, 0x6660, 0x6660, 0x6660},  // O
  {0x0E40, 0x4C40, 0x0E40, 0x4C40},  // T
  {0x0660, 0x0C60, 0x0660, 0x0C60},  // S
  {0x0C60, 0x0660, 0x0C60, 0x0660},  // Z
  {0x0E40, 0x4C00, 0x4E00, 0xC440},  // J
  {0x0E00, 0x8C00, 0x4E00, 0xC200}   // L
};

// Herní pole
uint8_t field[FIELD_WIDTH][FIELD_HEIGHT] = {0};

// Funkce pro vykreslení herního pole
void drawField() {
  tft.fillScreen(ILI9341_BLACK);
  for (int x = 0; x < FIELD_WIDTH; x++) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
      if (field[x][y]) {
        tft.fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, COLORS[field[x][y] - 1]);
      }
    }
  }
}

// Funkce pro obsluhu tlačítek
void checkButtons() {
  if (digitalRead(BTN_LEFT) == LOW) {
    // Logika pro pohyb vlevo
  }
  if (digitalRead(BTN_RIGHT) == LOW) {
    // Logika pro pohyb vpravo
  }
  if (digitalRead(BTN_ROTATE) == LOW) {
    // Logika pro rotaci
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    // Logika pro pohyb dolů
  }
}

// Hlavní smyčka
void setup() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ROTATE, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(3);  // Nastav orientaci displeje

  drawField();
}

void loop() {
  checkButtons();
  delay(100);  // Zpoždění pro snížení frekvence čtení tlačítek
}
