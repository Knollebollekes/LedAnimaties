// ============================================================
//  OOGJES
//  Bedacht door: Claude (eerste voorbeeld)
//  Wat doet het: twee ogen die rondkijken en af en toe knipperen
// ============================================================
#include "paneel.h"

// ---- Kleuren: verander ze maar! ----------------------------
CRGB OOGWIT = CRGB::White;
CRGB IRIS   = CRGB::Blue;          // het gekleurde rondje
CRGB PUPIL  = CRGB::Black;         // het zwarte puntje in het midden
CRGB OOGLID = CRGB(255, 140, 90);  // huidskleur van het ooglid

// ---- Waar staan de ogen? -----------------------------------
const int OOG_GROOTTE = 5;                 // een oog is 5 x 5 ledjes
const int LINKS_X  = BREEDTE / 2 - 6;      // linkeroog
const int RECHTS_X = BREEDTE / 2 + 1;      // rechteroog
const int OGEN_Y   = HOOGTE / 2 - 3;       // hoogte van de ogen

// ---- Snelheid ------------------------------------------------
const int KNIPPER_SNELHEID = 25;   // hoe snel het ooglid beweegt (kleiner = sneller)

// Hoort dit plekje bij het oog? De hoekjes laten we weg,
// dan ziet het oog er rond uit.
bool hoortBijOog(int x, int y) {
  if (x < 0 || y < 0 || x >= OOG_GROOTTE || y >= OOG_GROOTTE) return false;
  bool links  = (x == 0);
  bool rechts = (x == OOG_GROOTTE - 1);
  bool boven  = (y == 0);
  bool onder  = (y == OOG_GROOTTE - 1);
  return !((links || rechts) && (boven || onder));
}

// Kleur één plekje van een oog, maar alleen als het bij het oog hoort
void kleurOog(int oogX, int oogY, int x, int y, CRGB kleur) {
  if (hoortBijOog(x, y)) zetPixel(oogX + x, oogY + y, kleur);
}

// Teken één oog.
//   kijkX: -1 = links,  0 = midden, 1 = rechts
//   kijkY: -1 = omhoog, 0 = midden, 1 = omlaag
//   dicht: 0 = helemaal open ... 5 = helemaal dicht
void tekenOog(int oogX, int oogY, int kijkX, int kijkY, int dicht) {
  // 1. het oogwit
  for (int y = 0; y < OOG_GROOTTE; y++)
    for (int x = 0; x < OOG_GROOTTE; x++)
      kleurOog(oogX, oogY, x, y, OOGWIT);

  // 2. de iris (3 x 3) met de pupil in het midden
  int irisX = 1 + kijkX;
  int irisY = 1 + kijkY;
  for (int y = 0; y < 3; y++)
    for (int x = 0; x < 3; x++)
      kleurOog(oogX, oogY, irisX + x, irisY + y, IRIS);
  kleurOog(oogX, oogY, irisX + 1, irisY + 1, PUPIL);

  // 3. het ooglid zakt van boven naar beneden
  for (int y = 0; y < dicht; y++)
    for (int x = 0; x < OOG_GROOTTE; x++)
      kleurOog(oogX, oogY, x, y, OOGLID);
}

// Teken allebei de ogen en laat ze zien
void tekenOgen(int kijkX, int kijkY, int dicht) {
  wisAlles();
  tekenOog(LINKS_X,  OGEN_Y, kijkX, kijkY, dicht);
  tekenOog(RECHTS_X, OGEN_Y, kijkX, kijkY, dicht);
  toon();
}

// Eén keer knipperen: ooglid dicht en weer open
void knipper(int kijkX, int kijkY) {
  for (int dicht = 0; dicht <= OOG_GROOTTE; dicht++) {
    tekenOgen(kijkX, kijkY, dicht);
    wacht(KNIPPER_SNELHEID);
  }
  wacht(60);
  for (int dicht = OOG_GROOTTE; dicht >= 0; dicht--) {
    tekenOgen(kijkX, kijkY, dicht);
    wacht(KNIPPER_SNELHEID);
  }
}

void setup() {
  startPanelen();
}

void loop() {
  // Kies een willekeurige kant om naar te kijken
  int kijkX = random(-1, 2);   // -1, 0 of 1
  int kijkY = random(-1, 2);

  tekenOgen(kijkX, kijkY, 0);
  wacht(random(600, 2000));    // even blijven kijken

  // Ongeveer 1 op de 3 keer knipperen
  if (random(0, 3) == 0) {
    knipper(kijkX, kijkY);
  }
}
