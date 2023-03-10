#include "StdAfx.h"

#include "FileProvider.h"
#include <NsCore/Ptr.h>

CPakStream::CPakStream(const char* uri)
{
    auto handle = gEnv->pCryPak->FOpen(uri, "r");
    if (!handle)
        return;
}

CPakStream::~CPakStream()
{
    if (!m_handle)
        return;

    gEnv->pCryPak->FClose(m_handle);
    m_handle = nullptr;
}

void CPakStream::SetPosition(uint32_t pos)
{
    if (!m_handle)
        return;

    gEnv->pCryPak->FSeek(m_handle, pos, SEEK_SET);
}

uint32_t CPakStream::GetPosition() const
{
    if (!m_handle)
        return 0;

    return (uint32)gEnv->pCryPak->FTell(m_handle);
}

uint32_t CPakStream::GetLength() const
{
    if (!m_handle)
        return 0;

    return gEnv->pCryPak->FGetSize(m_handle);
}

uint32_t CPakStream::Read(void* buffer, uint32_t size)
{
    if (!m_handle)
        return 0;

    return gEnv->pCryPak->FReadRaw(buffer, 1, size, m_handle);
}

Noesis::Ptr<CPakStream> CPakStream::Open(const char* uri)
{
    auto handle = gEnv->pCryPak->FOpen(uri, "rb");
    if (!handle)
        return nullptr;

    return Noesis::MakePtr<CPakStream>(handle);
}


void CPakStream::Close()
{
    if (!m_handle)
        return;

    gEnv->pCryPak->FClose(m_handle);
    m_handle = nullptr;
}
