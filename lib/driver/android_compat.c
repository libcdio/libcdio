#include <errno.h>
#include <glob.h>
#include <iconv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Android exposes glob/iconv from API 28, but some applications build libcdio
 * for lower Android API levels. Provide the small subset libcdio needs without
 * importing newer bionic symbols.
 */

#if defined(__GNUC__)
#define ANDROID_COMPAT_HIDDEN __attribute__((visibility("hidden")))
#else
#define ANDROID_COMPAT_HIDDEN
#endif

typedef enum AndroidIconvMode {
    ANDROID_ICONV_COPY_BYTES,
    ANDROID_ICONV_LATIN1_TO_UTF8,
    ANDROID_ICONV_UTF8_TO_UTF16BE,
    ANDROID_ICONV_UTF16BE_TO_UTF8,
} AndroidIconvMode;

typedef struct AndroidIconv {
    AndroidIconvMode mode;
} AndroidIconv;

static bool android_charset_equal(const char *a, const char *b)
{
    while (*a != '\0' || *b != '\0') {
        while (*a == '-' || *a == '_' || *a == ' ')
            a++;
        while (*b == '-' || *b == '_' || *b == ' ')
            b++;

        char ca = *a;
        char cb = *b;
        if (ca >= 'a' && ca <= 'z')
            ca = (char)(ca - ('a' - 'A'));
        if (cb >= 'a' && cb <= 'z')
            cb = (char)(cb - ('a' - 'A'));
        if (ca != cb)
            return false;
        if (*a != '\0')
            a++;
        if (*b != '\0')
            b++;
    }

    return true;
}

static bool android_charset_is_utf8(const char *name)
{
    return android_charset_equal(name, "UTF8");
}

static bool android_charset_is_utf16be(const char *name)
{
    return android_charset_equal(name, "UCS2BE") ||
           android_charset_equal(name, "UTF16BE");
}

ANDROID_COMPAT_HIDDEN iconv_t iconv_open(const char *dst_encoding, const char *src_encoding)
{
    AndroidIconv *converter = (AndroidIconv *)calloc(1, sizeof(*converter));
    if (converter == NULL)
        return (iconv_t)-1;

    const bool src_utf8 = android_charset_is_utf8(src_encoding);
    const bool dst_utf8 = android_charset_is_utf8(dst_encoding);
    const bool src_utf16be = android_charset_is_utf16be(src_encoding);
    const bool dst_utf16be = android_charset_is_utf16be(dst_encoding);

    if (src_utf16be && dst_utf8) {
        converter->mode = ANDROID_ICONV_UTF16BE_TO_UTF8;
    } else if (src_utf8 && dst_utf16be) {
        converter->mode = ANDROID_ICONV_UTF8_TO_UTF16BE;
    } else if (dst_utf8 && !src_utf8) {
        converter->mode = ANDROID_ICONV_LATIN1_TO_UTF8;
    } else {
        converter->mode = ANDROID_ICONV_COPY_BYTES;
    }

    return (iconv_t)converter;
}

ANDROID_COMPAT_HIDDEN int iconv_close(iconv_t converter)
{
    if (converter == (iconv_t)-1 || converter == NULL) {
        errno = EBADF;
        return -1;
    }

    free((void *)converter);
    return 0;
}

