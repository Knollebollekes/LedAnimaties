// ============================================================
//  INSTELLINGEN VAN DE LEDPANELEN
//  Dit bestand vertelt de ESP32 hoe jullie panelen zijn
//  aangesloten. Pas dit aan het begin van een project aan,
//  daarna hoef je hier niet meer aan te komen.
// ============================================================
#pragma once

// --- Aansluiting ---------------------------------------------
#define DATA_PIN        14       // op welke pin van de ESP32 zit de datadraad?
#define LED_TYPE        WS2812B  // soort ledjes
#define KLEURVOLGORDE   GRB      // klopt rood/groen niet? probeer RGB

// --- Hoeveel panelen? ----------------------------------------
#define PANEEL_GROOTTE  16       // een paneel is 16 x 16 ledjes
#define PANELEN_BREED   1        // hoeveel panelen naast elkaar
#define PANELEN_HOOG    1        // hoeveel panelen onder elkaar

// --- Hoe lopen de ledjes BINNEN een paneel? -----------------
// Waar zit het eerste ledje (waar de draad binnenkomt)?
//   0 = linksboven   1 = rechtsboven   2 = linksonder   3 = rechtsonder
#define EERSTE_LED_HOEK   1
// true  = de ledjes lopen in rijen (van links naar rechts)
// false = de ledjes lopen in kolommen (van boven naar beneden)
#define RIJEN_HORIZONTAAL true
// true  = slangetje: de volgende rij loopt terug (meest gebruikelijk)
// false = elke rij begint weer aan dezelfde kant
#define SLANG             true

// --- Hoe zijn de panelen aan ELKAAR gekoppeld? ---------------
// Waar zit paneel 1 (waar de draad het eerste paneel in gaat)?
// false = paneel 1 is linksboven, de draad gaat naar rechts
// true  = paneel 1 is rechtsboven, de draad gaat naar links
#define PANELEN_VAN_RECHTS false
// true  = de tweede rij panelen loopt de andere kant op terug (slangetje)
// false = elke rij panelen begint weer aan dezelfde kant
#define PANELEN_ZIGZAG    false

// --- Veiligheid en stroom ------------------------------------
#define HELDERHEID        40     // 0 t/m 255. Buiten is 40-80 al fel genoeg!
#define MAX_MILLIAMPERE   1500   // powerbank (meestal 2-3 A per poort); ruim eronder
