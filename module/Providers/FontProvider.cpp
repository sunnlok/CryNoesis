#include "StdAfx.h"
#include <NsCore/Ptr.h>
#include "FontProvider.h"
#include "FileProvider.h"



void Cry::Ns::CFontProvider::ScanFolder(const char* folder)
{
	ScanFolder(folder, "otf");
	ScanFolder(folder, "ttf");
	ScanFolder(folder, "ttc");
}

void Cry::Ns::CFontProvider::ScanFolder(const char* folder, const char* ext)
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/*.%s", rootPath, folder, ext);

	_finddata_t findData;
	auto handle = gEnv->pCryPak->FindFirst(path, &findData);
	if (handle != -1)
	{
		do 
		{	
			RegisterFont(folder, findData.name);

		} while (gEnv->pCryPak->FindNext(handle, &findData) != -1);

		gEnv->pCryPak->FindClose(handle);
	}
}

Noesis::Ptr<Noesis::Stream> Cry::Ns::CFontProvider::OpenFont(const char* folder, const char* filename) const
{
	stack_string path;
	const char* rootPath = "."; //Make cvar
	path.Format("%s/%s/%s", rootPath, folder, filename);

	return Cry::Ns::CPakStream::Open(path);
}
