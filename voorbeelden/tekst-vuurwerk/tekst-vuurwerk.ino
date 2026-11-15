// ============================================================
//  TEKST MET VUURWERK
//  Bedacht door: de begeleiders (eerste test)
//  Wat doet het: lichtblauwe tekst schuift van rechts naar links,
//                en eromheen knalt vuurwerk in allerlei kleuren
// ============================================================
#include "paneel.h"

// ---- De tekst: verander hem maar! ---------------------------
const char* TEKST = "Averilde is lief";
CRGB TEKSTKLEUR = CRGB(110, 200, 255);   // lichtblauw
const int TEKST_SNELHEID = 80;           // ms per stapje (kleiner = sneller)
const int TEKST_Y = (HOOGTE - 7) / 2;    // hoogte van de tekst (midden)

// ---- Het vuurwerk -------------------------------------------
const int MAX_RAKETTEN   = 3;     // hoeveel pijlen tegelijk omhoog
const int VONKEN_PER_KNAL = 16;   // hoeveel vonkjes per knal
const int KANS_OP_RAKET  = 6;     // per beeldje 1 op de zoveel kans (kleiner = meer vuurwerk)
const float ZWAARTEKRACHT = 0.012; // hoe snel de vonkjes naar beneden vallen
const int BEELDJE_MS = 30;        // tijd per beeldje

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
//  VUURWERK
// ============================================================
struct Raket { bool actief; float x, y, snelheid, knalHoogte; CRGB kleur; };
struct Vonk  { bool actief; float x, y, vx, vy, leven; CRGB kleur; };

const int MAX_VONKEN = MAX_RAKETTEN * VONKEN_PER_KNAL * 2;
Raket raketten[MAX_RAKETTEN];
Vonk vonken[MAX_VONKEN];

int rond(float f) { return (int)floor(f + 0.5); }

// Een mooie felle kleur, elke keer anders
CRGB feestKleur() {
  return CHSV(random8(), random8(180, 255), 255);
}

// Schiet een nieuwe raket af (als er een plekje vrij is)
void schietRaket() {
  for (int i = 0; i < MAX_RAKETTEN; i++) {
    if (raketten[i].actief) continue;
    Raket& r = raketten[i];
    r.actief = true;
    r.x = random(2, BREEDTE - 2);
    r.y = HOOGTE;
    r.snelheid = 0.35 + random(0, 20) / 100.0;
    // knallen boven de tekst of eronder, dus "eromheen"
    if (random(0, 3) > 0) r.knalHoogte = random(1, max(2, TEKST_Y));                // boven
    else                  r.knalHoogte = random(TEKST_Y + 8, max(TEKST_Y + 9, HOOGTE - 1)); // onder
    r.kleur = feestKleur();
    return;
  }
}

// BOEM! Maak vonkjes die alle kanten op vliegen
void knal(float x, float y, CRGB kleur) {
  bool regenboog = random(0, 4) == 0;   // soms een knal met allemaal kleurtjes
  int gemaakt = 0;
  for (int i = 0; i < MAX_VONKEN && gemaakt < VONKEN_PER_KNAL; i++) {
    if (vonken[i].actief) continue;
    Vonk& v = vonken[i];
    float hoek = (gemaakt + random(0, 50) / 100.0) * 2 * PI / VONKEN_PER_KNAL;
    float kracht = 0.18 + random(0, 22) / 100.0;
    v.actief = true;
    v.x = x; v.y = y;
    v.vx = cos(hoek) * kracht;
    v.vy = sin(hoek) * kracht;
    v.leven = 1.0;
    v.kleur = regenboog ? feestKleur() : kleur;
    gemaakt++;
  }
}

// Laat alles een stapje bewegen
void beweegVuurwerk() {
  for (int i = 0; i < MAX_RAKETTEN; i++) {
    Raket& r = raketten[i];
    if (!r.actief) continue;
    r.y -= r.snelheid;
    if (r.y <= r.knalHoogte) {
      r.actief = false;
      knal(r.x, r.y, r.kleur);
    }
  }
  for (int i = 0; i < MAX_VONKEN; i++) {
    Vonk& v = vonken[i];
    if (!v.actief) continue;
    v.x += v.vx;
    v.y += v.vy;
    v.vx *= 0.96;                 // luchtweerstand
    v.vy = v.vy * 0.96 + ZWAARTEKRACHT;
    v.leven -= 0.025;
    if (v.leven <= 0) v.actief = false;
  }
}

// Tel een kleur op bij wat er al staat (dan mengen de knallen mooi)
void voegToe(int x, int y, CRGB kleur) {
  zetPixel(x, y, leesPixel(x, y) + kleur);
}

void tekenVuurwerk() {
  for (int i = 0; i < MAX_RAKETTEN; i++) {
    Raket& r = raketten[i];
    if (!r.actief) continue;
    voegToe(rond(r.x), rond(r.y), CRGB(255, 200, 120));       // de raket
    voegToe(rond(r.x), rond(r.y) + 1, CRGB(90, 40, 0));       // staartje
  }
  for (int i = 0; i < MAX_VONKEN; i++) {
    Vonk& v = vonken[i];
    if (!v.actief) continue;
    CRGB k = v.kleur;
    float licht = v.leven * v.leven;                // eerst fel, dan snel zachter
    if (v.leven < 0.35 && random(0, 3) == 0) licht = 0; // aan het eind twinkelen
    k.nscale8((uint8_t)(licht * 255));
    voegToe(rond(v.x), rond(v.y), k);
  }
}

// ============================================================
int tekstX = BREEDTE;            // tekst begint net rechts buiten beeld
unsigned long volgendeStap = 0;

void setup() {
  startPanelen();
}

void loop() {
  // vuurwerk
  if (random(0, KANS_OP_RAKET) == 0) schietRaket();
  beweegVuurwerk();

  // tekst een stapje naar links
  if (millis() >= volgendeStap) {
    volgendeStap = millis() + TEKST_SNELHEID;
    tekstX--;
    if (tekstX < -tekstBreedte(TEKST)) tekstX = BREEDTE;  // weer van voren af aan
  }

  // tekenen: eerst vuurwerk, dan de tekst er bovenop
  wisAlles();
  tekenVuurwerk();
  tekenTekst(TEKST, tekstX, TEKST_Y, TEKSTKLEUR);
  toon();
  wacht(BEELDJE_MS);
}
