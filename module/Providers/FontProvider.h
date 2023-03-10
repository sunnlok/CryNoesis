////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once
#include <NsGui/CachedFontProvider.h>


class CFontProvider final : public Noesis::CachedFontProvider
{
public:
    virtual void ScanFolder(const Noesis::Uri& folder) override;
    virtual Noesis::Ptr<Noesis::Stream> OpenFont(const Noesis::Uri& folder, const char* filename) const override;
private:
    void ScanFolder(const Noesis::Uri& path, const char* ext);
};