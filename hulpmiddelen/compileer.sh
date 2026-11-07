#!/bin/bash
# ============================================================
#  Controleer of een animatie compileert voor de ESP32.
#  Gebruik: hulpmiddelen/compileer.sh <map-van-animatie>
#  Eerst één keer: hulpmiddelen/installeer_hulpmiddelen.sh
# ============================================================
DOEL="${HULPMIDDELEN_MAP:-$HOME/.ledanimaties}"
if [ ! -x "$DOEL/arduino-cli" ]; then
  echo "Gereedschap ontbreekt. Draai eerst: hulpmiddelen/installeer_hulpmiddelen.sh" >&2
  exit 2
fi
"$DOEL/arduino-cli" config set network.proxy "${HTTPS_PROXY:-}" >/dev/null 2>&1
set -o pipefail
"$DOEL/arduino-cli" compile --fqbn esp32:esp32:esp32 \
  --libraries "$DOEL/libraries" \
  --build-property "runtime.tools.ctags.path=$DOEL/ctags" \
  "$1" 2>&1 | grep -v "Error initializing\|Loading index\|discovery\|Downloading index\|Some indexes"
