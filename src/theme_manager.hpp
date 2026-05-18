#pragma once

#include "raylib.h"
#include <string>

enum class ThemeId
{
    TokyoNight,
    OledBlack,
    Adwaita,
    Monokai
};

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

class ThemeManager
{
private:
    ThemeId currentTheme;
    ThemePalette palette;

    ThemeManager();
    void applyTheme(ThemeId theme);

public:
    static ThemeManager &instance();

    void setTheme(ThemeId theme);
    ThemeId getTheme() const noexcept;
    const ThemePalette &getPalette() const noexcept;
    std::string getThemeName() const;

    static std::string getThemeName(ThemeId theme);
};

inline ThemeManager::ThemeManager()
{
    applyTheme(ThemeId::TokyoNight);
}

inline ThemeManager &ThemeManager::instance()
{
    static ThemeManager manager;
    return manager;
}

inline void ThemeManager::applyTheme(ThemeId theme)
{
    currentTheme = theme;

    switch (theme)
    {
    case ThemeId::TokyoNight:
        palette = {
            {26, 27, 38, 255},
            {31, 41, 59, 255},
            {41, 52, 73, 255},
            {224, 230, 255, 255},
            {122, 162, 247, 255},
            {255, 255, 255, 255},
            {187, 154, 247, 255},
            {158, 206, 106, 255},
            {255, 158, 100, 255},
            {247, 118, 142, 255},
            {65, 72, 104, 255},
            {119, 133, 161, 255}
        };
        break;
    case ThemeId::OledBlack:
        palette = {
            {0, 0, 0, 255},
            {12, 12, 12, 255},
            {24, 24, 24, 255},
            {240, 240, 240, 255},
            {255, 255, 255, 255},
            {0, 0, 0, 255},
            {200, 200, 200, 255},
            {0, 255, 0, 255},
            {255, 215, 0, 255},
            {255, 64, 64, 255},
            {40, 40, 40, 255},
            {120, 120, 120, 255}
        };
        break;
    case ThemeId::Adwaita:
        palette = {
            {46, 52, 64, 255},
            {59, 66, 82, 255},
            {67, 76, 94, 255},
            {236, 239, 244, 255},
            {94, 129, 172, 255},
            {255, 255, 255, 255},
            {136, 192, 208, 255},
            {163, 190, 140, 255},
            {235, 203, 139, 255},
            {191, 97, 106, 255},
            {76, 86, 106, 255},
            {129, 161, 193, 255}
        };
        break;
    case ThemeId::Monokai:
        palette = {
            {39, 40, 34, 255},
            {73, 72, 62, 255},
            {102, 99, 84, 255},
            {248, 248, 242, 255},
            {249, 38, 114, 255},
            {255, 255, 255, 255},
            {174, 129, 255, 255},
            {166, 226, 46, 255},
            {230, 219, 116, 255},
            {249, 38, 114, 255},
            {117, 113, 94, 255},
            {166, 226, 46, 255}
        };
        break;
    }
}

inline void ThemeManager::setTheme(ThemeId theme)
{
    applyTheme(theme);
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
