#pragma once
#include <NsGui/TextureProvider.h>


namespace Cry
{
namespace Ns
{
	class CTextureProvider final : public Noesis::TextureProvider
	{
	public:
		virtual Noesis::TextureInfo				GetTextureInfo(const Noesis::Uri& uri) override;
		virtual Noesis::Ptr<Noesis::Texture>	LoadTexture(const Noesis::Uri& uri, Noesis::RenderDevice* device) override;
	};
}
}