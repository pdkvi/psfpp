#include "psfpp/Glyph.hpp"

#include <cassert>

namespace psfpp
{

Glyph::Glyph(std::uint8_t* bitmap, std::size_t width, std::size_t height, std::size_t charSize)
	: m_pBitmap(bitmap), m_Width(width), m_Height(height), m_CharSize(charSize)
{
}

std::uint8_t const* Glyph::GetRaw() const noexcept {
	return m_pBitmap;
}

std::size_t Glyph::GetWidth() const noexcept {
	return m_Width;
}

std::size_t Glyph::GetHeight() const noexcept {
	return m_Height;
}

std::size_t Glyph::GetCharSize() const noexcept {
	return m_CharSize;
}

bool Glyph::IsVisible(std::size_t x, std::size_t y) const noexcept {
	return ((*(m_pBitmap + y) >> x) & 1);
}

} // namespace psfpp
