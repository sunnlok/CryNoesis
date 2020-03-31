#pragma once
#include <NsGui/XamlProvider.h>

namespace Cry 
{
namespace Ns
{
	class CXamlProvider final : public Noesis::XamlProvider
	{
	public:
		virtual Noesis::Ptr<Noesis::Stream> LoadXaml(const char* uri) override;
	};

}
}