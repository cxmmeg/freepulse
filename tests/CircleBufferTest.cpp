#include "gtest/gtest.h"
#include "CircleBuffer.h"

namespace {

// The fixture for testing class Foo.
class CircleBufferTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  CircleBufferTest() {
    // You can do set-up work for each test here.
  }

  virtual ~CircleBufferTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

int mean(CircleBuffer<int> vals, int window_size) {
    int running_sum = 0;
    for (int i = 0; i < window_size; i++) {
        running_sum += vals[i];
    }
    return running_sum / window_size;
}

// Tests that the Foo::Bar() method does Abc.
TEST_F(CircleBufferTest, SimpleAdd) {
	CircleBuffer<int> a = CircleBuffer<int>();
	int payload = 1;
	a.add(payload);
	EXPECT_EQ(a[0], payload);
	EXPECT_EQ(a[0], a.newest());
}

TEST_F(CircleBufferTest, ModBehavior) {
	EXPECT_EQ(CircleBuffer<int>::mod(4,5), 4);
	EXPECT_EQ(CircleBuffer<int>::mod(5,4), 1);
	EXPECT_EQ(CircleBuffer<int>::mod(10,4), 2);
	EXPECT_EQ(CircleBuffer<int>::mod(-1,4), 3);
	EXPECT_EQ(CircleBuffer<int>::mod(-3,4), 1);
}

TEST_F(CircleBufferTest, AddLengthPlusOneElements) {
	CircleBuffer<int> a = CircleBuffer<int>(5);
	for (int i = 0; i < 6; i ++) {
		a.add(i);
	}
	EXPECT_EQ(a[0], 5);
	EXPECT_EQ(a.newest(), 5);
	EXPECT_EQ(a[4], 1);
}

TEST_F(CircleBufferTest, AddLotsOfElements) {
	CircleBuffer<int> a = CircleBuffer<int>(5);
	int k = 1005;
	for (int i = 0; i <= k; i ++) {
		a.add(i);
	}
	EXPECT_EQ(a[0], k);
	EXPECT_EQ(a.newest(), k);
	EXPECT_EQ(a[4], k-4);
}

TEST_F(CircleBufferTest, TakeMovingAverageOfElements) {
    CircleBuffer<int> a = CircleBuffer<int>(10);
    int mean_window = 5;
    for (int i = 0; i < 100; i++) {
        a.add(i);
        int running_sum = 0;
        for (int j = 0; j < 5; j++) {
            running_sum += i - j < 0 ? 0 : i - j;
        }
        int expected_mean = running_sum / mean_window;
        EXPECT_EQ(mean(a, mean_window), expected_mean);
    }
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
