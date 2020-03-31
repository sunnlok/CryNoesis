#include "StdAfx.h"
#include "TextureProvider.h"
#include "NsCore/Ptr.h"
#include "CryRenderer/IRenderer.h"
#include "Renderer/Texture.h"


Noesis::TextureInfo Cry::Ns::CTextureProvider::GetTextureInfo(const char* uri)
{
	_smart_ptr<ITexture> pTexture;
	pTexture.Assign_NoAddRef(gEnv->pRenderer->EF_LoadTexture(uri));

	if (!pTexture)
		return { 0, 0 };

	return { (uint32)pTexture->GetWidth(), (uint32)pTexture->GetHeight() };
}

Noesis::Ptr<Noesis::Texture> Cry::Ns::CTextureProvider::LoadTexture(const char* uri, Noesis::RenderDevice* device)
{

	auto pTextureRaw = gEnv->pRenderer->EF_LoadTexture(uri);
	if (!pTextureRaw)
		return nullptr;

	_smart_ptr<ITexture> pTexture;
	pTexture.Assign_NoAddRef(pTexture);

	return Noesis::MakePtr<CTextureWrapper>(std::move(pTexture));
}

