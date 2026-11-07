#!/usr/bin/env python3
"""
Maakt een browser-voorbeeld (voorbeeld.html) van een animatie.

De ECHTE animatiecode wordt op de computer gedraaid met een nep-FastLED
(hulpmiddelen/simulator). Elke FastLED.show() wordt een frame. Zo ziet het
voorbeeld er precies zo uit als wat de code doet.

Gebruik:
    python3 hulpmiddelen/maak_voorbeeld.py <map-van-animatie> [--duur SECONDEN]

Nodig: g++ (C++17) en python3.
"""
import argparse
import html as htmlmod
import json
import pathlib
import subprocess
import sys
import tempfile

HIER = pathlib.Path(__file__).resolve().parent
SIM = HIER / "simulator"


def lees_omschrijving(ino: pathlib.Path) -> tuple[str, str]:
    """Haalt 'Bedacht door' en 'Wat doet het' uit het commentaar bovenin."""
    door, wat = "", ""
    for regel in ino.read_text(encoding="utf-8").splitlines()[:30]:
        r = regel.strip().lstrip("/").strip()
        if r.lower().startswith("bedacht door:"):
            door = r.split(":", 1)[1].strip()
        elif r.lower().startswith("wat doet het:"):
            wat = r.split(":", 1)[1].strip()
    return door if door != "..." else "", wat if wat != "..." else ""


def simuleer(schets: pathlib.Path, duur_ms: int) -> dict:
    ino = schets / f"{schets.name}.ino"
    if not ino.exists():
        sys.exit(f"Geen {ino.name} gevonden in {schets} (naam van .ino moet gelijk zijn aan de map)")
    with tempfile.TemporaryDirectory() as tmp:
        tmp = pathlib.Path(tmp)
        wrapper = tmp / "sim.cpp"
        wrapper.write_text(
            f'#include "sim_fastled.h"\n#include "{ino}"\n#include "sim_main.inc"\n',
            encoding="utf-8",
        )
        exe = tmp / "sim"
        cmd = ["g++", "-std=c++17", "-O1", "-w", "-DSIMULATOR", f"-DSIM_DUUR_MS={duur_ms}",
               f"-I{SIM}", f"-I{schets}", str(wrapper), "-o", str(exe)]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            sys.exit("Compileren voor het voorbeeld mislukte:\n" + r.stderr)
        r = subprocess.run([str(exe)], capture_output=True, text=True, timeout=120)
        if r.returncode != 0:
            sys.exit("De simulatie liep vast:\n" + r.stderr)
        return json.loads(r.stdout)


