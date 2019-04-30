// Generated. Please do not edit.

#ifndef TYPEDTEXTDATABASE_HPP
#define TYPEDTEXTDATABASE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/TypedText.hpp>

namespace TypedTextDatabase
{
#ifndef NO_USING_NAMESPACE_TOUCHGFX
  using namespace touchgfx;
#endif

  class TypedTextData;
  const touchgfx::TypedText::TypedTextData* getInstance(touchgfx::LanguageId id);
  const touchgfx::TypedText::TypedTextData* getInstance();
  const touchgfx::Font* const * getFonts();
  uint16_t getInstanceSize();
}

#endif
