#include "gui_common.h"

using namespace GUI;

SDLKey GUI::WaitForKey(int delay)
{
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN) return event.key.keysym.sym;
        }
        SDL_Delay(delay);
    }
}

void GUI::WaitForKeyClick(int delay)
{
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYUP) return;
            if (event.type == SDL_MOUSEBUTTONDOWN) return;
        }
        SDL_Delay(delay);
    }
}

RGBColor GUI::RGB(ColorEnum colorkey) {
    RGBColor color = { 0, 0, 0};
    switch (colorkey) {
        case AliceBlue : color.r=0xF0; color.g=0xF8; color.b=0xFF; break;
        case AntiqueWhite : color.r=0xFA; color.g=0xEB; color.b=0xD7; break;
        case Aqua : color.r=0x00; color.g=0xFF; color.b=0xFF; break;
        case Aquamarine : color.r=0x7F; color.g=0xFF; color.b=0xD4; break;
        case Azure : color.r=0xF0; color.g=0xFF; color.b=0xFF; break;
        case Beige : color.r=0xF5; color.g=0xF5; color.b=0xDC; break;
        case Bisque : color.r=0xFF; color.g=0xE4; color.b=0xC4; break;
        case Black : color.r=0x00; color.g=0x00; color.b=0x00; break;
        case BlanchedAlmond : color.r=0xFF; color.g=0xEB; color.b=0xCD; break;
        case Blue : color.r=0x00; color.g=0x00; color.b=0xFF; break;
        case BlueViolet : color.r=0x8A; color.g=0x2B; color.b=0xE2; break;
        case Brown : color.r=0xA5; color.g=0x2A; color.b=0x2A; break;
        case BurlyWood : color.r=0xDE; color.g=0xB8; color.b=0x87; break;
        case CadetBlue : color.r=0x5F; color.g=0x9E; color.b=0xA0; break;
        case Chartreuse : color.r=0x7F; color.g=0xFF; color.b=0x00; break;
        case Chocolate : color.r=0xD2; color.g=0x69; color.b=0x1E; break;
        case Coral : color.r=0xFF; color.g=0x7F; color.b=0x50; break;
        case CornflowerBlue : color.r=0x64; color.g=0x95; color.b=0xED; break;
        case Cornsilk : color.r=0xFF; color.g=0xF8; color.b=0xDC; break;
        case Crimson : color.r=0xDC; color.g=0x14; color.b=0x3C; break;
        case Cyan : color.r=0x00; color.g=0xFF; color.b=0xFF; break;
        case DarkBlue : color.r=0x00; color.g=0x00; color.b=0x8B; break;
        case DarkCyan : color.r=0x00; color.g=0x8B; color.b=0x8B; break;
        case DarkGoldenrod : color.r=0xB8; color.g=0x86; color.b=0x0B; break;
        case DarkGray : color.r=0xA9; color.g=0xA9; color.b=0xA9; break;
        case DarkGreen : color.r=0x00; color.g=0x64; color.b=0x00; break;
        case DarkKhaki : color.r=0xBD; color.g=0xB7; color.b=0x6B; break;
        case DarkMagenta : color.r=0x8B; color.g=0x00; color.b=0x8B; break;
        case DarkOliveGreen : color.r=0x55; color.g=0x6B; color.b=0x2F; break;
        case DarkOrange : color.r=0xFF; color.g=0x8C; color.b=0x00; break;
        case DarkOrchid : color.r=0x99; color.g=0x32; color.b=0xCC; break;
        case DarkRed : color.r=0x8B; color.g=0x00; color.b=0x00; break;
        case DarkSalmon : color.r=0xE9; color.g=0x96; color.b=0x7A; break;
        case DarkSeaGreen : color.r=0x8F; color.g=0xBC; color.b=0x8F; break;
        case DarkSlateBlue : color.r=0x48; color.g=0x3D; color.b=0x8B; break;
        case DarkSlateGray : color.r=0x2F; color.g=0x4F; color.b=0x4F; break;
        case DarkTurquoise : color.r=0x00; color.g=0xCE; color.b=0xD1; break;
        case DarkViolet : color.r=0x94; color.g=0x00; color.b=0xD3; break;
        case DeepPink : color.r=0xFF; color.g=0x14; color.b=0x93; break;
        case DeepSkyBlue : color.r=0x00; color.g=0xBF; color.b=0xFF; break;
        case DimGray : color.r=0x69; color.g=0x69; color.b=0x69; break;
        case DodgerBlue : color.r=0x1E; color.g=0x90; color.b=0xFF; break;
        case FireBrick : color.r=0xB2; color.g=0x22; color.b=0x22; break;
        case FloralWhite : color.r=0xFF; color.g=0xFA; color.b=0xF0; break;
        case ForestGreen : color.r=0x22; color.g=0x8B; color.b=0x22; break;
        case Fuchsia : color.r=0xFF; color.g=0x00; color.b=0xFF; break;
        case Gainsboro : color.r=0xDC; color.g=0xDC; color.b=0xDC; break;
        case GhostWhite : color.r=0xF8; color.g=0xF8; color.b=0xFF; break;
        case Gold : color.r=0xFF; color.g=0xD7; color.b=0x00; break;
        case Goldenrod : color.r=0xDA; color.g=0xA5; color.b=0x20; break;
        case Gray : color.r=0x80; color.g=0x80; color.b=0x80; break;
        case Gray90 : color.r=0xff*0.1; color.g=0xff*0.1; color.b=0xff*0.1; break;
        case Gray80 : color.r=0xff*0.2; color.g=0xff*0.2; color.b=0xff*0.2; break;
        case Gray70 : color.r=0xff*0.3; color.g=0xff*0.3; color.b=0xff*0.3; break;
        case Gray60 : color.r=0xff*0.4; color.g=0xff*0.4; color.b=0xff*0.4; break;
        case Gray50 : color.r=0xff*0.5; color.g=0xff*0.5; color.b=0xff*0.5; break;
        case Gray40 : color.r=0xff*0.6; color.g=0xff*0.6; color.b=0xff*0.6; break;
        case Gray30 : color.r=0xff*0.7; color.g=0xff*0.7; color.b=0xff*0.7; break;
        case Gray20 : color.r=0xff*0.8; color.g=0xff*0.8; color.b=0xff*0.8; break;
        case Gray10 : color.r=0xff*0.9; color.g=0xff*0.9; color.b=0xff*0.9; break;
        case Green : color.r=0x00; color.g=0x80; color.b=0x00; break;
        case GreenYellow : color.r=0xAD; color.g=0xFF; color.b=0x2F; break;
        case Honeydew : color.r=0xF0; color.g=0xFF; color.b=0xF0; break;
        case HotPink : color.r=0xFF; color.g=0x69; color.b=0xB4; break;
        case IndianRed : color.r=0xCD; color.g=0x5C; color.b=0x5C; break;
        case Indigo : color.r=0x4B; color.g=0x00; color.b=0x82; break;
        case Ivory : color.r=0xFF; color.g=0xFF; color.b=0xF0; break;
        case Khaki : color.r=0xF0; color.g=0xE6; color.b=0x8C; break;
        case Lavender : color.r=0xE6; color.g=0xE6; color.b=0xFA; break;
        case LavenderBlush : color.r=0xFF; color.g=0xF0; color.b=0xF5; break;
        case LawnGreen : color.r=0x7C; color.g=0xFC; color.b=0x00; break;
        case LemonChiffon : color.r=0xFF; color.g=0xFA; color.b=0xCD; break;
        case LightBlue : color.r=0xAD; color.g=0xD8; color.b=0xE6; break;
        case LightCoral : color.r=0xF0; color.g=0x80; color.b=0x80; break;
        case LightCyan : color.r=0xE0; color.g=0xFF; color.b=0xFF; break;
        case LightGoldenrodYellow : color.r=0xFA; color.g=0xFA; color.b=0xD2; break;
        case LightGreen : color.r=0x90; color.g=0xEE; color.b=0x90; break;
        case LightGray : color.r=0xD3; color.g=0xD3; color.b=0xD3; break;
        case LightPink : color.r=0xFF; color.g=0xB6; color.b=0xC1; break;
        case LightSalmon : color.r=0xFF; color.g=0xA0; color.b=0x7A; break;
        case LightSeaGreen : color.r=0x20; color.g=0xB2; color.b=0xAA; break;
        case LightSkyBlue : color.r=0x87; color.g=0xCE; color.b=0xFA; break;
        case LightSlateGray : color.r=0x77; color.g=0x88; color.b=0x99; break;
        case LightSteelBlue : color.r=0xB0; color.g=0xC4; color.b=0xDE; break;
        case LightYellow : color.r=0xFF; color.g=0xFF; color.b=0xE0; break;
        case Lime : color.r=0x00; color.g=0xFF; color.b=0x00; break;
        case LimeGreen : color.r=0x32; color.g=0xCD; color.b=0x32; break;
        case Linen : color.r=0xFA; color.g=0xF0; color.b=0xE6; break;
        case Magenta : color.r=0xFF; color.g=0x00; color.b=0xFF; break;
        case Maroon : color.r=0x80; color.g=0x00; color.b=0x00; break;
        case MediumAquamarine : color.r=0x66; color.g=0xCD; color.b=0xAA; break;
        case MediumBlue : color.r=0x00; color.g=0x00; color.b=0xCD; break;
        case MediumOrchid : color.r=0xBA; color.g=0x55; color.b=0xD3; break;
        case MediumPurple : color.r=0x93; color.g=0x70; color.b=0xDB; break;
        case MediumSeaGreen : color.r=0x3C; color.g=0xB3; color.b=0x71; break;
        case MediumSlateBlue : color.r=0x7B; color.g=0x68; color.b=0xEE; break;
        case MediumSpringGreen : color.r=0x00; color.g=0xFA; color.b=0x9A; break;
        case MediumTurquoise : color.r=0x48; color.g=0xD1; color.b=0xCC; break;
        case MediumVioletRed : color.r=0xC7; color.g=0x15; color.b=0x85; break;
        case MidnightBlue : color.r=0x19; color.g=0x19; color.b=0x70; break;
        case MintCream : color.r=0xF5; color.g=0xFF; color.b=0xFA; break;
        case MistyRose : color.r=0xFF; color.g=0xE4; color.b=0xE1; break;
        case Moccasin : color.r=0xFF; color.g=0xE4; color.b=0xB5; break;
        case NavajoWhite : color.r=0xFF; color.g=0xDE; color.b=0xAD; break;
        case Navy : color.r=0x00; color.g=0x00; color.b=0x80; break;
        case OldLace : color.r=0xFD; color.g=0xF5; color.b=0xE6; break;
        case Olive : color.r=0x80; color.g=0x80; color.b=0x00; break;
        case OliveDrab : color.r=0x6B; color.g=0x8E; color.b=0x23; break;
        case Orange : color.r=0xFF; color.g=0xA5; color.b=0x00; break;
        case OrangeRed : color.r=0xFF; color.g=0x45; color.b=0x00; break;
        case Orchid : color.r=0xDA; color.g=0x70; color.b=0xD6; break;
        case PaleGoldenrod : color.r=0xEE; color.g=0xE8; color.b=0xAA; break;
        case PaleGreen : color.r=0x98; color.g=0xFB; color.b=0x98; break;
        case PaleTurquoise : color.r=0xAF; color.g=0xEE; color.b=0xEE; break;
        case PaleVioletRed : color.r=0xDB; color.g=0x70; color.b=0x93; break;
        case PapayaWhip : color.r=0xFF; color.g=0xEF; color.b=0xD5; break;
        case PeachPuff : color.r=0xFF; color.g=0xDA; color.b=0xB9; break;
        case Peru : color.r=0xCD; color.g=0x85; color.b=0x3F; break;
        case Pink : color.r=0xFF; color.g=0xC0; color.b=0xCB; break;
        case Plum : color.r=0xDD; color.g=0xA0; color.b=0xDD; break;
        case PowderBlue : color.r=0xB0; color.g=0xE0; color.b=0xE6; break;
        case Purple : color.r=0x80; color.g=0x00; color.b=0x80; break;
        case Red : color.r=0xFF; color.g=0x00; color.b=0x00; break;
        case RosyBrown : color.r=0xBC; color.g=0x8F; color.b=0x8F; break;
        case RoyalBlue : color.r=0x41; color.g=0x69; color.b=0xE1; break;
        case SaddleBrown : color.r=0x8B; color.g=0x45; color.b=0x13; break;
        case Salmon : color.r=0xFA; color.g=0x80; color.b=0x72; break;
        case SandyBrown : color.r=0xF4; color.g=0xA4; color.b=0x60; break;
        case SeaGreen : color.r=0x2E; color.g=0x8B; color.b=0x57; break;
        case Seashell : color.r=0xFF; color.g=0xF5; color.b=0xEE; break;
        case Sienna : color.r=0xA0; color.g=0x52; color.b=0x2D; break;
        case Silver : color.r=0xC0; color.g=0xC0; color.b=0xC0; break;
        case SkyBlue : color.r=0x87; color.g=0xCE; color.b=0xEB; break;
        case SlateBlue : color.r=0x6A; color.g=0x5A; color.b=0xCD; break;
        case SlateGray : color.r=0x70; color.g=0x80; color.b=0x90; break;
        case Snow : color.r=0xFF; color.g=0xFA; color.b=0xFA; break;
        case SpringGreen : color.r=0x00; color.g=0xFF; color.b=0x7F; break;
        case SteelBlue : color.r=0x46; color.g=0x82; color.b=0xB4; break;
        case Tan : color.r=0xD2; color.g=0xB4; color.b=0x8C; break;
        case Teal : color.r=0x00; color.g=0x80; color.b=0x80; break;
        case Thistle : color.r=0xD8; color.g=0xBF; color.b=0xD8; break;
        case Tomato : color.r=0xFF; color.g=0x63; color.b=0x47; break;
        case Turquoise : color.r=0x40; color.g=0xE0; color.b=0xD0; break;
        case Violet : color.r=0xEE; color.g=0x82; color.b=0xEE; break;
        case Wheat : color.r=0xF5; color.g=0xDE; color.b=0xB3; break;
        case White : color.r=0xFF; color.g=0xFF; color.b=0xFF; break;
        case WhiteSmoke : color.r=0xF5; color.g=0xF5; color.b=0xF5; break;
        case Yellow : color.r=0xFF; color.g=0xFF; color.b=0x00; break;
        case YellowGreen : color.r=0x9A; color.g=0xCD; color.b=0x32; break;
        default: color.r = color.g = color.b = 0xFF;
    }
    return color;
}

RGBColor GUI::GetRGB(const PointXY& point, Surface* orig)
{
    RGBColor color = {0, 0, 0};
    Uint32 pixel;
    Rect rect = {0, 0, orig->w, orig->h};
    Uint32 offset;
    if(CoordToPos(point, rect, &offset)) {
        if (Lock(orig)==0) {
            Uint32* mp = static_cast<Uint32* >(orig->pixels);
            mp += offset;
            pixel = *mp;
            Unlock(orig);
            SDL_GetRGB(pixel, orig->format, &color.r, &color.g, &color.b);
        }
    }
    return color;
}
