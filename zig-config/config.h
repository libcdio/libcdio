/* config.h for Zig build system — replaces autoconf-generated config.h */
#pragma once

/* What to put between the brackets for empty arrays (GCC/Clang: empty) */
#define EMPTY_ARRAY_SIZE

/* Standard headers available on all modern platforms */
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMORY_H 1
#define HAVE_MEMCPY 1
#define HAVE_MEMSET 1
#define HAVE_SNPRINTF 1
#define HAVE_STDBOOL_H 1
#define HAVE_STDARG_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_VSNPRINTF 1
#define STDC_HEADERS 1

/* ISO 9660 extensions */
#define HAVE_JOLIET 1
#define HAVE_ROCK 1

/* iconv() second arg is non-const on Linux/macOS, const on some BSDs */
#if defined(__APPLE__) || defined(__FreeBSD__)
#  define ICONV_CONST const
#else
#  define ICONV_CONST
#endif

/* Required guard used by all libcdio source files */
#define LIBCDIO_CONFIG_H 1

/* Package info */
#define PACKAGE "libcdio"
#define PACKAGE_NAME "libcdio"
#define PACKAGE_VERSION "2.3.0"
#define VERSION "2.3.0"

/* Platform-specific */
#if defined(_WIN32)
#  undef  EMPTY_ARRAY_SIZE
#  define EMPTY_ARRAY_SIZE 0
#  define HAVE_WIN32_CDROM 1
#  define HAVE_WINDOWS_H 1
#  define HAVE_NTDDSCSI_H 1
#  define HAVE_NTDDCDRM_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_UNISTD_H 1
#elif defined(__APPLE__)
#  define HAVE_DARWIN_CDROM 1
#  define HAVE_ICONV 1
#  define HAVE_COREFOUNDATION_CFBASE_H 1
#  define HAVE_IOKIT_IOKITLIB_H 1
#  define HAVE_DLFCN_H 1
#  define HAVE_GLOB_H 1
#  define HAVE_GMTIME_R 1
#  define HAVE_LOCALTIME_R 1
#  define HAVE_SETENV 1
#  define HAVE_STRINGS_H 1
#  define HAVE_SYS_CDIO_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_TM_GMTOFF 1
#  define HAVE_TIMEGM 1
#  define HAVE_TZSET 1
#  define HAVE_UNSETENV 1
#  define HAVE_UNISTD_H 1
#elif defined(__FreeBSD__)
#  define HAVE_FREEBSD_CDROM 1
#  define HAVE_SYS_CDIO_H 1
#  define HAVE_DLFCN_H 1
#  define HAVE_GLOB_H 1
#  define HAVE_GMTIME_R 1
#  define HAVE_LOCALTIME_R 1
#  define HAVE_SETENV 1
#  define HAVE_STRINGS_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_TM_GMTOFF 1
#  define HAVE_TIMEGM 1
#  define HAVE_UNSETENV 1
#  define HAVE_UNISTD_H 1
#elif defined(__ANDROID__)
/* Android's NDK exposes glob/iconv entry points only from API 28. Keep the
   image drivers usable for lower minSdk builds by avoiding glob-backed device
   enumeration and using the Zig build's local iconv compatibility symbols. */
#  define HAVE_ICONV 1
#  define HAVE_DLFCN_H 1
#  define HAVE_GMTIME_R 1
#  define HAVE_LOCALTIME_R 1
#  define HAVE_SETENV 1
#  define HAVE_STRINGS_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_TM_GMTOFF 1
#  define HAVE_TIMEGM 1
#  define HAVE_TZSET 1
#  define HAVE_TZNAME 1
#  define HAVE_UNSETENV 1
#  define HAVE_UNISTD_H 1
#  define _FILE_OFFSET_BITS 64
#  define _LARGE_FILES 1
#else
/* Linux and other POSIX */
#  define HAVE_ICONV 1
#  define HAVE_LINUX_CDROM 1
#  define HAVE_LINUX_CDROM_H 1
#  define HAVE_LINUX_VERSION_H 1
#  define HAVE_DLFCN_H 1
#  define HAVE_GLOB_H 1
#  define HAVE_GMTIME_R 1
#  define HAVE_LOCALTIME_R 1
#  define HAVE_SETENV 1
#  define HAVE_STRINGS_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_TM_GMTOFF 1
#  define HAVE_TIMEGM 1
#  define HAVE_TZSET 1
#  define HAVE_TZNAME 1
#  define HAVE_UNSETENV 1
#  define HAVE_UNISTD_H 1
#  define _FILE_OFFSET_BITS 64
#  define _LARGE_FILES 1
#endif
