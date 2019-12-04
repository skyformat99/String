#ifndef CORE_H
#define CORE_H

template<typename _T>
struct HexFormat
{
	template<typename _ArgT>
	constexpr explicit HexFormat(const _ArgT& t) : data(t)
	{
	}
	const _T data;
};

template<typename _T>
struct OctalFormat
{
	template<typename _ArgT>
	constexpr explicit OctalFormat(const _ArgT& t) : data(t)
	{
	}
	const _T data;
};

struct PointerFormat
{
	template<typename _ArgT>
	constexpr explicit PointerFormat(const _ArgT* t) : data(reinterpret_cast<const void*>(t))
	{
	}
	const void* data;
};


#include <cstdio>
#include <cstdlib>
#define FATAL_ERROR_IF(x, msg)                                                           \
	if (x)                                                                               \
	{                                                                                    \
		fprintf(stderr, "FATAL ERROR: %s: %s\n", __PRETTY_FUNCTION__, msg);              \
		abort();                                                                         \
	}

#define LOG(x) _log(__FILE__, __LINE__, __PRETTY_FUNCTION__, x, #x)

#include <iostream>
template<typename _T>
inline void _log(const char* file, unsigned long line, const char* function,
				 const _T& arg, const char* arg_name)
{
	fprintf(stdout, "LOG: %s:%lu (%s): %s = ", file, line, function, arg_name);
	std::cout << arg << std::endl;
}

#define DEBUGMODE 1
#if DEBUGMODE
#define ASSERT(cond) _assert((cond), __FILE__, __PRETTY_FUNCTION__, __LINE__, #cond)

inline void _assert(bool result, const char* file, const char* function, unsigned line,
					const char* condition_str)
{
	if (!result)
	{
		fprintf(stderr,
				"(( ASSERTION FAILED at %s:%u in `%s`. Failed condition: `%s`. ))\n",
				file, line, function, condition_str);
		abort();
	}
}

#include <cctype>
#include <cstring>

template<typename _Ptr>
inline void _print_memory(_Ptr from_arg, _Ptr to_arg, const char* what = "no info",
						  std::size_t block_size = 16)
{
	char* from = static_cast<char*>((void*)from_arg);
	char* to   = static_cast<char*>((void*)to_arg);

	printf("\nMEMORY DUMP FROM %p TO %p [SIZE=0x%x] (%s)\n", static_cast<void*>(from),
		   static_cast<void*>(to), unsigned(to - from), what);

	for (int i = -int(block_size); i < (to + block_size) - from; i += block_size)
	{
		printf("OFF  ");
		for (auto iter = from + i; iter != from + i + block_size; ++iter)
		{
			if (iter - from < 0)
				printf("-%0.3lx ", -(iter - from));
			else
				printf("%0.4lx ", iter - from);
		}
		printf("\n");

		printf("POS  ");
		for (auto iter = from + i; iter != from + i + block_size; ++iter)
		{
			printf("%0.4lx ", reinterpret_cast<unsigned long>(iter) & 0xFFFF);
		}
		printf("\n");

		printf("HEX  ");
		for (auto iter = from + i; iter != from + i + block_size; ++iter)
		{
			printf("%0.4x ", *(iter)&0xFFFF);
		}
		printf("\n");

		printf("CHR  ");
		for (auto iter = from + i; iter != from + i + block_size; ++iter)
		{
			if (isprint(*iter))
				printf(" '%c' ", *iter);
			else if (*iter == 0)
				printf("NULL ");
			else
				printf("???? ");
		}
		printf("\n");

		printf("DAT  ");

		for (auto iter = from + i; iter != from + i + block_size; ++iter)
		{
			if (iter == from)
				printf("[----");
			else if (iter == to - 1)
				printf("----]");
			else if (iter >= from && iter <= to - 1)
				printf("-----");
			else
				printf("     ");
		}

		// end of block
		printf("\n\n");
	}

	return;
}

#else

#define ASSERT(x)

template<typename... _Args>
inline constexpr void _print_memory(_Args&&...)
{
}
#endif

#endif // CORE_H
