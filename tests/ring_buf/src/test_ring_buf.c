/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */
#include <zephyr/logging/log.h>
#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

LOG_MODULE_REGISTER(test_ring_buf, LOG_LEVEL_INF);

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	//LOG_INF(" COUNT BEFORE PUSH: %d", rb_count());
	zassert_ok(rb_push(4), "Push should have succeeded");
	//LOG_INF(" COUNT AFTER PUSH: %d", rb_count());
	zassert_equal(rb_count(), 1, "Buffer does not have the amount of elements expected(== 1): %d", rb_count());
	zassert_ok(rb_init(4), "Ring buffer has NOT been initialized with success");
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Buffer does not have 0 elements, as expected. It has: %d", rb_count());
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int v = 0;
	zassert_ok(rb_push(42), "Push should have succeeded");
	zassert_ok(rb_pop(&v), "Pop should have succeeded");
	zassert_equal(v, 42, "Popped value:%d does not match pushed value:%d !!", v, 42);
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int v = 0;
	
	zassert_ok(rb_push(1), "Push(1) should have succeeded");
	zassert_ok(rb_push(2), "Push(2) should have succeeded");
	zassert_ok(rb_push(3), "Push(3) should have succeeded");
	zassert_ok(rb_pop(&v), "Pop(1) should have succeeded");
	zassert_equal(v, 1, "Popped value:%d does not match pushed value:%d !!", v, 1);
	zassert_ok(rb_pop(&v), "Pop(2) should have succeeded");
	zassert_equal(v, 2, "Popped value:%d does not match pushed value:%d !!", v, 2);
	zassert_ok(rb_pop(&v), "Pop(3) should have succeeded");
	zassert_equal(v, 3, "Popped value:%d does not match pushed value:%d !!", v, 3);
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	int ret = 0;

	zassert_ok(rb_push(1), "Push(1) should have succeeded");
	zassert_ok(rb_push(2), "Push(2) should have succeeded");
	zassert_ok(rb_push(3), "Push(3) should have succeeded");
	zassert_ok(rb_push(4), "Push(4) should have succeeded");
	zassert_true(rb_is_full(), "After pushing the same amount of capacity, buffer must be full");
	ret = rb_push(5);
	zassert_equal(ret, -ENOSPC, "Push(5) should return -ENOSPC, but returned: %d", ret);
	zassert_equal(rb_count(), 4, "Buffer does not have the amount of elements expected(== 4): %d", rb_count());
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int v = 0;

	zassert_ok(rb_push(7), "Push(7) should have succeeded");
	zassert_ok(rb_peek(&v), "Peek(7) should have succeeded");
	zassert_equal(v, 7, "Peeked value does match the expected(== 7): %d", v);
	zassert_ok(rb_peek(&v), "Peek(7) should have succeeded");
	zassert_equal(v, 7, "Peeked value does match the expected(== 7): %d", v);
	zassert_equal(rb_count(), 1, "Buffer does not have the amount of elements expected(== 1): %d", rb_count());
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	int ret = 0;
	ret = rb_pop(NULL);
	zassert_equal(ret, -EINVAL, "Pop(null) should return -EINVAL, but returned: %d", ret);
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	zassert_ok(rb_push(1), "Push(1) should have succeeded");
	zassert_ok(rb_push(2), "Push(2) should have succeeded");
	zassert_ok(rb_push(3), "Push(3) should have succeeded");
	zassert_ok(rb_push(4), "Push(4) should have succeeded");
	zassert_true(rb_is_full(), "After pushing the same amount of capacity, buffer must be full");
	zassert_equal(rb_count(), 4, "Buffer does not have the amount of elements expected(== 4): %d", rb_count());
}
