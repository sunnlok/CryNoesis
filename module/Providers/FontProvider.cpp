#include "StdAfx.h"

#include "FontProvider.h"
#include "FileProvider.h"
#include <NsCore/Ptr.h>

void CFontProvider::ScanFolder(const Noesis::Uri& folder)
{
	CryLogAlways("%s", folder.Str());

	ScanFolder(folder, "otf");
	ScanFolder(folder, "ttf");
	ScanFolder(folder, "ttc");
}

Noesis::Ptr<Noesis::Stream> CFontProvider::OpenFont(const Noesis::Uri& folder, const char* filename) const
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/%s", rootPath, folder, filename);

	return CPakStream::Open(path);
}

void CFontProvider::ScanFolder(const Noesis::Uri& folder, const char* ext)
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/*.%s/", rootPath, folder.Str(), ext);

	_finddata_t findData;
	auto handle = gEnv->pCryPak->FindFirst(path.c_str(), &findData);
	if (handle != -1)
	{
		do
		{
			RegisterFont(folder, findData.name);

		} while (gEnv->pCryPak->FindNext(handle, &findData) != -1);

		gEnv->pCryPak->FindClose(handle);
	}
}
