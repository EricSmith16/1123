#include "sdk.h"
#include "hooks.h"
#include "Menu.h"
#include "global.h"
#include "Hitmarker.h"
#include "ESP.h"
#include "ImGUI\imconfig.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_internal.h"
#include "ImGUI\stb_rect_pack.h"
#include "ImGUI\stb_textedit.h"
#include "ImGUI\stb_truetype.h"
#include "ImGUI\DX9\imgui_impl_dx9.h"
#include "Items.h"
#include "Config.h"
#include "GameUtils.h"

typedef void(*CL_FullUpdate_t)(void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

	Color colColor(255, 255, 255, 255);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
	}
}

void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(FindPattern(XorStr("engine.dll"), reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}

PresentFn oPresent;

ImFont* Impact;
ImFont* Default;
ImFont* Tabs;
ImFont* badcache;

tReset oResetScene;

void GUI_Init(IDirect3DDevice9* pDevice)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX9_Init(G::Window, pDevice);

	ImGuiStyle& style = ImGui::GetStyle();

	Impact = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 32.0f);
	Default = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f);
	Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Untitled1.ttf", 45.0f);
	badcache = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\AstriumWep.ttf", 25.f);

	ImGuiStyle& styled = ImGui::GetStyle();

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 1);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(15, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	G::Init = true;
}
#define RGBA_TO_FLOAT(r,g,b,a) (float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f
void color()
{
	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = Clientvariables->Colors.MenuColor[0] * 255;
	MenuGreen = Clientvariables->Colors.MenuColor[1] * 255;
	MenuBlue = Clientvariables->Colors.MenuColor[2] * 255;

	ImColor mainColor = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
	ImColor bodyColor = ImColor(24, 24, 24, 239);
	ImColor fontColor = ImColor(255, 255, 255, 255);

	ImGuiStyle& styled = ImGui::GetStyle();

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(RGBA_TO_FLOAT(255, 255, 255, 255));
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(RGBA_TO_FLOAT(170, 170, 170, 255));
	style.Colors[ImGuiCol_WindowBg] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(RGBA_TO_FLOAT(20, 20, 20, 255));
	style.Colors[ImGuiCol_Border] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 255));
	style.Colors[ImGuiCol_FrameBg] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 70));
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(RGBA_TO_FLOAT(40, 40, 40, 255));
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_TitleBg] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 230));
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 60));
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 240));
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(RGBA_TO_FLOAT(50, 50, 50, 255));
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(RGBA_TO_FLOAT(60, 60, 60, 255));
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 255));
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(RGBA_TO_FLOAT(70, 70, 70, 255));
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(RGBA_TO_FLOAT(110, 110, 110, 255));
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 180));
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(RGBA_TO_FLOAT(110, 110, 110, 255));
	style.Colors[ImGuiCol_Button] = ImVec4(RGBA_TO_FLOAT(60, 60, 60, 255));
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(RGBA_TO_FLOAT(77, 77, 77, 255));
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(RGBA_TO_FLOAT(77, 77, 77, 255));
	style.Colors[ImGuiCol_Header] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 255));
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(RGBA_TO_FLOAT(100, 100, 100, 170));
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(RGBA_TO_FLOAT(50, 50, 50, 255));
	style.Colors[ImGuiCol_Column] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(RGBA_TO_FLOAT(10, 10, 10, 255));
	style.Colors[ImGuiCol_CloseButton] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 230));
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(RGBA_TO_FLOAT(0, 0, 0, 230));

}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Text] = ImColor(205, 205, 205, 255);
	else
		style.Colors[ImGuiCol_Text] = ImColor(93, 93, 93, 255);
}

void buttonColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive) {
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
	else {
		style.Colors[ImGuiCol_Button] = ImColor(26, 25, 27);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(26, 25, 27);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(26, 25, 27);
	}
}

void lineColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive) {
		style.Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(60, 60, 60);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 60, 60);
	}
	else {
		style.Colors[ImGuiCol_Button] = ImColor(26, 25, 27);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(26, 25, 27);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(26, 25, 27);
	}
}

void lineColor1(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive) {
		style.Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(60, 60, 60);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 60, 60);
	}
	else {
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
}

void BtnNormal()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.78431372549f, 0.78431372549f, 0.78431372549f, 1.f);

}

