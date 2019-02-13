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
/**
 * @file sprite.h Sprite class
 */

#ifndef _NANO_OBJECT_H_
#define _NANO_OBJECT_H_

#include "point.h"
#include "rect.h"
#include "ssd1306_hal/io.h"

/**
 * @ingroup NANO_ENGINE_API
 * @{
 */

class NanoEngineTilerBase;
class NanoObjectList;

/**
 * This is base class for all NanoObjects.
 */
class NanoObject
{
public:
    friend class NanoEngineTilerBase;
    friend class NanoObjectList;
    /**
     * Creates basic object.
     */
    NanoObject(const NanoPoint &pos);

    NanoObject(const NanoPoint &pos, const NanoPoint &size);

    /**
     * Draws nano object Engine canvas
     */
    virtual void draw() = 0;

    /**
     * Marks nano object location for refreshing on the new frame
     */
    virtual void refresh();

    virtual void update();

    /**
     * Moves sprite to new position
     */
    void moveTo(const NanoPoint &p)
    {
        refresh();
        m_rect = { p, p + m_rect.size() - (NanoPoint){1,1} };
        refresh();
    }

    /**
     * Moves sprite to new position by specified offset
     */
    void moveBy(const NanoPoint &p)
    {
        refresh();
        m_rect += p;
        refresh();
    }

    /**
     * Returns bottom-center point of the sprite
     */
    const NanoPoint bottom() const
    {
        return { (m_rect.p1.x + m_rect.p2.x) >> 1, m_rect.p2.y  };
    }

    /**
     * Returns top-center point of the sprite
     */
    const NanoPoint top() const
    {
        return { (m_rect.p1.x + m_rect.p2.x) >> 1, m_rect.p1.y  };
    }

    /**
     * Returns left-center point of the sprite
     */
    const NanoPoint left() const
    {
        return { m_rect.p1.x, (m_rect.p1.y + m_rect.p2.y) >> 1  };
    }

    /**
     * Returns right-center point of the sprite
     */
    const NanoPoint right() const
    {
        return { m_rect.p2.x, (m_rect.p1.y + m_rect.p2.y) >> 1  };
    }

    /**
     * Returns center point of the sprite
     */
    const NanoPoint center() const
    {
        return { (m_rect.p1.x + m_rect.p2.x) >> 1, (m_rect.p1.y + m_rect.p2.y) >> 1  };
    }

    /**
     * Returns sprite x position
     */
    lcdint_t x( ) const { return m_rect.p1.x; }

    /**
     * Returns sprite y position
     */
    lcdint_t y( ) const { return m_rect.p1.y; }

    /**
     * Returns current sprite position (top-left corner)
     */
    const NanoPoint & getPosition() const { return m_rect.p1; }

protected:
    NanoRect       m_rect;
    NanoEngineTilerBase *m_tiler = nullptr;

    void set_tiler( NanoEngineTilerBase *tiler )
    {
         m_tiler = tiler;
    }

private:
    NanoObject     *m_next = nullptr;
};

class NanoObjectList: public NanoObject
{
public:
    using NanoObject::NanoObject;

    NanoObject *getNext(NanoObject *prev)
    {
        return prev ? prev->m_next : m_first;
    }

    void update() override
    {
        NanoObject *p = m_first;
        while (p)
        {
            p->update();
            p = p->m_next;
        }
    }

    void refresh() override
    {
        NanoObject *p = m_first;
        while (p)
        {
            p->m_tiler = m_tiler;
            p->refresh();
            p = p->m_next;
        }
    }

    void draw() override
    {
        NanoObject *p = m_first;
        while (p)
        {
            p->m_tiler = m_tiler;
            p->draw();
            p = p->m_next;
        }
    }

    void insert(NanoObject &object)
    {
        object.m_next = m_first;
        object.m_tiler = this->m_tiler;
        m_first = &object;
        object.refresh();
    }

    void remove(NanoObject &object)
    {
        if ( m_first == nullptr )
        {
        }
        else if ( &object == m_first )
        {
            object.refresh();
            m_first = object.m_next;
            object.m_next = nullptr;
            object.m_tiler = nullptr;
        }
        else
        {
            NanoObject *p = m_first;
            while ( p->m_next )
            {
                if ( p->m_next == &object )
                {
                    object.refresh();
                    p->m_next = object.m_next;
                    object.m_next = nullptr;
                    object.m_tiler = nullptr;
                    break;
                }
                p = p->m_next;
            }
        }
    }

private:
    NanoObject *m_first;
};

/**
 * @}
 */

#endif

