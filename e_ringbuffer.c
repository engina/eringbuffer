/**
 * e_ringbuffer.c
 *
 *  Created on: 12.Tem.2010
 *      Author: Engin
 *
 *	A ring (circular) buffer implementation.
 *
 */

#include "e_ringbuffer.h"

#include <string.h>

/**
 * Initiates a ring buffer.
 * @param rbuffer	ring buffer object to be initiated
 * @param buffer	allocated memory for the underlying buffer
 * @param size		size of the buffer.
 */
void e_ringbuffer_init(ERingBuffer* rbuffer, void* buffer, size_t size)
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
static size_t rbuf_read(ERingBuffer* rbuffer, void* dst, size_t n, int advance)
{
	size_t s0, s1, r0, r1;
	if(!rbuffer->Used)
		return 0;

	if(n > rbuffer->Used)
		n = rbuffer->Used;

	/* first segment (from beginning to read pointer) */
	s0 = rbuffer->ReadStream - rbuffer->Buffer;
	/* second segment (from read pointer to end) */
	s1 = rbuffer->Size - s0;

	/* first read attempt */
	r0 = n <= s1 ? n : s1;
	/* second read attempt */
	r1 = n > s1 ? n - s1 : 0;

	if(dst)
	{
		char* d = dst;
		memcpy(d, rbuffer->ReadStream, r0);

		if(r1)
		{
			d += r0;
			memcpy(d, rbuffer->Buffer, r1);
		}
	}

	if(advance)
	{
		rbuffer->Used -= n;
		/* wrapped read or not */
		if(r1)
			rbuffer->ReadStream = rbuffer->Buffer + r1;
		else
			rbuffer->ReadStream += r0;
	}

	return n;
}

/**
 * e_ringbuffer_peek attempts to copy n bytes from the ring buffer to
 * memory pointed by dst.
 *
 * e_ringbuffer_peek may copy fewer bytes than
 * n, if there is not sufficient data in the ring buffer.
 *
 * @param rbuffer 	ring buffer object
 * @param dst		destination memory the data would be copied to
 * @param n			requested data length in bytes
 * @return			actual read bytes
 */
size_t e_ringbuffer_peek(ERingBuffer* rbuffer, void* dst, size_t n)
{
	return rbuf_read(rbuffer, dst, n, 0);
}

/**
 * e_ringbuffer_read behaves exactly the same as e_ringbuffer_peek except that
 * it actually advances the read stream.
 *
 * @param rbuffer 	ring buffer object
 * @param dst		destination memory the data would be copied to
 * @param n			requested data length in bytes
 * @return			actual read bytes
 */
size_t e_ringbuffer_read(ERingBuffer* rbuffer, void* dst, size_t n)
{
	return rbuf_read(rbuffer, dst, n, 1);
}

/**
 * e_ringbuffer_advance attempts to advance the read stream by n bytes
 * without copying any data. However, it may advance less than requested
 * amount, if there is not sufficient data in the buffer. Actual advanced
 * bytes will be returned.
 *
 * @param rbuffer 	ring buffer object
 * @param n			requested data length to be advanced in bytes
 * @return			actual advanced bytes
 */
size_t e_ringbuffer_advance(ERingBuffer* rbuffer, size_t n)
{
	return rbuf_read(rbuffer, NULL, n, 1);
}

/**
 * e_ringbuffer_write attempts to write n bytes of data from memory pointed
 * by data to ring buffer. It may write fewer data than requested if there is
 * not enough space left in the buffer. The actual written bytes will be returned.
 * @param rbuffer	ring buffer object
 * @param data		memory location to be copied to ring buffer
 * @param n			requested amount of bytes to be copied
 * @return			actual written bytes
 */
size_t e_ringbuffer_write(ERingBuffer* rbuffer, const void* data, size_t n)
{
	size_t s0, s1, w0, w1;
	if(n > e_ringbuffer_free(rbuffer))
		n = e_ringbuffer_free(rbuffer);

	s0 = rbuffer->WriteStream - rbuffer->Buffer;
	s1 = rbuffer->Size - s0;
	w0 = n <= s1 ? n : s1;
	w1 = n > s1 ? n - s1 : 0;

	memcpy(rbuffer->WriteStream, data, w0);
	if(w1)
	{
		memcpy(rbuffer->Buffer, (char*)data + w0, w1);
		rbuffer->WriteStream = rbuffer->Buffer + w1;
	}
	else
	{
		rbuffer->WriteStream += w0;
	}
	rbuffer->Used += n;
	return n;
}
