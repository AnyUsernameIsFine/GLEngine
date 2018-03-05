#pragma once

#include "FontSize.h"

#include <memory>

namespace Framework
{
	class Font
	{
	public:
		~Font();
		bool loadFromFile(const string& filename);
		void setSize(int size);
		string getFamilyName() const;
		int getHeight() const;
		int getVerticalCenterOffset() const;
		const Glyph* getGlyph(uInt32 character) const;
		void useTexture() const;
		int getTextureSize() const;

	private:
		FT_Library freeType;
		FT_Face face = nullptr;
		vector<std::shared_ptr<FontSize>> fontSizes;
		std::shared_ptr<FontSize> fontSize = nullptr;
		int verticalCenterOffset = 0;

		std::shared_ptr<FontSize> findFontSize(int size) const;
	};
}
