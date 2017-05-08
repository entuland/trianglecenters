#ifndef GUI_FONT_H
#define GUI_FONT_H

#include "gui_common.h"
#include <string>
#include <vector>
#include <map>

namespace GUI {

class Font {
        class FontExcep {};
    public:
        Font(std::string FontFileName);
        SDL_Surface* GetChar(std::string Character,
                             RGBColor Foreground,
                             RGBColor Background, int PixSize);
        bool BadFont() {return badfont;}
    private:
        Font& operator=(Font&) { return *this;}
        Font(Font&) {}
        std::map<std::string, std::vector<size_t> > IntImage;
        bool badfont;
};

Uint16 RenderLine(std::string line, SDL_Surface* dest, Uint16 X, Uint16 Y,
                    Font& font, RGBColor ForeCol, RGBColor BackCol, int PixSize);

int LineLen(std::string line, Font& font, int PixSize);

std::string StringVideoSpecs();

Font* GetFont(bool fixed = false);

} // namespace GUI

#endif // GUI_FONT_H
