/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropHolder.hpp"
#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/python_helper/Convert.hpp"


using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace datastore {
namespace detail {

template<typename T>
PropHolder<T>::PropHolder(T && m)
  : obj(std::move(m))
{ }


template<typename T>
PropHolder<T>::PropHolder(const T & m)
  : obj(m)
{ }


template<typename T>
T & PropHolder<T>::GetRef(void) noexcept
{
    return obj;
}

template<typename T>
const T & PropHolder<T>::GetRef(void) const noexcept
{
    return obj;
}


template<typename T>
boost::python::object PropHolder<T>::GetPy(void) const
{
    return ConvertToPy(obj);
}








PropPlaceholderPtr PropHolderFactory(const boost::python::object & obj)
{
    std::string cl = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
    if(cl == "bool")
        return PropPlaceholderPtr(new PropHolder<bool>(ConvertToCpp<bool>(obj)));
    else if(cl == "int")
        return PropPlaceholderPtr(new PropHolder<long>(ConvertToCpp<long>(obj)));
    else if(cl == "float")
        return PropPlaceholderPtr(new PropHolder<double>(ConvertToCpp<double>(obj)));
    else if(cl == "str")
        return PropPlaceholderPtr(new PropHolder<std::string>(ConvertToCpp<std::string>(obj)));
    else if(cl == "list")
    {
        // get type of first element
        boost::python::list lst = boost::python::extract<boost::python::list>(obj);
        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
            throw GeneralException("Empty list passed from python",
                                   "location", "PropertyMap");

        std::string cl2 = boost::python::extract<std::string>(lst[0].attr("__class__").attr("__name__"));

        // check if this is a homogeneous container
        for(int i = 1; i < length; i++)
        {
            std::string cltmp = boost::python::extract<std::string>(lst[i].attr("__class__").attr("__name__"));
            if(cl2 != cltmp)
                throw GeneralException("Cannot convert heterogeneous python list",
                                       "type1", cl2, "type2", cltmp, "element", i);
        }

        // now parse list
        if(cl2 == "bool")
            return PropPlaceholderPtr(new PropHolder<std::vector<bool>>(ConvertToCpp<std::vector<bool>>(lst)));
        if(cl2 == "int")
            return PropPlaceholderPtr(new PropHolder<std::vector<long>>(ConvertToCpp<std::vector<long>>(lst)));
        else if(cl2 == "float")
            return PropPlaceholderPtr(new PropHolder<std::vector<double>>(ConvertToCpp<std::vector<double>>(lst)));
        else if(cl2 == "str")
            return PropPlaceholderPtr(new PropHolder<std::vector<std::string>>(ConvertToCpp<std::vector<std::string>>(lst)));
        else
        {
            throw GeneralException("Invalid type to convert from python",
                                   "location", "PropertyMap", "type", cl2,
                                   "detail", "In converting a list"); 
        }
    }
    else
    {
        throw GeneralException("Invalid type to convert from python",
                               "location", "PropertyMap", "type", cl);
    }
}



///////////////////////////////////
///////////////////////////////////
// Valid types for PropHolder
///////////////////////////////////
///////////////////////////////////
template class PropHolder<bool>;
template class PropHolder<int>;
template class PropHolder<long>;
template class PropHolder<float>;
template class PropHolder<double>;
template class PropHolder<std::string>;

template class PropHolder<std::vector<bool>>;
template class PropHolder<std::vector<int>>;
template class PropHolder<std::vector<long>>;
template class PropHolder<std::vector<float>>;
template class PropHolder<std::vector<double>>;
template class PropHolder<std::vector<std::string>>;
///////////////////////////////////
///////////////////////////////////
// End types for PropHolder
///////////////////////////////////
///////////////////////////////////


} // close namespace detail
} // close namespace datastore
} // close namespace bpmodule

