#pragma once
#include <NsRender/Texture.h>
#include <NsRender/RenderTarget.h>
#include "CryCore/smartptr.h"

struct ITexture;
namespace Cry
{
	namespace Ns
	{

		class CTextureWrapper final : public Noesis::Texture
		{
		public:
			CTextureWrapper() = default;
			CTextureWrapper(ITexture* pTexture) : m_pTexture(pTexture) {};
			CTextureWrapper(_smart_ptr<ITexture> pTexture) : m_pTexture(pTexture) {}
			virtual ~CTextureWrapper() = default;

			CTextureWrapper(const CTextureWrapper& other)				= default;
			CTextureWrapper(CTextureWrapper&& other)					= default;
			CTextureWrapper& operator=(CTextureWrapper&& other)			= default;
			CTextureWrapper& operator=(const CTextureWrapper& other)	= default;

			virtual uint32_t GetWidth() const override;

			virtual uint32_t GetHeight() const override;

			virtual bool HasMipMaps() const override;

			virtual bool IsInverted() const override;

			ITexture* GetActualTexture() const { return m_pTexture.get(); }
		protected:
			_smart_ptr<ITexture> m_pTexture = nullptr;
		};

		class CRenderTarget final : public Noesis::RenderTarget
		{
		public:
			CRenderTarget() = default;
			virtual ~CRenderTarget() = default;

			CRenderTarget(ITexture* pTexture);
			CRenderTarget(_smart_ptr<ITexture> pTexture);

			virtual Noesis::Texture* GetTexture() override;
		private:
			Noesis::Ptr<CTextureWrapper> m_pWrapper = nullptr;
		};
	}
}