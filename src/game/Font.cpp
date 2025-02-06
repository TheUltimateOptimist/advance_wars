#include "Font.hpp"
#include "SDL_ttf.h"
#include <stdexcept>
#include <vector>

namespace advanced_wars
{

Font::Font(std::string path) : m_fonts()
{
    std::vector<std::pair<FontSize, int>> sizes({
        std::pair(FontSize::NORMAL, 16),
        std::pair(FontSize::MENU, 24),
        std::pair(FontSize::TITLE, 48),
    });

    for (auto& [fontSize, size] : sizes)
    {
        TTF_Font* font = TTF_OpenFont(path.c_str(), size);
        if (!font)
        {
            throw std::runtime_error(
                "Failed to load font: " + std::string(TTF_GetError()) + " with size " +
                std::to_string(size));
        }

        m_fonts.push_back(std::pair(font, size));
    }
}

TTF_Font* Font::getFont(FontSize size)
{
    return this->m_fonts.at(static_cast<int>(size)).first;
}

int Font::getSize(FontSize size)
{
    return this->m_fonts.at(static_cast<int>(size)).second;
}

Font::~Font()
{
    for (auto& [font, size] : this->m_fonts)
    {
        TTF_CloseFont(font);
    }
}

} // namespace advanced_wars