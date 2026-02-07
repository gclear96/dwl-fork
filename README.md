# dwl-fork

Patch-stacked dwl build with somebar/someblocks workflow, Gruvbox-ish defaults,
opacity controls, and local SceneFX integration for blur.

## What is included

### Core features
- `dwl-ipc-unstable-v2` protocol
- `net-tapesoftware-dwl-wm-unstable-v1` protocol (somebar IPC)
- per-tag settings (`pertag`), `shiftview`, `rotatetags`, `sticky`
- vanity gaps + smart borders
- layout set: `tile`, `monocle`, `centeredmaster`, `snail`
- direction focus/swap + `movestack`, `moveresizekb`, `dragmfact`
- scratchpads, regex rules, custom float sizing

### Visual defaults
- Gruvbox Material-like palette in `config.h`
- transparent unfocused clients (`default_opacity_unfocus = 0.88`)
- SceneFX blur enabled by default (`blur = 1` and `blur_data` in `config.h`)

## Keybinds

Keybinds are configured in `config.h`.

Additional daily-driver bindings added in this setup:
- `Alt+Shift+Escape`: lock (`~/.config/dwl/lock.sh`)
- `Alt+Shift+V`: clipboard history picker (`cliphist`)
- `Alt+Shift+R`: toggle screen recording (`wf-recorder`)
- `Print`: full screenshot
- `Shift+Print`: area screenshot
- media/volume/brightness: mapped to XF86 keys

## Prerequisites

### Build
- C compiler and `make`
- `pkg-config`
- `meson` and `ninja` (for SceneFX)
- `wayland`, `wayland-protocols`, `wayland-scanner`
- `wlroots-0.19` headers/libs
- `wayland-server`, `xkbcommon`, `libinput`
- optional XWayland: enable `XWAYLAND` in `config.mk` and install `xcb` + `xcb-icccm`

### Runtime
- wlroots-compatible Wayland stack (DRM/KMS or nested backend)
- seat/session manager for DRM backends (logind/seatd)
- bar stack: `somebar` + `someblocks`
- session services: `mako`, `swayidle`, `swaylock`
- desktop integration: `xdg-desktop-portal`, `xdg-desktop-portal-wlr`, `xdg-desktop-portal-gtk`
- auth agent: `lxqt-policykit` or `polkit-gnome`
- helpers: `grim`, `slurp`, `wf-recorder`, `wl-clipboard`, `cliphist`, `playerctl`, `wpctl`, `brightnessctl`, `upower`

## Local SceneFX integration

This repository is configured to build against a local SceneFX checkout by default.

- default expected path: `../scenefx` (sibling directory)
- expected build output: `../scenefx/build/libscenefx-0.4.so`

Build SceneFX first:

```sh
cd /path/to/scenefx
meson setup build
ninja -C build
```

Then build dwl:

```sh
cd /path/to/dwl-fork
make clean && make
```

If your SceneFX path is different, override on make command:

```sh
make SCENEFX_DIR=/absolute/path/to/scenefx clean all
```

Note: dwl is linked with rpath to SceneFX build output. If you move SceneFX,
rebuild dwl.

## somebar / someblocks workflow

- autostart in `config.h` runs `~/.config/someblocks/run.sh`
- autostart in `config.h` also runs:
  - `~/.config/dwl/session-env.sh` (portal/dbus environment)
  - `mako`
  - `~/.config/dwl/polkit-agent.sh`
  - `~/.config/dwl/idle.sh`
  - `~/.config/dwl/cliphist-watch.sh`
  - `~/.config/dwl/battery-notify.sh`
- start dwl with somebar:

```sh
dwl -s "exec somebar </dev/null"
```

## Install (quick)

```sh
# somebar
cd /path/to/somebar-fork
meson setup build
ninja -C build
sudo ninja -C build install

# someblocks
cd /path/to/someblocks-fork
make
sudo make install

# dwl
cd /path/to/dwl-fork
make clean && make
sudo make install
```

## Notes

- This is a patch-stacked tree; rebasing onto upstream dwl will likely conflict in `dwl.c` and `config.h`.
- SceneFX integration here is local-build based (no system-wide SceneFX install required).
