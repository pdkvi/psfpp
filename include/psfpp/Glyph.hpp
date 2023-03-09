#ifndef PSFPP_GLYPH_HPP
#define PSFPP_GLYPH_HPP
#pragma once

#include <cstddef>
#include <cstdint>

namespace psfpp
{

class Glyph
{
	friend class Font;

public:
	Glyph() = delete;
	~Glyph() = default;

private:
	Glyph(std::uint8_t* bitmap, std::size_t width, std::size_t height, std::size_t charSize);

public:
	Glyph(Glyph const& rhs) = default;
	Glyph(Glyph&& rhs) = default;

	Glyph& operator=(Glyph const& rhs) = default;
	Glyph& operator=(Glyph&& rhs) = default;

public:
	std::uint8_t const* GetRaw() const noexcept;
	std::size_t GetWidth() const noexcept;
	std::size_t GetHeight() const noexcept;
	std::size_t GetCharSize() const noexcept;

	bool IsVisible(std::size_t x, std::size_t y) const noexcept;

private:
	std::uint8_t* m_pBitmap;

	std::size_t m_Width;
	std::size_t m_Height;
	std::size_t m_CharSize;
};

} // namespace psfpp

#endif // PSFPP_GLYPH_HPP
