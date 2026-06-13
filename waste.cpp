#include "Arduino.h"
#include "Colors.h"
#include "Dashboard.h"
#include "config.h"

#ifdef ENABLE_WASTE
void Dashboard::wasteIcon(int xInput, int yInput, int wasteType) {
  //   Wastetype:  Rest = 0; GFT = 1;  PMD = 2;  Papier = 3;  Rest + GFT = 4
  if (publicBackg == BLACK) {
    switch (wasteType) {
      case 0:
        file = sd.open("Waste/RestD.bin");
        break;
      case 1:
        file = sd.open("Waste/GFTD.bin");
        break;
      case 2:
        file = sd.open("Waste/PMDD.bin");
        break;
      case 3:
        file = sd.open("Waste/PaperD.bin");
        break;
      case 4:
        file = sd.open("Waste/GFTRestD.bin");
        break;
      case 5:
        file = sd.open("Waste/GFTPMDD.bin");
        break;
      case 6:
        file = sd.open("Waste/GFTPaperD.bin");
        break;     
    }

  } else {
    switch (wasteType) {
      case 0:
        file = sd.open("Waste/RestL.bin");
        break;
      case 1:
        file = sd.open("Waste/GFTL.bin");
        break;
      case 2:
        file = sd.open("Waste/PMDD.bin");
        break;
      case 3:
        file = sd.open("Waste/PaperL.bin");
        break;
      case 4:
        file = sd.open("Waste/GFTRestL.bin");
        break;
      case 5:
        file = sd.open("Waste/GFTPMDD.bin");
        break;
      case 6:
        file = sd.open("Waste/GFTPaperL.bin");
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