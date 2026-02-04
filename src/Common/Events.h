#ifndef __METEOREVENTS_H
#define __METEOREVENTS_H

#include <queue>

namespace Sun
{
    typedef enum SEventType
    {
        SE_NONE = 0,	// evTime is still valid
        SE_KEY,		// evValue is a key code, evValue2 is the down flag
        SE_CHAR,	// evValue is an ascii char
        SE_MOUSE,	// evValue and evValue2 are reletive signed x / y moves
        SE_MOUSE_CLICK, //evValue is 1:left 0:right
        SE_JOYSTICK_AXIS,	// evValue is an axis number and evValue2 is the current state (-127 to 127)
        SE_CONSOLE,	// evPtr is a char*
        SE_NET	 // evPtr is a netadr_t followed by data bytes to evPtrLength
    }SEventType;

    typedef struct SEvent
    {
        int				time;
        SEventType    	type;
        int				value, value2;
        int				ptrLength;	// bytes of data pointed to by evPtr, for journaling
        void*           ptr;
        int             width;
        int             height;
        SEvent():time(0),type(SE_NONE),value(0),value2(0),
        ptrLength(0),ptr(0),width(0),height(0){

        }
    } SEvent;

    extern std::queue<SEvent> g_evQue;
}



#endif