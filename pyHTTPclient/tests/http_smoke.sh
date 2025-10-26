#!/usr/bin/env bash
# Simple HTTP smoke test for modelCarController firmware
# Usage: ./http_smoke_fixed.sh [HOST]
# Default HOST is 10.0.0.104:4242

# Don't bail the whole script on a single curl failure; handle failures per-request.
set -uo pipefail
HOST=${1:-10.0.0.104:4242}

# Curl options: quiet, show errors, discard body, print HTTP code with newline.
# Add small connect + overall timeouts so requests don't hang indefinitely.
CURL_OPTS=( -s -S -o /dev/null -w "%{http_code}\n" \
  -H "Content-Type: application/json" --connect-timeout 3 --max-time 8 )

echo "Running HTTP smoke tests against http://${HOST}"

run() {
  local desc="$1"; shift
  local data="$1"; shift
  printf "%-35s " "$desc"
  # Capture output and exit code separately so set -o pipefail doesn't abort here.
  status=$(curl -X POST "http://${HOST}" "${CURL_OPTS[@]}" -d "$data")
  rc=$?
  if [ $rc -ne 0 ]; then
    # Common non-zero curl codes: 7=connection refused, 28=timeout
    printf "000 (curl err %d)\n" "$rc"
  else
    printf "%s" "$status"
  fi
}

# Left headlight ON
run "Left headlight ON" '{"config1": {"ID": "left headlight", "pin": 14, "isOn": true}}'
# Left headlight OFF
run "Left headlight OFF" '{"config1": {"ID": "left headlight", "pin": 14, "isOn": false}}'

# Right headlight ON
run "Right headlight ON" '{"config1": {"ID": "right headlight", "pin": 15, "isOn": true}}'
# Right headlight OFF
run "Right headlight OFF" '{"config1": {"ID": "right headlight", "pin": 15, "isOn": false}}'

# PWM flashlight (ramped)
run "Flashlight ramp" '{"config1": {"ID": "flashlight", "pin": 15, "isOn": false, "isRamp": true, "LMax": 85, "rampUpTimeMs": 5000}}'

echo "Smoke tests complete. Replace HOST with the device IP if needed."
