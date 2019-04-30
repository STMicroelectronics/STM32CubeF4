#include <stdarg.h>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/TypedText.hpp>
#include <texts/TypedTextDatabase.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/TextProvider.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

touchgfx::Font::StringWidthFunctionPointer touchgfx::Font::getStringWidthFunction = &touchgfx::Font::getStringWidthLTR;
touchgfx::LCD::DrawStringFunctionPointer touchgfx::LCD::drawStringFunction = &touchgfx::LCD::drawStringLTR;
touchgfx::TextProvider::UnicodeConverterInitFunctionPointer touchgfx::TextProvider::unicodeConverterInitFunction = static_cast<touchgfx::TextProvider::UnicodeConverterInitFunctionPointer>(0);
touchgfx::TextProvider::UnicodeConverterFunctionPointer touchgfx::TextProvider::unicodeConverterFunction = static_cast<touchgfx::TextProvider::UnicodeConverterFunctionPointer>(0);

extern const touchgfx::Unicode::UnicodeChar* const textsGbr[];
extern const touchgfx::Unicode::UnicodeChar* const textsDnk[];
extern const touchgfx::Unicode::UnicodeChar* const textsJpn[];
extern const touchgfx::Unicode::UnicodeChar* const textsUkr[];
extern const touchgfx::Unicode::UnicodeChar* const textsNld[];

TEXT_LOCATION_FLASH_PRAGMA
const touchgfx::Unicode::UnicodeChar* const* const languagesArray[] TEXT_LOCATION_FLASH_ATTRIBUTE =
{
    textsGbr,
    textsDnk,
    textsJpn,
    textsUkr,
    textsNld
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
const touchgfx::Unicode::UnicodeChar* const* touchgfx::Texts::currentLanguagePtr = languagesArray[currentLanguage];

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    currentLanguage = id;
    currentLanguagePtr = languagesArray[currentLanguage];

    touchgfx::TypedText::registerTypedTextDatabase(TypedTextDatabase::getInstance(currentLanguage),
            TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
}
