// ============================================================
//  TOVERSTAF
//  Bedacht door: dochter van de begeleiders (11 jaar)
//  Wat doet het: een toverstaf laadt zich op en schiet een
//                spreuk van glinsterende sterretjes, die eindigt
//                in een grote toverster
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
CRGB STAF_KLEUR   = CRGB(110, 55, 15);    // bruin hout
CRGB PUNT_KLEUR   = CRGB(255, 230, 120);  // het lichtje op de punt
const bool REGENBOOG_SPREUK = true;       // true = alle kleuren, false = één kleur
CRGB SPREUK_KLEUR = CRGB(200, 80, 255);   // die ene kleur (paars)
const int OPLADEN_MS  = 1200;             // hoe lang de staf zich oplaadt
const int SPREUK_MS   = 2600;             // hoe lang de spreuk duurt
const int RUST_MS     = 600;              // even wachten voor de volgende spreuk
const int BEELDJE_MS  = 30;

// ============================================================
// De staf ligt schuin: van linksonder naar het midden
const int STAF_BEGIN_X = 1,  STAF_BEGIN_Y = HOOGTE - 2;
const int PUNT_X = BREEDTE / 2 - 1, PUNT_Y = HOOGTE / 2;
// Waar de spreuk naartoe vliegt: rechtsboven
const float DOEL_X = BREEDTE - 4, DOEL_Y = 3;

struct Sterretje { bool actief; float x, y, vx, vy, leven; CRGB kleur; };
const int MAX_STERRETJES = 80;
Sterretje sterretjes[MAX_STERRETJES];

unsigned long beginTijd = 0;
bool sterGemaakt = false;
CRGB sterKleur;

float willekeurig(float laag, float hoog) {
  return laag + (hoog - laag) * random(0, 1001) / 1000.0;
}
int rond(float f) { return (int)floor(f + 0.5); }

void voegToe(int x, int y, CRGB kleur) {
  zetPixel(x, y, leesPixel(x, y) + kleur);
}

CRGB spreukKleur() {
  if (REGENBOOG_SPREUK) return CHSV(random8(), 200, 255);
  return SPREUK_KLEUR;
}

// Maak een nieuw sterretje
void maakSterretje(float x, float y, float vx, float vy, float leven, CRGB kleur) {
  for (int i = 0; i < MAX_STERRETJES; i++) {
    if (sterretjes[i].actief) continue;
    sterretjes[i] = {true, x, y, vx, vy, leven, kleur};
    return;
  }
}

// ---- tekenen ----------------------------------------------
void tekenStaf(float gloed) {
  // een lijntje van het begin naar de punt
  int stappen = max(abs(PUNT_X - STAF_BEGIN_X), abs(PUNT_Y - STAF_BEGIN_Y));
  for (int i = 0; i < stappen; i++) {
    int x = rond(STAF_BEGIN_X + (PUNT_X - STAF_BEGIN_X) * (float)i / stappen);
    int y = rond(STAF_BEGIN_Y + (PUNT_Y - STAF_BEGIN_Y) * (float)i / stappen);
    zetPixel(x, y, STAF_KLEUR);
    CRGB donker = STAF_KLEUR;
    donker.nscale8(120);
    zetPixel(x + 1, y, donker);   // een beetje dikker
  }
  // het lichtje op de punt, feller als hij opgeladen is
  CRGB punt = PUNT_KLEUR;
  punt.nscale8(80 + gloed * 175);
  zetPixel(PUNT_X, PUNT_Y, punt);
  if (gloed > 0.5) {               // een kruisje van licht eromheen
    CRGB rand = PUNT_KLEUR;
    rand.nscale8((gloed - 0.5) * 2 * 150);
    voegToe(PUNT_X + 1, PUNT_Y, rand); voegToe(PUNT_X - 1, PUNT_Y, rand);
    voegToe(PUNT_X, PUNT_Y + 1, rand); voegToe(PUNT_X, PUNT_Y - 1, rand);
  }
}

