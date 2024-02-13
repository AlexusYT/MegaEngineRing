//
// Created by alexus on 13.02.24.
//

#ifndef GL_H
#define GL_H
#include <cstdint>

namespace n::sdk::renderer {

enum class ClearBits : uint32_t {

	COLOR_BUFFER_BIT,
	DEPTH_BUFFER_BIT,
	STENCIL_BUFFER_BIT


}; // namespace n::sdk::renderer

constexpr ClearBits operator&(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) & static_cast<utype>(pY));
}

constexpr ClearBits operator|(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) | static_cast<utype>(pY));
}

constexpr ClearBits operator^(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) ^ static_cast<utype>(pY));
}

constexpr ClearBits operator~(ClearBits pX) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(~static_cast<utype>(pX));
}

inline ClearBits &operator&=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX & pY; }

inline ClearBits &operator|=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX | pY; }

inline ClearBits &operator^=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX ^ pY; }

class GL {
public:
	static void viewport(int pX, int pY, int pWidth, int pHeight);

	static void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha);

	static void clear(ClearBits pMask);
};

} // namespace n::sdk::renderer


#endif //GL_H
