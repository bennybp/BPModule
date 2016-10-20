Exceptions and Error Handling                                   {#except}
============================= 

Pulsar contains several custom classes for raising exceptions and asserting that
conditions are true.  The main exception class in Pulsar is the GeneralException
class, which as the name implies is for a general exception.  All other
exceptions are specializations of this class to specific, common, errors.  They
are:
- NotYetImplementedException: Prevents code flow into a path that is in progress
    or is planned, but does not yet work
- ValueOutOfRange: The requested value is not within (ironically) the domain of
    the function
- BasisSetException: The problem comes from how the basis set is set up or the
    fact that the basis set can't be set up
- DataStoreException: Either the OptionMap, Wavefunction, or Cache has
    encountered a problem
- MathException 
- ModuleCreateException 
- ModuleManagerException
- ModuleLoadException 
- SystemException 
- SerializationException 
- OptionException 
- PythonCallException
- PythonConvertException
