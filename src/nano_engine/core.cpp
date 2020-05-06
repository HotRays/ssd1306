/*
    MIT License

    Copyright (c) 2018-2019, Alexey Dynda

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "core.h"

#ifdef SDL_EMULATION
#include "sdl_core.h"
#endif
///////////////////////////////////////////////////////////////////////////////
////// NANO ENGINE INPUTS CLASS ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** Callback to call if buttons state needs to be updated */
TNanoEngineGetButtons NanoEngineInputs::m_onButtons = nullptr;

bool NanoEngineInputs::pressed(uint8_t buttons)
{
    return (m_onButtons() & buttons) == buttons;
}

bool NanoEngineInputs::notPressed(uint8_t buttons)
{
    return (m_onButtons() & buttons) == 0;
}

///////////////////////////////////////////////////////////////////////////////
////// NANO ENGINE CORE CLASS /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/** Defaut frame rate for all engines */
static const uint8_t ENGINE_DEFAULT_FPS = 30;

/** Duration between frames in milliseconds */
uint8_t   NanoEngineCore::m_frameDurationMs = 1000/ENGINE_DEFAULT_FPS;
/** Current fps */
uint8_t   NanoEngineCore::m_fps = ENGINE_DEFAULT_FPS;
/** Current cpu load in percents */
uint8_t   NanoEngineCore::m_cpuLoad = 0;
/** Last timestamp in milliseconds the frame was updated on oled display */
uint32_t  NanoEngineCore::m_lastFrameTs;
/** Callback to call before starting oled update */
TLoopCallback NanoEngineCore::m_loop = nullptr;


void NanoEngineCore::begin()
{
    m_lastFrameTs = millis();
}

void NanoEngineCore::setFrameRate(uint8_t fps)
{
    if ( fps > 0 )
    {
        m_fps = fps;
        m_frameDurationMs = 1000/fps;
    }
}

bool NanoEngineCore::nextFrame()
{
    bool needUpdate = (uint32_t)(millis() - m_lastFrameTs) >= m_frameDurationMs;
    if (needUpdate && m_loop) m_loop();
    return needUpdate;
}

