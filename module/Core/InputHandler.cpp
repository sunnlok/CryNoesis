#include "StdAfx.h"
#include "InputHandler.h"
#include "Implementation.h"
#include "CryInput/IHardwareMouse.h"

#include <NsGui/IView.h>
#include <NsGui/InputEnums.h>



Cry::Ns::CInputHandler::CInputHandler(CImplementation* pImpl)
	: m_pImplementation(pImpl)
{
	gEnv->pInput->AddEventListener(this);
	gEnv->pHardwareMouse->AddListener(this);

	//Map all the input events
	{
		using namespace Noesis;

		m_keyMap = {
			{ eKI_Escape , Key_Escape },
			{ eKI_1 , Key_D1 },
			{ eKI_2 , Key_D2 },
			{ eKI_3 , Key_D3 },
			{ eKI_4 , Key_D4 },
			{ eKI_5 , Key_D5 },
			{ eKI_6 , Key_D6 },
			{ eKI_7 , Key_D7 },
			{ eKI_8 , Key_D8 },
			{ eKI_9 , Key_D9 },
			{ eKI_0 , Key_D0 },
			{ eKI_Minus , Key_None },
			{ eKI_Equals , Key_None },
			{ eKI_Backspace , Key_Return },
			{ eKI_Tab , Key_None },
			{ eKI_Q , Key_Q },
			{ eKI_W , Key_W },
			{ eKI_E , Key_E },
			{ eKI_R , Key_R },
			{ eKI_T , Key_T },
			{ eKI_Y , Key_Y },
			{ eKI_U , Key_U },
			{ eKI_I , Key_I },
			{ eKI_O , Key_O },
			{ eKI_P , Key_P },
			{ eKI_LBracket , Key_None },
			{ eKI_RBracket , Key_None },
			{ eKI_Enter , Key_Enter },
			{ eKI_LCtrl , Key_None },
			{ eKI_A , Key_A },
			{ eKI_S , Key_S },
			{ eKI_D , Key_D },
			{ eKI_F , Key_F },
			{ eKI_G , Key_G },
			{ eKI_H , Key_H },
			{ eKI_J , Key_J },
			{ eKI_K , Key_K },
			{ eKI_L , Key_L },
			{ eKI_Semicolon , Key_None },
			{ eKI_Apostrophe , Key_None },
			{ eKI_Tilde , Key_None },
			{ eKI_LShift , Key_LeftShift },
			{ eKI_Backslash , Key_None },

			{ eKI_Z , Key_Z },
			{ eKI_X , Key_X },
			{ eKI_C , Key_C },
			{ eKI_V , Key_V },
			{ eKI_B , Key_B },
			{ eKI_N , Key_N },
			{ eKI_M , Key_M },
			{ eKI_Comma , Key_None },
			{ eKI_Period , Key_None },
			{ eKI_Slash , Key_None },
			{ eKI_RShift , Key_RightShift },
			{ eKI_NP_Multiply , Key_None },
			{ eKI_LAlt , Key_LeftAlt },
			{ eKI_Space , Key_Space },
			{ eKI_CapsLock , Key_CapsLock },
			{ eKI_F1 , Key_F1 },
			{ eKI_F2 , Key_F2 },
			{ eKI_F3 , Key_F3 },
			{ eKI_F4 , Key_F4 },
			{ eKI_F5 , Key_F5 },
			{ eKI_F6 , Key_F6 },
			{ eKI_F7 , Key_F7 },
			{ eKI_F8 , Key_F8 },
			{ eKI_F9 , Key_F9 },
			{ eKI_F10 , Key_F10 },
			{ eKI_NumLock , Key_NumLock },
			{ eKI_ScrollLock , Key_Scroll },
			{ eKI_NP_7 , Key_NumPad7 },
			{ eKI_NP_8 , Key_NumPad8 },
			{ eKI_NP_9 , Key_NumPad9 },
			{ eKI_NP_Substract , Key_Subtract },
			{ eKI_NP_4 , Key_NumPad4 },
			{ eKI_NP_5 , Key_NumPad5 },
			{ eKI_NP_6 , Key_NumPad6 },
			{ eKI_NP_Add , Key_Add },
			{ eKI_NP_1 , Key_NumPad1 },
			{ eKI_NP_2 , Key_NumPad2 },
			{ eKI_NP_3 , Key_NumPad3 },
			{ eKI_NP_0 , Key_NumPad0 },
			{ eKI_F11 , Key_F11 },
			{ eKI_F12 , Key_F12 },
			{ eKI_F13 , Key_F13 },
			{ eKI_F14 , Key_F14 },
			{ eKI_F15 , Key_F15 },
			{ eKI_Colon , Key_None },
			{ eKI_Underline , Key_None },
			{ eKI_NP_Enter , Key_Enter },
			{ eKI_RCtrl , Key_RightCtrl },
			{ eKI_NP_Period , Key_Decimal },
			{ eKI_NP_Divide , Key_Divide },
			{ eKI_Print , Key_None },
			{ eKI_RAlt , Key_RightAlt },
			{ eKI_Pause , Key_None },
			{ eKI_Home , Key_Home },
			{ eKI_Up , Key_Up },
			{ eKI_PgUp , Key_PageUp },
			{ eKI_Left , Key_Left },
			{ eKI_Right , Key_Right },
			{ eKI_End , Key_End },
			{ eKI_Down , Key_Down },
			{ eKI_PgDn , Key_PageDown },
			{ eKI_Insert , Key_Insert },
			{ eKI_Delete , Key_Delete },
			{ eKI_LWin , Key_LWin },
			{ eKI_RWin , Key_RWin },
			{ eKI_Apps , Key_Apps },
			{ eKI_OEM_102 , Key_None },

			{ eKI_XI_DPadUp , Key_GamepadUp },
				{eKI_XI_DPadDown, Key_GamepadDown },
				{eKI_XI_DPadLeft, Key_GamepadLeft },
				{eKI_XI_DPadRight, Key_GamepadRight },
				{eKI_XI_Start, Key_GamepadMenu },
				{eKI_XI_Back, Key_GamepadCancel },
				{eKI_XI_ThumbL, Key_None},
				{eKI_XI_ThumbR, Key_None },
				{eKI_XI_ShoulderL, Key_GamepadPageLeft },
				{eKI_XI_ShoulderR, Key_GamepadPageRight },
				{eKI_XI_A, Key_GamepadAccept },
				{eKI_XI_B, Key_GamepadCancel },
				{eKI_XI_X, Key_None },
				{eKI_XI_Y, Key_None },
				{eKI_XI_TriggerL, Key_None },
				{eKI_XI_TriggerR, Key_None },
				{eKI_XI_ThumbLX, Key_None },
				{eKI_XI_ThumbLY, Key_None },
				{eKI_XI_ThumbLUp, Key_None },
				{eKI_XI_ThumbLDown, Key_None },
				{eKI_XI_ThumbLLeft, Key_None },
				{eKI_XI_ThumbLRight, Key_None },
				{eKI_XI_ThumbRX, Key_None },
				{eKI_XI_ThumbRY, Key_None },
				{eKI_XI_ThumbRUp, Key_GamepadUp },
				{eKI_XI_ThumbRDown, Key_GamepadDown },
				{eKI_XI_ThumbRLeft, Key_GamepadLeft },
				{eKI_XI_ThumbRRight, Key_GamepadRight },
				{eKI_XI_TriggerLBtn, Key_None },
				{eKI_XI_TriggerRBtn, Key_None },

			{ eKI_Orbis_Options , Key_GamepadMenu },
			{ eKI_Orbis_L3, Key_GamepadPageLeft },
			{ eKI_Orbis_R3, Key_GamepadPageRight },
			{ eKI_Orbis_Up, Key_GamepadUp },
			{ eKI_Orbis_Right, Key_GamepadRight },
			{ eKI_Orbis_Down, Key_GamepadDown },
			{ eKI_Orbis_Left, Key_GamepadLeft },
			{ eKI_Orbis_L2, Key_None },
			{ eKI_Orbis_R2, Key_None },
			{ eKI_Orbis_L1, Key_None },
			{ eKI_Orbis_R1, Key_None },
			{ eKI_Orbis_Triangle, Key_None },
			{ eKI_Orbis_Circle, Key_None },
			{ eKI_Orbis_Cross, Key_GamepadAccept },
			{ eKI_Orbis_Square, Key_GamepadCancel },
			{ eKI_Orbis_StickLX, Key_None },
			{ eKI_Orbis_StickLY, Key_None },
			{ eKI_Orbis_StickRX, Key_None },
			{ eKI_Orbis_StickRY, Key_None },
			{ eKI_Orbis_RotX, Key_None },
			{ eKI_Orbis_RotY, Key_None },
			{ eKI_Orbis_RotZ, Key_None },
			{ eKI_Orbis_RotX_KeyL, Key_None },
			{ eKI_Orbis_RotX_KeyR, Key_None },
			{ eKI_Orbis_RotZ_KeyD, Key_None },
			{ eKI_Orbis_RotZ_KeyU, Key_None },
			{ eKI_Orbis_LeftTrigger, Key_None },
			{ eKI_Orbis_RightTrigger, Key_None },

		};
	}
}