HTML = """<!doctype html>
<html lang="nl">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>__TITEL__</title>
<style>
  :root { --bg:#14121a; --paneel:#1e1b26; --tekst:#f3eefc; --zacht:#a99fbd; --knop:#2c2738; --accent:#ffb020; }
  * { box-sizing:border-box; }
  body { margin:0; min-height:100vh; background:var(--bg); color:var(--tekst);
         font:16px/1.4 system-ui, -apple-system, "Segoe UI", sans-serif;
         display:flex; flex-direction:column; align-items:center; padding:24px 16px; gap:16px; }
  h1 { margin:0; font-size:1.6rem; text-align:center; }
  .sub { color:var(--zacht); margin:0; text-align:center; max-width:36rem; }
  canvas { background:var(--paneel); border-radius:14px; max-width:100%; height:auto;
           box-shadow:0 10px 40px #0008; }
  .knoppen { display:flex; gap:8px; flex-wrap:wrap; justify-content:center; }
  button { background:var(--knop); color:var(--tekst); border:0; border-radius:999px;
           padding:10px 18px; font:inherit; cursor:pointer; }
  button.aan { background:var(--accent); color:#1a1200; font-weight:600; }
  .klein { color:var(--zacht); font-size:.85rem; text-align:center; max-width:36rem; }
</style>
</head>
<body>
<h1>__TITEL__</h1>
<p class="sub">__SUB__</p>
<canvas id="scherm"></canvas>
<div class="knoppen">
  <button id="speel" class="aan">⏸ Pauze</button>
  <button data-snel="0.25">¼×</button>
  <button data-snel="0.5">½×</button>
  <button data-snel="1" class="aan">1×</button>
  <button data-snel="2">2×</button>
</div>
<p class="klein" id="info"></p>
<script>
const D = __DATA__;
const cv = document.getElementById('scherm'), ctx = cv.getContext('2d');
const STAP = 28, RAND = 18, TUSSEN = 10;   // pixels per ledje, rand, ruimte tussen panelen
const nb = D.breedte / D.paneel, nh = D.hoogte / D.paneel;
const W = RAND * 2 + D.breedte * STAP + (nb - 1) * TUSSEN, H = RAND * 2 + D.hoogte * STAP + (nh - 1) * TUSSEN;
const dpr = window.devicePixelRatio || 1;
cv.width = W * dpr; cv.height = H * dpr; cv.style.width = W + 'px';
ctx.scale(dpr, dpr);
const frames = D.frames.map(([t, hex]) => {
  const px = [];
  for (let i = 0; i < hex.length; i += 6) px.push([parseInt(hex.substr(i,2),16), parseInt(hex.substr(i+2,2),16), parseInt(hex.substr(i+4,2),16)]);
  return [t, px];
});
function pos(x, y) {
  return [RAND + x * STAP + Math.floor(x / D.paneel) * TUSSEN + STAP / 2,
          RAND + y * STAP + Math.floor(y / D.paneel) * TUSSEN + STAP / 2];
}
function teken(px) {
  ctx.clearRect(0, 0, W, H);
  for (let py = 0; py < nh; py++) for (let pxp = 0; pxp < nb; pxp++) {
    ctx.fillStyle = '#0c0b10';
    const [x0, y0] = pos(pxp * D.paneel, py * D.paneel);
    ctx.beginPath(); ctx.roundRect(x0 - STAP/2 - 4, y0 - STAP/2 - 4, D.paneel * STAP + 8, D.paneel * STAP + 8, 8); ctx.fill();
  }
  for (let y = 0; y < D.hoogte; y++) for (let x = 0; x < D.breedte; x++) {
    const [r, g, b] = px[y * D.breedte + x];
    const [cx, cy] = pos(x, y);
    const aan = r + g + b > 0;
    if (aan) {
      const gl = ctx.createRadialGradient(cx, cy, 0, cx, cy, STAP * 0.85);
      gl.addColorStop(0, `rgba(${r},${g},${b},0.55)`); gl.addColorStop(1, `rgba(${r},${g},${b},0)`);
      ctx.fillStyle = gl; ctx.fillRect(cx - STAP, cy - STAP, STAP * 2, STAP * 2);
    }
    ctx.beginPath(); ctx.arc(cx, cy, STAP * 0.36, 0, Math.PI * 2);
    ctx.fillStyle = aan ? `rgb(${r},${g},${b})` : '#26222f'; ctx.fill();
  }
}
let snel = 1, speelt = true, t = 0, vorige = performance.now(), idx = 0;
function stap(nu) {
  if (speelt) { t += (nu - vorige) * snel; if (t >= D.duur) { t = 0; idx = 0; } }
  vorige = nu;
  while (idx + 1 < frames.length && frames[idx + 1][0] <= t) idx++;
  if (idx > 0 && frames[idx][0] > t) idx = 0;
  teken(frames[idx][1]);
  requestAnimationFrame(stap);
}
document.getElementById('speel').onclick = e => {
  speelt = !speelt; e.target.textContent = speelt ? '⏸ Pauze' : '▶ Speel'; e.target.classList.toggle('aan', speelt);
};
document.querySelectorAll('[data-snel]').forEach(b => b.onclick = () => {
  snel = +b.dataset.snel;
  document.querySelectorAll('[data-snel]').forEach(x => x.classList.toggle('aan', x === b));
});
document.getElementById('info').textContent =
  `${D.breedte}×${D.hoogte} ledjes · ${nb * nh} paneel${nb * nh > 1 ? 'en' : ''} · eerste ${(D.duur/1000).toFixed(0)} seconden van de animatie, daarna begint het voorbeeld opnieuw. Op het echte paneel is alles minder fel (zie HELDERHEID).`;
requestAnimationFrame(stap);
</script>
</body>
</html>
"""


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("map", type=pathlib.Path, help="map van de animatie (met <naam>.ino erin)")
    p.add_argument("--duur", type=float, default=15, help="hoeveel seconden simuleren (standaard 15)")
    a = p.parse_args()
    schets = a.map.resolve()
    data = simuleer(schets, int(a.duur * 1000))
    if not data["frames"]:
        sys.exit("De animatie heeft niets laten zien (geen FastLED.show() / toon() aangeroepen?)")
    door, wat = lees_omschrijving(schets / f"{schets.name}.ino")
    titel = schets.name.replace("-", " ").replace("_", " ").capitalize()
    sub = " · ".join(x for x in [wat, f"bedacht door {door}" if door else ""] if x)
    html = (HTML.replace("__TITEL__", htmlmod.escape(titel))
                .replace("__SUB__", htmlmod.escape(sub))
                .replace("__DATA__", json.dumps(data, separators=(",", ":"))))
    uit = schets / "voorbeeld.html"
    uit.write_text(html, encoding="utf-8")
    print(f"Klaar: {uit} ({len(data['frames'])} frames, {uit.stat().st_size // 1024} KB)")


if __name__ == "__main__":
    main()
