// ============================================================
//  APPEL (groot scherm)
//  Bedacht door: een van de ouders
//  Wat doet het: een grote glanzende rode appel over het hele
//                scherm; hij deint, het blad wiebelt en de glans fonkelt
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
const int FELHEID = 75;              // 255 = zo fel mogelijk, kleiner = zachter
CRGB ROOD        = CRGB(230, 0, 0);
CRGB DONKERROOD  = CRGB(70, 0, 0);
CRGB BLOS        = CRGB(255, 80, 0);  // een beetje oranje-geel waar de zon op scheen
CRGB GLANS       = CRGB(255, 170, 150);
CRGB STEEL       = CRGB(80, 30, 0);
CRGB BLAD        = CRGB(30, 180, 0);
CRGB NERF        = CRGB(5, 70, 0);

const int DEINEN_MS  = 1800;         // hoe lang één keer op en neer duurt
const int BEELDJE_MS = 30;

// ============================================================
const float CX = (BREEDTE - 1) / 2.0;
const float CY = HOOGTE * 0.58;           // midden van de appel
const float S  = BREEDTE / 32.0;          // schaal: alles past zich aan het scherm aan

unsigned long beginTijd = 0;

void beginAnimatie() {
  beginTijd = millis();
}

CRGB meng(CRGB a, CRGB b, float hoeveel) {
  return blend(a, b, (uint8_t)(constrain(hoeveel, 0.0, 1.0) * 255));
}

void zet(int x, int y, CRGB k) {
  k.nscale8(FELHEID);
  zetPixel(x, y, k);
}

// Zit (x, y) in de appel? De appel is gemaakt van twee ovalen naast elkaar,
// daardoor krijgt hij vanzelf een deukje boven en onder.
float inAppel(float x, float y) {
  float a = sq((x - (CX - 4.5 * S)) / (11.0 * S)) + sq((y - CY) / (14.0 * S));
  float b = sq((x - (CX + 4.5 * S)) / (11.0 * S)) + sq((y - CY) / (14.0 * S));
  return min(a, b);   // kleiner dan 1 = binnen de appel
}

void tekenAppel(float dy, unsigned long tijd) {
  for (int y = 0; y < HOOGTE; y++) {
    for (int x = 0; x < BREEDTE; x++) {
      float binnen = inAppel(x, y - dy);
      if (binnen >= 1) continue;
      // licht van linksboven: rechtsonder is de appel donkerder
      float licht = 1.0 - 0.6 * ((x - CX) / (16 * S) + (y - dy - CY) / (16 * S) + 0.4);
      CRGB k = meng(DONKERROOD, ROOD, licht);
      // bovenaan een beetje blos, met strepen zoals op een echte appel
      float streep = sin(x * 1.3) * 0.5 + 0.5;
      if (y - dy < CY - 4 * S) k = meng(k, BLOS, 0.25 * streep * licht);
      // rand iets donkerder, dan lijkt hij rond
      if (binnen > 0.8) k = meng(k, DONKERROOD, (binnen - 0.8) * 2.5);
      zet(x, y, k);
    }
  }
  // glansplek linksboven, die af en toe fonkelt
  float gx = CX - 7 * S, gy = CY - 6 * S + dy;
  bool fonkel = ((tijd / 120) % 25) == 0;
  for (int y = 0; y < HOOGTE; y++)
    for (int x = 0; x < BREEDTE; x++) {
      float d = sq((x - gx) / (1.8 * S)) + sq((y - gy) / (3.5 * S));
      if (d < 1) zet(x, y, fonkel && d < 0.5 ? CRGB(255, 255, 255) : meng(ROOD, GLANS, 1.2 - d));
    }
}

void tekenSteel(float dy) {
  // een gebogen steeltje vanuit het deukje bovenin
  for (int i = 0; i <= 7 * S; i++) {
    float y = CY - 12 * S - i + dy;
    float x = CX + 0.15 * i;
    zet(round(x), round(y), STEEL);
    if (i < 3 * S) zet(round(x) + 1, round(y), STEEL);   // onderaan iets dikker
  }
}

void tekenBlad(float dy, unsigned long tijd) {
  // een ovaal blad dat schuin omhoog staat en zachtjes wiebelt
  float hoek = -0.5 + 0.15 * sin(tijd / 500.0);
  float bx = CX + 6 * S, by = CY - 17 * S + dy;
  float c = cos(hoek), s = sin(hoek);
  for (int y = 0; y < HOOGTE; y++)
    for (int x = 0; x < BREEDTE; x++) {
      float lx = (x - bx) * c + (y - by) * s;      // langs het blad
      float ly = -(x - bx) * s + (y - by) * c;     // dwars op het blad
      float d = sq(lx / (5.5 * S)) + sq(ly / (2.3 * S));
      if (d >= 1) continue;
      CRGB k = BLAD;
      if (fabs(ly) < 0.5) k = NERF;                // de nerf in het midden
      if (ly > 0.8) k = meng(BLAD, NERF, 0.3);     // onderkant iets donkerder
      zet(x, y, k);
    }
}

void tekenBeeldje(unsigned long tijd) {
  wisAlles();
  // de appel deint zachtjes op en neer
  float dy = sin(tijd * 2 * PI / DEINEN_MS) * 0.8;
  tekenSteel(dy);
  tekenBlad(dy, tijd);
  tekenAppel(dy, tijd);
}

void setup() {
  startPanelen();
  beginAnimatie();
}

void loop() {
  tekenBeeldje(millis() - beginTijd);
  toon();
  wacht(BEELDJE_MS);
}
