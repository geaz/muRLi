#ifndef MJSFUNCS_H
#define MJSFUNCS_H

#include <mjs.h>
#include <math.h>

namespace Murli
{
    void doubleMap(struct mjs *mjs) {
        mjs_val_t arg_x =       mjs_arg(mjs, 0);
        mjs_val_t arg_in_min =  mjs_arg(mjs, 1);
        mjs_val_t arg_in_max =  mjs_arg(mjs, 2);
        mjs_val_t arg_out_min = mjs_arg(mjs, 3);
        mjs_val_t arg_out_max = mjs_arg(mjs, 4);

        double x = mjs_get_double(mjs, arg_x);
        double in_min = mjs_get_double(mjs, arg_in_min);
        double in_max = mjs_get_double(mjs, arg_in_max);
        double out_min = mjs_get_double(mjs, arg_out_min);
        double out_max = mjs_get_double(mjs, arg_out_max);

        mjs_return(mjs, mjs_mk_number(mjs, (x-in_min)/(in_max-in_min)*(out_max-out_min)+out_min));
    }

    void xrgb(struct mjs *mjs)
    {
        mjs_val_t arg_r = mjs_arg(mjs, 0);
        mjs_val_t arg_g = mjs_arg(mjs, 1);
        mjs_val_t arg_b = mjs_arg(mjs, 2);

        uint8_t r = mjs_get_double(mjs, arg_r);
        uint8_t g = mjs_get_double(mjs, arg_g);
        uint8_t b = mjs_get_double(mjs, arg_b);

        mjs_return(mjs, mjs_mk_number(mjs, (r<<16) + (g<<8) + b));
    }

    void xhsv(struct mjs *mjs)
    {
        mjs_val_t arg_h = mjs_arg(mjs, 0);
        mjs_val_t arg_s = mjs_arg(mjs, 1);
        mjs_val_t arg_v = mjs_arg(mjs, 2);

        double h = mjs_get_double(mjs, arg_h);
        double s = mjs_get_double(mjs, arg_s);
        double v = mjs_get_double(mjs, arg_v);

        double chroma = v * s;
        double hPrime = fmod(h / 60.0, 6);
        double x = chroma * (1 - fabs(fmod(hPrime, 2) - 1));
        double m = v - chroma;
        
        double r = 0, g = 0, b = 0;
        if(0 <= hPrime && hPrime < 1) {
            r = chroma;
            g = x;
            b = 0;
        } else if(1 <= hPrime && hPrime < 2) {
            r = x;
            g = chroma;
            b = 0;
        } else if(2 <= hPrime && hPrime < 3) {
            r = 0;
            g = chroma;
            b = x;
        } else if(3 <= hPrime && hPrime < 4) {
            r = 0;
            g = x;
            b = chroma;
        } else if(4 <= hPrime && hPrime < 5) {
            r = x;
            g = 0;
            b = chroma;
        } else if(5 <= hPrime && hPrime < 6) {
            r = chroma;
            g = 0;
            b = x;
        } else {
            r = 0;
            g = 0;
            b = 0;
        }
        
        r += m;
        g += m;
        b += m;

        uint8_t normR = r*255;
        uint8_t normG = g*255;
        uint8_t normB = b*255;

        mjs_return(mjs, mjs_mk_number(mjs, (normR<<16) + (normG<<8) + normB));
    }
}

#endif // MJSFUNCS_H