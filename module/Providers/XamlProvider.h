#pragma once
#include <NsGui/XamlProvider.h>
#include <filesystem>

namespace Cry 
{
namespace Ns
{
	class CXamlProvider final : public Noesis::XamlProvider
	{
	public:
		CXamlProvider();
		~CXamlProvider();

		virtual Noesis::Ptr<Noesis::Stream> LoadXaml(const char* uri) override;

		void ReloadAllXaml();
		void ReloadSpecificXaml(const char* uri);

		void AddSearchPath(const char* path);
		void RemoveSearchPath(const char* path);
	protected: 

		std::set<std::string> m_xamls;

		std::set<std::filesystem::path> m_searchPaths;
	};

}
}