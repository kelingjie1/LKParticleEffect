//
// Created by willisdai on 2018/3/9.
//

#ifndef PARTICLESYSTEM_PRINTABLE_H
#define PARTICLESYSTEM_PRINTABLE_H

#include <string>


namespace LKKit {
    using namespace std;
    class Printable {
    public:
        virtual string to_string() = 0;
    };
}


#endif //PARTICLESYSTEM_PRINTABLE_H
