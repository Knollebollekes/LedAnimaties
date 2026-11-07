// ============================================================
//  NEP-FASTLED voor het browser-voorbeeld.
//  Hiermee draait dezelfde animatiecode op een gewone computer.
//  Elke FastLED.show() wordt een plaatje (frame) in het voorbeeld.
//  Alleen voor Claude/begeleiders; kinderen hoeven hier niets mee.
//
//  Ondersteund: CRGB, CHSV, kleurnamen (CRGB::Red, ...), millis,
//  delay, random/random8/random16, sin8/cos8/beatsin8, scale8,
//  qadd8/qsub8, map, constrain, min/max, fadeToBlackBy, fill_solid,
//  fill_rainbow, blend, HeatColor, nscale8, Serial (doet niets).
//  Gebruikt een animatie iets anders van FastLED? Voeg het hier toe.
// ============================================================
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <type_traits>

typedef uint8_t byte;
typedef bool boolean;

// ---------- tijd ----------
extern unsigned long sim_tijd_ms;
inline unsigned long millis() { return sim_tijd_ms; }
inline unsigned long micros() { return sim_tijd_ms * 1000UL; }
void delay(unsigned long ms);
inline void yield() {}

// ---------- rekenen ----------
template <class A, class B> inline auto min(A a, B b) -> typename std::common_type<A, B>::type { return a < b ? a : b; }
template <class A, class B> inline auto max(A a, B b) -> typename std::common_type<A, B>::type { return a > b ? a : b; }
template <class T, class L, class H> inline T constrain(T x, L lo, H hi) { return x < lo ? (T)lo : (x > hi ? (T)hi : x); }
inline long map(long x, long a, long b, long c, long d) { return (x - a) * (d - c) / (b - a) + c; }
inline long random(long hoog) { return hoog <= 0 ? 0 : std::rand() % hoog; }
inline long random(long laag, long hoog) { return hoog <= laag ? laag : laag + std::rand() % (hoog - laag); }
inline void randomSeed(unsigned long s) { std::srand(s); }
inline uint8_t random8() { return std::rand() & 0xFF; }
inline uint8_t random8(uint8_t hoog) { return hoog ? std::rand() % hoog : 0; }
inline uint8_t random8(uint8_t laag, uint8_t hoog) { return hoog > laag ? laag + std::rand() % (hoog - laag) : laag; }
inline uint16_t random16() { return std::rand() & 0xFFFF; }
inline uint16_t random16(uint16_t hoog) { return hoog ? std::rand() % hoog : 0; }
inline uint16_t random16(uint16_t laag, uint16_t hoog) { return hoog > laag ? laag + std::rand() % (hoog - laag) : laag; }
inline uint8_t scale8(uint8_t i, uint8_t s) { return ((uint16_t)i * (1 + (uint16_t)s)) >> 8; }
inline uint8_t scale8_video(uint8_t i, uint8_t s) { return (i && s) ? (((int)i * (int)s) >> 8) + 1 : 0; }
inline uint8_t qadd8(int a, int b) { int s = a + b; return s > 255 ? 255 : s; }
inline uint8_t qsub8(int a, int b) { int s = a - b; return s < 0 ? 0 : s; }
inline uint8_t sin8(uint8_t t) { return (uint8_t)(128 + 127.5 * std::sin(t * 2 * M_PI / 256.0)); }
inline uint8_t cos8(uint8_t t) { return sin8(t + 64); }
inline int16_t sin16(uint16_t t) { return (int16_t)(32767 * std::sin(t * 2 * M_PI / 65536.0)); }
inline int16_t cos16(uint16_t t) { return sin16(t + 16384); }
inline uint8_t lerp8by8(uint8_t a, uint8_t b, uint8_t f) { return a + (((int)b - a) * f) / 256; }
inline uint8_t beatsin8(int bpm, uint8_t laag = 0, uint8_t hoog = 255, uint32_t tijdbasis = 0, uint8_t fase = 0) {
  uint8_t beat = (uint8_t)(((millis() - tijdbasis) * bpm * 256UL) / 60000UL);
  return laag + scale8(sin8(beat + fase), hoog - laag);
}
inline uint8_t beat8(int bpm, uint32_t tijdbasis = 0) { return (uint8_t)(((millis() - tijdbasis) * bpm * 256UL) / 60000UL); }

