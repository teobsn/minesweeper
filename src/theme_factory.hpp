#pragma once

#include "raylib.h"
#include <memory>
#include <string>

enum class ThemeId
{
    TokyoNight,
    OledBlack,
    Adwaita,
    Monokai
};

class IThemeSurfaceColors
{
public:
    virtual ~IThemeSurfaceColors() = default;

    virtual Color background() const = 0;
    virtual Color surface() const = 0;
    virtual Color surfaceAlt() const = 0;
    virtual Color grid() const = 0;
    virtual Color muted() const = 0;
};

class IThemeAccentColors
{
public:
    virtual ~IThemeAccentColors() = default;

    virtual Color text() const = 0;
    virtual Color accent() const = 0;
    virtual Color accentText() const = 0;
    virtual Color highlight() const = 0;
    virtual Color success() const = 0;
    virtual Color warning() const = 0;
    virtual Color danger() const = 0;
};

class IThemeFactory
{
public:
    virtual ~IThemeFactory() = default;

    // Produsele sunt returnate prin unique_ptr deoarece fabrica transfera
    // proprietatea obiectului creat catre client, fara copii inutile si fara
    // risc de leakage sau slicing pe tipuri abstracte
    virtual std::unique_ptr<IThemeSurfaceColors> createSurfaceColors() const = 0;
    virtual std::unique_ptr<IThemeAccentColors> createAccentColors() const = 0;
    virtual std::string name() const = 0;
};

class TokyoNightTheme final : public IThemeSurfaceColors, public IThemeAccentColors
{
public:
    Color background() const override { return {26, 27, 38, 255}; }
    Color surface() const override { return {31, 41, 59, 255}; }
    Color surfaceAlt() const override { return {41, 52, 73, 255}; }
    Color grid() const override { return {65, 72, 104, 255}; }
    Color muted() const override { return {119, 133, 161, 255}; }
    Color text() const override { return {224, 230, 255, 255}; }
    Color accent() const override { return {122, 162, 247, 255}; }
    Color accentText() const override { return {255, 255, 255, 255}; }
    Color highlight() const override { return {187, 154, 247, 255}; }
    Color success() const override { return {158, 206, 106, 255}; }
    Color warning() const override { return {255, 158, 100, 255}; }
    Color danger() const override { return {247, 118, 142, 255}; }
};

class OledBlackTheme final : public IThemeSurfaceColors, public IThemeAccentColors
{
public:
    Color background() const override { return {0, 0, 0, 255}; }
    Color surface() const override { return {12, 12, 12, 255}; }
    Color surfaceAlt() const override { return {24, 24, 24, 255}; }
    Color grid() const override { return {40, 40, 40, 255}; }
    Color muted() const override { return {120, 120, 120, 255}; }
    Color text() const override { return {240, 240, 240, 255}; }
    Color accent() const override { return {255, 255, 255, 255}; }
    Color accentText() const override { return {0, 0, 0, 255}; }
    Color highlight() const override { return {200, 200, 200, 255}; }
    Color success() const override { return {0, 255, 0, 255}; }
    Color warning() const override { return {255, 215, 0, 255}; }
    Color danger() const override { return {255, 64, 64, 255}; }
};

class AdwaitaTheme final : public IThemeSurfaceColors, public IThemeAccentColors
{
public:
    Color background() const override { return {46, 52, 64, 255}; }
    Color surface() const override { return {59, 66, 82, 255}; }
    Color surfaceAlt() const override { return {67, 76, 94, 255}; }
    Color grid() const override { return {76, 86, 106, 255}; }
    Color muted() const override { return {129, 161, 193, 255}; }
    Color text() const override { return {236, 239, 244, 255}; }
    Color accent() const override { return {94, 129, 172, 255}; }
    Color accentText() const override { return {255, 255, 255, 255}; }
    Color highlight() const override { return {136, 192, 208, 255}; }
    Color success() const override { return {163, 190, 140, 255}; }
    Color warning() const override { return {235, 203, 139, 255}; }
    Color danger() const override { return {191, 97, 106, 255}; }
};

class MonokaiTheme final : public IThemeSurfaceColors, public IThemeAccentColors
{
public:
    Color background() const override { return {39, 40, 34, 255}; }
    Color surface() const override { return {73, 72, 62, 255}; }
    Color surfaceAlt() const override { return {102, 99, 84, 255}; }
    Color grid() const override { return {117, 113, 94, 255}; }
    Color muted() const override { return {166, 226, 46, 255}; }
    Color text() const override { return {248, 248, 242, 255}; }
    Color accent() const override { return {249, 38, 114, 255}; }
    Color accentText() const override { return {255, 255, 255, 255}; }
    Color highlight() const override { return {174, 129, 255, 255}; }
    Color success() const override { return {166, 226, 46, 255}; }
    Color warning() const override { return {230, 219, 116, 255}; }
    Color danger() const override { return {249, 38, 114, 255}; }
};

class TokyoNightThemeFactory final : public IThemeFactory
{
public:
    std::unique_ptr<IThemeSurfaceColors> createSurfaceColors() const override { return std::make_unique<TokyoNightTheme>(); }
    std::unique_ptr<IThemeAccentColors> createAccentColors() const override { return std::make_unique<TokyoNightTheme>(); }
    std::string name() const override { return "Tokyo Night"; }
};

class OledBlackThemeFactory final : public IThemeFactory
{
public:
    std::unique_ptr<IThemeSurfaceColors> createSurfaceColors() const override { return std::make_unique<OledBlackTheme>(); }
    std::unique_ptr<IThemeAccentColors> createAccentColors() const override { return std::make_unique<OledBlackTheme>(); }
    std::string name() const override { return "OLED Black"; }
};

class AdwaitaThemeFactory final : public IThemeFactory
{
public:
    std::unique_ptr<IThemeSurfaceColors> createSurfaceColors() const override { return std::make_unique<AdwaitaTheme>(); }
    std::unique_ptr<IThemeAccentColors> createAccentColors() const override { return std::make_unique<AdwaitaTheme>(); }
    std::string name() const override { return "Adwaita"; }
};

class MonokaiThemeFactory final : public IThemeFactory
{
public:
    std::unique_ptr<IThemeSurfaceColors> createSurfaceColors() const override { return std::make_unique<MonokaiTheme>(); }
    std::unique_ptr<IThemeAccentColors> createAccentColors() const override { return std::make_unique<MonokaiTheme>(); }
    std::string name() const override { return "Monokai"; }
};

// Il tinem inline in header ca sa evitam definiri multiple
inline std::unique_ptr<IThemeFactory> makeThemeFactory(ThemeId theme)
{
    switch (theme)
    {
    case ThemeId::TokyoNight:
        return std::make_unique<TokyoNightThemeFactory>();
    case ThemeId::OledBlack:
        return std::make_unique<OledBlackThemeFactory>();
    case ThemeId::Adwaita:
        return std::make_unique<AdwaitaThemeFactory>();
    case ThemeId::Monokai:
        return std::make_unique<MonokaiThemeFactory>();
    }

    return std::make_unique<TokyoNightThemeFactory>();
}
