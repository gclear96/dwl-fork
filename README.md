# dwl-hyprish

A dwl fork tuned to feel comfortable coming from Hyprland, with performance‑first defaults and a lightweight bar workflow.

## What’s included

### IPC + bar workflow
- **dwl‑ipc‑unstable‑v2** protocol enabled (for IPC tooling)
- **net‑tapesoftware‑dwl‑wm‑unstable‑v1** enabled (for somebar IPC)
- **someblocks** autostarted by default (feeds somebar)

### Layouts + gaps + borders
- Layouts: **tile**, **monocle**, **centered master**, **snail** (dwindle‑ish)
- **nextlayout** cycling
- **vanitygaps** (inner/outer, smart gaps)
- **smartborders** (no borders with a single tiled client)

### Workspace/QoL
- **pertag** (per‑tag layout/mfact/nmaster)
- **shiftview** and **rotatetags** for workspace navigation
- **switchtotag** rules support
- **sticky** clients

### Rules + scratchpads
- **regexrules**
- **customfloat** (rule‑controlled float geometry)
- **alwayscenter** floating windows
- **namedscratchpads**

### Ergonomics
- **movestack**
- **moveresizekb**
- **dragmfact**
- **focusdir + swapdir**

## Keybinds (Hypr‑inspired)
- **Alt+Return**: terminal (alacritty)
- **Alt+R**: app launcher (tofi-run)
- **Alt+F**: Firefox
- **Alt+Q**: kill client
- **Alt+V**: toggle floating
- **Alt+S** or **Alt+`**: scratchpad
- **Alt+B**: toggle somebar visibility (uses `somebar -c toggle selected`)
- **Alt+Shift+P**: power menu (tofi)
- **Alt+H/J/K/L**: focus left/down/up/right
- **Alt+Shift+H/J/K/L**: swap client in direction
- **Alt+Ctrl+H/L**: adjust mfact
- **Alt+[1–9]**: view workspace, **Alt+Shift+[1–9]**: move to workspace
- **Alt+P**: snail (dwindle‑ish) layout
- **Alt+T**: tile, **Alt+M**: monocle, **Alt+C**: centered master
- **Alt+N**: next layout

## Autostart
Configured in `config.def.h`:
- `wbg /path/to/your/image`
- `~/.config/someblocks/run.sh` (status updates)

You must start dwl with somebar via:
```
dwl -s somebar
```

Adjust as needed.

## Prereqs

### Build
- C compiler and `make`
- `pkg-config`
- `wayland-scanner` (from `wayland`)
- `wayland-protocols`
- `wlroots-0.19` development headers/libs
- `wayland-server`, `xkbcommon`, `libinput`
- Optional XWayland support: enable `XWAYLAND` in `config.mk` and install `xcb` + `xcb-icccm`

### Runtime
- A wlroots-compatible Wayland stack (DRM/KMS or nested backend)
- seat/session manager for DRM backends (e.g., logind or seatd)
- Defaults in `config.def.h` assume: `alacritty`, `walker`, `firefox`, `wbg`, `somebar`
- Build and install `someblocks` separately from `./someblocks` (it reads `blocks.h`, created from `blocks.def.h` on first build).

## Build
```
make clean && make
```

## Install (detailed)
These steps assume `/usr/local` as the install prefix. If you want a different prefix, set
`PREFIX=/path` when running install commands.

### 0) Clone the repos
```
git clone https://github.com/gclear96/dwl-fork.git
git clone https://github.com/gclear96/somebar-fork.git
git clone https://github.com/gclear96/someblocks-fork.git
git clone https://github.com/gclear96/dwl-dotfiles.git
```

### 1) Install somebar (IPC-patched)
```
cd /path/to/somebar-fork
cp src/config.def.hpp src/config.hpp
meson setup build
ninja -C build
sudo ninja -C build install
```

### 2) Install someblocks
```
cd /path/to/someblocks-fork
make
sudo make install
```
Note: the first `make` creates `blocks.h` from `blocks.def.h`. Edit `blocks.h` to customize blocks.

### 3) Install dwl
```
cd /path/to/dwl-fork
make clean && make
sudo make install
```

This installs `dwl` under `$(PREFIX)/bin` (default: `/usr/local/bin`) and the session file under
`$(DATADIR)/wayland-sessions`. The installed session launches somebar with stdin closed:
```
dwl -s "exec somebar </dev/null"
```

### 4) Install user scripts
```
mkdir -p ~/.config/someblocks
cp /path/to/dwl-dotfiles/someblocks/run.sh ~/.config/someblocks/
mkdir -p ~/.config/tofi
cp /path/to/dwl-dotfiles/tofi/config ~/.config/tofi/config
cp /path/to/dwl-dotfiles/tofi/run.sh ~/.config/tofi/run.sh
cp /path/to/dwl-dotfiles/tofi/power.sh ~/.config/tofi/power.sh
```

### 5) Autostart and session
- Autostart is configured in `config.def.h` to run `~/.config/someblocks/run.sh`.
- Start dwl via your greeter session entry or with:
```
dwl -s "exec somebar </dev/null"
```

## Notes
- This is a patch‑stacked fork of **dwl**. If you rebase onto upstream dwl, expect conflicts in `dwl.c` and `config.def.h`.
- IPC is required for dwlb; this fork includes `dwl-ipc-unstable-v2` and generates protocol code during build.
