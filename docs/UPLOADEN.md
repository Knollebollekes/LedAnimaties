# Een animatie op de ESP32 zetten

## De makkelijke manier: de website ⭐

Ga in **Chrome of Edge** naar **https://knollebollekes.github.io/LedAnimaties/**

1. Sluit de ESP32 met een USB-kabel aan.
2. Klik bij een animatie op **Zet op ESP32**.
3. Kies de ESP32 in het lijstje dat de browser laat zien en klik op **Installeer**.

Geen Arduino IDE nodig. Elke keer dat er nieuwe code in GitHub komt, bouwt GitHub de
website binnen een paar minuten opnieuw, dus er staat altijd de nieuwste versie op.

Lukt het niet?
- Verschijnt de ESP32 niet in het lijstje? Probeer een andere USB-kabel (sommige kunnen alleen
  opladen) of installeer de driver (zie stap 4 hieronder).
- Blijft hij hangen? Houd het knopje **BOOT** op de ESP32 ingedrukt als het installeren begint.
- Werkt het in Safari, Firefox of op een telefoon niet? Klopt: die browsers kunnen niet met USB-apparaten praten.

## De handmatige manier: Arduino IDE

Handig als je zelf aan de code wilt sleutelen. De eerste keer kost dit een half uurtje.

### Eén keer: de computer klaarmaken

1. **Arduino IDE installeren**: download versie 2 van [arduino.cc/en/software](https://www.arduino.cc/en/software) en installeer.
2. **ESP32 toevoegen**:
   - Open Arduino IDE → **Bestand → Voorkeuren** (Mac: Arduino IDE → Settings).
   - Plak bij *Additional boards manager URLs*:
     `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Ga naar **Hulpmiddelen → Board → Boards Manager**, zoek **esp32** (van Espressif) en klik **Install**.
3. **FastLED installeren**: **Hulpmiddelen → Bibliotheken beheren**, zoek **FastLED** en klik **Install**.
4. **Driver** (alleen als de ESP32 straks niet verschijnt): veel ESP32-bordjes hebben een
   CP210x- of CH340-chip. Zoek op "CP210x driver" of "CH340 driver" en installeer die.

### De code op je computer krijgen

Makkelijkst: ga naar [github.com/Knollebollekes/LedAnimaties](https://github.com/Knollebollekes/LedAnimaties),
klik op de groene knop **Code → Download ZIP** en pak het uit.
(Wil je vaker bijwerken? Installeer [GitHub Desktop](https://desktop.github.com) en kies *Clone*.
Dan is één klik op *Fetch/Pull* genoeg om de nieuwste animaties te krijgen.)

### Elke keer: uploaden

1. Sluit de ESP32 met een USB-kabel aan op de computer.
   (Let op: sommige kabels kunnen alleen opladen. Werkt het niet, probeer een andere.)
2. Open in Arduino IDE het `.ino`-bestand van de animatie, bijvoorbeeld
   `voorbeelden/oogjes/oogjes.ino`. De andere bestanden in die map gaan automatisch mee.
3. Kies **Hulpmiddelen → Board → esp32 → ESP32 Dev Module**.
4. Kies **Hulpmiddelen → Poort** en selecteer de ESP32 (vaak `COM3` of hoger, op Mac `/dev/cu.usbserial…`).
5. Klik op de **pijl naar rechts** (Uploaden). Na een minuutje staat er *Done uploading*.
   - Blijft hij hangen op *Connecting…*? Houd het knopje **BOOT** op de ESP32 ingedrukt tot het uploaden begint.

## Aansluiten van de panelen

- **Datadraad** (DIN van het eerste paneel) → pin **5** van de ESP32 (zie `DATA_PIN` in `instellingen.h`).
- **Min (GND)** van de panelen → GND van de ESP32 **én** min van de voeding. Gezamenlijke min is belangrijk!
- **Plus (5V)** van de panelen → plus van een **aparte 5V-voeding**, niet via de ESP32.
  Eén paneel kan op vol wit tot ~8 ampère trekken; met `HELDERHEID` 40 is het veel minder.
- Tip: een weerstandje van ±330 Ω in de datadraad en een grote condensator (1000 µF) over de
  plus en min bij het paneel maken het geheel betrouwbaarder.

## Klopt het beeld niet?

Upload eerst `hulpmiddelen/testpatroon`. Die laat zien of `instellingen.h` past bij hoe jullie
panelen bedraad zijn. In het bestand staat hoe het eruit hoort te zien.
