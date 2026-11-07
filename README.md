# LedAnimaties 🎭✨

Animaties voor de ledpanelen op de carnavalswagen van **Knollebollekes**.
Bedacht door kinderen, in code gezet samen met Claude.

## Hoe werkt het?

1. **Bedenk iets!** Knipperende ogen, vlammen onder een kookpot, een draaiend wiel…
2. **Vertel het aan Claude**, samen met een begeleider. Gewoon in je eigen woorden:
   *"Ik wil twee groene ogen die boos kijken en dan heel snel knipperen."*
3. **Bekijk het voorbeeld** in de browser. Niet goed? Zeg wat er anders moet.
4. **Zet het op het paneel** via de website
   **[knollebollekes.github.io/LedAnimaties](https://knollebollekes.github.io/LedAnimaties/)**:
   ESP32 aansluiten, op *Zet op ESP32* klikken, klaar! (Werkt in Chrome of Edge.)
5. Kijk hoe het straalt op de wagen! 🎉

## Zelf iets veranderen

In elke animatie staan bovenaan dingen die je makkelijk kunt aanpassen, zoals:

```cpp
CRGB IRIS = CRGB::Blue;           // het gekleurde rondje
const int KNIPPER_SNELHEID = 25;  // kleiner = sneller
```

Probeer maar eens `CRGB::Blue` te veranderen in `CRGB::Green` of `CRGB::Purple`!
Of maak je eigen kleur met rood, groen en blauw (elk van 0 tot 255): `CRGB(255, 100, 0)` is oranje.

## Wat staat waar?

| Map | Wat zit erin |
|---|---|
| `wagens/` | per carnavalswagen een map met alle animaties van dat jaar |
| `voorbeelden/` | voorbeelden om van te leren, zoals `oogjes` |
| `sjabloon/` | het beginpunt voor een nieuwe animatie |
| `hulpmiddelen/` | testpatroon, controle, browser-voorbeeld en de website (voor begeleiders) |
| `docs/` | uitleg voor begeleiders |

Alle animaties bewegen ook op de [website](https://knollebollekes.github.io/LedAnimaties/),
zodat je ze kunt bekijken zonder paneel.

## Voor begeleiders

- De website wordt automatisch gebouwd door GitHub (`.github/workflows/website.yml`)
  bij elke wijziging op `main`. Alles op `main` komt dus binnen een paar minuten op de site.
- Handmatig uploaden met de Arduino IDE kan ook: zie [docs/UPLOADEN.md](docs/UPLOADEN.md).
- Hardware: ESP32 met WS2812B-panelen van 12×12 ledjes, ook meerdere aan elkaar.
- Hoe de panelen zijn aangesloten staat in `instellingen.h`. Klopt het beeld niet,
  upload dan eerst `hulpmiddelen/testpatroon`.
- Deze repo is openbaar: gebruik alleen voornamen of groepsnamen van kinderen.
- Claude volgt de afspraken in [CLAUDE.md](CLAUDE.md) en
  [.claude/skills/ledanimaties/SKILL.md](.claude/skills/ledanimaties/SKILL.md).
