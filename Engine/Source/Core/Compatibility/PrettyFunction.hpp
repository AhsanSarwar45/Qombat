#ifdef QMBT_DEBUG
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!

// GCC / Metroworks / Intel C/C++ / Green Hill C/C++
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define QMBT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810) // Digital Mars
#define QMBT_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER)) // Microsoft Visual C++
#define QMBT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500)) // Intel C/C++ / IBM Compilers
#define QMBT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550) // Borland C++
#define QMBT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901) // C Standard is >= C99(199901)
#define QMBT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103) // C++ Standard is >= C++11(201103)
#define QMBT_FUNC_SIG __func__
#else
#define QMBT_FUNC_SIG "QMBT_FUNC_SIG unknown!"
#endif

#endif