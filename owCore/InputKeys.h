#pragma once

/* Mode */
#define OW_RELEASE                0
#define OW_PRESS                  1
#define OW_REPEAT                 2

/* The unknown key */
#define OW_KEY_UNKNOWN            -1

/* Printable keys */
#define OW_KEY_SPACE              32
#define OW_KEY_APOSTROPHE         39  /* ' */
#define OW_KEY_COMMA              44  /* , */
#define OW_KEY_MINUS              45  /* - */
#define OW_KEY_PERIOD             46  /* . */
#define OW_KEY_SLASH              47  /* / */
#define OW_KEY_0                  48
#define OW_KEY_1                  49
#define OW_KEY_2                  50
#define OW_KEY_3                  51
#define OW_KEY_4                  52
#define OW_KEY_5                  53
#define OW_KEY_6                  54
#define OW_KEY_7                  55
#define OW_KEY_8                  56
#define OW_KEY_9                  57
#define OW_KEY_SEMICOLON          59  /* ; */
#define OW_KEY_EQUAL              61  /* = */
#define OW_KEY_A                  65
#define OW_KEY_B                  66
#define OW_KEY_C                  67
#define OW_KEY_D                  68
#define OW_KEY_E                  69
#define OW_KEY_F                  70
#define OW_KEY_G                  71
#define OW_KEY_H                  72
#define OW_KEY_I                  73
#define OW_KEY_J                  74
#define OW_KEY_K                  75
#define OW_KEY_L                  76
#define OW_KEY_M                  77
#define OW_KEY_N                  78
#define OW_KEY_O                  79
#define OW_KEY_P                  80
#define OW_KEY_Q                  81
#define OW_KEY_R                  82
#define OW_KEY_S                  83
#define OW_KEY_T                  84
#define OW_KEY_U                  85
#define OW_KEY_V                  86
#define OW_KEY_W                  87
#define OW_KEY_X                  88
#define OW_KEY_Y                  89
#define OW_KEY_Z                  90
#define OW_KEY_LEFT_BRACKET       91  /* [ */
#define OW_KEY_BACKSLASH          92  /* \ */
#define OW_KEY_RIGHT_BRACKET      93  /* ] */
#define OW_KEY_GRAVE_ACCENT       96  /* ` */
#define OW_KEY_WORLD_1            161 /* non-US #1 */
#define OW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define OW_KEY_ESCAPE             256
#define OW_KEY_ENTER              257
#define OW_KEY_TAB                258
#define OW_KEY_BACKSPACE          259
#define OW_KEY_INSERT             260
#define OW_KEY_DELETE             261
#define OW_KEY_RIGHT              262
#define OW_KEY_LEFT               263
#define OW_KEY_DOWN               264
#define OW_KEY_UP                 265
#define OW_KEY_PAGE_UP            266
#define OW_KEY_PAGE_DOWN          267
#define OW_KEY_HOME               268
#define OW_KEY_END                269
#define OW_KEY_CAPS_LOCK          280
#define OW_KEY_SCROLL_LOCK        281
#define OW_KEY_NUM_LOCK           282
#define OW_KEY_PRINT_SCREEN       283
#define OW_KEY_PAUSE              284
#define OW_KEY_F1                 290
#define OW_KEY_F2                 291
#define OW_KEY_F3                 292
#define OW_KEY_F4                 293
#define OW_KEY_F5                 294
#define OW_KEY_F6                 295
#define OW_KEY_F7                 296
#define OW_KEY_F8                 297
#define OW_KEY_F9                 298
#define OW_KEY_F10                299
#define OW_KEY_F11                300
#define OW_KEY_F12                301
#define OW_KEY_F13                302
#define OW_KEY_F14                303
#define OW_KEY_F15                304
#define OW_KEY_F16                305
#define OW_KEY_F17                306
#define OW_KEY_F18                307
#define OW_KEY_F19                308
#define OW_KEY_F20                309
#define OW_KEY_F21                310
#define OW_KEY_F22                311
#define OW_KEY_F23                312
#define OW_KEY_F24                313
#define OW_KEY_F25                314
#define OW_KEY_KP_0               320
#define OW_KEY_KP_1               321
#define OW_KEY_KP_2               322
#define OW_KEY_KP_3               323
#define OW_KEY_KP_4               324
#define OW_KEY_KP_5               325
#define OW_KEY_KP_6               326
#define OW_KEY_KP_7               327
#define OW_KEY_KP_8               328
#define OW_KEY_KP_9               329
#define OW_KEY_KP_DECIMAL         330
#define OW_KEY_KP_DIVIDE          331
#define OW_KEY_KP_MULTIPLY        332
#define OW_KEY_KP_SUBTRACT        333
#define OW_KEY_KP_ADD             334
#define OW_KEY_KP_ENTER           335
#define OW_KEY_KP_EQUAL           336
#define OW_KEY_LEFT_SHIFT         340
#define OW_KEY_LEFT_CONTROL       341
#define OW_KEY_LEFT_ALT           342
#define OW_KEY_LEFT_SUPER         343
#define OW_KEY_RIGHT_SHIFT        344
#define OW_KEY_RIGHT_CONTROL      345
#define OW_KEY_RIGHT_ALT          346
#define OW_KEY_RIGHT_SUPER        347
#define OW_KEY_MENU               348

#define OW_KEY_LAST               OW_KEY_MENU

/* Mod keys */
#define OW_MOD_SHIFT              0x0001
#define OW_MOD_CONTROL            0x0002
#define OW_MOD_ALT                0x0004
#define OW_MOD_SUPER              0x0008

/* Mouse keys */
#define OW_MOUSE_BUTTON_1         0
#define OW_MOUSE_BUTTON_2         1
#define OW_MOUSE_BUTTON_3         2
#define OW_MOUSE_BUTTON_4         3
#define OW_MOUSE_BUTTON_5         4
#define OW_MOUSE_BUTTON_6         5
#define OW_MOUSE_BUTTON_7         6
#define OW_MOUSE_BUTTON_8         7
#define OW_MOUSE_BUTTON_LAST      OW_MOUSE_BUTTON_8
#define OW_MOUSE_BUTTON_LEFT      OW_MOUSE_BUTTON_1
#define OW_MOUSE_BUTTON_RIGHT     OW_MOUSE_BUTTON_2
#define OW_MOUSE_BUTTON_MIDDLE    OW_MOUSE_BUTTON_3

/* Common */
#define OW_KEYSCOUNT              349
#define OW_MOUSEBUTTONSCOUNT      8