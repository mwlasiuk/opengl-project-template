#include <App/ImGuiStyle.hpp>

#include <imgui.h>

namespace App
{
    void SetImGUIStyle()
    {
        auto AdjustHueAndSaturation = [](const ImVec4& Color, const float HueShift, const float SaturationScale) -> ImVec4
        {
            ImVec4 ResultColor = Color;
            ImGui::ColorConvertRGBtoHSV(ResultColor.x, ResultColor.y, ResultColor.z, ResultColor.x, ResultColor.y, ResultColor.z);

            ResultColor.x += HueShift;
            ResultColor.y *= SaturationScale;

            ImGui::ColorConvertHSVtoRGB(ResultColor.x, ResultColor.y, ResultColor.z, ResultColor.x, ResultColor.y, ResultColor.z);
            return ResultColor;
        };

        ImGuiStyle& Style = ImGui::GetStyle();

        const float Hue              = 0.0f; // [0,1] range.
        const float Saturation       = 1.0f; // [0,6] range.
        const float SaturationAccent = 1.0f; // [0,6] range.
        const float Transparency     = 0.95f;
        const float BorderSize       = 0.0f;

        Style.FrameBorderSize  = BorderSize;
        Style.ImageBorderSize  = BorderSize;
        Style.TabBorderSize    = BorderSize;
        Style.TabBarBorderSize = 3.0f;
        Style.WindowRounding   = 4.0f;
        Style.ChildRounding    = 4.0f;
        Style.FrameRounding    = 4.0f;
        Style.GrabRounding     = 4.0f;
        Style.TabRounding      = 4.0f;

        ImVec4 TextColor{1.000f, 1.000f, 1.000f, 1.000f};
        ImVec4 TextDimmedColor{0.357f, 0.482f, 0.549f, 1.000f};
        ImVec4 BackroundColor{0.110f, 0.149f, 0.169f, 1.000f};
        ImVec4 BackroundChildColor{0.090f, 0.122f, 0.141f, 1.000f};
        ImVec4 BackroundDimmedColor{0.000f, 0.000f, 0.000f, 0.600f};
        ImVec4 TitleColor{0.078f, 0.102f, 0.122f, 1.000f};
        ImVec4 HeaderColor{0.184f, 0.247f, 0.286f, 1.000f};
        ImVec4 Accent1Color{0.000f, 0.490f, 1.000f, 1.000f};
        ImVec4 Accent2Color{0.000f, 0.412f, 0.824f, 1.000f};
        ImVec4 Accent1AlternativeColor{0.302f, 0.408f, 0.475f, 1.000f};
        ImVec4 Accent2AlternativeColor{0.251f, 0.337f, 0.392f, 1.000f};
        ImVec4 TransparentColor{0.0f, 0.0f, 0.0f, 0.0f};

        TextColor               = AdjustHueAndSaturation(TextColor, Hue, Saturation);
        TextDimmedColor         = AdjustHueAndSaturation(TextDimmedColor, Hue, Saturation);
        BackroundColor          = AdjustHueAndSaturation(BackroundColor, Hue, Saturation);
        BackroundChildColor     = AdjustHueAndSaturation(BackroundChildColor, Hue, Saturation);
        BackroundDimmedColor    = AdjustHueAndSaturation(BackroundDimmedColor, Hue, Saturation);
        TitleColor              = AdjustHueAndSaturation(TitleColor, Hue, Saturation);
        HeaderColor             = AdjustHueAndSaturation(HeaderColor, Hue, Saturation);
        Accent1Color            = AdjustHueAndSaturation(Accent1Color, Hue, SaturationAccent);
        Accent2Color            = AdjustHueAndSaturation(Accent2Color, Hue, SaturationAccent);
        Accent1AlternativeColor = AdjustHueAndSaturation(Accent1AlternativeColor, Hue, SaturationAccent);
        Accent2AlternativeColor = AdjustHueAndSaturation(Accent2AlternativeColor, Hue, SaturationAccent);

        ImVec4 BackroundTransparentColor = BackroundColor;
        BackroundTransparentColor.w *= Transparency;

        ImVec4 BackroundChildTransparentColor = BackroundChildColor;
        BackroundChildTransparentColor.w *= Transparency;

        ImVec4 TitleTransparentColor = TitleColor;
        TitleTransparentColor.w *= Transparency;

        ImVec4 HeaderTransparentColor = HeaderColor;
        HeaderTransparentColor.w *= Transparency;

        Style.Colors[ImGuiCol_Text]                      = TextColor;
        Style.Colors[ImGuiCol_TextDisabled]              = TextDimmedColor;
        Style.Colors[ImGuiCol_WindowBg]                  = BackroundTransparentColor;
        Style.Colors[ImGuiCol_ChildBg]                   = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_PopupBg]                   = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_Border]                    = TitleColor;
        Style.Colors[ImGuiCol_BorderShadow]              = TransparentColor;
        Style.Colors[ImGuiCol_FrameBg]                   = HeaderTransparentColor;
        Style.Colors[ImGuiCol_FrameBgHovered]            = Accent1AlternativeColor;
        Style.Colors[ImGuiCol_FrameBgActive]             = Accent2AlternativeColor;
        Style.Colors[ImGuiCol_TitleBg]                   = HeaderTransparentColor;
        Style.Colors[ImGuiCol_TitleBgActive]             = TitleTransparentColor;
        Style.Colors[ImGuiCol_TitleBgCollapsed]          = HeaderTransparentColor;
        Style.Colors[ImGuiCol_MenuBarBg]                 = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_ScrollbarBg]               = TitleTransparentColor;
        Style.Colors[ImGuiCol_ScrollbarGrab]             = HeaderColor;
        Style.Colors[ImGuiCol_ScrollbarGrabHovered]      = Accent1AlternativeColor;
        Style.Colors[ImGuiCol_ScrollbarGrabActive]       = Accent2AlternativeColor;
        Style.Colors[ImGuiCol_CheckMark]                 = Accent1Color;
        Style.Colors[ImGuiCol_SliderGrab]                = Accent1Color;
        Style.Colors[ImGuiCol_SliderGrabActive]          = Accent2Color;
        Style.Colors[ImGuiCol_Button]                    = HeaderTransparentColor;
        Style.Colors[ImGuiCol_ButtonHovered]             = Accent1Color;
        Style.Colors[ImGuiCol_ButtonActive]              = Accent2Color;
        Style.Colors[ImGuiCol_Header]                    = HeaderTransparentColor;
        Style.Colors[ImGuiCol_HeaderHovered]             = Accent1Color;
        Style.Colors[ImGuiCol_HeaderActive]              = Accent2Color;
        Style.Colors[ImGuiCol_Separator]                 = HeaderColor;
        Style.Colors[ImGuiCol_SeparatorHovered]          = Accent1Color;
        Style.Colors[ImGuiCol_SeparatorActive]           = Accent2Color;
        Style.Colors[ImGuiCol_ResizeGrip]                = HeaderColor;
        Style.Colors[ImGuiCol_ResizeGripHovered]         = Accent1Color;
        Style.Colors[ImGuiCol_ResizeGripActive]          = Accent2Color;
        Style.Colors[ImGuiCol_InputTextCursor]           = TextColor;
        Style.Colors[ImGuiCol_TabHovered]                = Accent1Color;
        Style.Colors[ImGuiCol_Tab]                       = TransparentColor;
        Style.Colors[ImGuiCol_TabSelected]               = HeaderTransparentColor;
        Style.Colors[ImGuiCol_TabSelectedOverline]       = TransparentColor;
        Style.Colors[ImGuiCol_TabDimmed]                 = TransparentColor;
        Style.Colors[ImGuiCol_TabDimmedSelected]         = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_TabDimmedSelectedOverline] = TransparentColor;
        // Style.Colors[ImGuiCol_DockingPreview]            = HeaderColor;
        // Style.Colors[ImGuiCol_DockingEmptyBg]            = BackroundTransparentColor;
        Style.Colors[ImGuiCol_PlotLines]             = TextDimmedColor;
        Style.Colors[ImGuiCol_PlotLinesHovered]      = Accent1Color;
        Style.Colors[ImGuiCol_PlotHistogram]         = TextDimmedColor;
        Style.Colors[ImGuiCol_PlotHistogramHovered]  = Accent1Color;
        Style.Colors[ImGuiCol_TableHeaderBg]         = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_TableBorderStrong]     = TitleColor;
        Style.Colors[ImGuiCol_TableBorderLight]      = HeaderColor;
        Style.Colors[ImGuiCol_TableRowBg]            = BackroundTransparentColor;
        Style.Colors[ImGuiCol_TableRowBgAlt]         = BackroundChildTransparentColor;
        Style.Colors[ImGuiCol_TextLink]              = Accent1Color;
        Style.Colors[ImGuiCol_TextSelectedBg]        = Accent1Color;
        Style.Colors[ImGuiCol_TreeLines]             = TextDimmedColor;
        Style.Colors[ImGuiCol_DragDropTarget]        = Accent1Color;
        Style.Colors[ImGuiCol_DragDropTargetBg]      = TransparentColor;
        Style.Colors[ImGuiCol_UnsavedMarker]         = Accent1Color;
        Style.Colors[ImGuiCol_NavCursor]             = Accent1Color;
        Style.Colors[ImGuiCol_NavWindowingHighlight] = Accent1Color;
        Style.Colors[ImGuiCol_NavWindowingDimBg]     = BackroundDimmedColor;
        Style.Colors[ImGuiCol_ModalWindowDimBg]      = BackroundDimmedColor;
    }
}