// ---------- kleuren ----------
struct CHSV {
  uint8_t h, s, v;
  CHSV() : h(0), s(0), v(0) {}
  CHSV(uint8_t H, uint8_t S, uint8_t V) : h(H), s(S), v(V) {}
};

struct CRGB {
  uint8_t r, g, b;
  enum Kleur : uint32_t {
    Black = 0x000000, White = 0xFFFFFF, Red = 0xFF0000, Green = 0x008000, Lime = 0x00FF00,
    Blue = 0x0000FF, Yellow = 0xFFFF00, Orange = 0xFFA500, DarkOrange = 0xFF8C00,
    OrangeRed = 0xFF4500, Gold = 0xFFD700, Purple = 0x800080, Pink = 0xFFC0CB,
    HotPink = 0xFF69B4, DeepPink = 0xFF1493, Cyan = 0x00FFFF, Aqua = 0x00FFFF,
    Magenta = 0xFF00FF, Brown = 0xA52A2A, Gray = 0x808080, Grey = 0x808080,
    SkyBlue = 0x87CEEB, DarkBlue = 0x00008B, DarkGreen = 0x006400, DarkRed = 0x8B0000,
    Violet = 0xEE82EE, Indigo = 0x4B0082, Turquoise = 0x40E0D0, Crimson = 0xDC143C,
    Salmon = 0xFA8072, Chocolate = 0xD2691E, Navy = 0x000080, Teal = 0x008080,
    Amethyst = 0x9966CC, Coral = 0xFF7F50, Tomato = 0xFF6347, LightBlue = 0xADD8E6
  };
  CRGB() : r(0), g(0), b(0) {}
  CRGB(uint8_t R, uint8_t G, uint8_t B) : r(R), g(G), b(B) {}
  CRGB(uint32_t c) : r((c >> 16) & 0xFF), g((c >> 8) & 0xFF), b(c & 0xFF) {}
  CRGB(Kleur c) : CRGB((uint32_t)c) {}
  CRGB(const CHSV& hsv) { zetHSV(hsv); }
  CRGB& operator=(const CHSV& hsv) { zetHSV(hsv); return *this; }
  CRGB& operator=(uint32_t c) { *this = CRGB(c); return *this; }

  void zetHSV(const CHSV& c) {  // regenboog-omzetting zoals FastLED (benadering)
    float h = c.h * 360.0f / 256.0f, s = c.s / 255.0f, v = c.v / 255.0f;
    float k = std::fmod(h / 60.0f, 6.0f), f = k - std::floor(k);
    float p = v * (1 - s), q = v * (1 - s * f), t = v * (1 - s * (1 - f));
    float R, G, B;
    switch ((int)k) {
      case 0: R = v; G = t; B = p; break;
      case 1: R = q; G = v; B = p; break;
      case 2: R = p; G = v; B = t; break;
      case 3: R = p; G = q; B = v; break;
      case 4: R = t; G = p; B = v; break;
      default: R = v; G = p; B = q; break;
    }
    r = R * 255; g = G * 255; b = B * 255;
  }
  CRGB& setRGB(uint8_t R, uint8_t G, uint8_t B) { r = R; g = G; b = B; return *this; }
  CRGB& setHSV(uint8_t H, uint8_t S, uint8_t V) { zetHSV(CHSV(H, S, V)); return *this; }
  CRGB& setHue(uint8_t H) { return setHSV(H, 255, 255); }
  CRGB& nscale8(uint8_t s) { r = scale8(r, s); g = scale8(g, s); b = scale8(b, s); return *this; }
  CRGB& nscale8_video(uint8_t s) { r = scale8_video(r, s); g = scale8_video(g, s); b = scale8_video(b, s); return *this; }
  CRGB& fadeToBlackBy(uint8_t f) { return nscale8(255 - f); }
  CRGB& fadeLightBy(uint8_t f) { return nscale8(255 - f); }
  CRGB& operator+=(const CRGB& o) { r = qadd8(r, o.r); g = qadd8(g, o.g); b = qadd8(b, o.b); return *this; }
  CRGB& operator-=(const CRGB& o) { r = qsub8(r, o.r); g = qsub8(g, o.g); b = qsub8(b, o.b); return *this; }
  CRGB& operator%=(uint8_t s) { return nscale8_video(s); }
  CRGB& operator*=(uint8_t m) { r = qadd8(0, r * m); g = qadd8(0, g * m); b = qadd8(0, b * m); return *this; }
  CRGB& operator/=(uint8_t d) { r /= d; g /= d; b /= d; return *this; }
  uint8_t& operator[](int i) { return i == 0 ? r : (i == 1 ? g : b); }
  bool operator==(const CRGB& o) const { return r == o.r && g == o.g && b == o.b; }
  bool operator!=(const CRGB& o) const { return !(*this == o); }
  explicit operator bool() const { return r || g || b; }
  uint8_t getLuma() const { return (54 * r + 183 * g + 18 * b) >> 8; }
};
inline CRGB operator+(CRGB a, const CRGB& b) { return a += b; }
inline CRGB operator-(CRGB a, const CRGB& b) { return a -= b; }

