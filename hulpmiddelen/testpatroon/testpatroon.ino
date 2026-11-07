// ============================================================
//  TESTPATROON — klopt instellingen.h met de echte bedrading?
//  Bedacht door: Claude
//  Wat doet het: hoekjes in vaste kleuren, daarna een stip die het scherm afloopt
//
//  Zo hoort het eruit te zien:
//    linksboven  = ROOD      rechtsboven = GROEN
//    linksonder  = BLAUW     rechtsonder = WIT
//  Daarna loopt een gele stip rij voor rij van links naar rechts,
//  van boven naar beneden, over ALLE panelen.
//
//  Klopt het niet? Pas EERSTE_LED_HOEK, RIJEN_HORIZONTAAL, SLANG
//  of PANELEN_ZIGZAG aan in instellingen.h en probeer opnieuw.
// ============================================================
#include "paneel.h"

void tekenHoekjes() {
  zetPixel(0, 0, CRGB::Red);
  zetPixel(BREEDTE - 1, 0, CRGB::Green);
  zetPixel(0, HOOGTE - 1, CRGB::Blue);
  zetPixel(BREEDTE - 1, HOOGTE - 1, CRGB::White);
}

void setup() {
  startPanelen();
}

void loop() {
  // 1. Alleen de hoekjes, 4 seconden
  wisAlles();
  tekenHoekjes();
  toon();
  wacht(4000);

  // 2. Een stip die het scherm afloopt
  for (int y = 0; y < HOOGTE; y++) {
    for (int x = 0; x < BREEDTE; x++) {
      wisAlles();
      tekenHoekjes();
      zetPixel(x, y, CRGB::Yellow);
      toon();
      wacht(40);
    }
  }
}
