---
name: ledanimaties
description: Werkwijze voor LED-animaties op 16x16 WS2812B-panelen met een ESP32 voor de carnavalswagen van Knollebollekes, samen met kinderen van ~11 jaar. Gebruik bij elk verzoek om een animatie te maken, aan te passen of te testen in de repo LedAnimaties, en bij het starten van een nieuw wagenproject.
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
hulpmiddelen/site/         de pagina van de website
.github/workflows/         bouwt en publiceert de website bij elke push
docs/UPLOADEN.md           uitleg uploaden (website en Arduino IDE)
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
3. Op welke pin zit de datadraad? (standaard 14) → `DATA_PIN`
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

## De website ("zet op paneel")
Bij elke push naar `main` bouwt GitHub Actions (`.github/workflows/website.yml`,
script `hulpmiddelen/maak_site.py`) alle animaties en zet ze op
https://knollebollekes.github.io/LedAnimaties/ met per animatie een
"Zet op ESP32"-knop (ESP Web Tools, alleen Chrome/Edge). Alles op `main` staat dus
binnen een paar minuten live: daarom pas pushen na akkoord van de begeleider.
Nieuwe animatiemappen in `voorbeelden/` of `wagens/<wagen>/` verschijnen vanzelf;
de groepsnaam komt uit de eerste `# kop` van `wagens/<wagen>/README.md`.
Lokaal testen: `python3 hulpmiddelen/maak_site.py --uit <map>`.
Vertel na een push dat het over een paar minuten op de website staat.

## Als er iets misgaat: uitleg voor kinderen
Elke fout of elk probleem (compileren, voorbeeld, website-bouw, uploaden, paneel
doet raar) leg je in de chat uit zodat een kind van 11 het snapt. Nooit een
kale foutmelding plakken. Altijd deze vorm:

> **🔧 Oeps! [korte naam van het probleem]**
> **Wat is er gebeurd?** Eén of twee zinnen, met een vergelijking uit het
> dagelijks leven (recept, zin zonder punt, verkeerde stekker, ...).
> **Wie lost het op?** "Ik los het zelf op" of "Hier hebben we jullie hulp bij nodig".
> **Wat doen we nu?** Genummerde stapjes, elk één handeling die je kunt zien
> of aanraken ("kijk of het lampje op de ESP32 brandt").
> *Voor de begeleider:* één regel met de technische oorzaak (bv. de echte
> foutmelding, regelnummer, pin).

Regels:
- Kalm en positief: fouten horen bij programmeren, ook echte programmeurs
  maken ze de hele dag. Nooit de schuld bij een kind leggen.
- Fout in code die Claude zelf schreef: zeg eerlijk "Ik had een foutje
  gemaakt", los het op, test opnieuw en vertel daarna dat het gelukt is. Vraag
  pas iets aan de kinderen als het echt aan de hardware ligt.
- Eén probleem tegelijk; het meest waarschijnlijke eerst.
- Vraag na de stapjes: "Lukt het nu? Wat zie je?"

Vertalingen van veelvoorkomende fouten:
| Wat er gebeurt | Uitleg voor kinderen | Oorzaak (voor begeleider) |
|---|---|---|
| `expected ';'` | Er mist een punt aan het eind van een zin. De computer weet niet waar de zin ophoudt. | puntkomma vergeten |
| `was not declared in this scope` | De computer kent dat woord niet, net als een woord dat niet in het woordenboek staat. Vaak een typfout of een hoofdletter verkeerd. | onbekende naam / typfout / functie nog niet gedefinieerd |
| `expected '}'` / haakjes | Een haakje is open blijven staan, zoals een deur die je vergeet dicht te doen. | accolades/haakjes niet in balans |
| Website-bouw mislukt | De robot van GitHub die de animaties klaarzet, liep ergens op vast. | Actions-run mislukt; kijk naar de stap |
| ESP32 staat niet in het lijstje | De computer ziet de ESP32 niet. Soms is de kabel alleen een oplaadkabel, zonder "praatdraadjes". | kabel, driver (CP210x/CH340), Chrome/Edge |
| Blijft hangen bij verbinden | De ESP32 luistert niet. Houd het knopje BOOT ingedrukt, dan luistert hij wel. | BOOT-knop bij flashen |
| Paneel blijft donker | De ledjes krijgen geen bericht of geen stroom. Net als een lamp waar de stekker niet in zit. | DIN-kant, pin 14, gedeelde GND, 5V-voeding |
| Verkeerde kleuren (rood is groen) | De ledjes lezen de kleuren in een andere volgorde, alsof ze de kleuren door elkaar halen. | `KLEURVOLGORDE` GRB ↔ RGB |
| Beeld gespiegeld of door elkaar | Het paneel denkt dat de ledjes anders lopen dan ze echt lopen. | bedrading-instellingen; testpatroon |
| Flikkeren / willekeurige kleurtjes | Het bericht naar de ledjes komt rommelig aan, alsof iemand door de telefoon kraakt. | gedeelde GND, weerstand, kabel te lang |
| ESP32 herstart of wordt dimmer bij wit | De ledjes willen meer stroom dan de voeding kan geven. Ze hebben honger! | voeding te zwak, `HELDERHEID`/`MAX_MILLIAMPERE` |

## Wat de begeleider zelf doet
Uploaden naar de ESP32 doet de begeleider (of een kind met de begeleider) via de
website, of via de Arduino IDE (zie `docs/UPLOADEN.md`). Claude kan niet uploaden. Vraag na het uploaden hoe het er
op het echte paneel uitziet; kleuren en helderheid kunnen afwijken van het
voorbeeld.