inline void hsv2rgb_rainbow(const CHSV& h, CRGB& c) { c = CRGB(h); }
inline void hsv2rgb_spectrum(const CHSV& h, CRGB& c) { c = CRGB(h); }
inline CRGB blend(const CRGB& a, const CRGB& b, uint8_t f) {
  return CRGB(lerp8by8(a.r, b.r, f), lerp8by8(a.g, b.g, f), lerp8by8(a.b, b.b, f));
}
inline void fill_solid(CRGB* l, int n, const CRGB& c) { for (int i = 0; i < n; i++) l[i] = c; }
inline void fill_rainbow(CRGB* l, int n, uint8_t hue, uint8_t delta = 5) {
  for (int i = 0; i < n; i++) { l[i] = CHSV(hue, 240, 255); hue += delta; }
}
inline void fadeToBlackBy(CRGB* l, int n, uint8_t f) { for (int i = 0; i < n; i++) l[i].fadeToBlackBy(f); }
inline void nscale8(CRGB* l, int n, uint8_t s) { for (int i = 0; i < n; i++) l[i].nscale8(s); }
inline CRGB HeatColor(uint8_t t) {  // zwart -> rood -> geel -> wit
  uint8_t s = scale8(t, 191), r = (s & 0x3F) << 2;
  if (s & 0x80) return CRGB(255, 255, r);
  if (s & 0x40) return CRGB(255, r, 0);
  return CRGB(r, 0, 0);
}

// ---------- de "FastLED"-bibliotheek ----------
enum { WS2812B, WS2812, WS2811, NEOPIXEL, SK6812 };
enum { RGB, RBG, GRB, GBR, BRG, BGR };
struct SimFastLED {
  CRGB* leds = nullptr;
  int aantal = 0;
  uint8_t helderheid = 255;
  template <int TYPE, int PIN, int VOLGORDE> SimFastLED& addLeds(CRGB* l, int n) { leds = l; aantal = n; return *this; }
  template <int TYPE, int PIN> SimFastLED& addLeds(CRGB* l, int n) { leds = l; aantal = n; return *this; }
  void setBrightness(uint8_t b) { helderheid = b; }
  uint8_t getBrightness() { return helderheid; }
  void setMaxPowerInVoltsAndMilliamps(int, int) {}
  void setCorrection(uint32_t) {}
  void show();
  void show(uint8_t) { show(); }
  void clear(bool tonen = false) { if (leds) fill_solid(leds, aantal, CRGB::Black); if (tonen) show(); }
  void delay(unsigned long ms) { ::delay(ms); }
};
extern SimFastLED FastLED;
#define TypicalLEDStrip 0

// ---------- Serial (doet niets) ----------
struct SimSerial {
  void begin(long) {}
  template <class T> void print(T) {}
  template <class T> void println(T) {}
  void println() {}
  operator bool() const { return true; }
};
extern SimSerial Serial;

#define EVERY_N_MILLISECONDS(N) for (static unsigned long _volgende = 0; millis() >= _volgende; _volgende = millis() + (N))
#define EVERY_N_SECONDS(N) EVERY_N_MILLISECONDS((N) * 1000UL)
