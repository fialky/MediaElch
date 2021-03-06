#pragma once

#include <QCoreApplication>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>

namespace mediaelch {

class Locale
{
    Q_DECLARE_TR_FUNCTIONS(Locale)

public:
    /// \brief Locale for en-US. Can be used as default.
    static Locale English;

public:
    /*implicit*/ Locale(const char* locale) : Locale(QString(locale)) {}
    /*implicit*/ Locale(const QString& locale)
    {
        QStringList split = locale.split('-');
        m_lang = split.first();
        if (split.length() > 1) {
            m_country = split[1];
        }
    }

    Locale(QString language, QString country) : m_lang{language}, m_country{country} {}

    const QString& language() const { return m_lang; }
    const QString& country() const { return m_country; }
    QString toString(char delimiter = '-') const { return hasCountry() ? m_lang + delimiter + m_country : m_lang; }

    bool hasCountry() const { return !m_country.isEmpty(); }

    /// \brief Return a human readable, translated language name for the locale.
    /// \details If the locale, e.g. en-US, is not translated, it is checked whether "en" is available.
    ///          If so, then the country is appended in parentheses in a country is available.
    ///          If the language is still not found then simply the locale in string representation is returned.
    QString languageTranslated() const;

private:
    QString m_lang;
    QString m_country;
};

} // namespace mediaelch
