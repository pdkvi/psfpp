#ifndef PSFPP_FONT_HPP
#define PSFPP_FONT_HPP
#pragma once

#include <memory>
#include <filesystem>

#include "psfpp/Glyph.hpp"

namespace psfpp
{

#define PSFPP_PSF1_MAGIC_BYTE1 0x36
#define PSFPP_PSF1_MAGIC_BYTE2 0x04

#define PSFPP_PSF2_MAGIC_BYTE1 0x72
#define PSFPP_PSF2_MAGIC_BYTE2 0xb5
#define PSFPP_PSF2_MAGIC_BYTE3 0x4a
#define PSFPP_PSF2_MAGIC_BYTE4 0x86

union PsfHeader
{
	struct
	{
		std::uint8_t  mMagicBytes[2];
		std::uint8_t  mMode;
		std::uint8_t  mCharSize;
	} v1;

	struct
	{
		std::uint8_t  mMagicBytes[4];
		std::uint32_t mVersion;
		std::uint32_t mHeaderSize;
		std::uint32_t mFlags;
		std::uint32_t mGlyphsCount;
		std::uint32_t mBytesPerChar;
		std::uint32_t mHeight;
		std::uint32_t mWidth;
	} v2;
};

enum PsfVersion
{
	kPsfUnknown = -1,
	kPsfV1      = 1,
	kPsfV2      = 2
};

enum PsfModes : std::uint8_t
{
	kPsfMode512    = 0x01,
	kPsfModeHasTab = 0x02,
	kPsfModeSeq    = 0x04
};

enum PsfFlags : std::uint32_t
{
	kPsfFlagHasUnicodeTable = 0x01,
};

namespace fs = std::filesystem;

class Font
{
public:
	Font(fs::path const& filePath);
	~Font() = default;

public:
	Font(Font const& rhs) = default;
	Font(Font&& rhs) = default;

	Font& operator=(Font const& rhs) = default;
	Font& operator=(Font&& rhs) = default;

public:
	PsfVersion const& GetVersion() const;
	PsfHeader const& GetHeader() const;

	std::size_t GetWidth() const noexcept;
	std::size_t GetHeight() const noexcept;

	Glyph GetGlyph(char character);
	Glyph const GetGlyph(char character) const;

private:
	std::unique_ptr<std::uint8_t[]> m_pData;
	PsfHeader m_Header;
	PsfVersion m_Version;
};

} // namespace psfpp

#endif // PSFPP_FONT_HPP
