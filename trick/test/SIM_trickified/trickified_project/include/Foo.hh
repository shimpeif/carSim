// @trick_parse{everything}

#include <iostream>

class Foo {

    public:

    int i;

    void foo() {
        std::cout << i++ << '\n';
    }

};
