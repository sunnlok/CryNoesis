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

::Noesis::Texture* Cry::Ns::CRenderTarget::GetTexture()
{
	return m_target;
}

Cry::Ns::CRenderTarget::CRenderTarget(ITexture* pColor, ITexture* pDepth)
{
	m_target = ::Noesis::MakePtr<CTextureWrapper>(pColor);
	m_depthTarget = ::Noesis::MakePtr<CTextureWrapper>(pDepth);

	m_pColorTex = pColor;
	m_pDepthTex = pDepth;
}

Cry::Ns::CRenderTarget::CRenderTarget(_smart_ptr<ITexture> pColor, _smart_ptr<ITexture> pDepth)
{
	m_target = ::Noesis::MakePtr<CTextureWrapper>(pColor);
	m_depthTarget = ::Noesis::MakePtr<CTextureWrapper>(pDepth);

	m_pColorTex = pColor;
	m_pDepthTex = pDepth;
}
