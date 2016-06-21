#ifndef PULSAR_GUARD_MATH__MINMAX_HPP_
#define PULSAR_GUARD_MATH__MINMAX_HPP_

#include <algorithm>

namespace pulsar{
namespace math {


template<typename T>
T min(T v)
{
    return v;
}

template<typename T>
T max(T v)
{
    return v;
}

template<typename T>
T min_abs(T v)
{
    return std::abs(v);
}

template<typename T>
T max_abs(T v)
{
    return std::abs(v);
}


template<typename T, typename ... Targs>
T min(T v, Targs && ... args)
{
    return std::min(v, min(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T max(T v, Targs && ... args)
{
    return std::max(v, max(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T min_abs(T v, Targs && ... args)
{
    return std::min(std::abs(v), min_abs(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T max_abs(T v, Targs && ... args)
{
    return std::max(std::abs(v), max_abs(std::forward<Targs>(args)...));
}


} // close namespace math
} // close namespace pulsar



#endif
