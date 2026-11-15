// ============================================================
//  BRAND
//  Bedacht door: de begeleiders
//  Wat doet het: een echte vlam over het hele scherm, met
//                tongen die omhoog lekken, flakkeren en wiebelen
// ============================================================
#include "paneel.h"

// ---- Het vuur: verander het maar! ---------------------------
const int   HITTE_ONDER  = 230;   // hoe heet het onderaan is (0-255)
const int   AFKOELEN     = 85;    // hoe snel de warmte afkoelt (groter = lagere vlam)
const float RAND_KOEL    = 3.2;   // hoe veel de zijkanten afkoelen (groter = smallere punt)
const float WIEBELEN     = BREEDTE * 0.18;  // hoe ver de vlam heen en weer gaat
const int   VLAMMENTONGEN = 3;    // 1 op de zoveel beeldjes schiet er een hete tong omhoog
const int   BEELDJE_MS   = 35;

// Kleuren van koud naar heet
CRGB KLEUR_DONKER = CRGB(60, 0, 0);       // donkerrood (net nog warm)
CRGB KLEUR_ROOD   = CRGB(220, 30, 0);
CRGB KLEUR_ORANJE = CRGB(255, 110, 0);
CRGB KLEUR_GEEL   = CRGB(255, 200, 40);
CRGB KLEUR_WIT    = CRGB(255, 245, 200);  // het heetst

// ============================================================
uint8_t warmte[HOOGTE][BREEDTE];   // hoe warm elk plekje is

const float MIDDEN_X = (BREEDTE - 1) / 2.0;
float wiebel = 0, wiebelDoel = 0;

int rond(float f) { return (int)floor(f + 0.5); }

float willekeurig(float laag, float hoog) {
  return laag + (hoog - laag) * random(0, 1001) / 1000.0;
}

CRGB meng(CRGB a, CRGB b, float hoeveel) {
  return blend(a, b, (uint8_t)(constrain(hoeveel, 0.0, 1.0) * 255));
}

// Welke kleur hoort bij een warmte (0 = koud, 255 = heet)?
CRGB vuurKleur(uint8_t w) {
  float f = w / 255.0;
  if (f < 0.12) return meng(CRGB::Black, KLEUR_DONKER, f / 0.12);
  if (f < 0.35) return meng(KLEUR_DONKER, KLEUR_ROOD, (f - 0.12) / 0.23);
  if (f < 0.60) return meng(KLEUR_ROOD, KLEUR_ORANJE, (f - 0.35) / 0.25);
  if (f < 0.80) return meng(KLEUR_ORANJE, KLEUR_GEEL, (f - 0.60) / 0.20);
  return meng(KLEUR_GEEL, KLEUR_WIT, (f - 0.80) / 0.20);
}

// Waar is het midden van de vlam op hoogte y? (de punt wiebelt het meest)
float middenOp(int y) {
  float t = 1.0 - (float)y / (HOOGTE - 1);   // 0 onderaan, 1 bovenaan
  return MIDDEN_X + wiebel * t * t;
}

// De "koelkaart": plekken waar het vuur sneller afkoelt. Die kaart
// schuift steeds een rijtje omhoog, zo ontstaan losse vlammentongen.
uint8_t koelkaart[HOOGTE][BREEDTE];

void schuifKoelkaart() {
  for (int y = 0; y < HOOGTE - 1; y++)
    for (int x = 0; x < BREEDTE; x++)
      koelkaart[y][x] = koelkaart[y + 1][x];
  // nieuwe onderste rij: willekeurig, een beetje gladgestreken
  uint8_t nieuw[BREEDTE];
  for (int x = 0; x < BREEDTE; x++) nieuw[x] = random(0, 256);
  for (int x = 0; x < BREEDTE; x++)
    koelkaart[HOOGTE - 1][x] = (nieuw[max(x - 1, 0)] + nieuw[x] * 2 + nieuw[min(x + 1, BREEDTE - 1)]) / 4;
}

// 1. Alles koelt een beetje af. Aan de zijkanten en bovenin meer.
void koelAf() {
  for (int y = 0; y < HOOGTE; y++) {
    float t = 1.0 - (float)y / (HOOGTE - 1);           // 0 onder, 1 boven
    for (int x = 0; x < BREEDTE; x++) {
      float zij = fabs(x - middenOp(y)) / (BREEDTE / 2.0);   // 0 midden, 1 rand
      int k = koelkaart[y][x];
      int koel = (k * k / 255) * AFKOELEN / 255 * (0.3 + t * 1.4)
               + (int)(zij * zij * RAND_KOEL * 10 * (0.4 + t));
      warmte[y][x] = qsub8(warmte[y][x], koel);
    }
  }
}

// 2. Warmte stijgt op en vervaagt een beetje naar de buren
void stijgOp() {
  for (int y = 0; y < HOOGTE - 1; y++) {
    for (int x = 0; x < BREEDTE; x++) {
      int links  = warmte[y + 1][max(x - 1, 0)];
      int recht  = warmte[y + 1][x];
      int rechts = warmte[y + 1][min(x + 1, BREEDTE - 1)];
      int verder = warmte[min(y + 2, HOOGTE - 1)][x];
      warmte[y][x] = (links + recht * 5 + rechts + verder * 3) / 10;
    }
  }
}

// 3. Onderaan wordt het steeds opnieuw heet (het "brandhout")
void stookOp() {
  int onder = HOOGTE - 1;
  for (int x = 0; x < BREEDTE; x++) {
    float zij = fabs(x - middenOp(onder)) / (BREEDTE / 2.0);
    int w = HITTE_ONDER * (1.0 - 0.85 * zij * zij) + random(-50, 25);
    warmte[onder][x] = constrain(w, 0, 255);
  }
  // af en toe een hete tong die omhoog lekt
  if (random(0, VLAMMENTONGEN) == 0) {
    int x = rond(middenOp(onder) + willekeurig(-BREEDTE * 0.25, BREEDTE * 0.25));
    int y = HOOGTE - 2 - random(0, 3);
    for (int dx = -1; dx <= 1; dx++)
      if (x + dx >= 0 && x + dx < BREEDTE)
        warmte[y][x + dx] = qadd8(warmte[y][x + dx], dx == 0 ? 120 : 60);
  }
}

void beweegWiebel() {
  if (random(0, 15) == 0) wiebelDoel = willekeurig(-WIEBELEN, WIEBELEN);
  wiebel += (wiebelDoel - wiebel) * 0.06;
}

void tekenVuur() {
  for (int y = 0; y < HOOGTE; y++)
    for (int x = 0; x < BREEDTE; x++)
      zetPixel(x, y, vuurKleur(warmte[y][x]));
}

void setup() {
  startPanelen();
}

void loop() {
  beweegWiebel();
  schuifKoelkaart();
  koelAf();
  stijgOp();
  stookOp();
  tekenVuur();
  toon();
  wacht(BEELDJE_MS);
}
