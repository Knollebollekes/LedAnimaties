#!/bin/bash
# ============================================================
#  Zet de gereedschappen klaar om ESP32-code te CONTROLEREN
#  (compileren) zonder Arduino IDE. Gebruikt door Claude in zijn
#  werkomgeving; werkt ook op een gewone Linux-computer.
#
#  Alles komt van github.com, omdat downloads.arduino.cc in
#  sommige omgevingen geblokkeerd is.
#  Daarna: hulpmiddelen/compileer.sh <map-van-animatie>
# ============================================================
set -e
DOEL="${HULPMIDDELEN_MAP:-$HOME/.ledanimaties}"
mkdir -p "$DOEL" && cd "$DOEL"

if [ ! -x arduino-cli ]; then
  curl -sSL -o acli.tgz https://github.com/arduino/arduino-cli/releases/download/v1.3.1/arduino-cli_1.3.1_Linux_64bit.tar.gz
  tar xzf acli.tgz arduino-cli && rm acli.tgz
fi

if [ ! -x ctags/ctags ]; then
  mkdir -p ctags
  curl -sSL -o ctags.tbz https://github.com/arduino/ctags/releases/download/5.8-arduino11/ctags-5.8-arduino11-x86_64-pc-linux-gnu.tar.bz2
  tar xjf ctags.tbz -C ctags && rm ctags.tbz
  # het archief bevat een map; zet ctags direct in ctags/
  [ -x ctags/ctags ] || mv "$(find ctags -name ctags -type f | head -1)" ctags/ctags
fi

./arduino-cli config init --overwrite >/dev/null
./arduino-cli config set network.proxy "${HTTPS_PROXY:-}"
./arduino-cli config set board_manager.additional_urls \
  https://github.com/espressif/arduino-esp32/releases/latest/download/package_esp32_index.json
./arduino-cli core update-index >/dev/null 2>&1 || true
./arduino-cli core list 2>/dev/null | grep -q esp32:esp32 || ./arduino-cli core install esp32:esp32

mkdir -p libraries
[ -d libraries/FastLED ] || git clone --depth 1 --branch 3.7.8 https://github.com/FastLED/FastLED.git libraries/FastLED

python3 -c "import serial" 2>/dev/null || pip install -q --break-system-packages pyserial 2>/dev/null || pip install -q pyserial

echo "Klaar. Gereedschap staat in $DOEL"
