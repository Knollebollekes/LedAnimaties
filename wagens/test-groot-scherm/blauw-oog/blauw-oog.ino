// ============================================================
//  BLAUW OOG
//  Bedacht door: de begeleiders
//  Wat doet het: een echt lijkend blauw oog dat rondkijkt en
//                af en toe knippert (een beetje zoals het zuster-oog)
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
const int FELHEID = 75;               // 255 = zo fel mogelijk, kleiner = zachter

// Kleuren (vol gemaakt voor de ledjes)
CRGB HUID        = CRGB(255, 120, 35);   // ooglid en huid eromheen
CRGB HUID_DONKER = CRGB(140, 45, 10);    // schaduw en plooi
CRGB WIMPERS     = CRGB(20, 6, 2);       // bijna uit
CRGB WENKBRAUW   = CRGB(70, 25, 5);      // bruin
CRGB OOGWIT      = CRGB(235, 225, 215);
CRGB OOGHOEK     = CRGB(255, 70, 60);    // het roze hoekje aan de binnenkant
CRGB IRIS_LICHT  = CRGB(40, 150, 255);   // binnenkant van de iris
CRGB IRIS        = CRGB(0, 70, 210);     // het blauw
CRGB IRIS_RAND   = CRGB(0, 10, 70);      // donkere rand van de iris
CRGB PUPIL       = CRGB(0, 0, 0);
CRGB LICHTVLEK   = CRGB(255, 255, 255);

const float IRIS_GROOTTE  = 10.5;  // straal van de iris in lampjes
const float PUPIL_GROOTTE = 3.4;   // straal van de pupil
const float KIJK_BREED    = 6.0;   // hoe ver het oog opzij kijkt
const float KIJK_HOOG     = 3.0;   // hoe ver het oog omhoog/omlaag kijkt
const int   KNIPPER_MS    = 300;   // hoe lang een knipper duurt
const int   BEELDJE_MS    = 30;

// ============================================================
const float CX = (BREEDTE - 1) / 2.0;   // midden van het oog
const float CY = HOOGTE / 2.0;
const float HALF_BREED = BREEDTE / 2.0 + 0.5;    // ooghoeken net aan de rand
const float OPEN_BOVEN = HOOGTE * 0.27;          // hoe ver het bovenste ooglid open is
const float OPEN_ONDER = HOOGTE * 0.23;

float kijkX = 0, kijkY = 0, doelX = 0, doelY = 0;
unsigned long volgendeBlik = 0;
unsigned long knipperStart = 0, volgendeKnipper = 2500;
bool dubbel = false;

float willekeurig(float laag, float hoog) {
  return laag + (hoog - laag) * random(0, 1001) / 1000.0;
}

CRGB meng(CRGB a, CRGB b, float hoeveel) {
  return blend(a, b, (uint8_t)(constrain(hoeveel, 0.0, 1.0) * 255));
}

// ---- het oog beweegt ----------------------------------------
// Echte ogen schieten snel naar een nieuw punt en blijven dan even stil.
void beweegOog(unsigned long nu) {
  if (nu >= volgendeBlik) {
    if (random(0, 3) == 0) { doelX = 0; doelY = 0; }      // vaak terug naar het midden
    else { doelX = willekeurig(-KIJK_BREED, KIJK_BREED); doelY = willekeurig(-KIJK_HOOG, KIJK_HOOG); }
    volgendeBlik = nu + random(700, 2600);
  }
  kijkX += (doelX - kijkX) * 0.35;
  kijkY += (doelY - kijkY) * 0.35;
}

// Hoe ver is het oog dicht? 0 = open, 1 = helemaal dicht
float knipperStand(unsigned long nu) {
  if (nu >= volgendeKnipper) {
    knipperStart = nu;
    dubbel = random(0, 6) == 0;               // soms twee keer snel achter elkaar
    volgendeKnipper = nu + (dubbel ? KNIPPER_MS + 120 : random(2500, 6500));
  }
  long t = nu - knipperStart;
  if (t >= KNIPPER_MS) return 0;
  float f = (float)t / KNIPPER_MS;
  return f < 0.4 ? f / 0.4 : (1 - f) / 0.6;   // snel dicht, iets rustiger open
}

// ---- de vorm van de ooglidopening (amandel) -----------------
float vorm(float x) {
  float u = (x - CX) / HALF_BREED;
  return max(0.0f, 1 - u * u);
}

