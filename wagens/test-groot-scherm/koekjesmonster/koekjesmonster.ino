// ============================================================
//  KOEKJESMONSTER
//  Bedacht door: een kind van de Knollebollekes
//  Wat doet het: een paars monster met één groot oog en twee
//                hoorntjes eet koekjes: hap! kruimels! en daarna
//                springt hij blij op en neer
// ============================================================
#include "paneel.h"

// ---- Verander het maar! -------------------------------------
const int FELHEID = 75;              // 255 = zo fel mogelijk, kleiner = zachter

CRGB LIJF        = CRGB(120, 0, 200);   // paars
CRGB LIJF_DONKER = CRGB(50, 0, 90);
CRGB BUIK        = CRGB(190, 70, 255);  // lichtpaarse buik
CRGB HOORN       = CRGB(255, 170, 0);   // gele hoorntjes
CRGB OOGWIT      = CRGB(200, 200, 200);
CRGB IRIS        = CRGB(0, 190, 40);    // groen oog
CRGB PUPIL       = CRGB(0, 0, 0);
CRGB MOND        = CRGB(80, 0, 10);
CRGB TONG        = CRGB(255, 30, 80);
CRGB TAND        = CRGB(200, 200, 200);
CRGB KOEKJE      = CRGB(200, 90, 10);
CRGB CHOCO       = CRGB(35, 10, 0);
CRGB KRUIMEL     = CRGB(160, 70, 5);

const int AANTAL_KOEKJES = 3;        // hoeveel koekjes per keer
const int KOEKJE_MS      = 2200;     // tijd per koekje
const int BLIJ_MS        = 2500;     // daarna blij springen
const int BEELDJE_MS     = 30;

// ============================================================
const float CX = (BREEDTE - 1) / 2.0;
const float S  = BREEDTE / 32.0;            // schaal
const float MOND_X = CX, MOND_Y = HOOGTE * 0.69;
const float OOG_X = CX, OOG_Y = HOOGTE * 0.46;

struct Kruimel { bool actief; float x, y, vx, vy; };
Kruimel kruimels[30];
int laatsteHap = -1;
unsigned long beginTijd = 0;

void beginAnimatie() {
  beginTijd = millis();
  laatsteHap = -1;
  for (int i = 0; i < 30; i++) kruimels[i].actief = false;
}

float willekeurig(float laag, float hoog) {
  return laag + (hoog - laag) * random(0, 1001) / 1000.0;
}

void zet(int x, int y, CRGB k) {
  k.nscale8(FELHEID);
  zetPixel(x, y, k);
}

// ---- het monster --------------------------------------------
// Is (x, y) binnen een afgeronde rechthoek?
bool inBlok(float x, float y, float x0, float y0, float x1, float y1, float r) {
  float dx = max(max(x0 + r - x, x - (x1 - r)), 0.0f);
  float dy = max(max(y0 + r - y, y - (y1 - r)), 0.0f);
  return dx * dx + dy * dy <= r * r && x >= x0 && x <= x1 && y >= y0 && y <= y1;
}

// open: 0 = mond dicht, 1 = wijd open.  kijkX/kijkY: waar het oog naar kijkt.
// blij: oog dichtgeknepen van plezier.  dy: op en neer springen.
void tekenMonster(float open, float kijkX, float kijkY, bool blij, float dy) {
  float boven = 14 * S + dy, onder = HOOGTE - 3 + dy;
  float links = 4 * S, rechts = BREEDTE - 1 - 4 * S;

  for (int y = 0; y < HOOGTE; y++) {
    for (int x = 0; x < BREEDTE; x++) {
      CRGB k;
      bool iets = false;

      // hoorntjes: schuin naar buiten en omhoog
      for (int kant = -1; kant <= 1; kant += 2) {
        float hx = CX + kant * 8 * S, hy = boven + 1;
        float t = (hy - y) / (8 * S);                    // 0 onderaan, 1 in de punt
        if (t >= 0 && t <= 1) {
          float midden = hx + kant * 2.5 * S * t * t;    // buigt naar buiten
          float breed = 2.2 * S * (1 - t) + 0.4;
          if (fabs(x - midden) < breed) { k = HOORN; iets = true; }
        }
      }

      // lijf
      if (inBlok(x, y, links, boven, rechts, onder, 9 * S)) {
        k = LIJF;
        if (x > rechts - 3 * S) k = blend(LIJF, LIJF_DONKER, 140);      // schaduw rechts
        float buik = sq((x - CX) / (7 * S)) + sq((y - (onder - 6 * S)) / (5 * S));
        if (buik < 1) k = BUIK;
        iets = true;
      }
      // voetjes
      if (y >= onder - 1 && y <= onder + 2 && (fabs(x - (CX - 6 * S)) < 3 * S || fabs(x - (CX + 6 * S)) < 3 * S)) {
        k = LIJF_DONKER; iets = true;
      }

      // het grote oog
      float ox = OOG_X, oy = OOG_Y + dy;
      float oogR = 6.5 * S;
      float d = sqrt(sq(x - ox) + sq(y - oy));
      if (d < oogR) {
        if (blij) {
          // blij: het oog is een vrolijk boogje ^
          k = LIJF;
          float boog = oy + 0.09 * sq(x - ox) / S - 1.5;
          if (fabs(y - boog) < 1.1 && fabs(x - ox) < oogR - 1) k = OOGWIT;
        } else {
          k = OOGWIT;
          float ix = ox + kijkX * 2.5 * S, iy = oy + kijkY * 2.5 * S;
          float di = sqrt(sq(x - ix) + sq(y - iy));
          if (di < 3.5 * S) k = IRIS;
          if (di < 1.7 * S) k = PUPIL;
          if (fabs(x - (ix + 1.2 * S)) < 0.8 && fabs(y - (iy - 1.2 * S)) < 0.8) k = OOGWIT;  // lichtvlekje
        }
        iets = true;
      }

      // de mond
      float mx = MOND_X, my = MOND_Y + dy;
      float mb = 8 * S, mh = 0.6 + open * 4.5 * S;
      float m = sq((x - mx) / mb) + sq((y - my) / mh);
      if (m < 1 && y >= my - 0.5) {                      // onderste helft: een grote grijns
        k = (y > my + mh * 0.5 && fabs(x - mx) < mb * 0.5) ? TONG : MOND;
        iets = true;
      }
      if (open > 0.2 && fabs(y - my) < 0.6 && fabs(x - mx) < mb - 1 && ((int)(x + 0.5) % 2 == 0)) {
        k = TAND; iets = true;                          // puntige tandjes bovenaan
      }
      if (open <= 0.2 && fabs(y - (my + 0.03 * sq(x - mx) / S)) < 0.6 && fabs(x - mx) < mb) {
        k = MOND; iets = true;                          // mond dicht: een lach-streep
      }

      if (iets) zet(x, y, k);
    }
  }
}

