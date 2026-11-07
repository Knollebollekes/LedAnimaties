// ============================================================
//  INSTELLINGEN VAN DE LEDPANELEN
//  Dit bestand vertelt de ESP32 hoe jullie panelen zijn
//  aangesloten. Pas dit aan het begin van een project aan,
//  daarna hoef je hier niet meer aan te komen.
// ============================================================
#pragma once

// --- Aansluiting ---------------------------------------------
#define DATA_PIN        5        // op welke pin van de ESP32 zit de datadraad?
#define LED_TYPE        WS2812B  // soort ledjes
#define KLEURVOLGORDE   GRB      // klopt rood/groen niet? probeer RGB

// --- Hoeveel panelen? ----------------------------------------
#define PANEEL_GROOTTE  12       // een paneel is 12 x 12 ledjes
#define PANELEN_BREED   1        // hoeveel panelen naast elkaar
#define PANELEN_HOOG    1        // hoeveel panelen onder elkaar

// --- Hoe lopen de ledjes BINNEN een paneel? -----------------
// Waar zit het eerste ledje (waar de draad binnenkomt)?
//   0 = linksboven   1 = rechtsboven   2 = linksonder   3 = rechtsonder
#define EERSTE_LED_HOEK   0
// true  = de ledjes lopen in rijen (van links naar rechts)
// false = de ledjes lopen in kolommen (van boven naar beneden)
#define RIJEN_HORIZONTAAL true
// true  = slangetje: de volgende rij loopt terug (meest gebruikelijk)
// false = elke rij begint weer aan dezelfde kant
#define SLANG             true

// --- Hoe zijn de panelen aan ELKAAR gekoppeld? ---------------
// Paneel 1 is linksboven, daarna gaat de draad naar rechts.
// true  = de tweede rij panelen loopt terug van rechts naar links
// false = elke rij panelen begint weer links
#define PANELEN_ZIGZAG    false

// --- Veiligheid en stroom ------------------------------------
#define HELDERHEID        40     // 0 t/m 255. Buiten is 40-80 al fel genoeg!
#define MAX_MILLIAMPERE   3000   // wat de voeding mag leveren (5 volt)
