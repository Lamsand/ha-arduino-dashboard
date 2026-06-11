#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_WASTE
void Dashboard::wasteIcon(int xInput, int yInput, int wasteType) {
  //   Wastetype:  Rest = 0; GFT = 1;  PMD = 2;  Papier = 3;  Rest + GFT = 4
  if (publicBackg == BLACK) {
    switch (wasteType) {
      case 0:
        file = sd.open("RestD.bin");
        break;
      case 1:
        file = sd.open("GFTD.bin");
        break;
      case 2:
        file = sd.open("PMDD.bin");
        break;
      case 3:
        file = sd.open("PaperD.bin");
        break;
      case 4:
        file = sd.open("GFTRestD.bin");
        break;
    }

  } else {
    switch (wasteType) {
      case 0:
        file = sd.open("RestL.bin");
        break;
      case 1:
        file = sd.open("GFTL.bin");
        break;
      case 2:
        file = sd.open("PMDL.bin");
        break;
      case 3:
        file = sd.open("PaperL.bin");
        break;
      case 4:
        file = sd.open("GFTRestL.bin");
        break;
    }
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int w = 179;
  const int h = 134;
  uint32_t q = buffSize / w;
  while (y < h / q) {
    readLarge(file, (uint8_t*)rowBuffer, w * 2 * q);
    display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                          q);
    y++;
  }
  readLarge(file, (uint8_t*)rowBuffer, w * 2 * (h % q));
  display.drawRGBBitmap(xInput - w / 2, yInput - h / 2 + q * y, rowBuffer, w,
                        h % q);
  file.close();
}

void Dashboard::homeWasteImg(int x, int y, int w, int h, int wasteType) {
  wasteIcon(x + w / 2, y + h / 2, wasteType);
}
#endif