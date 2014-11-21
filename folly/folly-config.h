#ifndef _FOLLY_CONFIG_H
#define _FOLLY_CONFIG_H 1
 
/* folly-config.h. Generated automatically at end of configure. */
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to "final" if the compiler supports C++11 "final" */
#ifndef FOLLY_FINAL
#define FOLLY_FINAL final
#endif

/* Define to gflags namespace (usually "google" or "gflags") */
#ifndef FOLLY_GFLAGS_NAMESPACE
#define FOLLY_GFLAGS_NAMESPACE google
#endif

/* Define to 1 if you have the <bits/c++config.h> header file. */
#ifndef FOLLY_HAVE_BITS_C__CONFIG_H
#define FOLLY_HAVE_BITS_C__CONFIG_H 1
#endif

/* Define to 1 if you have the <bits/functexcept.h> header file. */
#ifndef FOLLY_HAVE_BITS_FUNCTEXCEPT_H
#define FOLLY_HAVE_BITS_FUNCTEXCEPT_H 1
#endif

/* define if the Boost library is available */
#ifndef FOLLY_HAVE_BOOST
#define FOLLY_HAVE_BOOST /**/
#endif

/* define if the Boost::Filesystem library is available */
#ifndef FOLLY_HAVE_BOOST_FILESYSTEM
#define FOLLY_HAVE_BOOST_FILESYSTEM /**/
#endif

/* define if the Boost::Regex library is available */
#ifndef FOLLY_HAVE_BOOST_REGEX
#define FOLLY_HAVE_BOOST_REGEX /**/
#endif

/* define if the Boost::System library is available */
#ifndef FOLLY_HAVE_BOOST_SYSTEM
#define FOLLY_HAVE_BOOST_SYSTEM /**/
#endif

/* define if the Boost::Thread library is available */
#ifndef FOLLY_HAVE_BOOST_THREAD
#define FOLLY_HAVE_BOOST_THREAD /**/
#endif

/* Define to 1 if you have the <byteswap.h> header file. */
#ifndef FOLLY_HAVE_BYTESWAP_H
#define FOLLY_HAVE_BYTESWAP_H 1
#endif

/* Define to 1 if we support clock_gettime(2). */
#ifndef FOLLY_HAVE_CLOCK_GETTIME
#define FOLLY_HAVE_CLOCK_GETTIME 1
#endif

/* Define to 1 if strlen(3) is constexpr. */
#ifndef FOLLY_HAVE_CONSTEXPR_STRLEN
#define FOLLY_HAVE_CONSTEXPR_STRLEN 1
#endif

/* Define to 1 if we have cplus_demangle_v3_callback. */
#ifndef FOLLY_HAVE_CPLUS_DEMANGLE_V3_CALLBACK
#define FOLLY_HAVE_CPLUS_DEMANGLE_V3_CALLBACK 1
#endif

/* Define if you want to support deprecated associative containers */
/* #undef HAVE_DEPRECATED_ASSOC */

/* Define to 1 if you have the <dlfcn.h> header file. */
#ifndef FOLLY_HAVE_DLFCN_H
#define FOLLY_HAVE_DLFCN_H 1
#endif

/* Define to 1 if you have the <emmintrin.h> header file. */
#ifndef FOLLY_HAVE_EMMINTRIN_H
#define FOLLY_HAVE_EMMINTRIN_H 1
#endif

/* Define to 1 if you have the <fcntl.h> header file. */
#ifndef FOLLY_HAVE_FCNTL_H
#define FOLLY_HAVE_FCNTL_H 1
#endif

/* Define to 1 if you have the <features.h> header file. */
#ifndef FOLLY_HAVE_FEATURES_H
#define FOLLY_HAVE_FEATURES_H 1
#endif

/* Define to 1 if you have the `getdelim' function. */
#ifndef FOLLY_HAVE_GETDELIM
#define FOLLY_HAVE_GETDELIM 1
#endif

/* Define to 1 if you have the `gettimeofday' function. */
#ifndef FOLLY_HAVE_GETTIMEOFDAY
#define FOLLY_HAVE_GETTIMEOFDAY 1
#endif

/* Define to 1 if the compiler supports ifunc */
#ifndef FOLLY_HAVE_IFUNC
#define FOLLY_HAVE_IFUNC 1
#endif

/* Define if __int128 does not exist */
#ifndef FOLLY_HAVE_INT128_T
#define FOLLY_HAVE_INT128_T 1
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef FOLLY_HAVE_INTTYPES_H
#define FOLLY_HAVE_INTTYPES_H 1
#endif

