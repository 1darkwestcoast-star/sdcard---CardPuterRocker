#include <M5Unified.h>
#include <SD.h>
#include <FS.h>

// ================== NORMAL ASCII =====================
const char* ASCII_NORMAL[] = {
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
const int ASCII_NORMAL_LINES = sizeof(ASCII_NORMAL) / sizeof(ASCII_NORMAL[0]);

// ================== WARNING ASCII (DARKER + FULLER) =====================
const char* ASCII_WARNING[] = {
"..............................................     ....................................................",
"                         ..........              ....................................................................................",
"                     ..................................................   ..   .....................................................",
"                     ................................................................................................................",
"                    ......................        .............................................................................   .......",
"                     ..................................................................................'''''',,,;;:ldxkkkkOOOOO0000",
"                      .................................................................................',;:cllloodxkkkOOOO000KKKXXX",
"            ..................................................................................'''......'';lxkOkkkkkxxddooooodddxxkk",
"           ......................................................................''.....''',,;:::::::cloxkO0KXXXXXXXXKK00OOkkkkxxxdd",
"       ..............................................................        ...';::;,,;codolc:cldk0KXXXXXXXXXXXXXXXXXXXXXXXXXXXKKK0",
"                                                                 ..',:ldxkO0K0Okdc:codxkxdolloxOKXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXKK",
"                                                             ..,cdkO0KXXXXXXXXXXK0kdlcclodxxxollldO0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
"                                                        .''.,:oOKXXXXXXXXXXXXXXXXXXXKOkdlcclodxxollldk0KXXXXXXXXXXXXXXXXXXXXXXXXXX",
"                                                        .',,;:lkKXXXXXXXXXXXXXXXXXXXXXXXKOxolccodxxdlclok0KXXXXXXXXXXXXKKK00000000",
"                                                          .,;;;;coxOKXXXXXXXXXXXXXXXXXXXXXXX0OxolloxkxdlccokOOOOkkkkkkxxxxkkkkkkkk",
"                                                            ...',,,;codkO0KXXXXXXXXXXXXXXXXXXXK0Odc::clloodxxxxxkkkkOOOOOOOOOOOOOO",
"                                                                    ...',:coddxkkkkkxxdoolc::;,'..  ..'';ldxkOOOOOOOOOOOOOOOOOOOOO",
"                                                                             .............................;loodddxkOOOOOOOOOOOOOOOO",
"                                                                              .......''''''''''''''''''.....',,:clooddxOOOOOOOOOOOO",
"                                                                                      ..................        ..';:cooddxkkOOOOO",
"                                                                                    ......................           ..,,,''',,,,",
"                                                                                     ..........................",
"                                                                                        ......................",
"                                                                                                   ............"
};
const int ASCII_WARNING_LINES = sizeof(ASCII_WARNING) / sizeof(ASCII_WARNING[0]);

// ================== DRAW ASCII =====================

void drawASCII(const char* art[], int lines) {
  M5.Display.clear();
  M5.Display.setFont(&fonts::Font0);  // monospace
  M5.Display.setTextSize(1);
  M5.Display.setCursor(2, 2);

  for (int i = 0; i < lines; i++) {
    M5.Display.println(art[i]);
  }
}

// ================== SD CARD INFO ==================

float getSDPercentUsed() {
  if (!SD.begin()) return -1;

  uint64_t totalBytes = SD.totalBytes();
  uint64_t usedBytes  = SD.usedBytes();

  if (totalBytes == 0) return -1;

  float percent = (float)usedBytes / (float)totalBytes * 100.0f;
  return percent;
}

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

// ================== SETUP ==========================

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(1);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(1);
  M5.Display.clear();

  float percent = getSDPercentUsed();

  if (percent >= 69.0f) {
    drawASCII(ASCII_WARNING, ASCII_WARNING_LINES);
  } else {
    drawASCII(ASCII_NORMAL, ASCII_NORMAL_LINES);
  }

  int yBase = 4 + ASCII_NORMAL_LINES * 10;
  showSDStats(yBase + 4);
}

// ================== LOOP ===========================

void loop() {
  M5.update();

  static uint32_t lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {

    float percent = getSDPercentUsed();

    // Redraw ASCII depending on SD usage
    if (percent >= 69.0f) {
      drawASCII(ASCII_WARNING, ASCII_WARNING_LINES);
    } else {
      drawASCII(ASCII_NORMAL, ASCII_NORMAL_LINES);
    }

    int yBase = 4 + ASCII_NORMAL_LINES * 10;

    // Clear SD info area
    M5.Display.fillRect(0, yBase, M5.Display.width(), M5.Display.height() - yBase, BLACK);

    showSDStats(yBase + 4);

    lastUpdate = millis();
  }
}
