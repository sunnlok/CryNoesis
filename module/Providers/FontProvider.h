#pragma once
#include <NsGui/CachedFontProvider.h>

namespace Cry
{
namespace Ns
{
	class CFontProvider final : public Noesis::CachedFontProvider
	{
	public:
		virtual void ScanFolder(const Noesis::Uri& folder) override;

		//virtual Noesis::Ptr<Noesis::Stream> OpenFont(const Uri& folder, const char* filename) const override;

		virtual Noesis::Ptr<Noesis::Stream> OpenFont(const Noesis::Uri& folder, const char* filename) const override;
	private:
		void ScanFolder(const Noesis::Uri& folder, const char* ext);
	};

}
}