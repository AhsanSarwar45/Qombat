#pragma once

#define NON_COPYABLE(Type)      \
	Type(const Type&) = delete; \
	Type& operator=(const Type&) = delete

#define BIND_EVENT_FUNCTION(function)                                 \
	[this](auto&&... args) -> decltype(auto) {                        \
		return this->function(std::forward<decltype(args)>(args)...); \
	}

#define GET_MACRO_1(_0, _1, NAME, ...) NAME
#define GET_MACRO_2(_1, _2, NAME, ...) NAME
#define GET_MACRO_3(_1, _2, _3, NAME, ...) NAME
#define GET_MACRO_4(_1, _2, _3, _4, NAME, ...) NAME