static size_t android_emit_utf8(uint32_t codepoint, char **outbuf, size_t *outbytesleft)
{
    char encoded[4];
    size_t encoded_len = 0;

    if (codepoint <= 0x7F) {
        encoded[encoded_len++] = (char)codepoint;
    } else if (codepoint <= 0x7FF) {
        encoded[encoded_len++] = (char)(0xC0 | (codepoint >> 6));
        encoded[encoded_len++] = (char)(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        encoded[encoded_len++] = (char)(0xE0 | (codepoint >> 12));
        encoded[encoded_len++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        encoded[encoded_len++] = (char)(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        encoded[encoded_len++] = (char)(0xF0 | (codepoint >> 18));
        encoded[encoded_len++] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        encoded[encoded_len++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        encoded[encoded_len++] = (char)(0x80 | (codepoint & 0x3F));
    } else {
        errno = EILSEQ;
        return (size_t)-1;
    }

    if (*outbytesleft < encoded_len) {
        errno = E2BIG;
        return (size_t)-1;
    }

    memcpy(*outbuf, encoded, encoded_len);
    *outbuf += encoded_len;
    *outbytesleft -= encoded_len;
    return 0;
}

static bool android_decode_utf8(const unsigned char *inbuf, size_t inbytesleft,
                                uint32_t *codepoint, size_t *used)
{
    const unsigned char b0 = inbuf[0];
    if (b0 < 0x80) {
        *codepoint = b0;
        *used = 1;
        return true;
    }

    if ((b0 & 0xE0) == 0xC0) {
        if (inbytesleft < 2) {
            errno = EINVAL;
            return false;
        }
        if ((inbuf[1] & 0xC0) != 0x80) {
            errno = EILSEQ;
            return false;
        }
        *codepoint = ((uint32_t)(b0 & 0x1F) << 6) |
                     (uint32_t)(inbuf[1] & 0x3F);
        *used = 2;
        if (*codepoint < 0x80) {
            errno = EILSEQ;
            return false;
        }
        return true;
    }

    if ((b0 & 0xF0) == 0xE0) {
        if (inbytesleft < 3) {
            errno = EINVAL;
            return false;
        }
        if ((inbuf[1] & 0xC0) != 0x80 || (inbuf[2] & 0xC0) != 0x80) {
            errno = EILSEQ;
            return false;
        }
        *codepoint = ((uint32_t)(b0 & 0x0F) << 12) |
                     ((uint32_t)(inbuf[1] & 0x3F) << 6) |
                     (uint32_t)(inbuf[2] & 0x3F);
        *used = 3;
        if (*codepoint < 0x800 || (*codepoint >= 0xD800 && *codepoint <= 0xDFFF)) {
            errno = EILSEQ;
            return false;
        }
        return true;
    }

    if ((b0 & 0xF8) == 0xF0) {
        if (inbytesleft < 4) {
            errno = EINVAL;
            return false;
        }
        if ((inbuf[1] & 0xC0) != 0x80 || (inbuf[2] & 0xC0) != 0x80 ||
            (inbuf[3] & 0xC0) != 0x80) {
            errno = EILSEQ;
            return false;
        }
        *codepoint = ((uint32_t)(b0 & 0x07) << 18) |
                     ((uint32_t)(inbuf[1] & 0x3F) << 12) |
                     ((uint32_t)(inbuf[2] & 0x3F) << 6) |
                     (uint32_t)(inbuf[3] & 0x3F);
        *used = 4;
        if (*codepoint < 0x10000 || *codepoint > 0x10FFFF) {
            errno = EILSEQ;
            return false;
        }
        return true;
    }

    errno = EILSEQ;
    return false;
}

static size_t android_convert_utf16be_to_utf8(char **inbuf, size_t *inbytesleft,
                                              char **outbuf, size_t *outbytesleft)
{
    while (*inbytesleft > 0) {
        if (*inbytesleft < 2) {
            errno = EINVAL;
            return (size_t)-1;
        }

        const unsigned char *input = (const unsigned char *)*inbuf;
        uint32_t codepoint = ((uint32_t)input[0] << 8) | (uint32_t)input[1];
        size_t consumed = 2;

        if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
            if (*inbytesleft < 4) {
                errno = EINVAL;
                return (size_t)-1;
            }
            const uint32_t low = ((uint32_t)input[2] << 8) | (uint32_t)input[3];
            if (low < 0xDC00 || low > 0xDFFF) {
                errno = EILSEQ;
                return (size_t)-1;
            }
            codepoint = 0x10000 + (((codepoint - 0xD800) << 10) | (low - 0xDC00));
            consumed = 4;
        } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
            errno = EILSEQ;
            return (size_t)-1;
        }

        char *saved_outbuf = *outbuf;
        size_t saved_outbytesleft = *outbytesleft;
        if (android_emit_utf8(codepoint, outbuf, outbytesleft) == (size_t)-1) {
            *outbuf = saved_outbuf;
            *outbytesleft = saved_outbytesleft;
            return (size_t)-1;
        }

        *inbuf += consumed;
        *inbytesleft -= consumed;
    }

    return 0;
}

static size_t android_convert_latin1_to_utf8(char **inbuf, size_t *inbytesleft,
                                             char **outbuf, size_t *outbytesleft)
{
    while (*inbytesleft > 0) {
        if (android_emit_utf8((unsigned char)**inbuf, outbuf, outbytesleft) == (size_t)-1)
            return (size_t)-1;
        (*inbuf)++;
        (*inbytesleft)--;
    }

    return 0;
}

static size_t android_convert_utf8_to_utf16be(char **inbuf, size_t *inbytesleft,
                                              char **outbuf, size_t *outbytesleft)
{
    while (*inbytesleft > 0) {
        uint32_t codepoint = 0;
        size_t consumed = 0;
        if (!android_decode_utf8((const unsigned char *)*inbuf, *inbytesleft,
                                 &codepoint, &consumed))
            return (size_t)-1;

        uint16_t encoded[2];
        size_t encoded_len = 0;
        if (codepoint <= 0xFFFF) {
            encoded[encoded_len++] = (uint16_t)codepoint;
        } else {
            codepoint -= 0x10000;
            encoded[encoded_len++] = (uint16_t)(0xD800 | (codepoint >> 10));
            encoded[encoded_len++] = (uint16_t)(0xDC00 | (codepoint & 0x3FF));
        }

        if (*outbytesleft < encoded_len * 2) {
            errno = E2BIG;
            return (size_t)-1;
        }

        for (size_t i = 0; i < encoded_len; i++) {
            *(*outbuf)++ = (char)(encoded[i] >> 8);
            *(*outbuf)++ = (char)(encoded[i] & 0xFF);
            *outbytesleft -= 2;
        }
        *inbuf += consumed;
        *inbytesleft -= consumed;
    }

    return 0;
}

ANDROID_COMPAT_HIDDEN size_t iconv(iconv_t converter, char **inbuf,
                                   size_t *inbytesleft, char **outbuf,
                                   size_t *outbytesleft)
{
    if (converter == (iconv_t)-1 || converter == NULL) {
        errno = EBADF;
        return (size_t)-1;
    }
    if (inbuf == NULL || *inbuf == NULL)
        return 0;
    if (inbytesleft == NULL || outbuf == NULL || *outbuf == NULL || outbytesleft == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }

    const AndroidIconv *state = (const AndroidIconv *)converter;
    switch (state->mode) {
    case ANDROID_ICONV_UTF16BE_TO_UTF8:
        return android_convert_utf16be_to_utf8(inbuf, inbytesleft, outbuf, outbytesleft);
    case ANDROID_ICONV_UTF8_TO_UTF16BE:
        return android_convert_utf8_to_utf16be(inbuf, inbytesleft, outbuf, outbytesleft);
    case ANDROID_ICONV_LATIN1_TO_UTF8:
        return android_convert_latin1_to_utf8(inbuf, inbytesleft, outbuf, outbytesleft);
    case ANDROID_ICONV_COPY_BYTES:
    default:
        break;
    }

    const size_t bytes_to_copy = (*inbytesleft < *outbytesleft) ? *inbytesleft : *outbytesleft;
    memcpy(*outbuf, *inbuf, bytes_to_copy);
    *inbuf += bytes_to_copy;
    *inbytesleft -= bytes_to_copy;
    *outbuf += bytes_to_copy;
    *outbytesleft -= bytes_to_copy;

    if (*inbytesleft > 0) {
        errno = E2BIG;
        return (size_t)-1;
    }

    return 0;
}

ANDROID_COMPAT_HIDDEN int glob(const char *pattern, int flags,
                               int (*errfunc)(const char *epath, int eerrno),
                               glob_t *pglob)
{
    (void)pattern;
    (void)errfunc;

    if (pglob != NULL) {
        const size_t gl_offs = (flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
        memset(pglob, 0, sizeof(*pglob));
        pglob->gl_offs = gl_offs;
        pglob->gl_flags = flags;
    }

    return GLOB_NOMATCH;
}

ANDROID_COMPAT_HIDDEN void globfree(glob_t *pglob)
{
    if (pglob == NULL)
        return;

    free(pglob->gl_pathv);
    pglob->gl_pathv = NULL;
    pglob->gl_pathc = 0;
    pglob->gl_matchc = 0;
}
