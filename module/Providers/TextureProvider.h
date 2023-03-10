////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <NsGui/TextureProvider.h>

class CTextureProvider final : public Noesis::TextureProvider
{
public:
    virtual Noesis::TextureInfo GetTextureInfo(const Noesis::Uri& uri) override;
    virtual Noesis::Ptr<Noesis::Texture> LoadTexture(const Noesis::Uri& uri, Noesis::RenderDevice* device) override;
};