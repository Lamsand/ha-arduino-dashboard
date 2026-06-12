#include "Dashboard.h"
#include "Arduino.h"
#include "Colors.h"

#ifdef ENABLE_evcc
void Dashboard::evcc(int PV, int grid, int consumption, float toCar) {

  // ── Energy flow calculations ───────────────────────────────────────────────
  // How much power goes to home vs car
  int toHome  = max(consumption - (int)(toCar * 1000.0f), 0);
  // Self-use: PV power consumed locally (not exported)
  int selfUse = min(PV, toHome);
  // Grid import (positive grid = buying from grid)
  int gridUse = max(grid, 0);
  // Grid export (negative grid = selling to grid)
  int toGrid  = max(-grid, 0);


  // ── Layout constants ──────────────────────────────────────────────────────
  // Main region
  const int16_t regX = 20;
  const int16_t regY = 10;
  const int16_t regW = SCREEN_W - 40;

  // Central energy bar
  const int16_t barX = regX;
  const int16_t barY = regY + 68;
  const int16_t barW = regW;
  const int16_t barH = 44;
  const int16_t barR = 6;          // corner radius

  // Bracket lines above and below the bar
  const int16_t bbotY = barY + barH;
  const int16_t bbotH = 16;

  // Icon vertical centres: sit on the bracket lines
  const int16_t sunY  = barY - bbotH;   // top bracket line (sources)
  const int16_t iconY = bbotY + bbotH;  // bottom bracket line (sinks)


  // ── Bar segment widths (pixels) ───────────────────────────────────────────
  // Total is used to scale each segment proportionally across barW
  int total  = selfUse + gridUse + toGrid;
  if (total <= 0) total = 1;

  int greenW  = constrain((int)((long)selfUse * barW / total), 0, barW);
  int greyW   = constrain((int)((long)gridUse * barW / total), 0, barW - greenW);
  int yellowW = barW - greenW - greyW;

  // Snap rounding remainders: if a flow is zero its segment must also be zero
  if (toGrid  == 0) { yellowW = 0; greyW  = barW - greenW; }
  if (gridUse == 0) { greyW   = 0; yellowW = barW - greenW; }


  // ── 0. Clear the widget area (avoids full-screen flicker) ─────────────────
  display.fillRect(regX - 15, regY + 35, regW + 15, 110, publicBackg);


  // ── 1. Title ("EVCC") ─────────────────────────────────────────────────────
  display.setTextColor(publicTEXT, publicBackg);
  display.setTextSize(2);
  display.setCursor(regX, regY + 8);
  display.print("EVCC");


  // ── 2. PV power — top centre ──────────────────────────────────────────────
  display.setTextColor(GREY, publicBackg);
  display.setTextSize(2);

  char pvStr[16];
  if (PV < 1000)
    snprintf(pvStr, sizeof(pvStr), "%d W",     PV);
  else
    snprintf(pvStr, sizeof(pvStr), "%.1f kW",  PV / 1000.0f);

  int16_t  tx, ty;
  uint16_t tw, th;
  display.getTextBounds(pvStr, 0, 0, &tx, &ty, &tw, &th);
  display.setCursor(regX + regW / 2 - tw / 2, regY + 8);
  display.print(pvStr);


  // ── 3. Grid power — top right (red = importing, green = exporting) ────────
  char gridStr[20];
  if (grid >= 0) {
    // Positive grid = importing → shown in red with a "+" prefix
    if (grid < 1000)
      snprintf(gridStr, sizeof(gridStr), "+%d W",    grid);
    else
      snprintf(gridStr, sizeof(gridStr), "+%.1f kW", grid / 1000.0f);
    display.setTextColor(lightRed, publicBackg);
  } else {
    // Negative grid = exporting → shown in green, snprintf adds the "-"
    if (-grid < 1000)
      snprintf(gridStr, sizeof(gridStr), "%d W",    grid);
    else
      snprintf(gridStr, sizeof(gridStr), "%.1f kW", grid / 1000.0f);
    display.setTextColor(evccGREEN, publicBackg);
  }

  display.setTextSize(2);
  display.getTextBounds(gridStr, 0, 0, &tx, &ty, &tw, &th);
  display.setCursor(regX + regW - tw, regY + 8);
  display.print(gridStr);


  // ── 4. "In" / "Out" labels — vertical, right edge ────────────────────────
  // "In"  sits beside the top bracket  (sources entering the bar)
  // "Out" sits beside the bottom bracket (sinks leaving the bar)
  display.setTextSize(1);
  display.setTextColor(GREY, publicBackg);

  display.setCursor(regX + regW + 6, barY);      display.print("I");
  display.setCursor(regX + regW + 6, barY +  9); display.print("n");
  display.setCursor(regX + regW + 6, bbotY +  2); display.print("O");
  display.setCursor(regX + regW + 6, bbotY + 11); display.print("u");
  display.setCursor(regX + regW + 6, bbotY + 20); display.print("t");


  // ── 5. Top bracket — open at the bottom, icons sit on this line ──────────
  display.drawRoundRect(barX, barY - bbotH, barW, barR + 40, barR, GREY);
  display.fillRect(barX, barY - 8, barW, 16, publicBackg);  // erase bottom edge → "open"

  // ── 6. Bottom bracket — open at the top, icons sit on this line ──────────
  display.drawRoundRect(barX, bbotY - 4, barW, barR + bbotH, barR, GREY);
  display.fillRect(barX, bbotY - 8, barW, 16, publicBackg); // erase top edge → "open"


  // ── 7. Three-segment energy bar ───────────────────────────────────────────
  // Draw order: fill full bar with rightmost colour, then overdraw leftward segments.
  // Green  = self-use (PV direct to home)
  // Grey   = grid import
  // Yellow = grid export

  // Base layer: fill entire bar with the rightmost active colour
  if      (yellowW > 0) display.fillRoundRect(barX, barY, barW, barH, barR, YELLOW);
  else if (greyW   > 0) display.fillRoundRect(barX, barY, barW, barH, barR, GREY);
  else                  display.fillRoundRect(barX, barY, barW, barH, barR, evccGREEN);

  // Overdraw grey segment on top of yellow (covers left portion)
  if (greyW > 0 && yellowW > 0) {
    display.fillRoundRect(barX, barY, greenW + greyW, barH, barR, GREY);
    display.fillRect(barX + greenW + greyW, barY, barR, barH, GREY); // square off right edge
  }

  // Overdraw green segment on top of grey/yellow
  if (greenW > 0) {
    display.fillRoundRect(barX, barY, greenW, barH, barR, evccGREEN);
    // Square off the right edge of the green segment so it butts cleanly against the next colour
    if (greyW > 0 || yellowW > 0)
      display.fillRect(barX + greenW, barY, barR, barH,
                       greyW > 0 ? (uint16_t)GREY : (uint16_t)YELLOW);
  }


  // ── 8. Watt labels inside each bar segment ────────────────────────────────
  // Only drawn when the segment is wide enough to fit the text (> 40 px)
  char wStr[16];
  display.setTextSize(2);

  // Green segment — self-use
  if (greenW > 40) {
    if (selfUse < 1000) snprintf(wStr, sizeof(wStr), "%d W",    selfUse);
    else                snprintf(wStr, sizeof(wStr), "%.1f kW", selfUse / 1000.0f);
    uint16_t lw, lh; int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK, publicBackg);
    display.setCursor(barX + (greenW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  // Grey segment — grid import
  if (greyW > 40) {
    if (gridUse < 1000) snprintf(wStr, sizeof(wStr), "%d W",    gridUse);
    else                snprintf(wStr, sizeof(wStr), "%.1f kW", gridUse / 1000.0f);
    uint16_t lw, lh; int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(publicTEXT);
    display.setCursor(barX + greenW + (greyW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }

  // Yellow segment — grid export
  if (yellowW > 40) {
    if (toGrid < 1000) snprintf(wStr, sizeof(wStr), "%d W",    toGrid);
    else               snprintf(wStr, sizeof(wStr), "%.1f kW", toGrid / 1000.0f);
    uint16_t lw, lh; int16_t lx, ly;
    display.getTextBounds(wStr, 0, 0, &lx, &ly, &lw, &lh);
    display.setTextColor(BLACK);
    display.setCursor(barX + greenW + greyW + (yellowW - lw) / 2, barY + (barH - lh) / 2);
    display.print(wStr);
  }


  // ── 9. Divider ticks between segments ────────────────────────────────────
  // Top tick: between green and grey (on the top bracket line)
  if (greyW > 1)
    display.drawLine(barX + greenW, sunY + 4,
                     barX + greenW, sunY + 12, GREY);

  // Bottom tick: between grey/green and yellow (on the bottom bracket line)
  if (yellowW > 1)
    display.drawLine(barX + greenW + greyW, bbotY + 4,
                     barX + greenW + greyW, bbotY + bbotH - 4, GREY);


  // ── 10. Source icons (top bracket line) ──────────────────────────────────
  // Sun icon — centred in the combined green+yellow (PV) zone
  if (greenW + yellowW > 1) {
    const int16_t sx = regX + (greenW + yellowW) / 2;
    evccIcon(sx, sunY, 0);  // 0 = sun
  }

  // Pylon icon — centred in the grey (grid import) zone
  if (greyW > 0) {
    const int16_t gx = barX + greenW + greyW / 2;
    evccIcon(gx, sunY, 1);  // 1 = pylon
  }


  // ── 11. Sink icons (bottom bracket line) ─────────────────────────────────
  // The bottom bracket has three zones: [ home | car | grid export ]
  // Car zone width is derived from toCar relative to total consumption.
  int carW = 0;
  if (toCar > 0.0f) {
    int carWatts = (int)(toCar * 1000.0f);
    int botTotal = toHome + (yellowW > 0 ? toGrid : 0);
    if (botTotal <= 0) botTotal = 1;
    carW = constrain((int)((long)carWatts * barW / botTotal), 0, barW - 10);
  }

  // Home icon — centred in the home zone (left of car zone)
  {
    const int16_t hx = barX + (greenW + greyW - carW) / 2;
    evccIcon(hx, iconY, 2);  // 2 = house
  }

  // Car icon — centred in the car zone (right of home, left of grid export)
  if (carW > 0) {
    // Divider tick between home and car zones
    display.drawLine(barX + greenW + greyW - carW, bbotY + 4,
                     barX + greenW + greyW - carW, bbotY + bbotH - 4, GREY);

    const int16_t cx = barX + greenW + greyW - carW + carW / 2;
    evccIcon(cx, iconY, 3);  // 3 = car
  }

  // Pylon icon — centred in the grid export zone (yellow zone, bottom)
  if (yellowW > 0) {
    const int16_t gx = barX + greenW + greyW + yellowW / 2;
    evccIcon(gx, iconY, 1);  // 1 = pylon
  }
}void Dashboard::evccIcon(int xInput, int yInput, int state) {
  switch (state) {
    case 0:
      if (publicBackg == BLACK) {
        file = sd.open("evcc/sunD.bin");
      } else {
        file = sd.open("evcc/sunL.bin");
      }
      break;
    case 1:
      if (publicBackg == BLACK) {
        file = sd.open("evcc/pylonD.bin");
      } else {
        file = sd.open("evcc/pylonL.bin");
      }
      break;
    case 2:
      if (publicBackg == BLACK) {
        file = sd.open("evcc/homeD.bin");
      } else {
        file = sd.open("evcc/homeL.bin");
      }
      break;
    case 3:
      if (publicBackg == BLACK) {
        file = sd.open("evcc/carD.bin");
      } else {
        file = sd.open("evcc/carL.bin");
      }
      break;
  }
  if (!file) return;
  file.seek(4);  // Skip the 4-byte header of the file
  int y = 0;
  const int h = 30;
  const int w = 30;
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
#endif