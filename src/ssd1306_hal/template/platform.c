/*
    MIT License

    Copyright (c) 2018, Alexey Dynda

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

#include "ssd1306_hal/io.h"

// TODO: DON'T FORGET ADD YOUR PLATFORM FILE TO MAKEFILE

#if defined(SSD1306_HDSC_PLATFORM)

#include "app_i2c.h"

#include "intf/ssd1306_interface.h"
////////////////////////////////////////////////////////////////////////////////////////
// !!! PLATFORM I2C IMPLEMENTATION OPTIONAL !!!
#if defined(CONFIG_PLATFORM_I2C_AVAILABLE) && defined(CONFIG_PLATFORM_I2C_ENABLE)
static uint8_t s_i2c_addr = 0x3C;

static void platform_i2c_start(void)
{
    app_i2c_start();
}

static void platform_i2c_stop(void)
{
    app_i2c_stop();
}

static void platform_i2c_send(uint8_t data)
{
    app_i2c_write_data(s_i2c_addr, data, 1);
}

static void platform_i2c_close(void)
{
    ///< FIXME: --- 
}

static void platform_i2c_send_buffer(const uint8_t *data, uint16_t len)
{
    app_i2c_write_data(s_i2c_addr, data, len);
}

void ssd1306_platform_i2cInit(int8_t busId, uint8_t addr, ssd1306_platform_i2cConfig_t * cfg)
{
    if (addr) s_i2c_addr = addr;
    ssd1306_intf.spi = 0;
    ssd1306_intf.start = &platform_i2c_start;
    ssd1306_intf.stop  = &platform_i2c_stop;
    ssd1306_intf.send  = &platform_i2c_send;
    ssd1306_intf.close = &platform_i2c_close;
    ssd1306_intf.send_buffer = &platform_i2c_send_buffer;
    // init your interface here
    //...
}
#endif

////////////////////////////////////////////////////////////////////////////////////////
// !!! PLATFORM SPI IMPLEMENTATION OPTIONAL !!!
#if defined(CONFIG_PLATFORM_SPI_AVAILABLE) && defined(CONFIG_PLATFORM_SPI_ENABLE)

#include "app_spi.h"
#include "app_timer.h"
#include "intf/spi/ssd1306_spi.h"

uint32_t millis(void)       // millis()
{
    return jffies_to_msc();
}

uint32_t micros(void)       // micros()
{
    return jffies_to_mic();
}

int random(int min, int max)  // random(a,b) - can be skipped if you don't use it
{
    return (min + (Rng_GetData1() % (max - min)));
}

void digitalWrite(int pin, int level)  // digitalWrite()
{
    if(level) {
        // DCS
        plat_set_DCS();
    } else {
        plat_clr_DCS();
    }
}

static void platform_spi_start(void)
{
    // ... Open spi channel for your device with specific s_ssd1306_cs, s_ssd1306_dc
    plat_spi_start();
}

static void platform_spi_stop(void)
{
    // ... Complete spi communication
    plat_spi_stop();
}

static void platform_spi_send(uint8_t data)
{
    // ... Send byte to spi communication channel
    plat_spi_xmit(&data, 1);
}

static void platform_spi_close(void)
{
    // ... free all spi resources here
    plat_spi_deinit();
}

static void platform_spi_send_buffer(const uint8_t *data, uint16_t len)
{
    // ... Send len bytes to spi communication channel here
    plat_spi_xmit(data, len);
}

void ssd1306_platform_spiInit(int8_t busId,
                              int8_t cesPin,
                              int8_t dcPin)
{
    if (cesPin>=0) s_ssd1306_cs = cesPin;
    if (dcPin>=0) s_ssd1306_dc = dcPin;
    ssd1306_intf.spi = 1;
    ssd1306_intf.start = &platform_spi_start;
    ssd1306_intf.stop  = &platform_spi_stop;
    ssd1306_intf.send  = &platform_spi_send;
    ssd1306_intf.close = &platform_spi_close;
    ssd1306_intf.send_buffer = &platform_spi_send_buffer;
    // init your interface here
    
    
}
#endif

#endif // SSD1306_HDSC_PLATFORM
