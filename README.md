# LedAnimaties 🎭✨

Animaties voor de ledpanelen op de carnavalswagen van **Knollebollekes**.
Bedacht door kinderen, in code gezet samen met Claude.

## Hoe werkt het?

1. **Bedenk iets!** Knipperende ogen, vlammen onder een kookpot, een draaiend wiel…
2. **Vertel het aan Claude**, samen met een begeleider. Gewoon in je eigen woorden:
   *"Ik wil twee groene ogen die boos kijken en dan heel snel knipperen."*
3. **Bekijk het voorbeeld** in de browser. Niet goed? Zeg wat er anders moet.
4. **Upload** naar het echte paneel (dat doet de begeleider, zie [docs/UPLOADEN.md](docs/UPLOADEN.md)).
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
| `hulpmiddelen/` | testpatroon, controle en het browser-voorbeeld (voor begeleiders) |
| `docs/` | uitleg voor begeleiders |

Bij elke animatie hoort een `voorbeeld.html`. Download die en open hem in je browser
om de animatie te zien, zonder paneel.

## Voor begeleiders

- Hardware: ESP32 met WS2812B-panelen van 12×12 ledjes, ook meerdere aan elkaar.
- Hoe de panelen zijn aangesloten staat in `instellingen.h`. Klopt het beeld niet,
  upload dan eerst `hulpmiddelen/testpatroon`.
- Deze repo is openbaar: gebruik alleen voornamen of groepsnamen van kinderen.
- Claude volgt de afspraken in [CLAUDE.md](CLAUDE.md) en
  [.claude/skills/ledanimaties/SKILL.md](.claude/skills/ledanimaties/SKILL.md).
