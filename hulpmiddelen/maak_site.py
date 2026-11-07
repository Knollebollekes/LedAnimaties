#!/usr/bin/env python3
"""
Bouwt de "zet-op-paneel"-website.

Voor elke animatie (een map met <naam>.ino erin, in voorbeelden/, wagens/<wagen>/
of hulpmiddelen/testpatroon):
  1. compileren voor de ESP32 en er één .bin-bestand van maken
  2. een manifest voor ESP Web Tools (de "Zet op ESP32"-knop)
  3. het browser-voorbeeld draaien (dezelfde code, nep-FastLED)
Daarna komt alles samen met de pagina uit hulpmiddelen/site/ in de uitvoermap.

Een animatie die niet compileert, staat op de site als "even kapot" maar
houdt de rest niet tegen.

Gebruik:
    python3 hulpmiddelen/maak_site.py --uit _site

Omgeving:
    ARDUINO_CLI   pad naar arduino-cli (standaard: arduino-cli op PATH,
                  anders ~/.ledanimaties/arduino-cli)
    GITHUB_REPOSITORY, GITHUB_SHA   (staan vanzelf goed in GitHub Actions)
"""
import argparse
import glob
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile

HIER = pathlib.Path(__file__).resolve().parent
REPO = HIER.parent
sys.path.insert(0, str(HIER))
from maak_voorbeeld import simuleer, lees_omschrijving  # noqa: E402

FQBN = "esp32:esp32:esp32"
HULP = pathlib.Path(os.environ.get("HULPMIDDELEN_MAP", pathlib.Path.home() / ".ledanimaties"))


def zoek_arduino_cli() -> str:
    if os.environ.get("ARDUINO_CLI"):
        return os.environ["ARDUINO_CLI"]
    if shutil.which("arduino-cli"):
        return "arduino-cli"
    if (HULP / "arduino-cli").exists():
        return str(HULP / "arduino-cli")
    sys.exit("arduino-cli niet gevonden")


def vind_schetsen():
    """Geeft (groep, map) voor elke animatie, in een vaste volgorde."""
    uit = []
    for wagen in sorted((REPO / "wagens").glob("*/"), reverse=True):  # nieuwste wagen bovenaan
        for m in sorted(wagen.glob("*/")):
            if (m / f"{m.name}.ino").exists():
                uit.append((groepsnaam(wagen), m))
    for m in sorted((REPO / "voorbeelden").glob("*/")):
        if (m / f"{m.name}.ino").exists():
            uit.append(("Voorbeelden", m))
    tp = REPO / "hulpmiddelen" / "testpatroon"
    if (tp / "testpatroon.ino").exists():
        uit.append(("Voor begeleiders", tp))
    return uit


def groepsnaam(wagen: pathlib.Path) -> str:
    readme = wagen / "README.md"
    if readme.exists():
        for regel in readme.read_text(encoding="utf-8").splitlines():
            if regel.startswith("# "):
                return regel[2:].strip()
    return "Wagen " + wagen.name.replace("-", " ")


def boot_app0() -> str:
    kandidaten = []
    for basis in [os.environ.get("ARDUINO_DIRECTORIES_DATA", ""), str(pathlib.Path.home() / ".arduino15")]:
        if basis:
            kandidaten += glob.glob(f"{basis}/packages/esp32/hardware/esp32/*/tools/partitions/boot_app0.bin")
    if not kandidaten:
        raise RuntimeError("boot_app0.bin niet gevonden (is de esp32-core geïnstalleerd?)")
    return sorted(kandidaten)[-1]


def esptool_cmd() -> list:
    try:
        import esptool  # noqa: F401
        return [sys.executable, "-m", "esptool"]
    except ImportError:
        pass
    gevonden = sorted(glob.glob(str(pathlib.Path.home() / ".arduino15/packages/esp32/tools/esptool_py/*/esptool.py")))
    if gevonden:
        return [sys.executable, gevonden[-1]]
    raise RuntimeError("esptool niet gevonden (pip install esptool)")


