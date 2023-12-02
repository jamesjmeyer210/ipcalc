//
// Created by james on 9/28/23.
//

#ifndef IPCALC_TEST_H
#define IPCALC_TEST_H

#define ASSERT(expr)\
if(expr)\
{\
  printf("PASS:\t%s: line: %d: %s\n", __FUNCTION__, __LINE__, #expr); \
}\
else{\
  printf("FAIL:\t%s: line: %d: %s\n", __FUNCTION__, __LINE__, #expr); \
}

#define ASSERT_EQ_size_t(A, B) \
if(A != B)                     \
{                              \
  printf("FAIL:\t%s.%d.%s: Expected %zu but got %zu\n", __FILE__, __LINE__, __FUNCTION__, A, B);\
}                              \
else                           \
{                              \
  printf("PASS:\t%s.%d.%s\n", __FILE__, __LINE__, __FUNCTION__);\
}

#define ASSERT_EQ(T, A, B) ASSERT_EQ_##T(A, B)

#endif //IPCALC_TEST_H
