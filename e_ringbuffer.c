/* e_ringbuffer.c - a zero dependency, c89 compatible, potable ring buffer implementation

  Copyright (C) 2010-2011 Engin AYDOGAN

  (zlib license)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Engin AYDOGAN engin@bzzzt.biz
*/

#include "e_ringbuffer.h"

#include <string.h>
#include <assert.h>

#define ASSERT_RBUF()                                                      \
    do                                                                     \
    {                                                                      \
        assert(rbuffer->Used <= rbuffer->Size);                            \
        assert(rbuffer->ReadStream >= rbuffer->Buffer);                    \
        assert(rbuffer->ReadStream <= (rbuffer->Buffer + rbuffer->Size));  \
        assert(rbuffer->WriteStream >= rbuffer->Buffer);                   \
        assert(rbuffer->WriteStream <= (rbuffer->Buffer + rbuffer->Size)); \
    } while (0)

#define ASSERT_SEGMENTS()                   \
    do                                      \
    {                                       \
        assert(s0 <= rbuffer->Size);        \
        assert(s1 <= rbuffer->Size);        \
        assert((s0 + s1) <= rbuffer->Size); \
    } while (0)

/**
 * Initiates a ring buffer.
 * @param rbuffer   ring buffer object to be initiated
 * @param buffer    allocated memory for the underlying buffer
 * @param size      size of the buffer.
 */
void e_ringbuffer_init(ERingBuffer *rbuffer, void *buffer, size_t size)
{
    rbuffer->Buffer = rbuffer->ReadStream = rbuffer->WriteStream = buffer;
    rbuffer->Size = size;
    rbuffer->Used = 0;
}

/**
 * @internal
 * @see e_ringbuffer_peek
 * @see e_ringbuffer_read
 */
static size_t rbuf_read(ERingBuffer *rbuffer, void *dst, size_t n, int advance)
{
    size_t s0, s1, r0, r1;
    ASSERT_RBUF();

    if (!rbuffer->Used)
        return 0;

    if (n > rbuffer->Used || n < 0)
        n = rbuffer->Used;

    /* first segment (from beginning to read pointer) */
    s0 = rbuffer->ReadStream - rbuffer->Buffer;
    /* second segment (from read pointer to end) */
    s1 = rbuffer->Size - s0;

    ASSERT_SEGMENTS();

    /* first read attempt */
    r0 = n <= s1 ? n : s1;
    /* second read attempt */
    r1 = n > s1 ? n - s1 : 0;

    if (dst)
    {
        char *d = dst;
        memcpy(d, rbuffer->ReadStream, r0);

        if (r1)
        {
            d += r0;
            memcpy(d, rbuffer->Buffer, r1);
        }
    }

    if (advance)
    {
        rbuffer->Used -= n;
        /* wrapped read or not */
        if (r1)
            rbuffer->ReadStream = rbuffer->Buffer + r1;
        else
            rbuffer->ReadStream += r0;
    }
    ASSERT_RBUF();
    return n;
}

/**
 * e_ringbuffer_peek attempts to copy n bytes from the ring buffer to
 * memory pointed by dst.
 *
 * e_ringbuffer_peek may copy fewer bytes than
 * n, if there is not sufficient data in the ring buffer.
 *
 * @param rbuffer     ring buffer object
 * @param dst        destination memory the data would be copied to
 * @param n            requested data length in bytes, n < 0 for all available data
 * @return            actual read bytes
 */
size_t e_ringbuffer_peek(ERingBuffer *rbuffer, void *dst, size_t n)
{
    return rbuf_read(rbuffer, dst, n, 0);
}

/**
 * e_ringbuffer_read behaves exactly the same as e_ringbuffer_peek except that
 * it actually advances the read stream.
 *
 * @param rbuffer     ring buffer object
 * @param dst        destination memory the data would be copied to
 * @param n            requested data length in bytes, n < 0 for all available data
 * @return            actual read bytes
 */
size_t e_ringbuffer_read(ERingBuffer *rbuffer, void *dst, size_t n)
{
    return rbuf_read(rbuffer, dst, n, 1);
}

/**
 * e_ringbuffer_advance attempts to advance the read stream by n bytes
 * without copying any data. However, it may advance less than requested
 * amount, if there is not sufficient data in the buffer. Actual advanced
 * bytes will be returned.
 *
 * @param rbuffer     ring buffer object
 * @param n            requested data length to be advanced in bytes
 * @return            actual advanced bytes
 */
size_t e_ringbuffer_advance(ERingBuffer *rbuffer, size_t n)
{
    return rbuf_read(rbuffer, NULL, n, 1);
}

/**
 * e_ringbuffer_write attempts to write n bytes of data from memory pointed
 * by data to ring buffer. It may write fewer data than requested if there is
 * not enough space left in the buffer. The actual written bytes will be returned.
 * @param rbuffer    ring buffer object
 * @param data        memory location to be copied to ring buffer
 * @param n            requested amount of bytes to be copied
 * @return            actual written bytes
 */
size_t e_ringbuffer_write(ERingBuffer *rbuffer, const void *data, size_t n)
{
    size_t s0, s1, w0, w1;
    ASSERT_RBUF();

    if (n > e_ringbuffer_free(rbuffer))
        n = e_ringbuffer_free(rbuffer);

    s0 = rbuffer->WriteStream - rbuffer->Buffer;
    s1 = rbuffer->Size - s0;

    ASSERT_SEGMENTS();

    w0 = n <= s1 ? n : s1;
    w1 = n > s1 ? n - s1 : 0;

    memcpy(rbuffer->WriteStream, data, w0);
    if (w1)
    {
        memcpy(rbuffer->Buffer, (char *)data + w0, w1);
        rbuffer->WriteStream = rbuffer->Buffer + w1;
    }
    else
    {
        rbuffer->WriteStream += w0;
    }
    rbuffer->Used += n;

    ASSERT_RBUF();
    return n;
}
