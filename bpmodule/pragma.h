#ifndef _GUARD_PRAGMA_H_
#define _GUARD_PRAGMA_H_

#ifdef __cplusplus
extern "C" {
#endif



#if defined(__ICC) || defined(__INTEL_COMPILER)

    // pragmas for Intel
    #define PRAGMA_WARNING_POP                                 _Pragma("warning(pop)")
    #define PRAGMA_WARNING_PUSH                                _Pragma("warning(push)")
    #define PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS            _Pragma("warning(disable:869)")
    #define PRAGMA_WARNING_IGNORE_FP_EQUALITY                  _Pragma("warning(disable:1572)")
    #define PRAGMA_WARNING_IGNORE_FP_CONVERT                   _Pragma("warning(disable:264)") 
    #define PRAGMA_WARNING_IGNORE_SWITCH_MISSING_DEFAULT       _Pragma("warning(disable:2338)")
    #define PRAGMA_WARNING_IGNORE_POINTLESS_COMPARISON_UINT_0  _Pragma("warning(disable:186)")
    #define PRAGMA_WARNING_IGNORE_STATEMENT_UNREACHABLE        _Pragma("warning(disable:111)")

#elif defined(__GNUC__) || defined(__GNUG__)

    // pragmas for GCC
    #define PRAGMA_WARNING_PUSH                                _Pragma("GCC diagnostic push")
    #define PRAGMA_WARNING_POP                                 _Pragma("GCC diagnostic pop")
    #define PRAGMA_WARNING_IGNORE_UNUSED_PARAMETERS            _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
    #define PRAGMA_WARNING_IGNORE_FP_EQUALITY                  _Pragma("GCC diagnostic ignored \"-Wfloat-equal\"") 
    #define PRAGMA_WARNING_IGNORE_FP_CONVERT                   _Pragma("GCC diagnostic ignored \"-Wfloat-conversion\"") 
    #define PRAGMA_WARNING_IGNORE_SWITCH_MISSING_DEFAULT       _Pragma("GCC diagnostic ignored \"-Wswitch-default\"")
    #define PRAGMA_WARNING_IGNORE_POINTLESS_COMPARISON_UINT_0  _Pragma("GCC diagnostic ignored \"-Wtype-limits\"")

    //! \todo find these?
    #define PRAGMA_WARNING_IGNORE_STATEMENT_UNREACHABLE       

#endif






#ifdef __cplusplus
}
#endif

#endif