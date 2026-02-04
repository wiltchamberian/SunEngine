/*****************************************************************************
* @brief : KeyBoard
* @author : acedtang
* @date : 2021/4/18
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_KEY_BOARD_H
#define __SUN_KEY_BOARD_H

namespace Sun {

#define MAX_KEYS 256

    typedef struct {
        bool	down;
        int			repeats;		// if > 1, it is autorepeating
        char* binding;
    } qkey_t;

    extern	qkey_t		g_keys[MAX_KEYS];

    extern int MapKey(int key);

    //this is from quake3!
    typedef enum {
        K_TAB = 9,
        K_ENTER = 13,
        K_ESCAPE = 27,
        K_SPACE = 32,

        K_BACKSPACE = 127,

        K_COMMAND = 128,
        K_CAPSLOCK,
        K_POWER,
        K_PAUSE,

        K_UPARROW,
        K_DOWNARROW,
        K_LEFTARROW,
        K_RIGHTARROW,

        K_ALT,
        K_CTRL,
        K_SHIFT,
        K_INS,
        K_DEL,
        K_PGDN,
        K_PGUP,
        K_HOME,
        K_END,

        K_F1,
        K_F2,
        K_F3,
        K_F4,
        K_F5,
        K_F6,
        K_F7,
        K_F8,
        K_F9,
        K_F10,
        K_F11,
        K_F12,
        K_F13,
        K_F14,
        K_F15,

        K_KP_HOME,
        K_KP_UPARROW,
        K_KP_PGUP,
        K_KP_LEFTARROW,
        K_KP_5,
        K_KP_RIGHTARROW,
        K_KP_END,
        K_KP_DOWNARROW,
        K_KP_PGDN,
        K_KP_ENTER,
        K_KP_INS,
        K_KP_DEL,
        K_KP_SLASH,
        K_KP_MINUS,
        K_KP_PLUS,
        K_KP_NUMLOCK,
        K_KP_STAR,
        K_KP_EQUALS,

        K_MOUSE1,
        K_MOUSE2,
        K_MOUSE3,
        K_MOUSE4,
        K_MOUSE5,

        K_MWHEELDOWN,
        K_MWHEELUP,

        K_JOY1,
        K_JOY2,
        K_JOY3,
        K_JOY4,
        K_JOY5,
        K_JOY6,
        K_JOY7,
        K_JOY8,
        K_JOY9,
        K_JOY10,
        K_JOY11,
        K_JOY12,
        K_JOY13,
        K_JOY14,
        K_JOY15,
        K_JOY16,
        K_JOY17,
        K_JOY18,
        K_JOY19,
        K_JOY20,
        K_JOY21,
        K_JOY22,
        K_JOY23,
        K_JOY24,
        K_JOY25,
        K_JOY26,
        K_JOY27,
        K_JOY28,
        K_JOY29,
        K_JOY30,
        K_JOY31,
        K_JOY32,

        K_AUX1,
        K_AUX2,
        K_AUX3,
        K_AUX4,
        K_AUX5,
        K_AUX6,
        K_AUX7,
        K_AUX8,
        K_AUX9,
        K_AUX10,
        K_AUX11,
        K_AUX12,
        K_AUX13,
        K_AUX14,
        K_AUX15,
        K_AUX16,

        K_LAST_KEY		// this had better be <256!
    } SKeyCode;

    static unsigned char s_scantokey[128] =
    {
        //  0           1       2       3       4       5       6       7 
        //  8           9       A       B       C       D       E       F 
            0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6',
            '7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
            'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
            'o',    'p',    '[',    ']',    13 ,    K_CTRL,'a',  's',      // 1 
            'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
            '\'' ,    '`',    K_SHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
            'b',    'n',    'm',    ',',    '.',    '/',    K_SHIFT,'*',
            K_ALT,' ',   K_CAPSLOCK  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
            K_F6, K_F7, K_F8, K_F9, K_F10,  K_PAUSE,    0  , K_HOME,
            K_UPARROW,K_PGUP,K_KP_MINUS,K_LEFTARROW,K_KP_5,K_RIGHTARROW, K_KP_PLUS,K_END, //4 
            K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11,
            K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5
            0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,
            0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
            0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,
            0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
    };
}

#endif