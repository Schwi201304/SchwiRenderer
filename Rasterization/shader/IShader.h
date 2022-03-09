#ifndef ISHADER_H
#define ISHADER_H

#include "../image/image.h"
#include "../geometry/geometry.h"
namespace schwi {
    class IShader {
    public:
        static Color sample2D(const Image& img, vec2& uvf) {
            return img.getColor(uvf[0] * img.width(), uvf[1] * img.height());
        }
        virtual bool fragment(const vec3 bar, Color& color) = 0;
    };

}

#endif
