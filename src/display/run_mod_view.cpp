#ifndef RUNMODVIEW_H
#define RUNMODVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class RunModView : public View
    {
        public:            
            void drawFrame(DisplayCtrl& display) override
            {
                for(uint8_t i = 0; i < frequencyRange.size(); i++)
                {
                    drawFrequencyBar(display, i, frequencyRange[i]);
                }

                display.setFontPosTop();
                display.setFont(u8g2_font_5x8_tf); 

                if(!_strPosCalculated)
                {
                    uint8_t volStrWidth = display.getStrWidth("000");
                    _volStrXPos = display.getDisplayWidth() - volStrWidth;

                    uint8_t freqStrWidth = display.getStrWidth("00000hz");
                    _freqStrXPos = _volStrXPos - freqStrWidth - 8;

                    _strPosCalculated = true;
                }       
                     
                display.drawStr(0, 0, modName.c_str());
                display.drawStr(_freqStrXPos, 0, getPaddedNumberString(7, "%ihz", dominantFrequency, 7).c_str()); 
                display.drawStr(_volStrXPos - 5, 0, "/");
                display.drawStr(_volStrXPos, 0, getPaddedNumberString(4, "%i", volume, 3).c_str());
                display.drawHLine(0, 10, display.getDisplayWidth());

                display.setFontPosBaseline();
            }

            void fadeFrequencyRange()
            {
                for(uint8_t i = 0; i < frequencyRange.size(); i++)
                {
                    if(frequencyRange[i] > 0) frequencyRange[i]--;
                }
            }
           
            std::string modName = "";
            uint8_t volume = 0;
            uint16_t dominantFrequency = 0;
            std::vector<uint8_t> frequencyRange
                = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        private:
            /**
             * Max index = 16 starting at 0
             * Max height = 9 starting at 0
             **/
            void drawFrequencyBar(DisplayCtrl& display, uint8_t index, uint8_t height)
            {
                for(uint8_t i = 0; i <= height; i++)
                {                        
                    uint8_t x = 5 + (index*2) /* gap */ + (index*5) /* size */;
                    uint8_t y = display.getDisplayHeight() - 17 /* bottom start (statusbar) */ - (i*4) /* gap and height */;
                    display.drawBox(x, y, 5, 2);
                }
            }

            std::string getPaddedNumberString(uint8_t strLength, const char* format, uint16_t number, uint8_t padWidth)
            {
                char strBuf[strLength];
                std::sprintf(strBuf, format, number);

                std::string fmtStr(strBuf);
                fmtStr.insert(fmtStr.begin(), padWidth-fmtStr.length(), '0');

                return fmtStr;
            }

            bool _strPosCalculated = false;
            uint8_t _freqStrXPos = 0;
            uint8_t _volStrXPos = 0;
    };
}

#endif // RUNMODVIEW_H