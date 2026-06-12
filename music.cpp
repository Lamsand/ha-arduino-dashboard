#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_MUSIC
void Dashboard::radio1(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd1D.bin");
  } else {
    file = sd.open("rd1L.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio2(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd2D.bin");
  } else {
    file = sd.open("rd2L.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vuurland(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("vrlD.bin");
  } else {
    file = sd.open("vrlL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vrtnws(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("nwsD.bin");
  } else {
    file = sd.open("nwsL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 300;
  const int w = 300;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::spotify(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("sptfD.bin");
  } else {
    file = sd.open("sptfL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::musicIcon(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("mcD.bin");
  } else {
    file = sd.open("mcL.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 170;
  const int w = 170;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio1S(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd1DS.bin");
  } else {
    file = sd.open("rd1LS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::radio2S(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("rd2DS.bin");
  } else {
    file = sd.open("rd2LS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vuurlandS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("vrlDS.bin");
  } else {
    file = sd.open("vrlLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::vrtnwsS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("nwsDS.bin");
  } else {
    file = sd.open("nwsLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::PommS(int xInput, int yInput) {
  if (publicBackg == BLACK) {
    file = sd.open("PommDS.bin");
  } else {
    file = sd.open("PommLS.bin");
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 210;
  const int w = 210;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);  // Draw one row at a time
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);  // Draw one row at a time
  file.close();
}
void Dashboard::play(int xInput, int yInput) {
  file = sd.open("play.bin");
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 92;
  const int w = 92;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.startWrite();
    for (int i = 0; i < w * q; i++) {
      if (rowBuffer[i] != 0xf81f) {
        int px = (xInput - w / 2) + (i % w);
        int py = (yInput - h / 2) + (q * y) + (i / w);
        display.writePixel(px, py, rowBuffer[i]);
      }
    }
    display.endWrite();
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.startWrite();
  for (int i = 0; i < w * (h % q); i++) {
    if (rowBuffer[i] != 0xf81f) {
      int px = (xInput - w / 2) + (i % w);
      int py = (yInput - h / 2) + (q * y) + (i / w);
      display.writePixel(px, py, rowBuffer[i]);
    }
  }
  display.endWrite();
  file.close();
}
void Dashboard::pause(int xInput, int yInput) {
  file = sd.open("pau.bin");
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 92;
  const int w = 92;
  int q = buffSize / w;
  while (y < h / q) {
    file.read((uint16_t*)rowBuffer, w * 2 * q);
    display.startWrite();
    for (int i = 0; i < w * q; i++) {
      if (rowBuffer[i] != 0xf81f) {
        int px = (xInput - w / 2) + (i % w);
        int py = (yInput - h / 2) + (q * y) + (i / w);
        display.writePixel(px, py, rowBuffer[i]);
      }
    }
    display.endWrite();
    y++;
  }
  file.read((uint16_t*)rowBuffer, w * 2 * (h % q));
  display.startWrite();
  for (int i = 0; i < w * (h % q); i++) {
    if (rowBuffer[i] != 0xf81f) {
      int px = (xInput - w / 2) + (i % w);
      int py = (yInput - h / 2) + (q * y) + (i / w);
      display.writePixel(px, py, rowBuffer[i]);
    }
  }
  display.endWrite();
  file.close();
}

void Dashboard::homeMusicImg(int x, int y, int w, int h) {
  musicIcon(x + w / 2, y + h / 2);
}

void Dashboard::music(const String& k_kanaal, const String& k_title,
                      bool k_state, const String& s_kanaal,
                      const String& s_title, bool s_state) {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  String displayName = k_kanaal;
  //******************* Keuken ************************
  if (k_kanaal == "VRT Radio 1" || k_kanaal == "Radio 1") {
    radio1(200, 200);
    displayName = "Radio 1";
  } else if (k_kanaal == "VRT Studio Brussel Vuurland" ||
             k_kanaal == "Studio Brussel Vuurland" || k_kanaal == "Vuurland") {
    vuurland(200, 200);
    displayName = "Vuurland";
  } else if (k_kanaal == "VRT NWS") {
    vrtnws(200, 200);
  } else if (k_kanaal == "Radio 2 vlaams-brabant" || k_kanaal == "Radio 2") {
    radio2(200, 200);
    displayName = "Radio 2";
  } else if (k_kanaal == "Spotify Connect" || k_kanaal == "" ||
             k_kanaal == "Spotify") {
    spotify(200, 200);
    displayName = "Spotify";
  }
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(3);
  display.setCursor(50, 360);
  display.print(k_kanaal);
  display.setTextSize(2);
  display.setCursor(50, 394);
  display.print(trimTitle(k_title, 20));
  //******************* Speelkamer ************************
  displayName = s_kanaal;
  if (s_kanaal == "Radio 1" || s_kanaal == "Radio 1") {
    radio1(600, 200);
  } else if (s_kanaal == "VRT Studio Brussel Vuurland" ||
             s_kanaal == "Studio Brussel Vuurland" || s_kanaal == "Vuurland") {
    vuurland(600, 200);
    displayName = "Vuurland";
  } else if (s_kanaal == "VRT NWS") {
    vrtnws(600, 200);
  } else if (s_kanaal == "Radio 2 vlaams-brabant" || s_kanaal == "Radio 2") {
    radio2(600, 200);
    displayName = "Radio 2";
  } else if (s_kanaal == "Spotify Connect" || s_kanaal == "" ||
             s_kanaal == "Spotify") {
    spotify(600, 200);
    displayName = "Spotify";
  }
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
  display.setTextColor(publicTEXT);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print(s_kanaal);
  display.setTextSize(2);
  display.setCursor(450, 394);
  display.print(trimTitle(s_title, 20));
}
void Dashboard::musicV(const String& k_kanaal, const String& k_title,
                       const String& s_kanaal, const String& s_title) {
  //******************* Keuken ************************
  // display.fillRect(50, 355, 380, 50, publicBackg);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setTextSize(3);
  display.setCursor(50, 360);
  display.print("Vuurland");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(50, 360);
  display.print(k_kanaal);
  display.setTextSize(2);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setCursor(50, 394);
  display.print("                      ");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(50, 394);
  display.print(trimTitle(k_title, 20));
  //******************* Speelkamer ************************
  // display.fillRect(450, 355, 380, 50, publicBackg);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print("Vuurland");

  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(3);
  display.setCursor(450, 360);
  display.print(s_kanaal);
  display.setTextSize(2);
  // Clear
  display.setTextColor(publicBackg, publicBackg);
  display.setCursor(450, 394);
  display.print("                      ");

  display.setTextColor(publicTEXT, publicBackg);
  display.setCursor(450, 394);
  display.print(trimTitle(s_title, 20));
}
void Dashboard::k_kanaal(const String& k_kanaal, bool k_state) {
  String displayName = k_kanaal;
  if (k_kanaal == "VRT Radio 1" || k_kanaal == "Radio 1") {
    radio1(200, 200);
    displayName = "Radio 1";
  } else if (k_kanaal == "VRT Studio Brussel Vuurland" ||
             k_kanaal == "Studio Brussel Vuurland" || k_kanaal == "Vuurland") {
    vuurland(200, 200);
    displayName = "Vuurland";
  } else if (k_kanaal == "VRT NWS") {
    vrtnws(200, 200);
  } else if (k_kanaal == "Radio 2 vlaams-brabant" || k_kanaal == "Radio 2") {
    radio2(200, 200);
    displayName = "Radio 2";
  } else if (k_kanaal == "Spotify Connect" || k_kanaal == "" ||
             k_kanaal == "Spotify") {
    spotify(200, 200);
    displayName = "Spotify";
  }
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
}
void Dashboard::s_kanaal(const String& s_kanaal, bool s_state) {
  String displayName = s_kanaal;
  if (s_kanaal == "Radio 1" || s_kanaal == "Radio 1") {
    radio1(600, 200);
  } else if (s_kanaal == "VRT Studio Brussel Vuurland" ||
             s_kanaal == "Studio Brussel Vuurland" || s_kanaal == "Vuurland") {
    vuurland(600, 200);
    displayName = "Vuurland";
  } else if (s_kanaal == "VRT NWS") {
    vrtnws(600, 200);
  } else if (s_kanaal == "Radio 2 vlaams-brabant" || s_kanaal == "Radio 2") {
    radio2(600, 200);
    displayName = "Radio 2";
  } else if (s_kanaal == "Spotify Connect" || s_kanaal == "" ||
             s_kanaal == "Spotify") {
    spotify(600, 200);
    displayName = "Spotify";
  }
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
}
void Dashboard::k_state(bool k_state) {
  if (!k_state) {
    play(300, 350);
  } else {
    pause(300, 350);
  }
}
void Dashboard::s_state(bool s_state) {
  if (!s_state) {
    play(700, 350);
  } else {
    pause(700, 350);
  }
}
String Dashboard::trimTitle(const String& title, int maxChars) {
  if (title.length() > maxChars) {
    return title.substring(0, maxChars - 3) + "...";
  }
  return title;
}
void Dashboard::chooseChannel() {
  display.fillRoundRect(1, 1, SCREEN_W - 2, SCREEN_H - 2, 10, publicBackg);
  radio1S(150, 125);
  radio2S(400, 125);
  PommS(650, 125);
  vrtnwsS(275, 363);
  vuurlandS(525, 363);
}
#endif