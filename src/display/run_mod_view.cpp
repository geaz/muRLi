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
                    uint8_t volStrWidth = display.getStrWidth("-00.0dB");
                    _volStrXPos = display.getDisplayWidth() - volStrWidth;
                    _strPosCalculated = true;
                }       
                                    
                char dBBuf[14]; 
                char freqBuf[14];

                std::sprintf(dBBuf, "%05.1fdB", decibel);
                std::sprintf(freqBuf, "%05ihz", dominantFrequency);

                display.drawStr(0, 0, freqBuf);
                display.drawStr(_volStrXPos, 0, dBBuf);
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
            
            float decibel = 0.0;
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

            bool _strPosCalculated = false;
            uint8_t _freqStrXPos = 0;
            uint8_t _volStrXPos = 0;
    };
}

#endif // RUNMODVIEW_H