Cry::Ns::CInputHandler::~CInputHandler()
{
	gEnv->pHardwareMouse->RemoveListener(this);
	gEnv->pInput->RemoveEventListener(this);
}

static int MapModifier(int in)
{
	int out = 0;

	using namespace Noesis;

	if (in & eMM_Ctrl)
		out |= ModifierKeys_Control;
	if (in & eMM_Alt)
		out |= ModifierKeys_Alt;
	if (in & eMM_Shift)
		out |= ModifierKeys_Shift;
	if (in & eMM_Win)
		out |= ModifierKeys_Windows;

	return out;
}


bool Cry::Ns::CInputHandler::HandleMouseEvent(const SInputEvent& event, int mappedModifier)
{
	auto& views = Cry::Ns::ViewManager::Get()->GetViews();

	int mouseKey = event.keyId - KI_MOUSE_BASE;

	//Mouse positions
	float x, y;
	//This needs to be adjusted for sandbox
	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x,&y);

	for (auto& pView : views)
	{
		if (mouseKey <= 3)
		{
			if (event.state == eIS_Pressed)
			{
				if (pView.pView->MouseButtonDown(x, y, (Noesis::MouseButton)mouseKey))
					return true;
			}	
			else if (event.state == eIS_Released)
			{
				if (pView.pView->MouseButtonUp(x, y, (Noesis::MouseButton)mouseKey))
					return true;
			}
		}
		else if ((event.keyId == eKI_MouseWheelUp || event.keyId == eKI_MouseWheelDown) && event.state == eIS_Down)
		{
			if (pView.pView->MouseWheel((int)x, (int)y, (int)event.value * 120))
				return true;
		}
		/*else if (event.keyId == eKI_MouseX || event.keyId == eKI_MouseY || event.keyId == eKI_MouseXAbsolute || event.keyId == eKI_MouseYAbsolute)
		{
			if (pView->MouseMove((int)x, (int)y))
				return true;
		}	*/
	}
	return false;
}

