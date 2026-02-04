#include "KeyBoard.h"

namespace Sun {

    qkey_t		g_keys[MAX_KEYS];

    int MapKey(int key)
    {
        int result;
        int modified;
        bool is_extended;

        modified = (key >> 16) & 255;

        if (modified > 127)
            return 0;

        if (key & (1 << 24))
        {
            is_extended = true;
        }
        else
        {
            is_extended = false;
        }

        result = s_scantokey[modified];

        if (!is_extended)
        {
            switch (result)
            {
            case K_HOME:
                return K_KP_HOME;
            case K_UPARROW:
                return K_KP_UPARROW;
            case K_PGUP:
                return K_KP_PGUP;
            case K_LEFTARROW:
                return K_KP_LEFTARROW;
            case K_RIGHTARROW:
                return K_KP_RIGHTARROW;
            case K_END:
                return K_KP_END;
            case K_DOWNARROW:
                return K_KP_DOWNARROW;
            case K_PGDN:
                return K_KP_PGDN;
            case K_INS:
                return K_KP_INS;
            case K_DEL:
                return K_KP_DEL;
            default:
                return result;
            }
        }
        else
        {
            switch (result)
            {
            case K_PAUSE:
                return K_KP_NUMLOCK;
            case 0x0D:
                return K_KP_ENTER;
            case 0x2F:
                return K_KP_SLASH;
            case 0xAF:
                return K_KP_PLUS;
            }
            return result;
        }
    }
}