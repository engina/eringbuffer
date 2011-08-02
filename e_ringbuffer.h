/* e_ringbuffer.h - a zero dependency, c89 compatible, potable ring buffer implementation

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

#ifndef E_RINGBUFFER_H_
#define E_RINGBUFFER_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct ERingBuffer ERingBuffer;

struct ERingBuffer
{
	uint8_t* Buffer;
	uint8_t* ReadStream;
	uint8_t* WriteStream;
	size_t	 Size;
	size_t	 Used;
};

void e_ringbuffer_init(ERingBuffer* rbuffer, void* buffer, size_t size);
size_t e_ringbuffer_write(ERingBuffer* rbuffer, const void* data, size_t size);
size_t e_ringbuffer_read(ERingBuffer* rbuffer, void* dst, size_t n);
size_t e_ringbuffer_peek(ERingBuffer* rbuffer, void* dst, size_t n);
size_t e_ringbuffer_advance(ERingBuffer* rbuffer, size_t n);

#define e_ringbuffer_used(rbuf)	((rbuf)->Used)
#define e_ringbuffer_free(rbuf)	((rbuf)->Size - e_ringbuffer_used(rbuf))

#endif /* ENDA_RINGBUFFER_H_ */
