#include "StdAfx.h"

#include "InputHandler.h"

CInputHandler::CInputHandler(CImplementation* pImpl)
	: m_pImplementation(pImpl)
{
	gEnv->pInput->AddEventListener(this);
	gEnv->pHardwareMouse->AddListener(this);
}

CInputHandler::~CInputHandler()
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

bool CInputHandler::OnInputEvent(const SInputEvent& event)
{
	return false;
}

bool CInputHandler::OnInputEventUI(const SUnicodeEvent& event)
{
	return false;
}

int CInputHandler::GetPriority() const
{
	return 0;
}

bool CInputHandler::HandleMouseEvent(const SInputEvent& event, int mappedModifier)
{
	return false;
}

bool CInputHandler::HandleKeyEvent(const SInputEvent& event, int mappedModifier)
{
	return false;
}

void CInputHandler::OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta)
{
}
