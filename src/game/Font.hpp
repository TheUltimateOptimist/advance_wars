#pragma once

#include "SDL_ttf.h"
#include <string>
#include <vector>

namespace advanced_wars
{

enum class FontSize
{
    NORMAL = 0,
    MENU = 1,
    TITLE = 2,
};

class Font
{
    public:
        Font(std::string path);

        TTF_Font* getFont(FontSize size);

        int getSize(FontSize size);

        ~Font();

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

    private:
        std::vector<std::pair<TTF_Font*, int>> m_fonts;
};

} // namespace advanced_wars