#ifndef _GUARD_HASHABLE_HPP_
#define _GUARD_HASHABLE_HPP_

#include <string>

class Hashable
{
    public:
        virtual std::string Hash(void) const = 0;
};

#endif
