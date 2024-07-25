#pragma once

namespace Cosair {

	class Texture {
	public:
		virtual ~Texture() { }

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;

	#ifdef CR_EXT_BINDLESS_TEXS
		inline virtual uint32_t GetIndex() const = 0;
		inline virtual uint64_t GetHandleId() const = 0;
	#endif
		inline virtual uint32_t GetTextureId() const = 0;

		inline virtual void Bind(uint32_t slot = 0) const = 0;
	protected:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

	#ifdef CR_EXT_BINDLESS_TEXS
		static uint32_t m_IndexOffset;

		uint32_t m_Index = 0;
		uint64_t m_HandleId = 0;
	#endif
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};

	using Texture2dRef = Ref<Texture2D>;

}