#include <Control_Surface.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === DISPLAY OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === MIDI USB ===
USBMIDI_Interface midi;

// === ENCODERS ===
struct EncoderConfig {
  uint8_t pinA;
  uint8_t pinB;
  uint8_t ccNumber;
  int8_t initialValue;
};

constexpr EncoderConfig encoders[] = {
  {13, 19, 21, 90},
  {20, 21, 22, 90},
  {8, 7, 23, 90},
  {6, 5, 24, 90},
};

constexpr uint8_t numEncoders = sizeof(encoders) / sizeof(encoders[0]);
bool prevA[numEncoders];
int8_t controlValue[numEncoders];

// === MATRIZ DE BOTÕES ===
constexpr uint8_t numRows = 5;
constexpr uint8_t numCols = 4;

const AddressMatrix<numRows, numCols> addresses = {{
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 16},
  {17, 18, 19, 20},
}};

NoteButtonMatrix<numRows, numCols> buttonmatrix = {
  {22, 23, 1, 0, 4},     // row pins
  {12, 11, 10, 9},       // column pins
  addresses,
  CHANNEL_1,
};

// === DISPLAY: BARRA DE VOLUME ===
void drawVolumeBar() {
  display.clearDisplay();

  // Moldura da barra
  display.drawRect(10, 30, 108, 10, WHITE);

  // Valor proporcional de 0 a 100
  int volumePercent = map(controlValue[0], 0, 127, 0, 100);
  int barWidth = map(volumePercent, 0, 100, 0, 106);

  // Preenchimento da barra
  display.fillRect(11, 31, barWidth, 8, WHITE);

  // Texto
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print("Volume: ");
  display.print(volumePercent);
  display.print("%");

  display.display();
}

// === TRATA ENCODERS ===
void handleEncoder(uint8_t index) {
  bool currentA = digitalRead(encoders[index].pinA);
  bool currentB = digitalRead(encoders[index].pinB);

  if (prevA[index] && !currentA) {
    if (currentB) {
      controlValue[index] = min(controlValue[index] + 5, 127);
    } else {
      controlValue[index] = max(controlValue[index] - 5, 0);
    }

    midi.sendCC(encoders[index].ccNumber, controlValue[index]);

    // Se for o primeiro encoder, atualizar display
    if (index == 0) {
      drawVolumeBar();
    }
  }

  prevA[index] = currentA;
}

// === SETUP ===
void setup() {
  Control_Surface.begin();

  // Inicializa encoders
  for (uint8_t i = 0; i < numEncoders; i++) {
    pinMode(encoders[i].pinA, INPUT_PULLUP);
    pinMode(encoders[i].pinB, INPUT_PULLUP);
    prevA[i] = digitalRead(encoders[i].pinA);
    controlValue[i] = encoders[i].initialValue;
  }

  // Inicializa o display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Fica preso se não encontrar o display
  }

  display.clearDisplay();
  display.display();
  drawVolumeBar();
}

// === LOOP ===
void loop() {
  Control_Surface.loop();

  for (uint8_t i = 0; i < numEncoders; i++) {
    handleEncoder(i);
  }
}
