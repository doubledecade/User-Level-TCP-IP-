//
// Created by MHP on 2021/12/14.
//

#include "tinytest.h"
#include "tuntap.h"
int test_tuntap()
{
    tun_init("test1","/dev/net/tap","10.0.0.5","10.0.0.0/24");
    char buf[BUFSIZ];
    while (tun_read(buf,BUFSIZ)>0)
    {
        printf("-->%s\n",buf);
    }


//    TINYTEST_ASSERT(n!=-1);
    return TINYTEST_SUCCESS;
}
TINYTEST_START_SUITE(TEST_TUNTAP);
TINYTEST_ADD_TEST(test_tuntap);
TINYTEST_END_SUITE();

TINYTEST_START_MAIN();
    TINYTEST_RUN_SUITE(TEST_TUNTAP);
TINYTEST_END_MAIN();