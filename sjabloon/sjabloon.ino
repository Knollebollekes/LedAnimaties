// ============================================================
//  SJABLOON — kopieer deze map voor een nieuwe animatie
//  Bedacht door: ...
//  Wat doet het: ...
// ============================================================
#include "paneel.h"

void setup() {
  startPanelen();
}

void loop() {
  // Een rode stip die over het scherm loopt
  for (int x = 0; x < BREEDTE; x++) {
    wisAlles();
    zetPixel(x, HOOGTE / 2, CRGB::Red);
    toon();
    wacht(100);
  }
}
