#pragma once
#ifndef TEXTID_H
#define TEXTID_H

#include <string_view>

#define RF_DECLARE_TEXT_ID_FUNCTIONS(context)                                                      \
public:                                                                                            \
    static inline std::string_view textIdContext()                                                 \
    {                                                                                              \
        return #context;                                                                           \
    }                                                                                              \
    static inline rfapp::TextId textId(const char *sourceText)                                     \
    {                                                                                              \
        return rfapp::TextId{#context, sourceText};                                                \
    }                                                                                              \
    static inline std::string_view textIdTr(const char *sourceText)                                \
    {                                                                                              \
        return textId(sourceText).tr();                                                            \
    }                                                                                              \
                                                                                                   \
private:

namespace rfapp
{

// Provides translatable text(message) identifier
struct TextId
{
    // Context used to find the text(key) to translate, typically a class name
    std::string_view trContext;
    std::string_view key; // Source text

public:
    // Returns the translation text of 'key' by querying registered Application::Translator objects
    // 'n' is used to support plural forms
    std::string_view tr(int n = -1) const;

    // Whether source text(key) is empty or not
    bool isEmpty() const;

    // Provides internationalization support for text output
    //     1st arg: message to be translated(TextId = context+key)
    //     2nd arg: when != -1 used to choose an appropriate form for the translation(e.g. "%n file
    //     found" vs. "%n files found") returns: translated message
    using TranslatorFunction = std::string_view(const TextId &, int);
    using TranslatorFunctionPtr = TranslatorFunction *;
    static void addTranslatorFunction(TranslatorFunctionPtr fn);
    static std::string_view translate(const TextId &textId, int n = -1);
};
} // namespace rfapp
#endif