/* Define to 1 if you have the `double-conversion' library
   (-ldouble-conversion). */
#ifndef FOLLY_HAVE_LIBDOUBLE_CONVERSION
#define FOLLY_HAVE_LIBDOUBLE_CONVERSION 1
#endif

/* Define to 1 if you have the `event' library (-levent). */
#ifndef FOLLY_HAVE_LIBEVENT
#define FOLLY_HAVE_LIBEVENT 1
#endif

/* Define to 1 if you have the `gflags' library (-lgflags). */
#ifndef FOLLY_HAVE_LIBGFLAGS
#define FOLLY_HAVE_LIBGFLAGS 1
#endif

/* Define to 1 if you have the `glog' library (-lglog). */
#ifndef FOLLY_HAVE_LIBGLOG
#define FOLLY_HAVE_LIBGLOG 1
#endif

/* Define to 1 if you have the `jemalloc' library (-ljemalloc). */
#ifndef FOLLY_HAVE_LIBJEMALLOC
#define FOLLY_HAVE_LIBJEMALLOC 1
#endif

/* Define to 1 if you have the `lz4' library (-llz4). */
#ifndef FOLLY_HAVE_LIBLZ4
#define FOLLY_HAVE_LIBLZ4 1
#endif

/* Define to 1 if you have the `lzma' library (-llzma). */
#ifndef FOLLY_HAVE_LIBLZMA
#define FOLLY_HAVE_LIBLZMA 1
#endif

/* Define to 1 if you have the `snappy' library (-lsnappy). */
#ifndef FOLLY_HAVE_LIBSNAPPY
#define FOLLY_HAVE_LIBSNAPPY 1
#endif

/* Define to 1 if you have the `ssl' library (-lssl). */
#ifndef FOLLY_HAVE_LIBSSL
#define FOLLY_HAVE_LIBSSL 1
#endif

/* Define to 1 if you have the `z' library (-lz). */
#ifndef FOLLY_HAVE_LIBZ
#define FOLLY_HAVE_LIBZ 1
#endif

/* Define to 1 if you have the <limits.h> header file. */
#ifndef FOLLY_HAVE_LIMITS_H
#define FOLLY_HAVE_LIMITS_H 1
#endif

/* Define to 1 if you have the <malloc.h> header file. */
#ifndef FOLLY_HAVE_MALLOC_H
#define FOLLY_HAVE_MALLOC_H 1
#endif

/* Define to 1 if you have the `malloc_size' function. */
/* #undef HAVE_MALLOC_SIZE */

/* Define to 1 if you have the `malloc_usable_size' function. */
#ifndef FOLLY_HAVE_MALLOC_USABLE_SIZE
#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1
#endif

/* Define to 1 if you have the `memmove' function. */
#ifndef FOLLY_HAVE_MEMMOVE
#define FOLLY_HAVE_MEMMOVE 1
#endif

/* Define to 1 if you have the <memory.h> header file. */
#ifndef FOLLY_HAVE_MEMORY_H
#define FOLLY_HAVE_MEMORY_H 1
#endif

/* Define to 1 if you have the `memrchr' function. */
#ifndef FOLLY_HAVE_MEMRCHR
#define FOLLY_HAVE_MEMRCHR 1
#endif

/* Define to 1 if you have the `memset' function. */
#ifndef FOLLY_HAVE_MEMSET
#define FOLLY_HAVE_MEMSET 1
#endif

/* Define to 1 if you have the <mutex.h> header file. */
/* #undef HAVE_MUTEX_H */

/* Define to 1 if you have the `pipe2' function. */
#ifndef FOLLY_HAVE_PIPE2
#define FOLLY_HAVE_PIPE2 1
#endif

/* Define to 1 if you have the `pow' function. */
#ifndef FOLLY_HAVE_POW
#define FOLLY_HAVE_POW 1
#endif

/* Define to 1 if you have the `pthread_yield' function. */
/* #undef HAVE_PTHREAD_YIELD */

/* Define to 1 if the system has the type `ptrdiff_t'. */
#ifndef FOLLY_HAVE_PTRDIFF_T
#define FOLLY_HAVE_PTRDIFF_T 1
#endif

/* Define to 1 if you have the <sched.h> header file. */
#ifndef FOLLY_HAVE_SCHED_H
#define FOLLY_HAVE_SCHED_H 1
#endif

