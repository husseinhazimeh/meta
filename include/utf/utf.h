/**
 * @file utf.h
 * @author Chase Geigle
 *
 * All files in META are dual-licensed under the MIT and NCSA licenses. For more
 * details, consult the file LICENSE.mit and LICENSE.ncsa in the root of the
 * project.
 */

#ifndef META_UTF8_H_
#define META_UTF8_H_

#include <array>
#include <functional>
#include <string>

#include <unicode/utf8.h>

namespace meta
{
namespace utf
{

/**
 * Helper method that appends a UTF-32 codepoint to the given utf8 string.
 * @param dest The string to append the codepoint to
 * @param codepoint The UTF-32 codepoint to append
 */
inline void utf8_append_codepoint(std::string& dest, uint32_t codepoint)
{
    std::array<uint8_t, U8_MAX_LENGTH> buf;
    int32_t len = 0;
    UBool err = FALSE;
    U8_APPEND(&buf[0], len, U8_MAX_LENGTH, codepoint, err);
    if (err)
        throw std::runtime_error{"failed to add codepoint to string"};
    dest.append(reinterpret_cast<char*>(&buf[0]), len);
}

/**
 * Converts a string from the given charset to utf8.
 * @param str The string to convert
 * @param charset The charset of the given string
 * @return a utf8 string
 */
std::string to_utf8(const std::string& str, const std::string& charset);

/**
 * Converts a string fro the given charset to utf16.
 * @param str The string to convert
 * @param charset The charset of the given string
 * @return a utf string
 */
std::u16string to_utf16(const std::string& str, const std::string& charset);

/**
 * Converts a string from utf16 to utf8.
 *
 * @param str The string to convert
 * @return a utf8 string
 */
std::string to_utf8(const std::u16string& str);

/**
 * Converts a string from utf8 to utf16.
 *
 * @param str The string to convert
 * @return a utf16 string
 */
std::u16string to_utf16(const std::string& str);

/**
 * Lowercases a utf8 string.
 *
 * @param str The string to convert
 * @return a lowercased utf8 string
 */
std::string tolower(const std::string& str);

/**
 * Uppercases a utf8 string.
 *
 * @param str The string to convert
 * @return an uppercased utf8 string.
 */
std::string toupper(const std::string& str);

/**
 * Folds the case of a utf8 string. This is like lowercase, but a bit more
 * general.
 *
 * @param str The string to convert
 * @return a case-folded utf8 string
 */
std::string foldcase(const std::string& str);

/**
 * Transliterates a utf8 string, using the rules defined in ICU.
 * @see http://userguide.icu-project.org/transforms
 *
 * @param str The string to transliterate
 * @param id The ICU identifier for the transliteration method to use
 * @return the transliterated string, in utf8
 */
std::string transform(const std::string& str, const std::string& id);

/**
 * Removes UTF-32 codepoints that match the given function.
 *
 * @param str The string to remove characters from
 * @param pred The predicate that returns true for codepoints that should
 * be removed
 * @return a utf8 formatted string with all codepoints matching pred
 * removed
 */
template <class Predicate>
std::string remove_if(const std::string& str, Predicate&& pred)
{
    std::string result;
    result.reserve(str.size());
    const char* s = str.c_str();
    int32_t length = str.length();
    for (int32_t i = 0; i < length;)
    {
        UChar32 codepoint;
        U8_NEXT(s, i, length, codepoint);
        if (pred(codepoint))
            continue;
        utf8_append_codepoint(result, codepoint);
    }
    return result;
}

/**
 * @return the number of code points in a utf8 string.
 * @param str The string to find the length of
 */
uint64_t length(const std::string& str);

/**
 * @return whether a code point is a letter character
 * @param codepoint The codepoint in question
 */
bool isalpha(uint32_t codepoint);

/**
 * @return whether a code point is a blank character
 * @param codepoint The codepoint in question
 */
bool isblank(uint32_t codepoint);
}
}

#endif
