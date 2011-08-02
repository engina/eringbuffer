/*
 * e_ringbuffer.h
 *
 *  Created on: 12.Tem.2010
 *      Author: Engin
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
