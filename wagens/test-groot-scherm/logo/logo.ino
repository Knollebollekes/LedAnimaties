// ============================================================
//  LOGO (groot scherm)
//  Bedacht door: de begeleiders
//  Wat doet het: het logo van C.V. Knollebollekes Berghem: het
//                Knollebolleke in de ring, hij knipoogt, de belletjes
//                en confetti glinsteren, en eronder schuift de naam
// ============================================================
#include "paneel.h"

// ---- Het plaatje: 32 x 32, elk teken is één lampje ----------
//  P = paarse rand/muts   Y = gele ring   B = blauwe achtergrond
//  G = blad   g = donker blad   K = gele muts-punt   b = belletje
//  O = oranje rand   C = de biet   W = oogwit   I = oog
//  M = mond   R = tong   w = wit (tanden, handjes)
//  r = roze confetti   l = groene confetti   . = uit
const char* PLAATJE[32] = {
  "............PPGGGGGP............",
  ".........PPYYYGGgGGGGGGGG.......",
  ".......PPGGGYGGGgGGGGGGGGG......",
  "......PPGGGGGGGGgGGGGgGGGG......",
  ".....PYGGgGGGGGGgGGGGGgGGGP.....",
  "....PYYGGGggGGPPPPGGGGGgGGYP....",
  "...PYYYGGGGGgGPPPPPGGGGGGGGYP...",
  "..PYYYYGGGGGGGGPPPPBBGGGGGGGYP..",
  "..PYYYYBBGGGGBPPPPPPBGGGgGGGYP..",
  ".PYYYYBBBBBBBBPPPPPPBGggggGGYYP.",
  ".PYYYPBKKKKKBPPPPPPPggggggGYYYP.",
  ".YYYYBKKKKKKKPPPPPPPPggggggYYYY.",
  "PYYYYBKKKKKKPPPPPPPPPgggggBPYYYP",
  "PYYYPBbbKKKKPPPPPPPPPggggbBPYYYP",
  "PYYYPBbbbKKPPPPPPPPPPgggbbbBYYYP",
  "PYYYBBbbBBBPPPPPPPPPPOBBbbbBYYYY",
  "PYYYBBBBBBOOOOOOOOOOOOOBBBBBYYYY",
  "PYYYPBBBBBOOOOOOOOOOOOOBBBBBYYYP",
  "PYYYPBBBYBBOCCCCCCCCOOBBBwBPYYYP",
  "PYYYYBBBYBBCCWWCCCWWCCBBBwBPYYYP",
  ".YYYYBBBBBCCWWWCCWWWCCCBwwwYYYY.",
  ".PYYYPBBBCCCWIWCCWIWCCCBwwwYYYP.",
  ".PYYYYBBBCrCWIWCCWIWCCCBwwwYYYP.",
  "..PYYYYlBCrCCWCCCCWCCCCBBwYYYY..",
  "..PYYYYPBCCCCCCCCCCCCCCBPYYYYP..",
  "...PYYYYYCCCCMwwwwwMCCrrYYYYP...",
  "....PYYYwwwCCMMRRRMMCCYYYYYP....",
  ".....PYYwwwYCCMRRRMCCYYYYYP.....",
  "......PYYwYYYYCCCCYYYYYYYP......",
  ".......PPYYYYYYCCYYYYYYYP.......",
  ".........PPYYYYYCYYYYPP.........",
  "............PPPPPPPP............",
};

// ---- Kleuren voor op de ledjes ------------------------------
// Ledjes laten lichte kleuren witter zien dan een computerscherm.
// Daarom zijn deze kleuren "voller" gemaakt dan in het plaatje.
const int FELHEID = 110;   // 255 = zo fel mogelijk, kleiner = zachter
CRGB PAARS       = CRGB(70, 12, 170);
CRGB GEEL        = CRGB(220, 140, 0);
CRGB BLAUW       = CRGB(0, 60, 200);
CRGB BLAD        = CRGB(5, 110, 10);
CRGB DONKER_BLAD = CRGB(0, 60, 8);
CRGB MUTS_GEEL   = CRGB(255, 120, 0);
CRGB BELLETJE    = CRGB(255, 200, 30);
CRGB RAND        = CRGB(255, 40, 0);
CRGB BIET        = CRGB(255, 120, 35);   // huidskleur op de ledjes
CRGB OOGWIT      = CRGB(200, 200, 200);
CRGB OOG         = CRGB(60, 25, 5);
CRGB MOND        = CRGB(70, 0, 5);
CRGB TONG        = CRGB(220, 10, 25);
CRGB WIT         = CRGB(200, 200, 200);
CRGB ROZE        = CRGB(230, 20, 90);
CRGB LICHTGROEN  = CRGB(40, 200, 30);

// ---- De tekst eronder ---------------------------------------
const char* TEKST = "C.V. Knollebollekes Berghem";
CRGB TEKSTKLEUR = CRGB(255, 170, 0);   // geel, net als de ring
const int TEKST_SNELHEID = 70;         // ms per stapje (kleiner = sneller)

