// ============================================================
//  SJABLOON — kopieer deze map voor een nieuwe animatie
//  Bedacht door: ...
//  Wat doet het: ...
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
const int FELHEID = 75;          // 255 = zo fel mogelijk, kleiner = zachter
CRGB STIP_KLEUR = CRGB::Red;
const int STAP_MS = 100;         // hoe snel de stip loopt
const int BEELDJE_MS = 30;

// ============================================================
unsigned long beginTijd = 0;

// Zet alles op de beginstand. Gebeurt bij het aanzetten, en later
// ook als de master deze animatie (opnieuw) laat starten.
void beginAnimatie() {
  beginTijd = millis();
}

// Tekent één beeldje. tijd = milliseconden sinds beginAnimatie().
void tekenBeeldje(unsigned long tijd) {
  wisAlles();
  // een rode stip die over het scherm loopt
  int x = (tijd / STAP_MS) % BREEDTE;
  CRGB k = STIP_KLEUR;
  k.nscale8(FELHEID);
  zetPixel(x, HOOGTE / 2, k);
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