void Border(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
	else
		style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
}

void BtnActive()
{
	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = Clientvariables->Colors.MenuColor[0] * 255;
	MenuGreen = Clientvariables->Colors.MenuColor[1] * 255;
	MenuBlue = Clientvariables->Colors.MenuColor[2] * 255;

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
}

void DrawRageTab()
{
	auto& style = ImGui::GetStyle();
	ImGui::Columns(3, "yes", false);
	TextColor(true);

	ImGui::Text("Aimbot");

	ImGui::BeginChild("Aimbot", ImVec2(246, 500), true);
	{
		ImGui::Separator();
		ImGui::Checkbox("Enable", &Clientvariables->Ragebot.EnableAimbot);

		ImGui::Text("Selection");
		ImGui::Combo("##selection", &Clientvariables->Ragebot.AimbotSelection, SelectionMode, ARRAYSIZE(SelectionMode));

		ImGui::Checkbox("Friendly fire", &Clientvariables->Ragebot.FriendlyFire);
		ImGui::Checkbox("Automatic fire", &Clientvariables->Ragebot.AutomaticFire);
		ImGui::Checkbox("Silent aim", &Clientvariables->Ragebot.SilentAimbot);
		ImGui::Checkbox("baim if in air", &Clientvariables->Ragebot.BaimInAir);
		ImGui::Checkbox("baim if standing", &Clientvariables->Ragebot.BaimIfStanding);
		ImGui::Checkbox("baim if fake walk", &Clientvariables->Ragebot.BaimIfFakeWalk);

		ImGui::Text("Minimum hit chance");
		ImGui::SliderInt("##hitchance", &Clientvariables->Ragebot.Minhitchance, 0, 100, "%.0f%%");

		ImGui::Text("Minimum damage");
		ImGui::SliderInt("##mindamage", &Clientvariables->Ragebot.Mindamage, 1, 101, "%.0f%%");

		ImGui::Checkbox("Automatic scope", &Clientvariables->Ragebot.AutomaticScope);
		ImGui::Checkbox("Automatic penetration", &Clientvariables->Ragebot.Autowall);
		ImGui::Checkbox("Hitscan penetration", &Clientvariables->Ragebot.AutowallHitscan);

		ImGui::Checkbox("Fakewalk", &Clientvariables->Misc.FakewalkEnable);
		if (Clientvariables->Misc.FakewalkEnable) {
			ImGui::Hotkey("##walkkey", &Clientvariables->Misc.FakewalkKey, ImVec2(150, 20));
		}
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::Text("Anti-aimbot");

	ImGui::BeginChild("##antiaimbot", ImVec2(246, 500), true);
	{
		ImGui::Separator();
		ImGui::Checkbox("Enable", &Clientvariables->Antiaim.AntiaimEnable);
		ImGui::Text("Pitch");
		ImGui::Combo("##pitch", &Clientvariables->Antiaim.Pitch, AntiaimbotPitch, ARRAYSIZE(AntiaimbotPitch));

		ImGui::Text("Anti-aimbot");
		ImGui::Combo("##freestanding", &Clientvariables->Antiaim.Freestanding, Freestanding, ARRAYSIZE(Freestanding));

		ImGui::Spacing();
		if (Clientvariables->Antiaim.Freestanding == 1)
		{
			ImGui::Text("Yaw");
			ImGui::Combo("##yaw", &Clientvariables->Antiaim.Yaw, AntiaimbotYaw, ARRAYSIZE(AntiaimbotYaw));
			ImGui::SliderInt("##yawadd", &Clientvariables->Antiaim.YawAdd, -180, 180);

			ImGui::Text("Move");
			ImGui::Combo("##yawrunning", &Clientvariables->Antiaim.YawRunning, AntiaimbotYawRun, ARRAYSIZE(AntiaimbotYawRun));
			ImGui::SliderInt("##yawrunadd", &Clientvariables->Antiaim.YawRunningAdd, -180, 180);

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);

			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);

			ImGui::Text("At Target");
			ImGui::Combo("##dynamic", &Clientvariables->Antiaim.AtPlayer, AtTarget, ARRAYSIZE(AtTarget));
		}
		if (Clientvariables->Antiaim.Freestanding == 2)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);

			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}
		if (Clientvariables->Antiaim.Freestanding == 3)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);

			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::Text("Other");

	ImGui::BeginChild("##other", ImVec2(246, 500), true);
	{

		style.ItemSpacing = ImVec2(1, 1);
		static int ragebottober = 0;
		ImGui::Separator();

		TextColor(ragebottober == 0);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ragebottober == 0) BtnActive(); else BtnNormal();
		if (ImGui::Button("Other", ImVec2(83, 32)))
			ragebottober = 0;

		ImGui::SameLine();

		TextColor(ragebottober == 1);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ragebottober == 1) BtnActive(); else BtnNormal();
		if (ImGui::Button("Hitscan", ImVec2(83, 32)))
			ragebottober = 1;

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::NewLine();
		style.ItemSpacing = ImVec2(7, 7);

		if (ragebottober == 0)
		{
			TextColor(true);
			ImGui::Checkbox("Remove spread", &Clientvariables->Ragebot.NoSpread);
			ImGui::Checkbox("Remove recoil", &Clientvariables->Ragebot.NoRecoil);
			ImGui::Checkbox("Anti-aim correction", &Clientvariables->Ragebot.AutomaticResolver);
			ImGui::Checkbox("Engine prediction", &Clientvariables->Ragebot.EnginePred);
			ImGui::Checkbox("Delay shot", &Clientvariables->Ragebot.PositionAdjustment);
			if (Clientvariables->Ragebot.PositionAdjustment)
			{
				ImGui::SliderInt("##delay", &Clientvariables->Ragebot.EnginePred1, 0, 10, "%.fms%");
			}
			ImGui::Checkbox("Multipoint", &Clientvariables->Ragebot.enablescale);
			ImGui::Combo("##hitbox", &Clientvariables->Ragebot.Hitbox, HitboxMode, ARRAYSIZE(HitboxMode));
			ImGui::Text("Head scale");
			ImGui::SliderFloat("##headscale", &Clientvariables->Ragebot.Headscale, 0.f, 1.f, "%.2f%%");

			ImGui::Text("Body scale");
			ImGui::SliderFloat("##bodyscale", &Clientvariables->Ragebot.Bodyscale, 0.f, 1.f, "%.2f%%");
			ImGui::Separator();
		}
		if (ragebottober == 1)
		{
			TextColor(true);
			ImGui::Text("Hitscan");
			ImGui::Combo("##multibox", &Clientvariables->Ragebot.Hitscan, HitscanMode, ARRAYSIZE(HitscanMode));
			if (Clientvariables->Ragebot.Hitscan == 4)
			{
				ImGui::Text("Bones to hitscan");
				ImGui::Separator();
				ImGui::Selectable("Head", &Clientvariables->Ragebot.HeadH);
				ImGui::Selectable("Neck", &Clientvariables->Ragebot.NeckH);
				ImGui::Selectable("Body", &Clientvariables->Ragebot.BodyH);
				ImGui::Selectable("Arms", &Clientvariables->Ragebot.ArmsH);
				ImGui::Selectable("Legs", &Clientvariables->Ragebot.LegsH);
				ImGui::Selectable("Foot", &Clientvariables->Ragebot.FootsH);
			}

			ImGui::Separator();
		}
	}
	ImGui::EndChild();
}