const int KNIPOOG_ELKE_MS = 3000;
const int OGEN_DICHT_MS   = 250;
const int BEELDJE_MS      = 30;

// ============================================================
//  LETTERS: elke letter is 7 rijen hoog. # = lampje aan.
//  Mis je een letter? Teken hem er zelf bij!
// ============================================================
struct Letter { char teken; const char* rij[7]; };

const Letter LETTERS[] = {
  {' ', {"...", "...", "...", "...", "...", "...", "..."}},
  {'!', {"#", "#", "#", "#", "#", ".", "#"}},
  {'.', {".", ".", ".", ".", ".", ".", "#"}},
  {'A', {".###.", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"}},
  {'B', {"####.", "#...#", "#...#", "####.", "#...#", "#...#", "####."}},
  {'C', {".###.", "#...#", "#....", "#....", "#....", "#...#", ".###."}},
  {'D', {"####.", "#...#", "#...#", "#...#", "#...#", "#...#", "####."}},
  {'E', {"#####", "#....", "#....", "####.", "#....", "#....", "#####"}},
  {'F', {"#####", "#....", "#....", "####.", "#....", "#....", "#...."}},
  {'G', {".###.", "#...#", "#....", "#.###", "#...#", "#...#", ".####"}},
  {'H', {"#...#", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"}},
  {'I', {"###", ".#.", ".#.", ".#.", ".#.", ".#.", "###"}},
  {'J', {"..###", "...#.", "...#.", "...#.", "#..#.", "#..#.", ".##.."}},
  {'K', {"#...#", "#..#.", "#.#..", "##...", "#.#..", "#..#.", "#...#"}},
  {'L', {"#....", "#....", "#....", "#....", "#....", "#....", "#####"}},
  {'M', {"#...#", "##.##", "#.#.#", "#.#.#", "#...#", "#...#", "#...#"}},
  {'N', {"#...#", "#...#", "##..#", "#.#.#", "#..##", "#...#", "#...#"}},
  {'O', {".###.", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."}},
  {'P', {"####.", "#...#", "#...#", "####.", "#....", "#....", "#...."}},
  {'Q', {".###.", "#...#", "#...#", "#...#", "#.#.#", "#..#.", ".##.#"}},
  {'R', {"####.", "#...#", "#...#", "####.", "#.#..", "#..#.", "#...#"}},
  {'S', {".####", "#....", "#....", ".###.", "....#", "....#", "####."}},
  {'T', {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."}},
  {'U', {"#...#", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."}},
  {'V', {"#...#", "#...#", "#...#", "#...#", "#...#", ".#.#.", "..#.."}},
  {'W', {"#...#", "#...#", "#...#", "#.#.#", "#.#.#", "#.#.#", ".#.#."}},
  {'X', {"#...#", "#...#", ".#.#.", "..#..", ".#.#.", "#...#", "#...#"}},
  {'Y', {"#...#", "#...#", ".#.#.", "..#..", "..#..", "..#..", "..#.."}},
  {'Z', {"#####", "....#", "...#.", "..#..", ".#...", "#....", "#####"}},
  {'a', {"....", "....", ".##.", "...#", ".###", "#..#", ".###"}},
  {'b', {"#...", "#...", "###.", "#..#", "#..#", "#..#", "###."}},
  {'c', {"....", "....", ".###", "#...", "#...", "#...", ".###"}},
  {'d', {"...#", "...#", ".###", "#..#", "#..#", "#..#", ".###"}},
  {'e', {"....", "....", ".##.", "#..#", "####", "#...", ".###"}},
  {'f', {"..##", ".#..", ".#..", "###.", ".#..", ".#..", ".#.."}},
  {'g', {"....", ".###", "#..#", "#..#", ".###", "...#", ".##."}},
  {'h', {"#...", "#...", "###.", "#..#", "#..#", "#..#", "#..#"}},
  {'i', {"#", ".", "#", "#", "#", "#", "#"}},
  {'j', {"..#", "...", "..#", "..#", "..#", "#.#", ".#."}},
  {'k', {"#...", "#...", "#..#", "#.#.", "##..", "#.#.", "#..#"}},
  {'l', {"#.", "#.", "#.", "#.", "#.", "#.", ".#"}},
  {'m', {".....", ".....", "##.#.", "#.#.#", "#.#.#", "#.#.#", "#.#.#"}},
  {'n', {"....", "....", "###.", "#..#", "#..#", "#..#", "#..#"}},
  {'o', {"....", "....", ".##.", "#..#", "#..#", "#..#", ".##."}},
  {'p', {"....", "###.", "#..#", "#..#", "###.", "#...", "#..."}},
  {'q', {"....", ".###", "#..#", "#..#", ".###", "...#", "...#"}},
  {'r', {"....", "....", "#.##", "##..", "#...", "#...", "#..."}},
  {'s', {"....", "....", ".###", "#...", ".##.", "...#", "###."}},
  {'t', {".#..", ".#..", "####", ".#..", ".#..", ".#..", "..##"}},
  {'u', {"....", "....", "#..#", "#..#", "#..#", "#..#", ".###"}},
  {'v', {".....", ".....", "#...#", "#...#", "#...#", ".#.#.", "..#.."}},
  {'w', {".....", ".....", "#...#", "#...#", "#.#.#", "#.#.#", ".#.#."}},
  {'x', {"....", "....", "#..#", "#..#", ".##.", "#..#", "#..#"}},
  {'y', {"....", "#..#", "#..#", "#..#", ".###", "...#", ".##."}},
  {'z', {"....", "....", "####", "...#", ".##.", "#...", "####"}},
};
const int AANTAL_LETTERS = sizeof(LETTERS) / sizeof(LETTERS[0]);

// Zoek een letter op. Niet gevonden? Dan wordt het een spatie.
const Letter& zoekLetter(char c) {
  for (int i = 0; i < AANTAL_LETTERS; i++)
    if (LETTERS[i].teken == c) return LETTERS[i];
  return LETTERS[0];
}

int letterBreedte(char c) {
  return strlen(zoekLetter(c).rij[0]);
}

// Hoe breed is de hele tekst (met 1 lampje ruimte tussen de letters)?
int tekstBreedte(const char* tekst) {
  int b = 0;
  for (int i = 0; tekst[i]; i++) b += letterBreedte(tekst[i]) + 1;
  return b;
}

// Teken de tekst met de linkerkant op plek x
void tekenTekst(const char* tekst, int x, int y, CRGB kleur) {
  for (int i = 0; tekst[i]; i++) {
    const Letter& l = zoekLetter(tekst[i]);
    int b = letterBreedte(tekst[i]);
    if (x + b >= 0 && x < BREEDTE) {          // alleen als hij te zien is
      for (int ry = 0; ry < 7; ry++)
        for (int rx = 0; rx < b; rx++)
          if (l.rij[ry][rx] == '#') zetPixel(x + rx, y + ry, kleur);
    }
    x += b + 1;
  }
}

// ============================================================
//  HET LOGO
// ============================================================
// Het logo staat bovenaan in het midden, de tekst eronder
const int LOGO_X = (BREEDTE - 32) / 2;
const int LOGO_Y = 0;
const int TEKST_Y = 32 + (HOOGTE - 32 - 7) / 2;

unsigned long volgendeKnipoog = 2000;
bool alleenRechts = false;

CRGB kleurVan(char c) {
  switch (c) {
    case 'P': return PAARS;
    case 'Y': return GEEL;
    case 'B': return BLAUW;
    case 'G': return BLAD;
    case 'g': return DONKER_BLAD;
    case 'K': return MUTS_GEEL;
    case 'b': return BELLETJE;
    case 'O': return RAND;
    case 'C': return BIET;
    case 'W': return OOGWIT;
    case 'I': return OOG;
    case 'M': return MOND;
    case 'R': return TONG;
    case 'w': return WIT;
    case 'r': return ROZE;
    case 'l': return LICHTGROEN;
  }
  return CRGB::Black;
}

void tekenLogo(bool ogenDicht) {
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 32; x++) {
      char c = PLAATJE[y][x];
      CRGB k = kleurVan(c);
      // ogen dicht: het oog wordt biet, met onderaan een streepje
      bool ditOog = !alleenRechts || x >= 16;
      if (ogenDicht && (c == 'W' || c == 'I') && ditOog) {
        bool onderkant = PLAATJE[y + 1][x] != 'W' && PLAATJE[y + 1][x] != 'I';
        k = onderkant ? MOND : BIET;
      }
      // belletjes en confetti glinsteren
      if (c == 'b') k.nscale8(120 + sin8(millis() / 3 + x * 40) * 135 / 255);
      if (c == 'r' || c == 'l') k.nscale8(sin8(millis() / 5 + x * 70 + y * 30));
      k.nscale8(FELHEID);
      zetPixel(LOGO_X + x, LOGO_Y + y, k);
    }
  }
}

// ============================================================
int tekstX = BREEDTE;
unsigned long volgendeStap = 0;

void setup() {
  startPanelen();
}

void loop() {
  unsigned long nu = millis();

  // knipogen
  bool ogenDicht = false;
  if (nu >= volgendeKnipoog) {
    ogenDicht = true;
    if (nu >= volgendeKnipoog + OGEN_DICHT_MS) {
      volgendeKnipoog = nu + KNIPOOG_ELKE_MS + random(0, 1500);
      alleenRechts = random(0, 2) == 0;
    }
  }

  // tekst een stapje naar links
  if (nu >= volgendeStap) {
    volgendeStap = nu + TEKST_SNELHEID;
    tekstX--;
    if (tekstX < -tekstBreedte(TEKST)) tekstX = BREEDTE;
  }

  wisAlles();
  tekenLogo(ogenDicht);
  CRGB tk = TEKSTKLEUR;
  tk.nscale8(FELHEID);
  tekenTekst(TEKST, tekstX, TEKST_Y, tk);
  toon();
  wacht(BEELDJE_MS);
}
