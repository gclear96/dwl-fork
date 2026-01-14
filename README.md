# dwl-hyprish

A dwl fork tuned to feel comfortable coming from Hyprland, with performance‑first defaults and a lightweight bar workflow.

## What’s included

### IPC + bar workflow
- **dwl‑ipc‑unstable‑v2** protocol enabled (for dwlb and IPC tooling)
- **dwlb** autostarted by default

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
- **Alt+R**: app launcher (walker)
- **Alt+F**: Firefox
- **Alt+Q**: kill client
- **Alt+V**: toggle floating
- **Alt+S** or **Alt+`**: scratchpad
- **Alt+B**: toggle bar visibility (IPC)
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
- `dwlb`

Adjust as needed.

## Build
```
make clean && make
```

## Notes
- This is a patch‑stacked fork of **dwl**. If you rebase onto upstream dwl, expect conflicts in `dwl.c` and `config.def.h`.
- IPC is required for dwlb; this fork includes `dwl-ipc-unstable-v2` and generates protocol code during build.
