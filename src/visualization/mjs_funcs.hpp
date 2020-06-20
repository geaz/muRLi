#ifndef MJSFUNCS_H
#define MJSFUNCS_H

#include <mjs.h>
#include <math.h>

namespace Murli
{
    void mjsSetDelay(struct mjs *mjs)
    {
        mjs_val_t arg_delay = mjs_arg(mjs, 0);
        double delay = mjs_get_double(mjs, arg_delay);

        ScriptContextPointer->setDelay(delay);
    }

    void mjsSetLed(struct mjs *mjs)
    {
        mjs_val_t arg_index = mjs_arg(mjs, 0);
        mjs_val_t arg_color = mjs_arg(mjs, 1);

        double ledIndex = mjs_get_double(mjs, arg_index);
        double hexColor = mjs_get_double(mjs, arg_color);

        ScriptContextPointer->setLed(ledIndex, Color::fromHex(hexColor));
    }

    /**
     * @brief Maps a value in a specific range to a value in another range.
     * This function is used by mJS and takes five parameters.
     * 
     * @param mjs The mJS instance to use
     */
    void mjsMap(struct mjs *mjs) {
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

    /**
     * @brief Rounds a decimal to the next full integer.
     * This function is used by mJS and takes one paramter.
     * 
     * @param mjs The mJS instance to use
     */
    void mjsRound(struct mjs *mjs) {
        mjs_val_t arg_x =       mjs_arg(mjs, 0);
        double x = mjs_get_double(mjs, arg_x);

        mjs_return(mjs, mjs_mk_number(mjs, round(x)));
    }

    /**
     * @brief Converts a RGB value to it coressponding HEX Color value.
     * This function is used by mJS and takes three parameters.
     * 
     * @param mjs The mJS instance to use 
     */
    void xrgb(struct mjs *mjs)
    {
        mjs_val_t arg_r = mjs_arg(mjs, 0);
        mjs_val_t arg_g = mjs_arg(mjs, 1);
        mjs_val_t arg_b = mjs_arg(mjs, 2);

        uint32_t r = mjs_get_double(mjs, arg_r);
        uint32_t g = mjs_get_double(mjs, arg_g);
        uint32_t b = mjs_get_double(mjs, arg_b);

        mjs_return(mjs, mjs_mk_number(mjs, (r<<16) + (g<<8) + b));
    }

    /**
     * @brief Converts a HSV value to it coressponding HEX Color value.
     * Code was taken from https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
     * It was adapted a bit for the muRLi use case.
     * 
     * @param mjs The mJS instance to use
     */
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

        uint32_t normR = r*255;
        uint32_t normG = g*255;
        uint32_t normB = b*255;

        mjs_return(mjs, mjs_mk_number(mjs, (normR<<16) + (normG<<8) + normB));
    }
}

#endif // MJSFUNCS_H