// ---- koekjes en kruimels ------------------------------------
void tekenKoekje(float kx, float ky) {
  float r = 4 * S;
  for (int y = (int)(ky - r) - 1; y <= ky + r + 1; y++)
    for (int x = (int)(kx - r) - 1; x <= kx + r + 1; x++) {
      float d = sqrt(sq(x - kx) + sq(y - ky));
      if (d > r) continue;
      CRGB k = KOEKJE;
      // chocoladestukjes
      int px = (int)floor(x - kx + 10), py = (int)floor(y - ky + 10);
      if ((px * 7 + py * 3) % 9 == 0) k = CHOCO;
      if (d > r - 0.8) k = blend(KOEKJE, CHOCO, 70);
      zet(x, y, k);
    }
}

void maakKruimels(float x, float y) {
  for (int i = 0; i < 30; i++) {
    if (kruimels[i].actief) continue;
    kruimels[i] = {true, x + willekeurig(-3, 3), y, willekeurig(-0.4, 0.4), willekeurig(-0.6, 0.1)};
    if (random(0, 3) == 0) break;   // niet allemaal tegelijk
  }
}

void beweegEnTekenKruimels() {
  for (int i = 0; i < 30; i++) {
    Kruimel& k = kruimels[i];
    if (!k.actief) continue;
    k.x += k.vx; k.y += k.vy; k.vy += 0.06;
    if (k.y >= HOOGTE) { k.actief = false; continue; }
    zet((int)round(k.x), (int)round(k.y), KRUIMEL);
  }
}

// ============================================================
void tekenBeeldje(unsigned long tijd) {
  const unsigned long RONDE = AANTAL_KOEKJES * KOEKJE_MS + BLIJ_MS;
  unsigned long t = tijd % RONDE;
  int nummer = t / KOEKJE_MS;                 // welk koekje (of: blij)
  wisAlles();

  float open = 0, kijkX = 0, kijkY = 0, dy = 0;
  bool blij = false;
  bool koekjeTonen = false;
  float kx = 0, ky = 0;

  if (nummer < AANTAL_KOEKJES) {
    unsigned long kt = t % KOEKJE_MS;
    const int VLIEG = 1300, HAP = 150;
    // waar komt het koekje vandaan? om de beurt rechts- en linksboven
    float startX = (nummer % 2 == 0) ? BREEDTE + 4 : -5;
    float startY = MOND_Y - 10;                          // van opzij, langs het oog heen
    if (kt < VLIEG) {
      float f = (float)kt / VLIEG;
      kx = startX + (MOND_X - startX) * f;
      ky = startY + (MOND_Y - startY) * f * f;           // boogje naar de mond
      koekjeTonen = true;
      // het oog volgt het koekje
      kijkX = constrain((kx - OOG_X) / 12.0, -1.0, 1.0);
      kijkY = constrain((ky - OOG_Y) / 12.0, -1.0, 1.0);
      open = f > 0.45 ? (f - 0.45) / 0.55 : 0;            // mond gaat steeds verder open
    } else if (kt < VLIEG + HAP) {
      open = 1.0 - (float)(kt - VLIEG) / HAP;             // HAP!
    } else {
      open = (((kt - VLIEG) / 180) % 2) * 0.35;            // kauwen: mond op en neer
      kijkY = 0.4;
    }
    if (kt >= VLIEG + HAP / 2 && laatsteHap != (int)(tijd / KOEKJE_MS)) {
      laatsteHap = tijd / KOEKJE_MS;
      maakKruimels(MOND_X, MOND_Y);
    }
    if (kt > VLIEG + HAP && kt < VLIEG + HAP + 400 && random(0, 4) == 0) maakKruimels(MOND_X, MOND_Y + 1);
  } else {
    // blij! op en neer springen met een vrolijk oog
    unsigned long bt = t - AANTAL_KOEKJES * KOEKJE_MS;
    dy = -fabs(sin(bt * PI / 500.0)) * 3 * S;
    blij = true;
    open = 0.6;                                           // grote blije grijns
  }

  tekenMonster(open, kijkX, kijkY, blij, dy);
  if (koekjeTonen) tekenKoekje(kx, ky);
  beweegEnTekenKruimels();
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
