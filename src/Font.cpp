#include "psfpp/Font.hpp"

#include <cstring>
#include <fstream>

namespace psfpp
{

Font::Font(fs::path const& filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);
	std::size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::ptrdiff_t offset = 2;
	file.read(reinterpret_cast<char*>(&m_Header), offset);

	if (m_Header.v1.mMagicBytes[0] == PSFPP_PSF1_MAGIC_BYTE1 &&
		m_Header.v1.mMagicBytes[1] == PSFPP_PSF1_MAGIC_BYTE2)
	{
		m_Version = PsfVersion::kPsfV1;
		file.read(reinterpret_cast<char*>(&m_Header) + offset, sizeof(PsfHeader::v1) - offset);
		offset = sizeof(PsfHeader::v1);
	}

	else
	{
		file.read(reinterpret_cast<char*>(&m_Header) + offset, offset);
		offset += 2;

		if (m_Header.v2.mMagicBytes[0] == PSFPP_PSF2_MAGIC_BYTE1 &&
			m_Header.v2.mMagicBytes[1] == PSFPP_PSF2_MAGIC_BYTE2 &&
			m_Header.v2.mMagicBytes[2] == PSFPP_PSF2_MAGIC_BYTE3 &&
			m_Header.v2.mMagicBytes[3] == PSFPP_PSF2_MAGIC_BYTE4)
		{
			m_Version = PsfVersion::kPsfV2;
			file.read(reinterpret_cast<char*>(&m_Header) + offset, sizeof(PsfHeader::v2) - offset);
			offset = sizeof(PsfHeader::v2);
		}
	}

	fileSize -= offset;

	m_pData = std::make_unique<std::uint8_t[]>(fileSize);
	file.read(reinterpret_cast<char*>(m_pData.get()), fileSize);
}

PsfVersion const& Font::GetVersion() const {
	return m_Version;
}

PsfHeader const& Font::GetHeader() const {
	return m_Header;
}

std::size_t Font::GetWidth() const noexcept
{
	std::size_t ret = 0;

	if (m_Version == PsfVersion::kPsfV1) {
		ret = m_Header.v1.mCharSize;
	}
	else if (m_Version == PsfVersion::kPsfV2) {
		ret = m_Header.v2.mWidth;
	}

	return ret;
}

std::size_t Font::GetHeight() const noexcept
{
	std::size_t ret = 0;

	if (m_Version == PsfVersion::kPsfV1) {
		ret = m_Header.v1.mCharSize;
	}
	else if (m_Version == PsfVersion::kPsfV2) {
		ret = m_Header.v2.mHeight;
	}

	return ret;
}

Glyph Font::GetGlyph(char character)
{
	std::uint8_t* bitmap = nullptr;
	std::size_t width = 0, height = 0, charSize = 0;

	if (m_Version == kPsfV1)
	{
		bitmap = &m_pData[m_Header.v1.mCharSize * character];
		width = height = charSize = m_Header.v1.mCharSize;
	}

	else if (m_Version == kPsfV2)
	{
		bitmap = &m_pData[m_Header.v2.mBytesPerChar * character];
		width = m_Header.v2.mWidth;
		height = m_Header.v2.mHeight;
		charSize = m_Header.v2.mBytesPerChar;
	}
	
	return Glyph(bitmap, width, height, charSize);
}

Glyph const Font::GetGlyph(char character) const
{
	std::uint8_t* bitmap = nullptr;
	std::size_t width = 0, height = 0, charSize = 0;

	if (m_Version == kPsfV1)
	{
		bitmap = &m_pData[m_Header.v1.mCharSize * character];
		width = height = charSize = m_Header.v1.mCharSize;
	}

	else if (m_Version == kPsfV2)
	{
		bitmap = &m_pData[m_Header.v2.mBytesPerChar * character];
		width = m_Header.v2.mWidth;
		height = m_Header.v2.mHeight;
		charSize = m_Header.v2.mBytesPerChar;
	}
	
	return Glyph(bitmap, width, height, charSize);
}

} // namespace psfpp
