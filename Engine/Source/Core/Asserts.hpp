#pragma once


#ifdef QMBT_DEBUG
#define QMBT_ENABLE_ASSERTS
#define QMBT_ENABLE_VERIFICATION
#endif

#ifdef QMBT_ENABLE_ASSERTS

#define QMBT_ASSERT(x, ...)                                  \
	{                                                        \
		if (!(x))                                            \
		{                                                    \
			LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			debug_break();                                   \
		}                                                    \
	}
#define QMBT_CORE_ASSERT(x, ...)                                  \
	{                                                             \
		if (!(x))                                                 \
		{                                                         \
			LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			debug_break();                                        \
		}                                                         \
	}

#define QMBT_BARE_ASSERT(x, ...)                                  \
	{                                                        \
		if (!(x))                                            \
		{                                                    \
			debug_break();                                   \
		}                                                    \
	}
#define QMBT_CORE_BARE_ASSERT(x, ...                                  \
	{                                                             \
		if (!(x))                                                 \
		{                                                         \
			debug_break();                                        \
		}                                                         \
	}
#else
#define QMBT_ASSERT(x, ...)
#define QMBT_CORE_ASSERT(x, ...)
#define QMBT_BARE_ASSERT(x, ...)
#define QMBT_CORE_BARE_ASSERT(x, ...)
#endif //  QMBT_ENABLE_ASSERTS

#ifdef QMBT_ENABLE_VERIFICATION

#define QMBT_VERIFY(x, ...)                                     \
	{                                                           \
		if (!(x))                                               \
		{                                                       \
			LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
			debug_break();                                      \
		}                                                       \
	}
#define QMBT_CORE_VERIFY(x, ...)                                     \
	{                                                                \
		if (!(x))                                                    \
		{                                                            \
			LOG_CORE_ERROR("Verification Failed: {0}", __VA_ARGS__); \
			debug_break();                                           \
		}                                                            \
	}
#else
#define QMBT_VERIFY(x, ...) x
#define QMBT_CORE_VERIFY(x, ...) x
#endif //  QMBT_ENABLE_ASSERTS

