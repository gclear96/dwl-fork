/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartborders              = 1;
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static const int monoclegaps               = 0;  /* 1 means outer gaps in monocle layout */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const unsigned int gappih           = 12; /* horiz inner gap between windows */
static const unsigned int gappiv           = 12; /* vert inner gap between windows */
static const unsigned int gappoh           = 12; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov           = 16; /* vert outer gap between windows and screen edge */
static const float rootcolor[]             = COLOR(0x282828ff);
static const float bordercolor[]           = COLOR(0x5a524cff);
static const float focuscolor[]            = COLOR(0xd8a657ff);
static const float urgentcolor[]           = COLOR(0xea6962ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = COLOR(0x282828ff); /* You can also use glsl colors */
static const int blur                      = 1;
static const struct blur_data blur_data    = {
	.radius = 5,
	.num_passes = 3,
	.noise = 0.02f,
	.brightness = 0.90f,
	.contrast = 0.90f,
	.saturation = 1.10f,
};
static const int respect_monitor_reserved_area = 0;  /* 1 to monitor center while respecting the monitor's reserved area, 0 to monitor center */
static const float default_opacity_unfocus = 0.88f;
static const float default_opacity_focus   = 1.00f;

enum {
    VIEW_L = -1,
    VIEW_R = 1,
    SHIFT_L = -2,
    SHIFT_R = 2,
} RotateTags;

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
	"/bin/sh", "-c", "$HOME/.config/dwl/wallpaper.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/session-env.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/output-setup.sh", NULL,
	"mako", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/polkit-agent.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/idle.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/cliphist-watch.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/dwl/battery-notify.sh", NULL,
	"/bin/sh", "-c", "$HOME/.config/someblocks/run.sh", NULL,
	NULL /* terminate */
};

