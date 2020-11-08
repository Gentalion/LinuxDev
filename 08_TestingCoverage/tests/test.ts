#include "buf.h"

#test init
    float *a = 0;
    ck_assert_msg(buf_capacity(a) == 0, "capacity init");
    ck_assert_msg(buf_size(a) == 0, "size init");
    buf_push(a, 1.3f);
    ck_assert_msg(buf_size(a) == 1, "size 1");
    ck_assert_msg(a[0] == (float)1.3f, "value");
    buf_clear(a);
    ck_assert_msg(buf_size(a) == 0, "clear");
    ck_assert_msg(a != 0, "clear not-free");
    buf_free(a);
    ck_assert_msg(a == 0, "free");


#test clear
    float *a = 0;
    buf_clear(a);
    ck_assert_msg(buf_size(a) == 0, "clear empty");
    ck_assert_msg(a == 0, "clear no-op");


#test push_at
    long *ai = 0;
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
    ck_assert_msg(buf_size(ai) == 10000, "size 10000");
    int match = 0;
    for (int i = 0; i < (int)(buf_size(ai)); i++)
        match += ai[i] == i;
    
    ck_assert_msg(match == 10000, "match 10000");
    buf_free(ai);


#test grow_trunk
    long *ai = 0;
    buf_grow(ai, 1000);
    ck_assert_msg(buf_capacity(ai) == 1000, "grow 1000");
    ck_assert_msg(buf_size(ai) == 0, "size 0 (grow)");
    buf_trunc(ai, 100);
    ck_assert_msg(buf_capacity(ai) == 100, "trunc 100");
    buf_free(ai);


#test pop
    float *a = 0;
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
    ck_assert_msg(buf_size(a) == 4, "size 4");
    ck_assert_msg(buf_pop(a) == (float)1.4f, "pop 3");
    buf_trunc(a, 3);
    ck_assert_msg(buf_size(a) == 3, "size 3");
    ck_assert_msg(buf_pop(a) == (float)1.3f, "pop 2");
    ck_assert_msg(buf_pop(a) == (float)1.2f, "pop 1");
    ck_assert_msg(buf_pop(a) == (float)1.1f, "pop 0");
    ck_assert_msg(buf_size(a) == 0, "size 0 (pop)");
buf_free(a);