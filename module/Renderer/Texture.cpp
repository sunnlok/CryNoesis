#include "StdAfx.h"

#include "Texture.h"

uint32_t CTextureWrapper::GetWidth() const
{
    if (!m_pTexture)
        return 0;

    return m_pTexture->GetWidth();
}

uint32_t CTextureWrapper::GetHeight() const
{
    if (!m_pTexture)
        return 0;

    return m_pTexture->GetHeight();
}

bool CTextureWrapper::HasMipMaps() const
{
    if (!m_pTexture)
        return false;

    return m_pTexture->GetNumMips() > 1;
}

bool CTextureWrapper::IsInverted() const
{
    return false;
}

bool CTextureWrapper::HasAlpha() const
{
    return false;
}

CRenderTarget::CRenderTarget(ITexture* pColor, ITexture* pDepth)
{
    m_target = Noesis::MakePtr<CTextureWrapper>(pColor);
    m_depthTarget = Noesis::MakePtr<CTextureWrapper>(pDepth);

    m_pColorTex = pColor;
    m_pDepthTex = pDepth;
}

CRenderTarget::CRenderTarget(_smart_ptr<ITexture> pColor, _smart_ptr<ITexture> pDepth)
{
    m_target = Noesis::MakePtr<CTextureWrapper>(pColor);
    m_depthTarget = Noesis::MakePtr<CTextureWrapper>(pDepth);

    m_pColorTex = pColor;
    m_pDepthTex = pDepth;
}

Noesis::Texture* CRenderTarget::GetTexture()
{
    return m_target;
}
