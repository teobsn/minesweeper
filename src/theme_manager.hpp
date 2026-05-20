#pragma once

#include "theme_factory.hpp"
#include <string>

struct ThemePalette
{
    Color background;
    Color surface;
    Color surfaceAlt;
    Color text;
    Color accent;
    Color accentText;
    Color highlight;
    Color success;
    Color warning;
    Color danger;
    Color grid;
    Color muted;
};

class ThemeManager final
{
private:
    ThemeId currentTheme;
    ThemePalette palette;

    ThemeManager();
    void applyFactory(const IThemeFactory &factory);

public:
    static ThemeManager &instance();

    ThemeManager(const ThemeManager &) = delete;
    ThemeManager &operator=(const ThemeManager &) = delete;

    void setTheme(std::unique_ptr<IThemeFactory> factory);
    void setTheme(ThemeId theme);
    ThemeId getTheme() const noexcept;
    const ThemePalette &getPalette() const noexcept;
    std::string getThemeName() const;

    static std::string getThemeName(ThemeId theme);
};

inline ThemeManager::ThemeManager()
{
    setTheme(makeThemeFactory(ThemeId::TokyoNight));
}

inline ThemeManager &ThemeManager::instance()
{
    static ThemeManager manager;
    return manager;
}

inline void ThemeManager::applyFactory(const IThemeFactory &factory)
{
    const auto surface = factory.createSurfaceColors();
    const auto accent = factory.createAccentColors();

    palette = {
        surface->background(),
        surface->surface(),
        surface->surfaceAlt(),
        accent->text(),
        accent->accent(),
        accent->accentText(),
        accent->highlight(),
        accent->success(),
        accent->warning(),
        accent->danger(),
        surface->grid(),
        surface->muted()};
}

inline void ThemeManager::setTheme(std::unique_ptr<IThemeFactory> factory)
{
    if (!factory)
        return;

    if (factory->name() == "Tokyo Night")
        currentTheme = ThemeId::TokyoNight;
    else if (factory->name() == "OLED Black")
        currentTheme = ThemeId::OledBlack;
    else if (factory->name() == "Adwaita")
        currentTheme = ThemeId::Adwaita;
    else if (factory->name() == "Monokai")
        currentTheme = ThemeId::Monokai;
    else
        currentTheme = ThemeId::TokyoNight;

    applyFactory(*factory);
}

inline void ThemeManager::setTheme(ThemeId theme)
{
    setTheme(makeThemeFactory(theme));
}

inline ThemeId ThemeManager::getTheme() const noexcept
{
    return currentTheme;
}

inline const ThemePalette &ThemeManager::getPalette() const noexcept
{
    return palette;
}

inline std::string ThemeManager::getThemeName() const
{
    return getThemeName(currentTheme);
}

inline std::string ThemeManager::getThemeName(ThemeId theme)
{
    switch (theme)
    {
    case ThemeId::TokyoNight:
        return "Tokyo Night";
    case ThemeId::OledBlack:
        return "OLED Black";
    case ThemeId::Adwaita:
        return "Adwaita";
    case ThemeId::Monokai:
        return "Monokai";
    }

    return "Unknown";
}
