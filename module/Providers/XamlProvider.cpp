#include "StdAfx.h"
#include "XamlProvider.h"
#include "NsCore/Ptr.h"
#include "FileProvider.h"
#include <filesystem>



Noesis::Ptr<Noesis::Stream> Cry::Ns::CXamlProvider::LoadXaml(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri);

	if (!path.has_extension())
		path += std::filesystem::path(".xaml");

	std::filesystem::path basePath = "UI";
	basePath /= path;

	std::string pathString = basePath.string();


	return CPakStream::Open(pathString.c_str());
}
