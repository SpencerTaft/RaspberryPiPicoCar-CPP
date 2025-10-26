# Quick guide for AI agents working in this repo

This file documents the essential, discoverable knowledge to be productive quickly.

- Build & flash (reproducible):
  - Use an out-of-source build in the `build/` directory.
  - Example (from repository notes):

    ```bash
    cd build
    rm -rf ./*
    PICOHTTPS_WIFI_PASSWORD=yourpassword cmake -D"PICO_BOARD=pico_w" ..
    make
    ```

  - The build produces `build/model_car_controller.uf2` and a `picotool/` helper in the build tree. You can flash by copying the UF2 to the Pico in USB mass-storage boot mode or by using the included `picotool` binary.

- Serial & runtime debugging:
  - The device logs are available over USB serial. The typical command used in notes is:

    ```bash
    minicom -D /dev/ttyACM0 -b 115200
    ```

- HTTP control surface & testing:
  - The firmware exposes a small HTTP server. See `httpServer/` (`httpServer/src/httpServer.cpp`, `httpServer/include/httpServer.hpp`) for request handling and JSON structure.
  - A Python test client lives in `pyHTTPclient/` (`pyClient.py`). The repo notes include practical curl examples to toggle lights; replicate those for automated tests.

- Project layout & important files to read first:
  - `CMakeLists.txt` (top-level) — wiring of pico-sdk and submodules.
  - `main.cpp` — entry point and overall initialization flow.
  - `httpServer/` — HTTP API and JSON payload format used to control HW.
  - `hwControl/` and `hwControl/runnables/` — pin mapping, runnables and PWM/ramp logic.
  - `scheduler/` — task scheduling primitives used across runnables.
  - `notes` — contains developer run/debug commands and curl examples (useful for reproducing manual tests).

- Conventions and patterns to follow (observable in repo):
  - CMake-based multi-directory project using pico-sdk and generated code under `generated/`.
  - HTTP control messages use small JSON objects containing `ID`, `pin`, `isOn`, and optional ramp parameters (see `notes` curl examples).
  - Hardware behaviours are implemented as "runnables" under `hwControl/runnables/` and coordinated by `scheduler/`.

- Integration points & external dependencies:
  - Relies on the Raspberry Pi Pico SDK (included under `pico-sdk/` in the workspace).
  - `picotool` is available in the build tree for flashing and UF2 handling.
  - The firmware expects WiFi credentials to be injected at CMake time via `PICOHTTPS_WIFI_PASSWORD`.

- HTTP examples (useful for manual tests & automated smoke tests):
  - Replace the host IP with the device IP on your network (the `notes` file records recent addresses).
  - Turn left headlight ON/OFF:

    ```bash
    curl -X POST http://10.0.0.103:4242 \
      -d '{"config1": {"ID": "left headlight", "pin": 14, "isOn": true}}' \
      -H "Content-Type: application/json"

    curl -X POST http://10.0.0.103:4242 \
      -d '{"config1": {"ID": "left headlight", "pin": 14, "isOn": false}}' \
      -H "Content-Type: application/json"
    ```

  - Turn right headlight ON/OFF:

    ```bash
    curl -X POST http://10.0.0.104:4242 \
      -d '{"config1": {"ID": "right headlight", "pin": 15, "isOn": true}}' \
      -H "Content-Type: application/json"

    curl -X POST http://10.0.0.104:4242 \
      -d '{"config1": {"ID": "right headlight", "pin": 15, "isOn": false}}' \
      -H "Content-Type: application/json"
    ```

  - PWM flashlight (ramped) example:

    ```bash
    curl -X POST http://10.0.0.104:4242 \
      -d '{"config1": {"ID": "flashlight", "pin": 15, "isOn": false, "isRamp": true, "LMax": 85, "rampUpTimeMs": 5000}}' \
      -H "Content-Type: application/json"
    ```

- Suggestions to improve agent suggestions in this repo:
  - Add a short "agent hints" block containing:
    - common IP addresses used in dev (or where to find them: `notes`).
    - canonical JSON schema examples for all HTTP commands (one-liners for each `configX`).
    - mapping of logical IDs to pins (e.g., `left headlight -> pin 14`, `right headlight -> pin 15`) so the agent can make correct code changes or tests.
  - Inline one-line summaries at the top of files that implement public behavior: e.g., add a single-line comment in `httpServer/src/httpServer.cpp` describing accepted payload shapes and routing.
  - If you want the agent to edit hardware code, add a `DEV_NOTES.md` listing safe pins and which GPIOs are in use to avoid dangerous suggestions.
