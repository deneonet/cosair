#pragma once

namespace Cosair {

	class Texture {
	public:
		virtual ~Texture() {}

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;

		inline virtual void Bind(uint32_t slot = 0) const = 0;
	protected:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};

	using Texture2dRef = Ref<Texture2D>;
}