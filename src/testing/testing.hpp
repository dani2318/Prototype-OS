#pragma once

#define _assert(condition, message){ if (!condition) {return;}}
#define _assert_equals(expected, actual, message)
#define _assert_not_equals(expected, actual, message)

#define TEST(test_function)

int test_function(){
  return -1;
}
