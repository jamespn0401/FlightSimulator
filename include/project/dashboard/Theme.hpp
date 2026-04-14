#pragma once

#include "imgui.h"

namespace project::dashboard
{
    struct Typography
    {
        ImFont *body{nullptr};
        ImFont *mono{nullptr};
        ImFont *heading{nullptr};
        ImFont *hero{nullptr};
    };

    struct Spacing
    {
        float xs{4.0f};
        float sm{8.0f};
        float md{12.0f};
        float lg{16.0f};
        float xl{24.0f};
        float xxl{32.0f};
    };

    struct Theme
    {
        Typography fonts{};
        Spacing spacing{};
    };

    Theme &GetTheme();

    bool InitializeTheme(float dpiScale = 1.0f);
    void ApplyDarkDashboardStyle(float dpiScale = 1.0f);

    void TextBody(const char *fmt, ...);
    void TextMono(const char *fmt, ...);
    void TextHeading(const char *fmt, ...);
    void TextHero(const char *fmt, ...);
}