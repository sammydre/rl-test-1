#if 0
// C++11 way -- but G++ does not support codecvt at the time of writing.
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

std::u32string utf8_to_utf32(const std::string &str_utf8)
{
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
  std::u32string utf32 = utf32conv.from_bytes(std_utf8.c_str());
  return utf32;
}

#else

#include <boost/locale/encoding_utf.hpp>
#include <string>

std::u32string utf8_to_utf32(const std::string &str_utf8)
{
  return boost::locale::conv::utf_to_utf<char32_t>(str_utf8.c_str(),
      str_utf8.c_str() + str_utf8.size());
}

#endif
