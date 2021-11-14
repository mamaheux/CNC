#include <cnc/collections/BoundedQueue.h>
#include <unity.h>

void test_BoundedQueue_popPush() {
  BoundedQueue<int, 1> a;
  BoundedQueue<int, 2> b;
  BoundedQueue<int, 3> c;
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(0, c.size());

  TEST_ASSERT_TRUE(a.push(10));
  TEST_ASSERT_TRUE(b.push(20));
  TEST_ASSERT_TRUE(c.push(30));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(1, b.size());
  TEST_ASSERT_EQUAL(1, c.size());

  TEST_ASSERT_FALSE(a.push(11));
  TEST_ASSERT_TRUE(b.push(21));
  TEST_ASSERT_TRUE(c.push(31));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(2, b.size());
  TEST_ASSERT_EQUAL(2, c.size());

  TEST_ASSERT_FALSE(a.push(12));
  TEST_ASSERT_FALSE(b.push(22));
  TEST_ASSERT_TRUE(c.push(32));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(2, b.size());
  TEST_ASSERT_EQUAL(3, c.size());

  TEST_ASSERT_FALSE(a.push(13));
  TEST_ASSERT_FALSE(b.push(23));
  TEST_ASSERT_FALSE(c.push(33));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(2, b.size());
  TEST_ASSERT_EQUAL(3, c.size());

  TEST_ASSERT_TRUE(10 == a.pop());
  TEST_ASSERT_TRUE(20 == b.pop());
  TEST_ASSERT_TRUE(30 == c.pop());
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(1, b.size());
  TEST_ASSERT_EQUAL(2, c.size());

  TEST_ASSERT_TRUE(tl::nullopt == a.pop());
  TEST_ASSERT_TRUE(21 == b.pop());
  TEST_ASSERT_TRUE(31 == c.pop());
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(1, c.size());

  TEST_ASSERT_TRUE(tl::nullopt == a.pop());
  TEST_ASSERT_TRUE(tl::nullopt == b.pop());
  TEST_ASSERT_TRUE(32 == c.pop());
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(0, c.size());

  TEST_ASSERT_TRUE(tl::nullopt == a.pop());
  TEST_ASSERT_TRUE(tl::nullopt == b.pop());
  TEST_ASSERT_TRUE(tl::nullopt == c.pop());
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(0, c.size());

  TEST_ASSERT_TRUE(a.push(14));
  TEST_ASSERT_TRUE(b.push(24));
  TEST_ASSERT_TRUE(c.push(34));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(1, b.size());
  TEST_ASSERT_EQUAL(1, c.size());

  TEST_ASSERT_FALSE(a.push(15));
  TEST_ASSERT_TRUE(b.push(25));
  TEST_ASSERT_TRUE(c.push(35));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(2, b.size());
  TEST_ASSERT_EQUAL(2, c.size());

  TEST_ASSERT_FALSE(a.push(16));
  TEST_ASSERT_FALSE(b.push(26));
  TEST_ASSERT_TRUE(c.push(36));
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(2, b.size());
  TEST_ASSERT_EQUAL(3, c.size());

  int va, vb, vc;
  TEST_ASSERT_TRUE(a.pop(va));
  TEST_ASSERT_TRUE(b.pop(vb));
  TEST_ASSERT_TRUE(c.pop(vc));
  TEST_ASSERT_EQUAL(14, va);
  TEST_ASSERT_EQUAL(24, vb);
  TEST_ASSERT_EQUAL(34, vc);
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(1, b.size());
  TEST_ASSERT_EQUAL(2, c.size());

  TEST_ASSERT_FALSE(a.pop(va));
  TEST_ASSERT_TRUE(b.pop(vb));
  TEST_ASSERT_TRUE(c.pop(vc));
  TEST_ASSERT_EQUAL(25, vb);
  TEST_ASSERT_EQUAL(35, vc);
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(1, c.size());

  TEST_ASSERT_FALSE(a.pop(va));
  TEST_ASSERT_FALSE(b.pop(vb));
  TEST_ASSERT_TRUE(c.pop(vc));
  TEST_ASSERT_EQUAL(36, vc);
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());
  TEST_ASSERT_EQUAL(0, c.size());
}

void test_BoundedQueue_isEmpty() {
  BoundedQueue<int, 1> a;
  BoundedQueue<int, 2> b;
  TEST_ASSERT_TRUE(a.isEmpty());
  TEST_ASSERT_TRUE(b.isEmpty());

  a.push(10);
  b.push(20);
  TEST_ASSERT_FALSE(a.isEmpty());
  TEST_ASSERT_FALSE(b.isEmpty());
}

void test_BoundedQueue_isFull() {
  BoundedQueue<int, 1> a;
  BoundedQueue<int, 2> b;
  TEST_ASSERT_FALSE(a.isFull());
  TEST_ASSERT_FALSE(b.isFull());

  a.push(10);
  b.push(20);
  TEST_ASSERT_TRUE(a.isFull());
  TEST_ASSERT_FALSE(b.isFull());

  b.push(21);
  TEST_ASSERT_TRUE(b.isFull());
}

void test_BoundedQueue_size() {
  BoundedQueue<int, 1> a;
  BoundedQueue<int, 2> b;
  TEST_ASSERT_EQUAL(0, a.size());
  TEST_ASSERT_EQUAL(0, b.size());

  a.push(10);
  b.push(20);
  TEST_ASSERT_EQUAL(1, a.size());
  TEST_ASSERT_EQUAL(1, b.size());

  b.push(21);
  TEST_ASSERT_EQUAL(2, b.size());
}
