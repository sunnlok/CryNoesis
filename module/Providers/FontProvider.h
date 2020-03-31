#pragma once
#include <NsGui/CachedFontProvider.h>

namespace Cry
{
namespace Ns
{
	class CFontProvider final : public Noesis::CachedFontProvider
	{
	public:

		virtual void ScanFolder(const char* folder) override;

		virtual Noesis::Ptr<Noesis::Stream> OpenFont(const char* folder, const char* filename) const override;
	private:
		void ScanFolder(const char* path, const char* ext);
	};

}
}