// Een grote ster: een plusje met puntjes schuin ertussen
void tekenGroteSter(int cx, int cy, int grootte, CRGB kleur) {
  voegToe(cx, cy, CRGB(255, 255, 255));
  for (int i = 1; i <= grootte; i++) {
    CRGB k = kleur;
    k.nscale8(255 - i * 50);
    voegToe(cx + i, cy, k); voegToe(cx - i, cy, k);
    voegToe(cx, cy + i, k); voegToe(cx, cy - i, k);
    if (i <= grootte / 2 + 1) {
      voegToe(cx + i, cy + i, k); voegToe(cx - i, cy - i, k);
      voegToe(cx + i, cy - i, k); voegToe(cx - i, cy + i, k);
    }
  }
}

void tekenSterretjes() {
  for (int i = 0; i < MAX_STERRETJES; i++) {
    Sterretje& s = sterretjes[i];
    if (!s.actief) continue;
    CRGB k = s.kleur;
    float licht = min(1.0f, s.leven * 2);
    if (random(0, 4) == 0) licht *= 0.3;     // twinkelen!
    k.nscale8(licht * 200);
    voegToe(rond(s.x), rond(s.y), k);
  }
}

// ---- bewegen ----------------------------------------------
void beweegSterretjes() {
  for (int i = 0; i < MAX_STERRETJES; i++) {
    Sterretje& s = sterretjes[i];
    if (!s.actief) continue;
    s.x += s.vx; s.y += s.vy;
    s.vx *= 0.97; s.vy *= 0.97;
    s.leven -= 0.02;
    if (s.leven <= 0) s.actief = false;
  }
}

// Tijdens het opladen komen er sterretjes naar de punt toe vliegen
void opladenSterretjes() {
  if (random(0, 3) != 0) return;
  float hoek = willekeurig(0, 2 * PI);
  float x = PUNT_X + cos(hoek) * 5, y = PUNT_Y + sin(hoek) * 5;
  maakSterretje(x, y, (PUNT_X - x) / 14, (PUNT_Y - y) / 14, 0.5, CRGB(255, 220, 140));
}

// Schiet een stukje spreuk af: sterretjes waaieren uit naar rechtsboven
void schietSpreuk() {
  float richting = atan2(DOEL_Y - PUNT_Y, DOEL_X - PUNT_X);
  for (int i = 0; i < 2; i++) {
    float hoek = richting + willekeurig(-0.45, 0.45);   // uitwaaieren
    float snel = willekeurig(0.2, 0.5);
    maakSterretje(PUNT_X, PUNT_Y, cos(hoek) * snel, sin(hoek) * snel,
                  willekeurig(0.7, 1.2), spreukKleur());
  }
}

// Aan het eind: een grote toverster die uit elkaar spat
void maakToverster() {
  sterKleur = spreukKleur();
  for (int i = 0; i < 16; i++) {
    float hoek = i * 2 * PI / 16;
    maakSterretje(DOEL_X, DOEL_Y, cos(hoek) * 0.3, sin(hoek) * 0.3, 0.9, sterKleur);
  }
}

// ============================================================
void setup() {
  startPanelen();
}

void loop() {
  unsigned long t = millis() - beginTijd;
  float gloed = 0;

  if (t < OPLADEN_MS) {                       // 1. opladen
    gloed = (float)t / OPLADEN_MS;
    opladenSterretjes();
  } else if (t < OPLADEN_MS + SPREUK_MS) {    // 2. de spreuk!
    if (t < OPLADEN_MS + 700) schietSpreuk();     // 0,7 seconde lang sterretjes
    if (!sterGemaakt && t > OPLADEN_MS + 900) { maakToverster(); sterGemaakt = true; }
    gloed = max(0.0f, 1.0f - (t - OPLADEN_MS) / 400.0f);
  } else if (t > OPLADEN_MS + SPREUK_MS + RUST_MS) {   // 3. opnieuw
    beginTijd = millis();
    sterGemaakt = false;
  }

  beweegSterretjes();

  wisAlles();
  tekenStaf(gloed);
  // de grote ster flitst even op
  long sterTijd = (long)t - (OPLADEN_MS + 900);
  if (sterGemaakt && sterTijd < 700) {
    int grootte = sterTijd < 200 ? 2 : 3;
    CRGB k = sterKleur;
    k.nscale8(255 - sterTijd * 255 / 700);
    tekenGroteSter(rond(DOEL_X), rond(DOEL_Y), grootte, k);
  }
  tekenSterretjes();
  toon();
  wacht(BEELDJE_MS);
}
