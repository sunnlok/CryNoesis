#pragma once
#include <NsGui/TextureProvider.h>


namespace Cry
{
namespace Ns
{
	class CTextureProvider final : public Noesis::TextureProvider
	{
	public:
		virtual Noesis::TextureInfo				GetTextureInfo(const char* uri) override;
		virtual Noesis::Ptr<Noesis::Texture>	LoadTexture(const char* uri, Noesis::RenderDevice* device) override;
	};
}
}