// ---- de iris ------------------------------------------------
CRGB irisKleur(float dx, float dy, float pupilR) {
  float r = sqrt(dx * dx + dy * dy);
  float deel = (r - pupilR) / (IRIS_GROOTTE - pupilR);   // 0 bij de pupil, 1 aan de rand
  CRGB k = meng(IRIS_LICHT, IRIS, deel * 1.3);
  if (deel > 0.75) k = meng(k, IRIS_RAND, (deel - 0.75) / 0.25);
  // strepen die vanuit het midden naar buiten lopen
  float hoek = atan2(dy, dx);
  float streep = sin(hoek * 13) * 0.5 + sin(hoek * 7 + 1.3) * 0.5;
  if (streep > 0.35) k = meng(k, IRIS_LICHT, 0.35);
  if (streep < -0.55) k.nscale8(170);
  return k;
}

// ---- de kleur van één lampje --------------------------------
CRGB kleurOp(int x, int y, float dicht, float pupilR) {
  float v = vorm(x);
  float sluitLijn = CY + 4 * v;                                   // waar de leden elkaar raken
  float boven = (CY - OPEN_BOVEN * v) * (1 - dicht) + sluitLijn * dicht;
  float onder = (CY + OPEN_ONDER * v) * (1 - dicht * 0.3) + sluitLijn * dicht * 0.3;

  // --- buiten de opening: huid -------------------------------
  if (y < boven || y > onder || v <= 0) {
    CRGB k = HUID;
    // wenkbrauw: een boog bovenin
    float u = (x - CX) / (BREEDTE * 0.5);
    float brauw = HOOGTE * 0.10 + 3.0 * u * u - 1.2 * u;
    if (fabs(y - brauw) < 1.3 - 0.5 * fabs(u) && fabs(u) < 0.95) return WENKBRAUW;
    float plooi = boven - 5 - 3 * v;                               // ooglidplooi boven het oog
    if (fabs(y - plooi) < 0.8 && v > 0.15) k = meng(k, HUID_DONKER, 0.7);
    if (y < boven && boven - y < 1.0) k = WIMPERS;                 // wimperlijn
    // wimpers: korte streepjes schuin omhoog
    int wx = (int)(x - CX + 100) % 3;
    float hoogte = boven - y;
    if (y < boven && wx == 0 && hoogte < 2.6 && v > 0.3 && dicht < 0.6) k = WIMPERS;
    if (y > onder && y - onder < 1.0) k = meng(HUID, HUID_DONKER, 0.6); // rand onderste ooglid
    // verder van het oog af wordt de huid donkerder, dan valt het oog meer op
    float afstand = fabs(y - CY) / (HOOGTE / 2.0);
    k.nscale8(255 * constrain(1.15 - afstand, 0.25, 1.0));
    return k;
  }

  // --- in de opening: oogwit, iris en pupil ------------------
  float ix = CX + kijkX, iy = CY - 1 + kijkY;
  float dx = x - ix, dy = y - iy;
  float r = sqrt(dx * dx + dy * dy);

  CRGB k = OOGWIT;
  // oogwit is in de hoeken wat donkerder, dan lijkt het bol
  float hoekje = fabs(x - CX) / HALF_BREED;
  k = meng(k, HUID_DONKER, hoekje * hoekje * 0.6);
  if (x < CX - HALF_BREED * 0.75) k = meng(k, OOGHOEK, 0.5);      // roze binnenhoek

  if (r < IRIS_GROOTTE + 0.5) {
    CRGB iris = (r < pupilR) ? PUPIL : irisKleur(dx, dy, pupilR);
    if (r < pupilR + 0.5 && r >= pupilR) iris = meng(PUPIL, iris, r - pupilR + 0.5);
    k = meng(iris, k, r - IRIS_GROOTTE + 0.5);                     // zachte rand
  }
  // lichtvlekjes (een groot en een klein)
  if (fabs(x - (ix + 3)) < 1.1 && fabs(y - (iy - 3)) < 1.1) k = LICHTVLEK;
  if (fabs(x - (ix - 2.5)) < 0.6 && fabs(y - (iy + 3)) < 0.6) k = meng(k, LICHTVLEK, 0.7);

  // schaduw van het bovenste ooglid op het oog
  float schaduw = y - boven;
  if (schaduw < 3) k.nscale8(150 + schaduw * 35);
  return k;
}

// ============================================================
void setup() {
  startPanelen();
}

void loop() {
  unsigned long nu = millis();
  beweegOog(nu);
  float dicht = knipperStand(nu);
  float pupilR = PUPIL_GROOTTE + sin(nu / 2500.0) * 0.7;   // pupil wordt langzaam iets groter/kleiner

  for (int y = 0; y < HOOGTE; y++) {
    for (int x = 0; x < BREEDTE; x++) {
      CRGB k = kleurOp(x, y, dicht, pupilR);
      k.nscale8(FELHEID);
      zetPixel(x, y, k);
    }
  }
  toon();
  wacht(BEELDJE_MS);
}
