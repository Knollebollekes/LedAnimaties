// ============================================================
//  Start van het browser-voorbeeld. Wordt NA de animatie
//  ingevoegd (zie maak_voorbeeld.py). Draait setup() en loop()
//  met een nep-klok en schrijft alle frames als JSON.
// ============================================================
#include <cstdio>
#include <string>
#include <vector>

unsigned long sim_tijd_ms = 0;
SimFastLED FastLED;
SimSerial Serial;

#ifndef SIM_DUUR_MS
#define SIM_DUUR_MS 15000
#endif

static std::vector<std::pair<unsigned long, std::string>> sim_frames;
static std::string sim_vorige;

static void sim_klaar() {
  std::printf("{\"breedte\":%d,\"hoogte\":%d,\"paneel\":%d,\"duur\":%lu,\"frames\":[",
              BREEDTE, HOOGTE, PANEEL_GROOTTE, sim_tijd_ms);
  for (size_t i = 0; i < sim_frames.size(); i++)
    std::printf("%s[%lu,\"%s\"]", i ? "," : "", sim_frames[i].first, sim_frames[i].second.c_str());
  std::printf("]}\n");
  std::exit(0);
}

static void sim_tijd_erbij(unsigned long ms) {
  sim_tijd_ms += ms;
  if (sim_tijd_ms >= SIM_DUUR_MS) sim_klaar();
}

void delay(unsigned long ms) { sim_tijd_erbij(ms); }

void SimFastLED::show() {
  // Frame vastleggen in schermvolgorde (rij voor rij), niet in draadvolgorde
  std::string s;
  s.reserve(BREEDTE * HOOGTE * 6);
  char buf[8];
  for (int y = 0; y < HOOGTE; y++)
    for (int x = 0; x < BREEDTE; x++) {
      CRGB c = leesPixel(x, y);
      std::snprintf(buf, sizeof buf, "%02x%02x%02x", c.r, c.g, c.b);
      s += buf;
    }
  if (s != sim_vorige) {
    sim_frames.push_back({sim_tijd_ms, s});
    sim_vorige = s;
  }
  // Doorsturen naar echte ledjes kost ongeveer 30 microseconden per ledje
  unsigned long kost = (unsigned long)AANTAL_LEDS * 30 / 1000;
  sim_tijd_erbij(kost < 1 ? 1 : kost);
}

int main() {
  std::srand(42);
  setup();
  for (;;) {
    loop();
    sim_tijd_erbij(1);  // ook een loop() zonder wachten kost een beetje tijd
  }
}
