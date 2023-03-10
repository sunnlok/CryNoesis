////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <NsGui/XamlProvider.h>
#include <filesystem>

class CXamlProvider final : public Noesis::XamlProvider
{
public:
	CXamlProvider();
	~CXamlProvider();

	virtual Noesis::Ptr<Noesis::Stream> LoadXaml(const Noesis::Uri& uri) override;

	void ReloadAllXaml();
	void ReloadSpecificXaml(const char* uri);

	void AddSearchPath(const char* path);
	void RemoveSearchPath(const char* path);

protected:

	std::set<string> m_xamls;

	std::set<std::filesystem::path> m_searchPaths;
};