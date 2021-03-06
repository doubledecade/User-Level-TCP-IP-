/*! 
    Simple unit testing for c/c++
    Copyright Cosmin Cremarenco.
    Licence: Apache 2.0

    Purpose: facilitate the unit testing for programs written in c/c++

    Use:
    Define your tests as functions that don't take any arguments
    but return "int". Then add them to a test suite. Finally,
    if all you want to run are the tests inside the same .c/.cpp translation
    unit add call to TINYTEST_MAIN_SINGLE_SUITE which takes as argument
    the name of the test suite to run.
    Otherwise, if you wish to gather test suites across translation units
    you need to declare your test suites using TINYTEST_DECLARE_SUITE
    and include the .h with the declarations inside a .c/.cpp file
    which will call TINYTEST_START_MAIN/TINYTEST_END_MAIN in which 
    you will put all test suites.

    Please see examples for more details.

    Tests are meant to be in the same place as the tested code.
    Declare TINYTEST_NOTEST if you don't want to compile the test code.
    Usually you will declare TINYTEST_NOTEST together with NDEBUG.
 */

#ifndef __TINYTEST_H__
#define __TINYTEST_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TINYTEST_FAILTURE 0
#define TINYTEST_SUCCESS 1

typedef int (*TinyTestFunc)(void);

typedef struct TinyTestStruct
{
  TinyTestFunc m_func;
  const char* m_name;
  struct TinyTestStruct* m_next;
} TinyTest;

typedef void(TinyTestVoidCall)(void);
typedef struct TinyTestSuiteStruct
{
  TinyTestVoidCall* __attribute__((unused))startUp;
  TinyTestVoidCall* __attribute__((unused))tearDown;
  struct TinyTestStruct* m_head;
  const char* m_name;
  struct TinyTestStruct* m_headTest;
  struct TinyTestSuiteStruct* m_next;
} TinyTestSuite;


typedef struct TinyTestRegistryStruct
{
  TinyTestSuite* m_headSuite;
} TinyTestRegistry;

#ifndef TINYTEST_NOTESTING

#define TINYTEST_EQUAL_MSG(expected, actual, msg)                       \
  if ( (expected) != (actual) )                                         \
  {                                                                     \
    printf("%s:%d expected %d, actual: %d\n",                           \
           __FILE__, __LINE__, expected, actual);                     \
    if ( msg ) printf(msg);                                             \
    return TINYTEST_FAILTURE;                                           \
  }

#define TINYTEST_EQUAL(expected, actual)                                \
  TINYTEST_EQUAL_MSG(expected, actual, " ")

