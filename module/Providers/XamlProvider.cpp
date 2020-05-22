#include "StdAfx.h"
#include "XamlProvider.h"
#include "NsCore/Ptr.h"
#include "FileProvider.h"
//#include <filesystem>
#include "CrySystem/ConsoleRegistration.h"


static Cry::Ns::CXamlProvider* g_pProvider = nullptr;

static void ReloadAllCMD(IConsoleCmdArgs* args)
{
	if (g_pProvider == nullptr)
		return;

	g_pProvider->ReloadAllXaml();
}

static void ReloadCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->ReloadSpecificXaml(args->GetArg(1));
}

static void AddSearchPathCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->AddSearchPath(args->GetArg(1));
}

static void RemoveSearchPathCMD(IConsoleCmdArgs* args)
{
	if (args->GetArgCount() < 2 || g_pProvider == nullptr)
		return;

	g_pProvider->RemoveSearchPath(args->GetArg(1));
}


Cry::Ns::CXamlProvider::CXamlProvider()
{
	ConsoleRegistrationHelper::AddCommand("Noesis.ReloadAll", &ReloadAllCMD, 0, "Reloads all loaded xamls");
	ConsoleRegistrationHelper::AddCommand("Noesis.Reload", &ReloadCMD, 0, "Reloads a specific loaded xamls");

	ConsoleRegistrationHelper::AddCommand("Noesis.AddSearchPath", &AddSearchPathCMD, 0, "Adds a path to search for xamls in");
	ConsoleRegistrationHelper::AddCommand("Noesis.RemoveSearchPath", &RemoveSearchPathCMD, 0, "Removes a xaml search path");

	g_pProvider = this;
}

Cry::Ns::CXamlProvider::~CXamlProvider()
{
	g_pProvider = nullptr;
}


Noesis::Ptr<Noesis::Stream> Cry::Ns::CXamlProvider::LoadXaml(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri).make_preferred();

	if (!path.has_extension())
		path += std::filesystem::path(".xaml");

	std::string pathString = path.string();

	auto pStream = CPakStream::Open(pathString.c_str());

	if (!pStream.GetPtr())
	{
		std::filesystem::path currentPath;
		for (auto& searchPath : m_searchPaths)
		{
			currentPath = searchPath;
			currentPath /= path;
			pathString = currentPath.string();

			pStream = CPakStream::Open(pathString.c_str());

			if (pStream.GetPtr())
				break;
		}
	}

	if (pStream.GetPtr())
		m_xamls.emplace(uri);

	return pStream;
}

void Cry::Ns::CXamlProvider::ReloadAllXaml()
{
	for (auto& xaml : m_xamls)
		RaiseXamlChanged(xaml.c_str());
}

void Cry::Ns::CXamlProvider::ReloadSpecificXaml(const char* uri)
{
	auto xaml = m_xamls.find(uri);
	if (xaml != m_xamls.end())
		RaiseXamlChanged(xaml->c_str());
}

void Cry::Ns::CXamlProvider::AddSearchPath(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri).make_preferred();
	
	m_searchPaths.emplace(path);
}

void Cry::Ns::CXamlProvider::RemoveSearchPath(const char* uri)
{
	std::filesystem::path  path = std::filesystem::u8path(uri).make_preferred();
	m_searchPaths.erase(path);
}

