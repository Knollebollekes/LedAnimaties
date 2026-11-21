// ============================================================
//  BELGIË MET FRIETJES
//  Bedacht door: de begeleiders (voorbeeld voor de kleding)
//  Wat doet het: de Belgische vlag wappert, dan schuift er een
//                frietzak omhoog, vallen de frietjes erin en komt
//                er een flinke klodder mayo op
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
const int FELHEID = 75;               // 255 = zo fel mogelijk, kleiner = zachter

// De vlag: zwart, geel, rood (zwart is een heel klein beetje aan,
// anders zie je die baan niet wapperen)
CRGB VLAG_1 = CRGB(30, 30, 30);       // zwart
CRGB VLAG_2 = CRGB(255, 170, 0);      // geel
CRGB VLAG_3 = CRGB(255, 0, 0);        // rood

CRGB FRIET        = CRGB(255, 150, 0);   // goudgele frietjes
CRGB FRIET_DONKER = CRGB(200, 80, 0);    // knapperig puntje
CRGB ZAK_ROOD     = CRGB(220, 0, 0);     // de frietzak: rood-wit gestreept
CRGB ZAK_WIT      = CRGB(200, 200, 200);
CRGB MAYO         = CRGB(255, 230, 160);
CRGB ZOUT         = CRGB(255, 255, 255);

// Hoe lang elk stukje duurt (in milliseconden)
const int VLAG_MS     = 4000;   // vlag wappert
const int ZAK_MS      = 800;    // frietzak schuift omhoog
const int FRIET_MS    = 2200;   // frietjes vallen erin
const int MAYO_MS     = 700;    // mayo erop
const int SMULLEN_MS  = 3000;   // even kijken (en het zout glinstert)
const int BEELDJE_MS  = 30;

// ============================================================
// Waar komt de bovenkant van elk frietje? (per kolom, -1 = geen frietje)
const int FRIET_TOP[16] = {-1, -1, -1, 4, 2, 3, 1, 2, 1, 3, 2, 4, 3, -1, -1, -1};
// In welke volgorde vallen ze?
const int VAL_VOLGORDE[10] = {6, 9, 4, 11, 8, 3, 7, 12, 5, 10};
const int ZAK_BOVEN = 8;   // bovenrand van de frietzak

int rond(float f) { return (int)floor(f + 0.5); }

void zet(int x, int y, CRGB k) {
  k.nscale8(FELHEID);
  zetPixel(x, y, k);
}

// ---- de vlag ------------------------------------------------
void tekenVlag(unsigned long t) {
  for (int x = 0; x < 16; x++) {
    // elke kolom golft een beetje op en neer: wapperen!
    float golf = sin(x * 0.55 - t * 0.006);
    int verschuif = rond(golf * 1.2);
    CRGB k = (x < 5) ? VLAG_1 : (x < 11) ? VLAG_2 : VLAG_3;
    // waar de vlag "bolt" is hij lichter, in de plooi donkerder
    k.nscale8(190 + golf * 65);
    for (int y = 2; y <= 13; y++) zet(x, y + verschuif, k);
  }
}

// ---- de frietzak --------------------------------------------
// De zak wordt naar onderen smaller. "omhoog" schuift hem van onderen in beeld.
void tekenZak(int omhoog) {
  for (int y = ZAK_BOVEN; y < 16; y++) {
    float links  = 2 + (y - ZAK_BOVEN) * 0.57;
    float rechts = 13 - (y - ZAK_BOVEN) * 0.57;
    for (int x = rond(links); x <= rond(rechts); x++) {
      CRGB k = ((x / 2) % 2 == 0) ? ZAK_ROOD : ZAK_WIT;   // strepen
      if (y == ZAK_BOVEN) k = ZAK_WIT;                       // witte rand bovenaan
      zet(x, y + omhoog, k);
    }
  }
}

// Eén frietje in kolom x, met de bovenkant op hoogte top.
// Een frietje is altijd even lang: tot in de zak als hij helemaal gevallen is.
void tekenFriet(int x, int top) {
  int lengte = ZAK_BOVEN + 2 - FRIET_TOP[x];
  for (int y = top; y < top + lengte && y < ZAK_BOVEN + 2; y++) {
    CRGB k = FRIET;
    if (x % 2 == 1) k.nscale8(220);        // om en om iets donkerder: losse frietjes
    if (y == top) k = FRIET_DONKER;          // knapperig puntje
    zet(x, y, k);
  }
}

void tekenMayo(float groei) {
  // de klodder groeit van onder naar boven
  if (groei > 0.0) { for (int x = 5; x <= 10; x++) zet(x, 4, MAYO); }
  if (groei > 0.4) { for (int x = 6; x <= 9; x++)  zet(x, 3, MAYO); }
  if (groei > 0.8) { zet(7, 2, MAYO); zet(8, 2, MAYO); }
}

void tekenZout(unsigned long t) {
  // een paar zoutkorreltjes die om de beurt glinsteren
  const int zx[5] = {4, 11, 6, 9, 12};
  const int zy[5] = {4, 5, 6, 6, 5};
  for (int i = 0; i < 5; i++)
    if (((t / 180) + i * 3) % 7 == 0) zet(zx[i], zy[i], ZOUT);
}

// ============================================================
void setup() {
  startPanelen();
}

void loop() {
  const unsigned long RONDE = VLAG_MS + ZAK_MS + FRIET_MS + MAYO_MS + SMULLEN_MS;
  unsigned long nu = millis();
  unsigned long t = nu % RONDE;
  wisAlles();

  if (t < VLAG_MS) {
    tekenVlag(nu);
  } else {
    t -= VLAG_MS;
    // 1. de zak schuift omhoog
    int omhoog = 0;
    if (t < ZAK_MS) omhoog = rond((1.0 - (float)t / ZAK_MS) * (16 - ZAK_BOVEN));
    // 2. de frietjes vallen erin, één voor één
    if (t >= ZAK_MS) {
      float tijd = t - ZAK_MS;
      float perFriet = FRIET_MS / 10.0;
      for (int i = 0; i < 10; i++) {
        int x = VAL_VOLGORDE[i];
        float start = i * perFriet * 0.7;
        if (tijd < start) continue;
        float val = min(1.0f, (tijd - start) / (perFriet * 1.3f));
        int top = rond(-8 + (FRIET_TOP[x] + 8) * val * val);   // vallen gaat steeds sneller
        tekenFriet(x, top);
      }
    }
    tekenZak(omhoog);
    // 3. mayo erop
    unsigned long mayoStart = ZAK_MS + FRIET_MS;
    if (t >= mayoStart) tekenMayo((float)(t - mayoStart) / MAYO_MS);
    // 4. smullen: het zout glinstert
    if (t >= mayoStart + MAYO_MS) tekenZout(nu);
  }

  toon();
  wacht(BEELDJE_MS);
}
