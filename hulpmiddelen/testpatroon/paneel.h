// ============================================================
//  PANEEL-HULPJE
//  Hiermee kun je denken in "kolom x, rij y" in plaats van
//  "ledje nummer 87". Linksboven is (0, 0).
//  x gaat naar rechts, y gaat naar beneden.
//
//  Kinderen hoeven dit bestand niet te veranderen.
// ============================================================
#pragma once

#ifdef SIMULATOR
  #include "sim_fastled.h"   // voor het voorbeeld in de browser
#else
  #include <FastLED.h>       // voor de echte ESP32
#endif

#include "instellingen.h"

// Hoe groot is het hele scherm (alle panelen samen)?
#define BREEDTE   (PANEEL_GROOTTE * PANELEN_BREED)
#define HOOGTE    (PANEEL_GROOTTE * PANELEN_HOOG)
#define AANTAL_LEDS (BREEDTE * HOOGTE)

CRGB leds[AANTAL_LEDS];

// Rekent (x, y) om naar het nummer van het ledje op de draad.
// Geeft -1 als (x, y) buiten het scherm valt.
int ledNummer(int x, int y) {
  if (x < 0 || y < 0 || x >= BREEDTE || y >= HOOGTE) return -1;

  const int P = PANEEL_GROOTTE;
  int paneelKolom = x / P;
  int paneelRij   = y / P;
  int lx = x % P;
  int ly = y % P;

  if (PANELEN_ZIGZAG && (paneelRij % 2 == 1)) {
    paneelKolom = PANELEN_BREED - 1 - paneelKolom;
  }
  int paneel = paneelRij * PANELEN_BREED + paneelKolom;

  // Draai zo dat het eerste ledje "linksboven" komt te liggen
  if (EERSTE_LED_HOEK == 1 || EERSTE_LED_HOEK == 3) lx = P - 1 - lx;
  if (EERSTE_LED_HOEK == 2 || EERSTE_LED_HOEK == 3) ly = P - 1 - ly;

  int nummer;
  if (RIJEN_HORIZONTAAL) {
    if (SLANG && (ly % 2 == 1)) lx = P - 1 - lx;
    nummer = ly * P + lx;
  } else {
    if (SLANG && (lx % 2 == 1)) ly = P - 1 - ly;
    nummer = lx * P + ly;
  }
  return paneel * P * P + nummer;
}

// Zet één ledje op een kleur. Buiten het scherm? Dan gebeurt er niets.
void zetPixel(int x, int y, CRGB kleur) {
  int n = ledNummer(x, y);
  if (n >= 0) leds[n] = kleur;
}

// Welke kleur heeft een ledje nu?
CRGB leesPixel(int x, int y) {
  int n = ledNummer(x, y);
  if (n >= 0) return leds[n];
  return CRGB::Black;
}

// Maak het hele scherm één kleur
void vulAlles(CRGB kleur) {
  for (int i = 0; i < AANTAL_LEDS; i++) leds[i] = kleur;
}

// Alles uit
void wisAlles() {
  vulAlles(CRGB::Black);
}

// Teken een rechthoek (gevuld)
void tekenBlok(int x, int y, int breed, int hoog, CRGB kleur) {
  for (int yy = y; yy < y + hoog; yy++)
    for (int xx = x; xx < x + breed; xx++)
      zetPixel(xx, yy, kleur);
}

// Teken een gevulde cirkel rond (mx, my)
void tekenCirkel(float mx, float my, float straal, CRGB kleur) {
  for (int y = 0; y < HOOGTE; y++)
    for (int x = 0; x < BREEDTE; x++) {
      float dx = x - mx, dy = y - my;
      if (dx * dx + dy * dy <= straal * straal) zetPixel(x, y, kleur);
    }
}

// Laat zien wat je getekend hebt
void toon() {
  FastLED.show();
}

// Wacht een aantal milliseconden (1000 = 1 seconde)
void wacht(int milliseconden) {
  delay(milliseconden);
}

// Zet de panelen klaar. Roep dit één keer aan in setup().
void startPanelen() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, KLEURVOLGORDE>(leds, AANTAL_LEDS);
  FastLED.setBrightness(HELDERHEID);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPERE);
  wisAlles();
  toon();
}