def compileer(cli: str, schets: pathlib.Path, doel_bin: pathlib.Path) -> tuple[bool, str]:
    with tempfile.TemporaryDirectory() as tmp:
        cmd = [cli, "compile", "--fqbn", FQBN, "--output-dir", tmp]
        if (HULP / "libraries").exists():
            cmd += ["--libraries", str(HULP / "libraries")]
        if (HULP / "ctags" / "ctags").exists():
            cmd += ["--build-property", f"runtime.tools.ctags.path={HULP / 'ctags'}"]
        cmd.append(str(schets))
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            fouten = [l for l in (r.stdout + r.stderr).splitlines() if "error" in l.lower()]
            return False, "\n".join(fouten[-8:]) or (r.stdout + r.stderr)[-1500:]
        samenvatting = r.stdout.strip().splitlines()[-1] if r.stdout.strip() else ""
        naam = schets.name + ".ino"
        t = pathlib.Path(tmp)
        merged = t / f"{naam}.merged.bin"
        if merged.exists():  # nieuwere esp32-core maakt dit zelf
            shutil.copy(merged, doel_bin)
        else:
            cmd = esptool_cmd() + [
                "--chip", "esp32", "merge_bin", "-o", str(doel_bin),
                "--flash_mode", "keep", "--flash_freq", "keep", "--flash_size", "keep",
                "0x1000", str(t / f"{naam}.bootloader.bin"),
                "0x8000", str(t / f"{naam}.partitions.bin"),
                "0xe000", boot_app0(),
                "0x10000", str(t / f"{naam}.bin"),
            ]
            r = subprocess.run(cmd, capture_output=True, text=True)
            if r.returncode != 0:
                return False, "Samenvoegen van de firmware mislukte:\n" + r.stderr[-1500:]
        return True, samenvatting


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--uit", type=pathlib.Path, default=REPO / "_site")
    p.add_argument("--duur", type=float, default=15, help="seconden voorbeeld per animatie")
    a = p.parse_args()

    uit = a.uit.resolve()
    if uit.exists():
        shutil.rmtree(uit)
    shutil.copytree(HIER / "site", uit)
    (uit / "firmware").mkdir()
    (uit / "data").mkdir()

    cli = zoek_arduino_cli()
    repo_naam = os.environ.get("GITHUB_REPOSITORY", "Knollebollekes/LedAnimaties")
    versie = os.environ.get("GITHUB_SHA", "lokaal")[:7]
    lijst, kapot = [], 0

    for groep, schets in vind_schetsen():
        rel = schets.relative_to(REPO).as_posix()
        ident = re.sub(r"[^a-z0-9-]+", "--", rel.lower())
        door, wat = lees_omschrijving(schets / f"{schets.name}.ino")
        item = {
            "id": ident,
            "groep": groep,
            "titel": schets.name.replace("-", " ").replace("_", " ").capitalize(),
            "door": door,
            "wat": wat,
            "code": f"https://github.com/{repo_naam}/tree/main/{rel}",
        }
        print(f"→ {rel}", flush=True)

        ok, melding = compileer(cli, schets, uit / "firmware" / f"{ident}.bin")
        item["ok"] = ok
        if ok:
            manifest = {
                "name": f"LedAnimaties: {item['titel']}",
                "version": versie,
                "new_install_prompt_erase": False,
                "new_install_improv_wait_time": 0,
                "builds": [{"chipFamily": "ESP32", "parts": [{"path": f"{ident}.bin", "offset": 0}]}],
            }
            (uit / "firmware" / f"{ident}.json").write_text(json.dumps(manifest, indent=1), encoding="utf-8")
        else:
            kapot += 1
            item["fout"] = melding
            print(f"::warning title=Compileert niet::{rel}: {melding.splitlines()[0] if melding else ''}")

        try:
            data = simuleer(schets, int(a.duur * 1000))
            (uit / "data" / f"{ident}.json").write_text(json.dumps(data, separators=(",", ":")), encoding="utf-8")
            item["voorbeeld"] = True
        except SystemExit as e:
            item["voorbeeld"] = False
            print(f"::warning title=Geen voorbeeld::{rel}: {e}")
        lijst.append(item)

    (uit / "animaties.json").write_text(
        json.dumps({"versie": versie, "repo": repo_naam, "animaties": lijst}, ensure_ascii=False, indent=1),
        encoding="utf-8",
    )
    print(f"Klaar: {len(lijst)} animaties, {kapot} kapot → {uit}")


if __name__ == "__main__":
    main()
