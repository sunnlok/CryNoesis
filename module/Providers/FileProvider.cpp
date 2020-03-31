#include "StdAfx.h"
#include "FileProvider.h"
#include "CrySystem/File/ICryPak.h"
#include <NsCore/Ptr.h>

using namespace Cry;

Ns::CPakStream::CPakStream(const char* uri)
{
	auto handle = gEnv->pCryPak->FOpen(uri, "r");
	if (!handle)
		return;
}

Ns::CPakStream::~CPakStream()
{
	if (!m_handle)
		return;

	gEnv->pCryPak->FClose(m_handle);
	m_handle = nullptr;
}

void Ns::CPakStream::SetPosition(uint32_t pos)
{
	if (!m_handle)
		return;

	gEnv->pCryPak->FSeek(m_handle, pos, SEEK_SET);
}

uint32_t Ns::CPakStream::GetPosition() const
{
	if (!m_handle)
		return 0;

	return (uint32)gEnv->pCryPak->FTell(m_handle);
}

uint32_t Ns::CPakStream::GetLength() const
{
	if (!m_handle)
		return 0;

	return gEnv->pCryPak->FGetSize(m_handle);
}

uint32_t Ns::CPakStream::Read(void* buffer, uint32_t size)
{
	if (!m_handle)
		return 0;

	return gEnv->pCryPak->FReadRaw(buffer, 1, size, m_handle);
}

Noesis::Ptr<Ns::CPakStream> Ns::CPakStream::Open(const char* uri)
{
	auto handle = gEnv->pCryPak->FOpen(uri, "rb");
	if (!handle)
		return nullptr;

	return Noesis::MakePtr<Ns::CPakStream>(handle);
}

void Cry::Ns::CPakStream::Close()
{
	if (!m_handle)
		return;

	gEnv->pCryPak->FClose(m_handle);
	m_handle = nullptr;
}
