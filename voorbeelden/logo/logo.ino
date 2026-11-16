// ============================================================
//  LOGO
//  Bedacht door: de begeleiders
//  Wat doet het: het Knollebolleke van C.V. Knollebollekes in
//                lampjes; hij knipoogt en de belletjes glinsteren
// ============================================================
#include "paneel.h"

// ---- Het plaatje: elk teken is één lampje -------------------
//  G = blad   g = donker blad   P = paarse muts   Y = belletje
//  O = oranje rand   C = knol (gezicht)   W = oogwit   I = oog
//  M = donkere mond  R = tong   . = achtergrond (blauw, gele ring)
const char* PLAATJE[16] = {
  "...GG.......GG..",
  "..GGGG.PP..GGGG.",
  ".GGgG.PPPP.GgGG.",
  ".GGGGPPPPPPGGGG.",
  "..Y.PPPPPPPP.Y..",
  "...OOOOOOOOOO...",
  "...OOOOOOOOOO...",
  "...CCCCCCCCCC...",
  "..CCWWCCCCWWCC..",
  "..CCWICCCCWICC..",
  "..CCCCCCCCCCCC..",
  "..CCMWWWWWWMCC..",
  "..CCCMRRRRMCCC..",
  "...CCCMMMMCCC...",
  "....CCCCCCCC....",
  ".......CC.......",
};

// ---- Kleuren: verander ze maar! -----------------------------
CRGB BLAD        = CRGB(20, 170, 40);
CRGB DONKER_BLAD = CRGB(0, 90, 20);
CRGB MUTS        = CRGB(100, 40, 170);
CRGB BELLETJE    = CRGB(255, 210, 0);
CRGB RAND        = CRGB(255, 90, 0);
CRGB KNOL        = CRGB(255, 225, 140);
CRGB OOGWIT      = CRGB(255, 255, 255);
CRGB OOG         = CRGB(30, 90, 220);
CRGB MOND        = CRGB(90, 0, 10);
CRGB TONG        = CRGB(230, 30, 40);
CRGB ACHTERGROND = CRGB(20, 120, 210);   // blauw zoals in het logo
CRGB RING        = CRGB(255, 210, 0);    // gele ring eromheen

const int KNIPOOG_ELKE_MS = 3000;   // hoe vaak hij knipoogt
const int OGEN_DICHT_MS   = 250;    // hoe lang de ogen dicht zijn
const int BEELDJE_MS      = 30;

// ============================================================
// Het plaatje staat in het midden van het scherm
const int LINKS = (BREEDTE - 16) / 2;
const int BOVEN = (HOOGTE - 16) / 2;

unsigned long volgendeKnipoog = 2000;
bool alleenRechts = false;   // soms knipoogt hij met één oog

CRGB kleurVan(char c, int x, int y) {
  switch (c) {
    case 'G': return BLAD;
    case 'g': return DONKER_BLAD;
    case 'P': return MUTS;
    case 'Y': return BELLETJE;
    case 'O': return RAND;
    case 'C': return KNOL;
    case 'W': return OOGWIT;
    case 'I': return OOG;
    case 'M': return MOND;
    case 'R': return TONG;
  }
  // achtergrond: blauwe cirkel met een gele ring, buiten de cirkel uit
  float afstand = sqrt((x - 7.5) * (x - 7.5) + (y - 7.5) * (y - 7.5));
  if (afstand > 7.9) return CRGB::Black;
  if (afstand > 6.8) return RING;
  return ACHTERGROND;
}

void tekenPlaatje(bool ogenDicht) {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      char c = PLAATJE[y][x];
      CRGB k = kleurVan(c, x, y);
      // ogen dicht: oogwit en oog worden knol, met een donker streepje
      bool oog = (c == 'W' || c == 'I') && (y == 8 || y == 9);
      bool ditOog = !alleenRechts || x >= 8;
      if (ogenDicht && oog && ditOog) k = (y == 9) ? MOND : KNOL;
      // de belletjes glinsteren
      if (c == 'Y') k.nscale8(150 + sin8(millis() / 4 + x * 60) * 105 / 255);
      zetPixel(LINKS + x, BOVEN + y, k);
    }
  }
}

void setup() {
  startPanelen();
}

void loop() {
  unsigned long nu = millis();
  bool ogenDicht = false;
  if (nu >= volgendeKnipoog) {
    ogenDicht = true;
    if (nu >= volgendeKnipoog + OGEN_DICHT_MS) {
      volgendeKnipoog = nu + KNIPOOG_ELKE_MS + random(0, 1500);
      alleenRechts = random(0, 2) == 0;
    }
  }
  wisAlles();
  tekenPlaatje(ogenDicht);
  toon();
  wacht(BEELDJE_MS);
}
