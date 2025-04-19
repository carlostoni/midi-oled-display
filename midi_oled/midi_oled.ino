#include <Control_Surface.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== DISPLAY OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== INTERFACE MIDI USB =====
USBMIDI_Interface midi;

// ===== CONFIGURAÇÃO DO ENCODER =====
const uint8_t pinA = 4;           // Pino A do encoder
const uint8_t pinB = 5;           // Pino B do encoder
const uint8_t ccNumber = 21;      // Número do controle MIDI
int8_t controlValue = 90;         // Valor inicial (0 a 127)
bool prevA;

// ===== SETUP =====
void setup() {
  Control_Surface.begin();

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  prevA = digitalRead(pinA);

  // Inicializa o display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Fica preso se não encontrar o display
  }

  display.clearDisplay();
  display.display();
  drawVolumeBar();
}

// ===== FUNÇÃO PARA DESENHAR A BARRA DE VOLUME =====
void drawVolumeBar() {
  display.clearDisplay();

  // Moldura da barra
  display.drawRect(10, 30, 108, 10, WHITE);

  // Calcula largura da barra de volume com base no valor MIDI (0–127)
  int barWidth = map(controlValue, 0, 127, 0, 106);
  display.fillRect(11, 31, barWidth, 8, WHITE);

  // Calcula volume em porcentagem de 0 a 100
  int volumePercent = map(controlValue, 0, 127, 0, 100);

  // Texto "Volume" com valor de 0 a 100
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print("Volume: ");
  display.print(volumePercent);
  display.print(" ");

  display.display();
}

// ===== TRATAMENTO DO ENCODER =====
void handleEncoder() {
  bool currentA = digitalRead(pinA);
  bool currentB = digitalRead(pinB);

  if (prevA && !currentA) {
    if (currentB) {
      controlValue = min(controlValue + 5, 127);
    } else {
      controlValue = max(controlValue - 5, 0);
    }

    midi.sendCC(ccNumber, controlValue);
    drawVolumeBar();
  }

  prevA = currentA;
}

// ===== LOOP PRINCIPAL =====
void loop() {
  Control_Surface.loop();
  handleEncoder();
}