#define TINYTEST_INT_LTE_MSG(expected, actual, msg)                       \
  if ( (expected) <= (actual) )                                          \
  {                                                                     \
    printf("%s:%d expected %s, actual: %ld\n",                           \
           __FILE__, __LINE__, #expected, actual);                     \
    if ( msg ) printf(msg);                                             \
    return TINYTEST_FAILTURE;                                           \
  }

#define TINYTEST_INT_LTE(expected, actual)                                \
  TINYTEST_INT_LTE_MSG(expected, actual, " ")

#define TINYTEST_INT_GT_MSG(expected, actual, msg)                       \
  if ( (expected) > (actual) )                                          \
  {                                                                     \
    printf("%s:%d expected %s, actual: %ld\n",                           \
           __FILE__, __LINE__, #expected, actual);                     \
    if ( msg ) printf(msg);                                             \
    return TINYTEST_FAILTURE;                                           \
  }

#define TINYTEST_INT_GT(expected, actual)                                \
  TINYTEST_INT_GT_MSG(expected, actual, " ")


#define TINYTEST_STR_EQUAL_MSG(expected, actual, msg)                   \
  if ( strcmp((expected), (actual)) )                                   \
  {                                                                     \
    printf("%s:%d expected \"%s\", actual: \"%s\"\n",                   \
           __FILE__, __LINE__, expected, actual);                       \
    if(msg)  printf(msg);                                               \
    return TINYTEST_FAILTURE;                                           \
  }



#define TINYTEST_STR_EQUAL(expected, actual)                            \
  TINYTEST_STR_EQUAL_MSG(expected, actual, " ")

#define TINYTEST_ASSERT_MSG(assertion, msg)                             \
  if ( !(assertion) )                                                   \
  {                                                                     \
    printf("%s:%d assertion failed: \"%s\"\n",                          \
           __FILE__, __LINE__, #assertion);                             \
    if ( msg ) printf(msg);                                             \
    return TINYTEST_FAILTURE;                                           \
  }

#define TINYTEST_ASSERT(assertion)                                      \
  TINYTEST_ASSERT_MSG(assertion, NULL)

#define TINYTEST_DECLARE_SUITE(suiteName)                               \
  void Suite##suiteName(TinyTestRegistry* registry)

#define TINYTEST_START_SUITE(suiteName)                                 \
void Suite##suiteName(TinyTestRegistry* registry)                       \
{                                                                       \
  TinyTestSuite* suite = (TinyTestSuite*)malloc(sizeof(TinyTestSuite)); \
  suite->startUp= NULL;             \
  suite->tearDown= NULL;             \
  suite->m_head = NULL;             \
  suite->m_name = #suiteName;                                           \
  suite->m_headTest = NULL;                                             \
  suite->m_next = NULL

#define TINYTEST_SET_STARTUP(xstartUp)                                   \
    suite->startUp=xstartUp

#define TINYTEST_SET_TEARDOWN(xtearDown)                                 \
    suite->tearDown=xtearDown
  
#define TINYTEST_ADD_TEST(test)                                         \
  TinyTest* test##decl = (TinyTest*)malloc(sizeof(TinyTest));           \
  test##decl->m_func = test;                                            \
  test##decl->m_name = #test;                                           \
  test##decl->m_next = suite->m_headTest;                               \
  suite->m_headTest = test##decl         

#define TINYTEST_END_SUITE()                                            \
  suite->m_next = registry->m_headSuite;                                \
  registry->m_headSuite = suite;                                        \
}

#define TINYTEST_START_MAIN()                                           \
  int main(__attribute__((unused)) int argc, __attribute__((unused))char* argv[])       \
  {                                                                     \
    TinyTestRegistry registry;                                          \
    registry.m_headSuite = NULL;                                        \
    int anyfailed = 0

#define TINYTEST_RUN_SUITE(suiteName)                                   \
  Suite##suiteName(&registry) 

#define TINYTEST_INTERNAL_FREE_TESTS()                                  \
  {                                                                     \
    TinyTestSuite* s = registry.m_headSuite;                            \
    TinyTestSuite* ss = NULL;                                           \
    for ( ; s; s = ss )                                                 \
    {                                                                   \
      ss = s->m_next;                                                   \
      TinyTest* t = s->m_headTest;                                      \
      TinyTest* tt = NULL;                                              \
      for ( ; t; t = tt )                                               \
      {                                                                 \
        tt = t->m_next;                                                 \
        free(t);                                                        \
      }                                                                 \
      free(s);                                                          \
    }                                                                   \
  }

#define TINYTEST_INTERNAL_RUN_TESTS()                                   \
  {                                                                     \
    int okTests = 0;                                                    \
    int failedTests = 0;                                                \
    TinyTestSuite* s = registry.m_headSuite;                            \
    for ( ; s; s = s->m_next )                                          \
    {                                                                   \
        if(s->startUp != NULL)                                          \
        {                                                               \
            s->startUp();                                               \
        }                                                               \
      TinyTest* t = s->m_headTest;                                      \
      for ( ; t; t = t->m_next )                                        \
      {                                                                 \
        if ( (*t->m_func)() )                                           \
        {                                                               \
          printf(".");                                                  \
          ++okTests;                                                    \
        }                                                               \
        else                                                            \
        {                                                               \
          printf("x");                                                  \
          ++failedTests;                                                \
          anyfailed = 1;                                                \
        }                                                               \
      }                                                                 \
      if(s->tearDown)                                                   \
      {                                                                 \
          s->tearDown();                                                \
      }                                                                 \
    }                                                                   \
    printf("\nOK: %d", okTests);                                        \
    if ( failedTests )                                                  \
    {                                                                   \
      printf(" FAILED: %d", failedTests);                               \
    }                                                                   \
    printf("\n");                                                       \
  }


#define TINYTEST_END_MAIN()                                             \
    TINYTEST_INTERNAL_RUN_TESTS();                                      \
    printf("\n");                                                       \
    TINYTEST_INTERNAL_FREE_TESTS()                                      \
    if(anyfailed != 0)  {                                               \
        return -1;                                                      \
    }                                                                   \
    return 0;                                                           \
  }

#define TINYTEST_MAIN_SINGLE_SUITE(suiteName)                           \
  TINYTEST_START_MAIN();                                                \
  TINYTEST_RUN_SUITE(suiteName);                                        \
  TINYTEST_END_MAIN();

#else // TINYTEST_NOTESTING
#define TINYTEST_EQUAL_MSG(expected, actual, msg) (void)0 
#define TINYTEST_EQUAL(expected, actual) (void)0 
#define TINYTEST_STR_EQUAL_MSG(expected, actual, msg) (void)0
#define TINYTEST_STR_EQUAL(expected, actual) (void)0
#define TINYTEST_ASSERT_MSG(assertion, msg) (void)0
#define TINYTEST_ASSERT(assertion) (void)0

#define TINYTEST_START_SUITE(suiteName)
#define TINYTEST_ADD_TEST(test)
#define TINYTEST_END_SUITE()
#define TINYTEST_START_MAIN()
#define TINYTEST_RUN_SUITE(suiteName)
#define TINYTEST_INTERNAL_FREE_TESTS()
#define TINYTEST_INTERNAL_RUN_TESTS()
#define TINYTEST_END_MAIN()
#define TINYTEST_MAIN_SINGLE_SUITE(suiteName)
#endif // TINYTEST_NOTESTING

#endif

