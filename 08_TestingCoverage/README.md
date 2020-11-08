This is rework of https://github.com/skeeto/growable-buf in educational purposes to learn autotool+check

To run test do following:
1. autoreconf -fisv
2. ./configure
3. make
4. make check

To run it with gcov do following:
1. autoreconf -fisv
2. ./configure --enable-gcov
3. make
4. make check
5. gcov src/.libs/buf

If you want to clean all generates use "make gitclean"