/* Define to 1 if you have the `sched_yield' function. */
#ifndef FOLLY_HAVE_SCHED_YIELD
#define FOLLY_HAVE_SCHED_YIELD 1
#endif

/* Define to 1 if stdbool.h conforms to C99. */
#ifndef FOLLY_HAVE_STDBOOL_H
#define FOLLY_HAVE_STDBOOL_H 1
#endif

/* Define if g++ supports C++0x features. */
#ifndef FOLLY_HAVE_STDCXX_0X
#define FOLLY_HAVE_STDCXX_0X /**/
#endif

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef FOLLY_HAVE_STDINT_H
#define FOLLY_HAVE_STDINT_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef FOLLY_HAVE_STDLIB_H
#define FOLLY_HAVE_STDLIB_H 1
#endif

/* Define to 1 if we have a usable std::is_trivially_copyable<T>
   implementation. */
/* #undef HAVE_STD__IS_TRIVIALLY_COPYABLE */

/* Define to 1 if std::this_thread::sleep_for() is defined. */
#ifndef FOLLY_HAVE_STD__THIS_THREAD__SLEEP_FOR
#define FOLLY_HAVE_STD__THIS_THREAD__SLEEP_FOR 1
#endif

/* Define to 1 if you have the `strerror' function. */
#ifndef FOLLY_HAVE_STRERROR
#define FOLLY_HAVE_STRERROR 1
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef FOLLY_HAVE_STRINGS_H
#define FOLLY_HAVE_STRINGS_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef FOLLY_HAVE_STRING_H
#define FOLLY_HAVE_STRING_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef FOLLY_HAVE_SYS_STAT_H
#define FOLLY_HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/time.h> header file. */
#ifndef FOLLY_HAVE_SYS_TIME_H
#define FOLLY_HAVE_SYS_TIME_H 1
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef FOLLY_HAVE_SYS_TYPES_H
#define FOLLY_HAVE_SYS_TYPES_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef FOLLY_HAVE_UNISTD_H
#define FOLLY_HAVE_UNISTD_H 1
#endif

/* Define to 1 if the linker supports weak symbols. */
#ifndef FOLLY_HAVE_WEAK_SYMBOLS
#define FOLLY_HAVE_WEAK_SYMBOLS 1
#endif

/* Define to 1 if the system has the type `_Bool'. */
/* #undef HAVE__BOOL */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#ifndef FOLLY_LT_OBJDIR
#define FOLLY_LT_OBJDIR ".libs/"
#endif

/* Define to "override" if the compiler supports C++11 "override" */
#ifndef FOLLY_OVERRIDE
#define FOLLY_OVERRIDE override
#endif

/* Name of package */
#ifndef FOLLY_PACKAGE
#define FOLLY_PACKAGE "folly"
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef FOLLY_PACKAGE_BUGREPORT
#define FOLLY_PACKAGE_BUGREPORT "folly@fb.com"
#endif

/* Define to the full name of this package. */
#ifndef FOLLY_PACKAGE_NAME
#define FOLLY_PACKAGE_NAME "folly"
#endif

/* Define to the full name and version of this package. */
#ifndef FOLLY_PACKAGE_STRING
#define FOLLY_PACKAGE_STRING "folly 15.0"
#endif

/* Define to the one symbol short name of this package. */
#ifndef FOLLY_PACKAGE_TARNAME
#define FOLLY_PACKAGE_TARNAME "folly"
#endif

/* Define to the home page for this package. */
#ifndef FOLLY_PACKAGE_URL
#define FOLLY_PACKAGE_URL ""
#endif

/* Define to the version of this package. */
#ifndef FOLLY_PACKAGE_VERSION
#define FOLLY_PACKAGE_VERSION "15.0"
#endif

/* Define to 1 if you have the ANSI C header files. */
#ifndef FOLLY_STDC_HEADERS
#define FOLLY_STDC_HEADERS 1
#endif

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#ifndef FOLLY_TIME_WITH_SYS_TIME
#define FOLLY_TIME_WITH_SYS_TIME 1
#endif

/* Define to 1 if the gflags namespace is not "gflags" */
#ifndef FOLLY_UNUSUAL_GFLAGS_NAMESPACE
#define FOLLY_UNUSUAL_GFLAGS_NAMESPACE 1
#endif

/* Define to 1 if we're using libc++. */
/* #undef USE_LIBCPP */

/* Version number of package */
#ifndef FOLLY_VERSION
#define FOLLY_VERSION "15.0"
#endif

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */
 
/* once: _FOLLY_CONFIG_H */
#endif
