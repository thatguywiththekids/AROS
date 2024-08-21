/*
 * sdl.hidd - SDL graphics/sound/keyboard for AROS hosted
 * Copyright (C) 2007 Robert Norris. All rights reserved.
 * Copyright (C) 2010 The AROS Development Team. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the same terms as AROS itself.
 */

#include <exec/types.h>

#include <devices/rawkeycodes.h>

#ifdef __THROW
#undef __THROW
#endif
#ifdef __CONCAT
#undef __CONCAT
#endif

#include "sdl_intern.h"

#define DEBUG 0
#include <aros/debug.h>

struct keymap {
    SDL_Scancode sdl;
    UBYTE        aros;
};

// This array maps SDL scancodes to raw AROS keys.
// The available AROS keys are defined in devices/rawkeycodes.h.
static const struct keymap keymap[] = {
    { SDL_SCANCODE_BACKSPACE, RAWKEY_BACKSPACE },
    { SDL_SCANCODE_TAB,       RAWKEY_TAB       },
    { SDL_SCANCODE_RETURN,    RAWKEY_RETURN    },
    { SDL_SCANCODE_PAUSE,     RAWKEY_PAUSE     },
    { SDL_SCANCODE_ESCAPE,    RAWKEY_ESCAPE    },
    { SDL_SCANCODE_SPACE,     RAWKEY_SPACE     },
    { SDL_SCANCODE_COMMA,     RAWKEY_COMMA     },
    { SDL_SCANCODE_MINUS,     RAWKEY_MINUS     },
    { SDL_SCANCODE_PERIOD,    RAWKEY_PERIOD    },
    { SDL_SCANCODE_SLASH,     RAWKEY_SLASH     },
    { SDL_SCANCODE_0,         RAWKEY_0         },
    { SDL_SCANCODE_1,         RAWKEY_1         },
    { SDL_SCANCODE_2,         RAWKEY_2         },
    { SDL_SCANCODE_3,         RAWKEY_3         },
    { SDL_SCANCODE_4,         RAWKEY_4         },
    { SDL_SCANCODE_5,         RAWKEY_5         },
    { SDL_SCANCODE_6,         RAWKEY_6         },
    { SDL_SCANCODE_7,         RAWKEY_7         },
    { SDL_SCANCODE_8,         RAWKEY_8         },
    { SDL_SCANCODE_9,         RAWKEY_9         },
    { SDL_SCANCODE_SEMICOLON, RAWKEY_SEMICOLON },
    { SDL_SCANCODE_EQUALS,    RAWKEY_EQUAL     },
    { SDL_SCANCODE_BACKSLASH, RAWKEY_BACKSLASH },
    { SDL_SCANCODE_BACKQUOTE, RAWKEY_TILDE     },
    { SDL_SCANCODE_A,         RAWKEY_A         },
    { SDL_SCANCODE_B,         RAWKEY_B         },
    { SDL_SCANCODE_C,         RAWKEY_C         },
    { SDL_SCANCODE_D,         RAWKEY_D         },
    { SDL_SCANCODE_E,         RAWKEY_E         },
    { SDL_SCANCODE_F,         RAWKEY_F         },
    { SDL_SCANCODE_G,         RAWKEY_G         },
    { SDL_SCANCODE_H,         RAWKEY_H         },
    { SDL_SCANCODE_I,         RAWKEY_I         },
    { SDL_SCANCODE_J,         RAWKEY_J         },
    { SDL_SCANCODE_K,         RAWKEY_K         },
    { SDL_SCANCODE_L,         RAWKEY_L         },
    { SDL_SCANCODE_M,         RAWKEY_M         },
    { SDL_SCANCODE_N,         RAWKEY_N         },
    { SDL_SCANCODE_O,         RAWKEY_O         },
    { SDL_SCANCODE_P,         RAWKEY_P         },
    { SDL_SCANCODE_Q,         RAWKEY_Q         },
    { SDL_SCANCODE_R,         RAWKEY_R         },
    { SDL_SCANCODE_S,         RAWKEY_S         },
    { SDL_SCANCODE_T,         RAWKEY_T         },
    { SDL_SCANCODE_U,         RAWKEY_U         },
    { SDL_SCANCODE_V,         RAWKEY_V         },
    { SDL_SCANCODE_W,         RAWKEY_W         },
    { SDL_SCANCODE_X,         RAWKEY_X         },
    { SDL_SCANCODE_Y,         RAWKEY_Y         },
    { SDL_SCANCODE_Z,         RAWKEY_Z         },
    { SDL_SCANCODE_DELETE,    RAWKEY_DELETE    },
    { SDL_SCANCODE_KP0,       RAWKEY_KP_0      },
    { SDL_SCANCODE_KP1,       RAWKEY_KP_1      },
    { SDL_SCANCODE_KP2,       RAWKEY_KP_2      },
    { SDL_SCANCODE_KP3,       RAWKEY_KP_3      },
    { SDL_SCANCODE_KP4,       RAWKEY_KP_4      },
    { SDL_SCANCODE_KP5,       RAWKEY_KP_5      },
    { SDL_SCANCODE_KP6,       RAWKEY_KP_6      },
    { SDL_SCANCODE_KP7,       RAWKEY_KP_7      },
    { SDL_SCANCODE_KP8,       RAWKEY_KP_8      },
    { SDL_SCANCODE_KP9,       RAWKEY_KP_9      },
    { SDL_SCANCODE_KP_PERIOD, RAWKEY_KP_DECIMAL},
    { SDL_SCANCODE_KP_PLUS,   RAWKEY_KP_PLUS   },
    { SDL_SCANCODE_KP_ENTER,  RAWKEY_KP_ENTER  },
    { SDL_SCANCODE_UP,        RAWKEY_UP        },
    { SDL_SCANCODE_DOWN,      RAWKEY_DOWN      },
    { SDL_SCANCODE_RIGHT,     RAWKEY_RIGHT     },
    { SDL_SCANCODE_LEFT,      RAWKEY_LEFT      },
    { SDL_SCANCODE_INSERT,    RAWKEY_INSERT    },
    { SDL_SCANCODE_HOME,      RAWKEY_HOME      },
    { SDL_SCANCODE_END,       RAWKEY_END       },
    { SDL_SCANCODE_PAGEUP,    RAWKEY_PAGEUP    },
    { SDL_SCANCODE_PAGEDOWN,  RAWKEY_PAGEDOWN  },
    { SDL_SCANCODE_F1,        RAWKEY_F1        },
    { SDL_SCANCODE_F2,        RAWKEY_F2        },
    { SDL_SCANCODE_F3,        RAWKEY_F3        },
    { SDL_SCANCODE_F4,        RAWKEY_F4        },
    { SDL_SCANCODE_F5,        RAWKEY_F5        },
    { SDL_SCANCODE_F6,        RAWKEY_F6        },
    { SDL_SCANCODE_F7,        RAWKEY_F7        },
    { SDL_SCANCODE_F8,        RAWKEY_F8        },
    { SDL_SCANCODE_F9,        RAWKEY_F9        },
    { SDL_SCANCODE_F10,       RAWKEY_F10       },
    { SDL_SCANCODE_F11,       RAWKEY_F11       },
    { SDL_SCANCODE_F12,       RAWKEY_F12       },
    { SDL_SCANCODE_CAPSLOCK,  RAWKEY_CAPSLOCK  },
    { SDL_SCANCODE_RSHIFT,    RAWKEY_RSHIFT    },
    { SDL_SCANCODE_LSHIFT,    RAWKEY_LSHIFT    },
    { SDL_SCANCODE_RCTRL,     RAWKEY_CONTROL   },
    { SDL_SCANCODE_LCTRL,     RAWKEY_LCONTROL  },
    { SDL_SCANCODE_RALT,      RAWKEY_RALT      },
    { SDL_SCANCODE_LALT,      RAWKEY_LALT      },
    { SDL_SCANCODE_RMETA,     RAWKEY_RAMIGA    },
    { SDL_SCANCODE_LMETA,     RAWKEY_LAMIGA    },
    { SDL_SCANCODE_LSUPER,    RAWKEY_LAMIGA    },
    { SDL_SCANCODE_RSUPER,    RAWKEY_RAMIGA    },
    { SDL_SCANCODE_HELP,      RAWKEY_HELP      },
    { 0xff,           0xff             }
};

void sdl_keymap_init(LIBBASETYPEPTR LIBBASE) {
    int i;
    const struct keymap *pair;

    D(bug("[sdl] sdl_keymap_init\n"));

    for (i = 0; i < SDL_SCANCODE_LAST; i++)
        LIBBASE->keycode[i] = 0xff;

    for (pair = keymap; pair->sdl != 0xff && pair->aros != 0xff; pair++)
        LIBBASE->keycode[pair->sdl] = pair->aros;
}
