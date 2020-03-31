#pragma once
#include <NsGui/MemoryStream.h>

namespace Cry
{
namespace Ns
{
	class CPakStream final : public Noesis::Stream
	{
	public:
		static Noesis::Ptr<CPakStream> Open(const char* uri);

		CPakStream() {};
		CPakStream(const char* uri);
		CPakStream(FILE* handle) : m_handle(handle) {}

		virtual ~CPakStream();

		virtual void SetPosition(uint32_t pos) override;
		virtual uint32_t GetPosition() const override;
		virtual uint32_t GetLength() const override;
		virtual uint32_t Read(void* buffer, uint32_t size) override;
		virtual void Close() override;

		CPakStream(const CPakStream& other) = delete;

		CPakStream(CPakStream&& other){
			m_handle = std::exchange(other.m_handle, nullptr);
		}

		CPakStream& operator=(CPakStream&& other){
			m_handle = std::exchange(other.m_handle, nullptr);
		}

		const FILE* GetHandle() const { return m_handle; }
	private:

		FILE* m_handle = nullptr;
	};

}
}