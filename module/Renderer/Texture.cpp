#include "StdAfx.h"
#include "Texture.h"
#include "CryRenderer/ITexture.h"

uint32_t Cry::Ns::CTextureWrapper::GetWidth() const
{
	if (!m_pTexture)
		return 0;

	return m_pTexture->GetWidth();
}

uint32_t Cry::Ns::CTextureWrapper::GetHeight() const
{
	if (!m_pTexture)
		return 0;

	return m_pTexture->GetHeight();
}

bool Cry::Ns::CTextureWrapper::HasMipMaps() const
{
	if (!m_pTexture)
		return false;

	return m_pTexture->GetNumMips() > 1;
}

bool Cry::Ns::CTextureWrapper::IsInverted() const
{
	return false;
}

Noesis::Texture* Cry::Ns::CRenderTarget::GetTexture()
{
	return m_pWrapper;
}

Cry::Ns::CRenderTarget::CRenderTarget(ITexture* pTexture)
{
	m_pWrapper = Noesis::MakePtr<CTextureWrapper>(pTexture);
}

Cry::Ns::CRenderTarget::CRenderTarget(_smart_ptr<ITexture> pTexture)
{
	m_pWrapper = Noesis::MakePtr<CTextureWrapper>(pTexture);
}
