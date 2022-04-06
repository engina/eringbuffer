/**
 * @file e_ringbuffer.test.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-04-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "e_ringbuffer.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define BUF_SIZE 1024

#define S0(rb) (size_t)((rb)->ReadStream - (rb)->Buffer)
#define S1(rb) ((rb)->Size - S0(rb))

#define ASSERT_RBUF(rb)                                                \
  do                                                                   \
  {                                                                    \
    CU_ASSERT_FATAL((rb)->Used <= (rb)->Size);                         \
    CU_ASSERT_FATAL((rb)->ReadStream >= (rb)->Buffer);                 \
    CU_ASSERT_FATAL((rb)->ReadStream <= ((rb)->Buffer + (rb)->Size));  \
    CU_ASSERT_FATAL((rb)->WriteStream >= (rb)->Buffer);                \
    CU_ASSERT_FATAL((rb)->WriteStream <= ((rb)->Buffer + (rb)->Size)); \
    CU_ASSERT_FATAL(S0(rb) <= (rb)->Size);                             \
    CU_ASSERT_FATAL(S1(rb) <= (rb)->Size);                             \
    CU_ASSERT_FATAL((S0(rb) + S1(rb)) <= (rb)->Size);                  \
  } while (0)

void test()
{
  ERingBuffer rb;
  char buf[BUF_SIZE];
  e_ringbuffer_init(&rb, buf, sizeof buf);
  ASSERT_RBUF(&rb);
  e_ringbuffer_write(&rb, "hello", 5);
  CU_ASSERT_EQUAL(rb.Used, 5);
  ASSERT_RBUF(&rb);
  e_ringbuffer_write(&rb, "world!", 6);
  CU_ASSERT_EQUAL(rb.Used, 11);
  CU_ASSERT_EQUAL(e_ringbuffer_free(&rb), sizeof buf - 11);
  ASSERT_RBUF(&rb);
  char dst[1024];
  e_ringbuffer_peek(&rb, dst, 6);
  CU_ASSERT_EQUAL(strncmp(dst, "hellow", 6), 0);
  e_ringbuffer_read(&rb, dst, 6);
  CU_ASSERT_EQUAL(strncmp(dst, "hellow", 6), 0);
  e_ringbuffer_peek(&rb, dst, 3);
  CU_ASSERT_EQUAL(strncmp(dst, "orl", 3), 0);
  CU_ASSERT_EQUAL(rb.Used, 5);
  e_ringbuffer_write(&rb, "append", 6);
  CU_ASSERT_EQUAL(rb.Used, 11);
  size_t read = e_ringbuffer_peek(&rb, dst, -1);
  CU_ASSERT_EQUAL(read, 11);
  // Read available data even when more is requested
  read = e_ringbuffer_peek(&rb, dst, 50);
  CU_ASSERT_EQUAL(read, 11);
  ASSERT_RBUF(&rb);
}

int main(void)
{
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("e_ringbuffer", NULL, NULL);
  if (pSuite == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "test_write", test);
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  return 0;
}
