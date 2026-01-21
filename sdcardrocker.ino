#include <M5Unified.h>
#include <SD.h>
#include <FS.h>

// ================== ASCII ART =====================
const char* ASCII_ART[] = {
  "              ~~..'~                 ",
  "               (  \\\\ )               ",
  "                \\\\ =/                ",
  "               _\\\\/_                 ",
  "             /    -_                 ",
  "            <       \\\\              ",
  "             \\\\^-_-^\\\\ \\\\   iii      ",
  "             <\\\\v =======uu==>       ",
  "                                      ",
  "                                      ",
  "      Cardputer Fastfetch Status      "
};
const int ASCII_LINES = sizeof(ASCII_ART) / sizeof(ASCII_ART[0]);

// ================== SD CARD INFO ==================

void showSDStats(int y) {
  if (!SD.begin()) {
    M5.Display.setCursor(4, y);
    M5.Display.println("SD: init failed");
    return;
  }

  uint64_t totalBytes = SD.totalBytes();
  uint64_t usedBytes  = SD.usedBytes();
  uint64_t freeBytes  = totalBytes - usedBytes;

  M5.Display.setCursor(4, y);
  M5.Display.printf("SD Used : %llu KB\n", (unsigned long long)(usedBytes / 1024));

  M5.Display.setCursor(4, y + 12);
  M5.Display.printf("SD Free : %llu KB\n", (unsigned long long)(freeBytes / 1024));
}

// ================== ASCII DRAW =====================

void drawASCII() {
  M5.Display.clear();
  M5.Display.setFont(&fonts::Font0);  // monospace for perfect ASCII
  M5.Display.setTextSize(1);
  M5.Display.setCursor(4, 4);

  for (int i = 0; i < ASCII_LINES; i++) {
    M5.Display.println(ASCII_ART[i]);
  }
}

// ================== SETUP ==========================

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(1);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(1);
  M5.Display.clear();

  drawASCII();

  int yBase = 4 + ASCII_LINES * 10;  // space below ASCII
  showSDStats(yBase + 4);
}

// ================== LOOP ===========================

void loop() {
  M5.update();

  // Refresh SD stats every 5 seconds
  static uint32_t lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    int yBase = 4 + ASCII_LINES * 10;

    // Clear only the SD info area
    M5.Display.fillRect(0, yBase, M5.Display.width(), M5.Display.height() - yBase, BLACK);

    showSDStats(yBase + 4);
    lastUpdate = millis();
  }
}
