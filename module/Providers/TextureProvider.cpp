#include "StdAfx.h"
#include "TextureProvider.h"

#include "Renderer/Texture.h"
#include <CryRenderer/IRenderer.h>
#include <NsCore/Ptr.h>
#include <NsGui/Uri.h>
#include <CryCore/smartptr.h>

Noesis::TextureInfo CTextureProvider::GetTextureInfo(const Noesis::Uri& uri)
{
	auto pTextureRaw = gEnv->pRenderer->EF_LoadTexture(uri.Str());
	if (!pTextureRaw)
		return {0,0};

	_smart_ptr<ITexture> pTexture;
	pTexture.Assign_NoAddRef(pTexture);

	return { (uint32)pTexture->GetWidth(), (uint32)pTexture->GetHeight() };
}

Noesis::Ptr<Noesis::Texture> CTextureProvider::LoadTexture(const Noesis::Uri& uri, Noesis::RenderDevice* device)
{
	auto pTextureRaw = gEnv->pRenderer->EF_LoadTexture(uri.Str());
	if (!pTextureRaw)
		return nullptr;

	_smart_ptr<ITexture> pTexture;
	pTexture.Assign_NoAddRef(pTexture);

	return Noesis::MakePtr<CTextureWrapper>(std::move(pTexture));
}