static const Rule rules[] = {
	/* app_id             title         tags mask     switchtotag   isfloating   alpha unfocus           monitor   x   y   width   height scratchkey */
	{ "Gimp_EXAMPLE",     NULL,         0,            0,            1,           default_opacity_unfocus, -1,       -1, -1, 1000,   0.75,  0 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,         1 << 8,       1,            0,           1.00,                    -1,       -1, -1, -1,     -1,    0 }, /* Start on ONLY tag "9" */
	{ NULL,               "scratchpad", 0,            0,            1,           default_opacity_unfocus, -1,       -1, -1, -1,     -1,   's' },
    /* default/example rule: can be changed but cannot be eliminated; at least one rule must exist */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ "@|@",      snail },
	{ NULL,       NULL },    /* terminate */
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients due to
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899 */
static const MonitorRule monrules[] = {
   /* name        mfact  nmaster scale layout       rotate/reflect                x    y
    * example of a HiDPI laptop monitor:
    { "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 }, */
	{ NULL,       0.50f, 1,      1,    &layouts[4], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	/* default monitor rule: can be changed but cannot be eliminated; at least one monitor rule must exist */
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_ALT

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "alacritty", NULL };
static const char *menucmd[] = { "/bin/sh", "-c", "$HOME/.config/tofi/run.sh", NULL };
static const char *powermenucmd[] = { "/bin/sh", "-c", "$HOME/.config/tofi/power.sh", NULL };
static const char *wallpapercmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/wallpaper-picker.sh", NULL };
static const char *togglebarcmd[] = { "somebar", "-c", "toggle", "selected", NULL };
static const char *lockcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/lock.sh", NULL };
static const char *clipmenucmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/clipmenu.sh", NULL };
static const char *screenshotfullcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/screenshot.sh full", NULL };
static const char *screenshotareacmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/screenshot.sh area", NULL };
static const char *recordtogglecmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/record.sh toggle", NULL };
static const char *networkcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/network.sh", NULL };
static const char *bluetoothcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/bluetooth.sh", NULL };
static const char *volupcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/volume.sh up", NULL };
static const char *voldowncmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/volume.sh down", NULL };
static const char *volmutecmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/volume.sh mute", NULL };
static const char *micmutecmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/volume.sh micmute", NULL };
static const char *brightnessupcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/brightness.sh up", NULL };
static const char *brightnessdowncmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/brightness.sh down", NULL };
static const char *mediaplaycmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/media.sh play-pause", NULL };
static const char *medianextcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/media.sh next", NULL };
static const char *mediaprevcmd[] = { "/bin/sh", "-c", "$HOME/.config/dwl/media.sh previous", NULL };
/* named scratchpads - first arg matches the scratchkey in rules */
static const char *scratchpadcmd[] = { "s", "alacritty", "-t", "scratchpad", NULL };

#include "shiftview.c"

static const Key keys[] = {
	/* Note that Shift changes certain key codes: 2 -> at, etc. */
	/* modifier                  key                  function          argument */
	{ MODKEY,                    XKB_KEY_r,           spawn,            {.v = menucmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Escape,      spawn,            {.v = lockcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_v,           spawn,            {.v = clipmenucmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_r,           spawn,            {.v = recordtogglecmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_n,           spawn,            {.v = networkcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_b,           spawn,            {.v = bluetoothcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_w,           spawn,            {.v = wallpapercmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_p,           spawn,            {.v = powermenucmd} },
	{ MODKEY,                    XKB_KEY_Return,      spawn,            {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_f,           spawn,            {.v = (const char*[]){ "firefox", NULL } } },
	{ MODKEY,                    XKB_KEY_b,           spawn,            {.v = togglebarcmd} },
	{ MODKEY,                    XKB_KEY_grave,       togglescratch,    {.v = scratchpadcmd } },
	{ MODKEY,                    XKB_KEY_s,           togglescratch,    {.v = scratchpadcmd } },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_j,           focusstack,       {.i = +1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_k,           focusstack,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,           focusdir,         {.ui = 0} },
	{ MODKEY,                    XKB_KEY_l,           focusdir,         {.ui = 1} },
	{ MODKEY,                    XKB_KEY_k,           focusdir,         {.ui = 2} },
	{ MODKEY,                    XKB_KEY_j,           focusdir,         {.ui = 3} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_H,           swapdir,          {.ui = 0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_L,           swapdir,          {.ui = 1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,           swapdir,          {.ui = 2} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,           swapdir,          {.ui = 3} },
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_J, movestack, {.i = +1} },
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_K, movestack, {.i = -1} },
	{ MODKEY,                    XKB_KEY_i,           incnmaster,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_d,           incnmaster,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_bracketleft, rotatetags,       {.i = VIEW_L} },
	{ MODKEY,                    XKB_KEY_bracketright,rotatetags,       {.i = VIEW_R} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_bracketleft, rotatetags,       {.i = SHIFT_L} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_bracketright,rotatetags,       {.i = SHIFT_R} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_h,           setmfact,         {.f = -0.05f} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_l,           setmfact,         {.f = +0.05f} },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_h,           incgaps,          {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_l,           incgaps,          {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_SHIFT,   XKB_KEY_H,         incogaps,         {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_SHIFT,   XKB_KEY_L,         incogaps,         {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_CTRL,    XKB_KEY_h,         incigaps,         {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_CTRL,    XKB_KEY_l,         incigaps,         {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_0,           togglegaps,       {0} },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_SHIFT,   XKB_KEY_parenright,defaultgaps,      {0} },
	{ MODKEY,                    XKB_KEY_y,           incihgaps,        {.i = +1 } },
	{ MODKEY,                    XKB_KEY_o,           incihgaps,        {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_y,           incivgaps,        {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_o,           incivgaps,        {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_y,           incohgaps,        {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_o,           incohgaps,        {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Y,           incovgaps,        {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_O,           incovgaps,        {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,      zoom,             {0} },
	{ MODKEY,                    XKB_KEY_Tab,         view,             {0} },
	{ MODKEY,                    XKB_KEY_a,           shiftview,        {.i = -1 } },
	{ MODKEY,                    XKB_KEY_semicolon,   shiftview,        {.i = 1 } },
	{ MODKEY,                    XKB_KEY_q,           killclient,       {0} },
	{ MODKEY,                    XKB_KEY_t,           setlayout,        {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_m,           setlayout,        {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_c,           setlayout,        {.v = &layouts[3]} },
	{ MODKEY,                    XKB_KEY_p,           setlayout,        {.v = &layouts[4]} },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_k,           setopacityunfocus, {.f = +0.1f} },
	{ MODKEY|WLR_MODIFIER_LOGO,  XKB_KEY_j,           setopacityunfocus, {.f = -0.1f} },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_SHIFT, XKB_KEY_K, setopacityfocus, {.f = +0.1f} },
	{ MODKEY|WLR_MODIFIER_LOGO|WLR_MODIFIER_SHIFT, XKB_KEY_J, setopacityfocus, {.f = -0.1f} },
	{ MODKEY,                    XKB_KEY_space,       setlayout,        {0} },
	{ MODKEY,                    XKB_KEY_n,           nextlayout,       {0} },
	{ MODKEY,                    XKB_KEY_v,           togglefloating,   {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,       togglefloating,   {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_s,           togglesticky,     {0} },
	{ MODKEY,                    XKB_KEY_e,           togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_Down,        moveresizekb,     {.v = (int []){ 0, 40, 0, 0 }}},
	{ MODKEY,                    XKB_KEY_Up,          moveresizekb,     {.v = (int []){ 0, -40, 0, 0 }}},
	{ MODKEY,                    XKB_KEY_Right,       moveresizekb,     {.v = (int []){ 40, 0, 0, 0 }}},
	{ MODKEY,                    XKB_KEY_Left,        moveresizekb,     {.v = (int []){ -40, 0, 0, 0 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Down,        moveresizekb,     {.v = (int []){ 0, 0, 0, 40 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Up,          moveresizekb,     {.v = (int []){ 0, 0, 0, -40 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Right,       moveresizekb,     {.v = (int []){ 0, 0, 40, 0 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Left,        moveresizekb,     {.v = (int []){ 0, 0, -40, 0 }}},
	{ MODKEY,                    XKB_KEY_0,           view,             {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright,  tag,              {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,       focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,      focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,        tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,     tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                        0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                            1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                    2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                        3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                       4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                   5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                     6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                      7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                     8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_q,           quit,             {0} },
	{ 0,                         XKB_KEY_Print,       spawn,            {.v = screenshotfullcmd} },
	{ WLR_MODIFIER_SHIFT,        XKB_KEY_Print,       spawn,            {.v = screenshotareacmd} },
	{ 0,                         XKB_KEY_XF86AudioRaiseVolume, spawn,    {.v = volupcmd} },
	{ 0,                         XKB_KEY_XF86AudioLowerVolume, spawn,    {.v = voldowncmd} },
	{ 0,                         XKB_KEY_XF86AudioMute, spawn,           {.v = volmutecmd} },
	{ 0,                         XKB_KEY_XF86AudioMicMute, spawn,        {.v = micmutecmd} },
	{ 0,                         XKB_KEY_XF86MonBrightnessUp, spawn,      {.v = brightnessupcmd} },
	{ 0,                         XKB_KEY_XF86MonBrightnessDown, spawn,    {.v = brightnessdowncmd} },
	{ 0,                         XKB_KEY_XF86AudioPlay, spawn,           {.v = mediaplaycmd} },
	{ 0,                         XKB_KEY_XF86AudioNext, spawn,           {.v = medianextcmd} },
	{ 0,                         XKB_KEY_XF86AudioPrev, spawn,           {.v = mediaprevcmd} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, moveresize,     {.ui = Curmfact} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
