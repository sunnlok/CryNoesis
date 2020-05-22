#pragma once
#include "CryInput/IInput.h"
#include "NsGui/KeyboardNavigation.h"
#include "CryInput/IHardwareMouse.h"

namespace Cry::Ns
{
	class CImplementation;


	class CInputHandler : public IInputEventListener, IHardwareMouseEventListener
	{
	public:
		CInputHandler(CImplementation* pImpl);
		~CInputHandler();


		bool OnInputEvent(const SInputEvent& event) override;
		bool OnInputEventUI(const SUnicodeEvent& event) override;
		int GetPriority() const override;

		bool HandleMouseEvent(const SInputEvent& event, int mappedModifier);
		bool HandleKeyEvent(const SInputEvent& event, int mappedModifier);

		void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta = 0) override;

	protected:
		std::unordered_map<EKeyId,Noesis::Key> m_keyMap;


		std::array< Noesis::Key, eKI_MouseLast - KI_MOUSE_BASE> m_mouseButtons;

		CImplementation* m_pImplementation;
	};
}