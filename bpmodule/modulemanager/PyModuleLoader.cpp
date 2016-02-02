



class PyModuleLoader
{

public:
    LoadPyModule(pybind11::object mod, const ModuleInfo & minfo)
    {
        if(!objmap_.count(minfo.path))
        {
            if(!python::HasCallableAttr(mod, "InsertSupermodule"))
                throw exception

            if(python::HasCallableAttr(mod, "InitializeSupermodule"))
                CallPyFuncAttr(mod, "InitializeSupermodule");

            creators = python::CallPyFuncAttr(mod, "InsertSupermodule");
            
            PyModInfo pyinfo{obj, std::move(creators)};
        }
        
        Assert<

    }


private:

    struct PyModInfo
    {
        pybind11::object obj;
        ModuleCreationFuncs creators; 
    };

    std::map<std::string, PyModInfo> objmap_;
};