bool Cry::Ns::CInputHandler::HandleKeyEvent(const SInputEvent& event, int mappedModifier)
{
	auto& views = Cry::Ns::ViewManager::Get()->GetViews();
	for (auto& pView : views)
	{
		if (event.state == eIS_Pressed)
		{
			if (pView.pView->KeyDown(m_keyMap[event.keyId]))
				return true;
		}
		else if (event.state == eIS_Released)
		{
			if (pView.pView->KeyUp(m_keyMap[event.keyId]))
				return true;	
		}
	}
	return false;
}

bool Cry::Ns::CInputHandler::OnInputEvent(const SInputEvent& event)
{
	int modifiers = MapModifier(event.modifiers);


	if (event.keyId >= KI_MOUSE_BASE && event.keyId < eKI_MouseLast)
	{
		return HandleMouseEvent(event, modifiers);
	}
	else
	{
		return HandleKeyEvent(event, modifiers);
	}
}

bool Cry::Ns::CInputHandler::OnInputEventUI(const SUnicodeEvent& event)
{
	auto& views = Cry::Ns::ViewManager::Get()->GetViews();
	for (auto& pView : views)
	{
		if (pView.pView->Char(event.inputChar))
			return true;
	}

	return false;
}

int Cry::Ns::CInputHandler::GetPriority() const
{
	//The method or operation is not implemented.
	return 0;
}

void Cry::Ns::CInputHandler::OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta /*= 0*/)
{	
	if (eHardwareMouseEvent == HARDWAREMOUSEEVENT_MOVE && gEnv->pHardwareMouse->IsCursorVisible())
	{
		float x, y;

		gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x, &y);

		auto& views = Cry::Ns::ViewManager::Get()->GetViews();
		for (auto& pView : views)
		{
			if (pView.pView->MouseMove((int)x, (int)y))
				break;
		}
	}
}