void DrawLegitTab()
{
	auto& style = ImGui::GetStyle();
	static int LegitSubTab;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.ItemSpacing = ImVec2(3, 3);

	ImGui::PushFont(badcache);

	ImGui::Separator();
	ImGui::NewLine();
	TextColor(LegitSubTab == 0);
	buttonColor(false);
	if (ImGui::Button("G", ImVec2(85.83, 50)))
		LegitSubTab = 0;
	ImGui::SameLine();
	TextColor(LegitSubTab == 1);
	buttonColor(false);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	if (ImGui::Button("P", ImVec2(85.83, 50)))
		LegitSubTab = 1;
	ImGui::SameLine();
	TextColor(LegitSubTab == 2);
	buttonColor(false);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	if (ImGui::Button("W", ImVec2(85.83, 50)))
		LegitSubTab = 2;
	ImGui::SameLine();
	TextColor(LegitSubTab == 3);
	buttonColor(false);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	if (ImGui::Button("d", ImVec2(85.83, 50)))
		LegitSubTab = 3;
	ImGui::SameLine();
	TextColor(LegitSubTab == 4);
	buttonColor(false);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	if (ImGui::Button("f", ImVec2(85.83, 50)))
		LegitSubTab = 4;
	ImGui::SameLine();
	TextColor(LegitSubTab == 5);
	buttonColor(false);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	if (ImGui::Button("a", ImVec2(85.83, 50)))
		LegitSubTab = 5;
	ImGui::PopFont();
	ImGui::NewLine();
	ImGui::Separator();
	if (LegitSubTab == 0) {

		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
		ImGui::Columns(3, "yes", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
	else if (LegitSubTab == 1) {
		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
	    ImGui::Columns(3, "yes2", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
	else if (LegitSubTab == 2) {
		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
		ImGui::Columns(3, "yes3", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
	else if (LegitSubTab == 3) {
		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
		ImGui::Columns(3, "yes4", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
	else if (LegitSubTab == 4) {
		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
		ImGui::Columns(3, "yes5", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
	else if (LegitSubTab == 5) {
		TextColor(true);
		style.ItemSpacing = ImVec2(7, 7);
		ImGui::PushFont(Default);
		ImGui::Columns(3, "yes6", false);
		ImGui::BeginChild("##aimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Text("Aimbot");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##trigger", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Text(("trigger"));
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		ImGui::BeginChild("##miscaimbot", ImVec2(265, 500), true);
		{
			ImGui::Separator();
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Text("backtrack");
			ImGui::Separator();
		}
		ImGui::EndChild();

		ImGui::PopFont();
	}
}

void DrawVisualTab()
{
	auto& style = ImGui::GetStyle();
	ImGui::Columns(2, "yes", false);
	ImGui::Text("Effects");
	ImGui::BeginChild("##effects", ImVec2(250, 350), true);
	{
		TextColor(true);
		ImGui::Separator();
		ImGui::Checkbox("Remove smoke grenades", &Clientvariables->Visuals.Nosmoke);
		ImGui::Checkbox("Remove fog", &Clientvariables->Visuals.RemoveParticles);
		ImGui::Checkbox("Remove visual recoil", &Clientvariables->Visuals.Novisrevoil);
		ImGui::Checkbox("Remove scope", &Clientvariables->Visuals.Noscope);
		ImGui::Checkbox("Nightmode", &Clientvariables->Visuals.nightmode);

		ImGui::Text("Skybox");
		ImGui::Combo("##Skybox", &Clientvariables->Visuals.Skybox, Skyboxmode, ARRAYSIZE(Skyboxmode));

		ImGui::Text("Thirdperson");
		ImGui::Hotkey("##thirdperson", &Clientvariables->Misc.TPKey, ImVec2(164, 20));
		ImGui::Combo("##angles", &Clientvariables->Misc.TPangles, ThirdpersonAngles, ARRAYSIZE(ThirdpersonAngles));
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	static int visualpage;
	ImGui::Text("Player ESP");
	ImGui::BeginChild("##player", ImVec2(275, 360), true);
	{
		style.ItemSpacing = ImVec2(1, 1);

		ImGui::Separator();
		TextColor(visualpage == 0);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (visualpage == 0) BtnActive(); else BtnNormal();
		if (ImGui::Button("Visuals", ImVec2(56, 50)))
			visualpage = 0;

		ImGui::SameLine();

		TextColor(visualpage == 1);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (visualpage == 1) BtnActive(); else BtnNormal();
		if (ImGui::Button("Models", ImVec2(56, 50)))
			visualpage = 1;

		ImGui::SameLine();

		TextColor(visualpage == 2);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (visualpage == 2) BtnActive(); else BtnNormal();
		if (ImGui::Button("Others", ImVec2(64, 50)))
			visualpage = 2;

		ImGui::SameLine();

		TextColor(visualpage == 3);
		buttonColor(false);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (visualpage == 3) BtnActive(); else BtnNormal();
		if (ImGui::Button("Filters", ImVec2(64, 50)))
			visualpage = 3;

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::NewLine();
		style.ItemSpacing = ImVec2(7, 7);

		if (visualpage == 0)
		{
			TextColor(true);
			ImGui::Checkbox("Enable", &Clientvariables->Visuals.EspEnable);
			ImGui::Checkbox("Bounding box", &Clientvariables->Visuals.BoundingBox);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##box", Clientvariables->Colors.BoundingBox, 1 << 7);
			ImGui::Checkbox("Health bar", &Clientvariables->Visuals.Health);
			ImGui::Checkbox("Name", &Clientvariables->Visuals.Name);
			ImGui::Checkbox("Weapons", &Clientvariables->Visuals.Weapon);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##colorweapons", Clientvariables->Colors.WeaponsCAT, 1 << 7);
			ImGui::Checkbox("Ammo", &Clientvariables->Visuals.Ammo);
			ImGui::Checkbox("Resolver", &Clientvariables->Visuals.Flags);
			ImGui::Checkbox("Armor", &Clientvariables->Visuals.Armor);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##armorcolor", Clientvariables->Colors.ArmorColor, 1 << 7);
			ImGui::Checkbox("Line of sight", &Clientvariables->Visuals.SnapLines);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##snapline", Clientvariables->Colors.Snaplines, 1 << 7);
			ImGui::Checkbox("Skeleton", &Clientvariables->Visuals.Bones);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##bones", Clientvariables->Colors.Skeletons, 1 << 7);
			ImGui::Checkbox("Glow", &Clientvariables->Visuals.Glow);
			if (Clientvariables->Visuals.Glow) {
				ImGui::SliderFloat("##glowforce", &Clientvariables->Visuals.Opacity, 0.f, 1.f, "%.2f%%");
				ImGui::SameLine(216);
				ImGui::MyColorEdit3("##glow", Clientvariables->Colors.Glow, 1 << 7);
			}
			ImGui::Text("Hitmarker");
			ImGui::Combo("##hitmarkerwav", &Clientvariables->Visuals.Hitmarker, Hitmarkerwavs, ARRAYSIZE(Hitmarkerwavs));
			ImGui::Separator();
		}
		if (visualpage == 1)
		{
			TextColor(true);
			ImGui::Checkbox("Enable", &Clientvariables->Visuals.ChamsEnable);
			if (Clientvariables->Visuals.ChamsEnable) {
				ImGui::Combo("##texture", &Clientvariables->Visuals.ChamsStyle, ModelsMode, ARRAYSIZE(ModelsMode));
			}

			ImGui::Checkbox("Player", &Clientvariables->Visuals.ChamsPlayer);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##chams", Clientvariables->Colors.PlayerChams, 1 << 7);
			ImGui::Checkbox("Player (behind wall)", &Clientvariables->Visuals.ChamsPlayerWall);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##chamswall", Clientvariables->Colors.PlayerChamsWall, 1 << 7);

			ImGui::Checkbox("Hand Chams", &Clientvariables->Visuals.HandEnableChams);
			if (Clientvariables->Visuals.HandEnableChams)
			{
				ImGui::Checkbox("Hand chams", &Clientvariables->Visuals.ChamsHands);
				ImGui::SameLine(216);
				ImGui::MyColorEdit3("##handchams", Clientvariables->Colors.ChamsHand, 1 << 7);

				ImGui::Checkbox("Hand wireframe", &Clientvariables->Visuals.ChamsHandsWireframe);
				ImGui::SameLine(216);
				ImGui::MyColorEdit3("##handwireframe", Clientvariables->Colors.WireframeHand, 1 << 7);
			}

			ImGui::Checkbox("Weapon Chams", &Clientvariables->Visuals.WeaponEnableChams);
			if (Clientvariables->Visuals.WeaponEnableChams)
			{
				ImGui::Checkbox("Weapon chams", &Clientvariables->Visuals.WeaponChams);
				ImGui::SameLine(216);
				ImGui::MyColorEdit3("##weaponchams", Clientvariables->Colors.ChamsWeapon, 1 << 7);

				ImGui::Checkbox("Weapon wireframe", &Clientvariables->Visuals.WeaponWireframe);
				ImGui::SameLine(216);
				ImGui::MyColorEdit3("##weaponwireframe", Clientvariables->Colors.WireframeWeapon, 1 << 7);
			}

			ImGui::Text("Ghost-chams");
			ImGui::Combo("##ghost", &Clientvariables->Visuals.FakeAngleGhost, Fakeghost, ARRAYSIZE(Fakeghost));
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##ghostcolor", Clientvariables->Colors.FakeAngleGhost, 1 << 7);
			ImGui::Separator();
		}
		if (visualpage == 2)
		{
			TextColor(true);
			ImGui::Text("Dropped weapons");
			ImGui::Checkbox("event Logs", &Clientvariables->Visuals.EventLogs);
			ImGui::Combo("##droppedweapon", &Clientvariables->Visuals.DroppedWeapons, DroppedWeapons, ARRAYSIZE(DroppedWeapons));
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##weaponcolor", Clientvariables->Colors.DroppedWeapon, 1 << 7);
			ImGui::Checkbox("Grenades", &Clientvariables->Visuals.ThrownNades);
			ImGui::Checkbox("Bomb", &Clientvariables->Visuals.Bomb);
			ImGui::Checkbox("Grenade trajectory", &Clientvariables->Visuals.GrenadePrediction);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##nadetrajectory", Clientvariables->Colors.GrenadePrediction, 1 << 7);
			ImGui::Checkbox("Bullet tracers", &Clientvariables->Visuals.BulletTracers);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##tracers", Clientvariables->Colors.Bulletracer, 1 << 7);
			ImGui::Checkbox("Damage indicator", &Clientvariables->Visuals.DamageIndicators);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##damageindicator", Clientvariables->Colors.DamageIndicator, 1 << 7);
			ImGui::Checkbox("Asus props", &Clientvariables->Visuals.TransparensWallsEnable);
			ImGui::Separator();
		}
		if (visualpage == 3)
		{
			TextColor(true);
			ImGui::Checkbox("Enemy only", &Clientvariables->Visuals.EnemyOnly);
			ImGui::Checkbox("Self ESP", &Clientvariables->Visuals.LocalPlayer);
			ImGui::Checkbox("Self Chams", &Clientvariables->Visuals.LocalChams);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##localchams", Clientvariables->Colors.LocalChams, 1 << 7);
			ImGui::Checkbox("Self Glow", &Clientvariables->Visuals.LocalGlow);
			ImGui::SameLine(216);
			ImGui::MyColorEdit3("##localglow", Clientvariables->Colors.LocalGlow, 1 << 7);
			ImGui::Separator();
		}
	}
	ImGui::EndChild();

}

void DrawMiscTab()
{
	auto& style = ImGui::GetStyle();
	ImGui::Columns(2, "yes", false);
	TextColor(true);
	ImGui::Text("Miscellaneous");
	ImGui::BeginChild("##misc", ImVec2(250, 220), true);
	{
		ImGui::Separator();
		ImGui::Checkbox("Clan-tag", &Clientvariables->Misc.clantag);
		ImGui::Checkbox("Anti-untrusted", &Clientvariables->Misc.AntiUT);
	    ImGui::Checkbox("Enable buybot", &Clientvariables->Misc.buybot);
		if (Clientvariables->Misc.buybot) {
			ImGui::Text("Buy bot");
			ImGui::Combo("##buybot", &Clientvariables->Misc.buybot2, BuyBot, ARRAYSIZE(BuyBot));
			ImGui::Checkbox("Helmet and grenades", &Clientvariables->Misc.buybot3);
		}
		ImGui::Checkbox("Bunnyhop", &Clientvariables->Misc.AutoJump);
		ImGui::Checkbox("Air strafe", &Clientvariables->Misc.AutoStrafe);
		if (Clientvariables->Misc.AutoStrafe) {
			ImGui::Text("Circle-Strafe");
			ImGui::Hotkey("##speedkey", &Clientvariables->Misc.PrespeedKey, ImVec2(150, 20));
			ImGui::SliderInt("##retrack", &Clientvariables->Misc.Retrack, 1, 8);
		}
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::Text("Fake-lag");

	ImGui::BeginChild("##fakelag", ImVec2(246, 130), true);
	{
		ImGui::Separator();
		ImGui::Checkbox("Enable", &Clientvariables->Misc.FakelagEnable);
		ImGui::Combo("##mode", &Clientvariables->Misc.FakelagMode, FakelagMode, ARRAYSIZE(FakelagMode));

		ImGui::Text("Ticks");
		ImGui::SliderInt("##choke", &Clientvariables->Misc.FakelagAmount, 1, 14);

		ImGui::Checkbox("Fakelag on ground", &Clientvariables->Misc.FakelagOnground);
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::Text("Settings");
	ImGui::BeginChild("##settings", ImVec2(246, 300), true);
	{
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::SameLine(40);
		ImGui::Text("Menu color");
		ImGui::SameLine(216);
		ImGui::MyColorEdit3("##menucolor", Clientvariables->Colors.MenuColor, 1 << 7);

		ImGui::NewLine();
		ImGui::SameLine(40);
		ImGui::Text("Menu key");
		ImGui::NewLine();
		ImGui::SameLine(35);
		ImGui::Hotkey("##menukey", &Clientvariables->Misc.MenuKey, ImVec2(150, 20));

		ImGui::NewLine();
		style.Colors[ImGuiCol_Button] = ImColor(26, 28, 33, 180);
		ImGui::SameLine(40);
		ImGui::Combo("##cfg", &Clientvariables->Misc.ConfigSelection, Configs, ARRAYSIZE(Configs));

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Save", ImVec2(156, 27)))
			ConSys->SaveConfig();

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Load", ImVec2(156, 27)))
			ConSys->LoadConfig();

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Reset", ImVec2(156, 27)))
			ConSys->Reset();

	}
	ImGui::EndChild();

	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "buff.xyz");
	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "[<3]");

}

HRESULT __stdcall Hooks::D3D9_EndScene(IDirect3DDevice9* pDevice)
{
	HRESULT result = d3d9VMT->GetOriginalMethod<EndSceneFn>(42)(pDevice);

	if (!G::Init) {
		GUI_Init(pDevice);
	}
	else {
		if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
			ImGui::GetIO().MouseDrawCursor = G::Opened;
		else
			ImGui::GetIO().MouseDrawCursor = true;
		ImGui_ImplDX9_NewFrame();

		auto& style = ImGui::GetStyle();

		if (G::Opened && style.Alpha < 1.f)
		{
			G::ShowMenu = true;
			if (style.Alpha > 1.f)
				style.Alpha = 1.f;
			else if (style.Alpha != 1.f)
				style.Alpha += 0.03f;
		}
		else if (style.Alpha > 0.f)
		{
			if (style.Alpha < 0.f)
				style.Alpha = 0.f;
			else if (style.Alpha != 0.f)
				style.Alpha -= 0.03f;
			if (style.Alpha == 0.f)
				G::ShowMenu = false;
		}

		static int tab;

		if (G::ShowMenu) {
			ImGui::PushFont(Default);
			ImVec2 mainWindowPos;
			color();
			ImGui::SetNextWindowSize(ImVec2(570, 530));
			style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
			int w, h;
			g_pEngine->GetScreenSize(w, h);
			ImGui::Begin(XorStr("buff"), &G::ShowMenu, ImVec2(575, 680), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
			{
				style.ItemSpacing = ImVec2(0, 0);

				if (tab == 0) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("HVH", ImVec2(138, 41))) tab = 0;
				ImGui::SameLine();

				if (tab == 1) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("LEGIT", ImVec2(138, 41))) tab = 1;
				ImGui::SameLine();

				if (tab == 2) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("VISUALS", ImVec2(138, 41))) tab = 2;
				ImGui::SameLine();

				if (tab == 3) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("MISC", ImVec2(138, 41))) tab = 3;

				ImGui::PushFont(Default);

				TextColor(true);
				color();
				style.ItemSpacing = ImVec2(8, 6);
				style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 0);
				style.WindowPadding = ImVec2(8, 8); 
				ImGui::BeginChild(XorStr("masterchild"), ImVec2(550, 550), true);
				{
					if (tab == 0) {
						DrawRageTab();
					}
					else if (tab == 1) {
						DrawLegitTab();
					}
					else if (tab == 2) {
						DrawVisualTab();
					}
					else if (tab == 3) {
						DrawMiscTab();
					}

				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
		ImGui::Render();
	}
	return result;
}

HRESULT __stdcall Hooks::hkdReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!G::Init)
		return oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto newr = oResetScene(pDevice, pPresParam);
	ImGui_ImplDX9_CreateDeviceObjects();

	return newr;
}
































































































































































































































































































































































































































































