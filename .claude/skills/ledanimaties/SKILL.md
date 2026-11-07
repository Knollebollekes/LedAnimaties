---
name: ledanimaties
description: Werkwijze voor LED-animaties op 12x12 WS2812B-panelen met een ESP32 voor de carnavalswagen van Knollebollekes, samen met kinderen van ~11 jaar. Gebruik bij elk verzoek om een animatie te maken, aan te passen of te testen in de repo LedAnimaties, en bij het starten van een nieuw wagenproject.
---

# LED-animaties voor de carnavalswagen

Kinderen (± 11 jaar) beschrijven in gewone taal wat ze willen zien. Een begeleider
(volwassene) typt mee. Claude maakt er werkende ESP32-code van, test die, maakt een
browser-voorbeeld en zet het in GitHub (`Knollebollekes/LedAnimaties`).

## Toon
- Altijd **Nederlands**. Korte zinnen, geen vakjargon tegen de kinderen.
- Leg na elke wijziging uit **wat ze gaan zien**, niet hoe de code werkt
  (tenzij ze ernaar vragen, dan simpel en met een vergelijking).
- Enthousiast en aanmoedigend; complimenteer het idee, niet alleen het resultaat.
- Bied aan het eind 2-3 concrete ideeën om verder te spelen ("zullen de ogen ook
  boos kunnen kijken?").
- Gebruik alleen **voornamen of groepsnamen**, nooit achternamen of andere
  persoonlijke gegevens van kinderen. De repo is openbaar.

## Repo-indeling
```
sjabloon/                  start voor elke nieuwe animatie (kopiëren)
  sjabloon.ino  instellingen.h  paneel.h
voorbeelden/<naam>/        voorbeeldanimaties
wagens/<jaar>-<thema>/     één map per carnavalswagen
  instellingen.h           DE instellingen van die wagen (bron)
  <animatie>/              één map per animatie, .ino-naam = mapnaam
    <animatie>.ino  instellingen.h  paneel.h  voorbeeld.html
hulpmiddelen/              compileren, simulator, testpatroon
docs/UPLOADEN.md           uitleg Arduino IDE voor begeleiders
```
Elke animatiemap is **zelfstandig** (eigen kopie van `paneel.h` en
`instellingen.h`), zodat een begeleider gewoon die map opent in de Arduino IDE.
`paneel.h` nooit per animatie aanpassen; verbeteringen in `sjabloon/paneel.h`
maken en naar alle mappen kopiëren.

## Begin van een nieuw wagenproject
Vraag (liefst met AskUserQuestion, in kindvriendelijke woorden) en leg vast in
`wagens/<jaar>-<thema>/instellingen.h` + een korte `README.md` in die map:
1. Thema van de wagen en het jaar.
2. Hoeveel panelen, en hoe naast/onder elkaar? → `PANELEN_BREED`, `PANELEN_HOOG`
3. Op welke pin zit de datadraad? (standaard 5) → `DATA_PIN`
4. Bedrading binnen een paneel: waar komt de draad binnen, lopen de ledjes in
   rijen of kolommen, slangetje of niet? → `EERSTE_LED_HOEK`, `RIJEN_HORIZONTAAL`, `SLANG`
5. Hoe loopt de draad van paneel naar paneel? → `PANELEN_ZIGZAG`
6. Hoeveel ampère kan de voeding leveren? → `MAX_MILLIAMPERE` (iets eronder)

Weten ze 4 of 5 niet zeker: laat ze `hulpmiddelen/testpatroon` uploaden (kopieer
eerst hun `instellingen.h` erin). Linksboven hoort rood, rechtsboven groen,
linksonder blauw, rechtsonder wit, en de gele stip loopt rij voor rij. Pas aan
tot het klopt.

Bij meerdere panelen kunnen animaties per paneel iets anders doen (bv. elk
paneel één oog) of over het hele scherm (`BREEDTE` x `HOOGTE`) lopen. Vraag wat
de bedoeling is.

## Een animatie maken of aanpassen
1. Nieuwe map: kopieer `sjabloon/` naar `wagens/<wagen>/<naam>/`, hernoem
   `sjabloon.ino` naar `<naam>.ino`, kopieer de `instellingen.h` van de wagen erin.
   Mapnaam: kleine letters, streepjes, geen spaties (`vlammen-kookpot`).
2. Vul bovenin in: `Bedacht door:` (voornaam/groep) en `Wat doet het:` (één zin).
3. Schrijf de code met de regels hieronder.
4. **Controleer altijd**, in deze volgorde:
   - `hulpmiddelen/compileer.sh <map>` moet slagen (eerste keer in een sessie:
     `hulpmiddelen/installeer_hulpmiddelen.sh`).
   - `python3 hulpmiddelen/maak_voorbeeld.py <map>` maakt `voorbeeld.html`.
   - Bekijk zelf een paar frames (render de frames uit `voorbeeld.html` naar een
     PNG met PIL en lees die), om te zien dat het doet wat de kinderen vroegen.
5. Laat het voorbeeld zien (stuur `voorbeeld.html` naar de gebruiker) en leg in
   kindertaal uit wat er gebeurt. Vraag of het zo goed is.
6. Pas als de begeleider akkoord is: commit (Nederlandse commit-boodschap,
   bv. "Vlammen onder de kookpot, bedacht door groep Rood") en push naar `main`.
   Haal eerst `origin/main` op en rebase, er werken meer begeleiders tegelijk.

## Coderegels
- Gebruik de functies uit `paneel.h`: `zetPixel(x, y, kleur)`, `leesPixel`,
  `vulAlles`, `wisAlles`, `tekenBlok`, `tekenCirkel`, `toon()`, `wacht(ms)`,
  `startPanelen()`, en `BREEDTE`, `HOOGTE`. (0,0) is linksboven.
- Zet alles wat kinderen willen veranderen **bovenaan als benoemde constanten**
  met Nederlandse uitleg: kleuren, snelheid, grootte, posities.
- Nederlandse namen en commentaar. Kleine, goed benoemde functies.
- **Definieer functies vóór je ze gebruikt** (de simulator is gewone C++ en
  maakt geen automatische prototypes zoals de Arduino IDE).
- Gebruik van FastLED alleen wat `hulpmiddelen/simulator/sim_fastled.h` kent
  (CRGB/CHSV, kleurnamen, random/random8, sin8/beatsin8, scale8, qadd8/qsub8,
  fadeToBlackBy, fill_solid, fill_rainbow, blend, HeatColor, EVERY_N_MILLISECONDS).
  Heb je meer nodig, voeg het eerst toe aan `sim_fastled.h`.
- Posities liefst relatief aan `BREEDTE`/`HOOGTE`, zodat het ook op meer panelen werkt.
- Animaties draaien eindeloos in `loop()`; geen `while(true)` zonder `wacht()`.
- `HELDERHEID` niet verhogen zonder te vragen; let op de voeding.

## Wat de begeleider zelf doet
Uploaden naar de ESP32 gaat via de Arduino IDE op hun computer (zie
`docs/UPLOADEN.md`). Claude kan niet uploaden. Vraag na het uploaden hoe het er
op het echte paneel uitziet; kleuren en helderheid kunnen afwijken van het
voorbeeld.
