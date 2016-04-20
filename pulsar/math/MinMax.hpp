#ifndef PULSAR_GUARD_MATH__MINMAX_HPP_
#define PULSAR_GUARD_MATH__MINMAX_HPP_

#include <algorithm>

namespace pulsar{
namespace math {


template<typename T>
T Min(T v)
{
    return v;
}

template<typename T>
T Max(T v)
{
    return v;
}

template<typename T>
T MinAbs(T v)
{
    return std::abs(v);
}

template<typename T>
T MaxAbs(T v)
{
    return std::abs(v);
}


template<typename T, typename ... Targs>
T Min(T v, Targs && ... args)
{
    return std::min(v, Min(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T Max(T v, Targs && ... args)
{
    return std::max(v, Max(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T MinAbs(T v, Targs && ... args)
{
    return std::min(std::abs(v), MinAbs(std::forward<Targs>(args)...));
}


template<typename T, typename ... Targs>
T MaxAbs(T v, Targs && ... args)
{
    return std::max(std::abs(v), MaxAbs(std::forward<Targs>(args)...));
}


} // close namespace math
} // close namespace pulsar



#endif
