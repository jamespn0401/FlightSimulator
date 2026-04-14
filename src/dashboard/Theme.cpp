#include "project/dashboard/Theme.hpp"

#include <cstdarg>
#include <cstdio>

namespace project::dashboard
{
    namespace
    {
        Theme g_theme{};

        void TextWithFont(ImFont *font, const char *fmt, va_list args)
        {
            char buffer[1024];
#if defined(_MSC_VER)
            vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
#else
            vsnprintf(buffer, sizeof(buffer), fmt, args);
#endif

            if (font != nullptr)
            {
                ImGui::PushFont(font);
                ImGui::TextUnformatted(buffer);
                ImGui::PopFont();
            }
            else
            {
                ImGui::TextUnformatted(buffer);
            }
        }
    }

    Theme &GetTheme()
    {
        return g_theme;
    }

    bool InitializeTheme(float dpiScale)
    {
        ImGuiIO &io = ImGui::GetIO();

        // Clear previously loaded fonts if needed.
        io.Fonts->Clear();
        io.Fonts->AddFontDefault();
        // Replace these with your actual font files.
        // Good choices:
        // - Inter / Roboto / Segoe UI for body
        // - JetBrains Mono / Consolas for telemetry
        const char *bodyFontPath = "assets/fonts/JetBrainsMono-Medium.ttf";
        const char *headingFontPath = "assets/fonts/JetBrainsMono-Bold.ttf";
        const char *monoFontPath = "assets/fonts/JetBrainsMono-Regular.ttf";

        g_theme.fonts.body = io.Fonts->AddFontFromFileTTF(bodyFontPath, 18.0f * dpiScale);
        g_theme.fonts.mono = io.Fonts->AddFontFromFileTTF(monoFontPath, 18.0f * dpiScale);
        g_theme.fonts.heading = io.Fonts->AddFontFromFileTTF(headingFontPath, 24.0f * dpiScale);
        g_theme.fonts.hero = io.Fonts->AddFontFromFileTTF(headingFontPath, 34.0f * dpiScale);

        if (g_theme.fonts.body == nullptr ||
            g_theme.fonts.mono == nullptr ||
            g_theme.fonts.heading == nullptr ||
            g_theme.fonts.hero == nullptr)
        {
            return false;
        }

        io.FontDefault = g_theme.fonts.body;
        io.Fonts->Build();

        return true;
    }

    void ApplyDarkDashboardStyle(float dpiScale)
    {
        ImGuiStyle &style = ImGui::GetStyle();

        style.WindowRounding = 10.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 8.0f;
        style.GrabRounding = 6.0f;
        style.TabRounding = 6.0f;

        style.WindowPadding = ImVec2(16.0f, 16.0f);
        style.FramePadding = ImVec2(10.0f, 8.0f);
        style.ItemSpacing = ImVec2(10.0f, 10.0f);
        style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
        style.CellPadding = ImVec2(8.0f, 6.0f);

        style.ScaleAllSizes(dpiScale);

        ImVec4 *colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.06f, 0.08f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.22f, 0.25f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.12f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.16f, 0.21f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.21f, 0.27f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.07f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.09f, 0.12f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.22f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.27f, 0.36f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.14f, 0.18f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.25f, 0.34f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.30f, 0.40f, 1.00f);
        colors[ImGuiCol_Text] = ImVec4(0.92f, 0.94f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.60f, 0.68f, 1.00f);
    }

    void TextBody(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        TextWithFont(g_theme.fonts.body, fmt, args);
        va_end(args);
    }

    void TextMono(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        TextWithFont(g_theme.fonts.mono, fmt, args);
        va_end(args);
    }

    void TextHeading(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        TextWithFont(g_theme.fonts.heading, fmt, args);
        va_end(args);
    }

    void TextHero(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        TextWithFont(g_theme.fonts.hero, fmt, args);
        va_end(args);
    }
}