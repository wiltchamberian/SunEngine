#ifndef __GTEXTURE_H
#define __GTEXTURE_H

namespace Sun {

    class GTexture {
    public:
        GTexture();
        ~GTexture();
        virtual int width() = 0;
        virtual int height() = 0;

    };

}

#endif