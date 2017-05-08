#include <iostream>

#include "gui_screen.h"
#include "gui_font.h"

using namespace GUI;

namespace {
    Screen* curscreen;
}

Screen* GUI::GetScreen() {
    return curscreen;
}

Screen::Screen(Uint32 W, Uint32 H, Uint32 BPP) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)){
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0) {
            std::clog << "Unable to initialize SDL Library: aborting.\n";
            exit(1);
        }
        srf = SDL_SetVideoMode(W, H, BPP, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if (!srf) {
            std::clog << "Unable to set '" << W << "x" << H << "@" << BPP << "bpp' video mode \n";
            exit(1);
        }
        curscreen = this;
        bgcol = RGB(Black);
        fgcol = RGB(White);
        axes.axis = RGB(Silver);
        axes.grid = RGB(Gray);
        axes.tick = RGB(White);
        axes.showgrid = false;
        axes.ticksize = 3;
        axes.tickspacing = 10;
        status.origin = PrecXY(14,14);
        mouselock = 0;
        focused = 0;
        clear();
        std::clog << "SDL video mode initialized correctly.\n";
    } else {
        std::clog << "Double Screen instantiation.\n";
        std::clog << "Throwing \"double_screen\" exception...\n";
        throw double_screen();
    }
}

Screen::~Screen() {
    SDL_Quit();
    std::clog << "SDL Quitted.\n";
}

void Canvas::drawaxes() {
    double startx = 0;
    double starty = 0;
    double stopx = width() + startx - 1;
    double stopy = height() + starty - 1;
    axes.tickspacing = fabs(axes.tickspacing);

    PrecXY p1, p2;

    p1.x = p2.x = 0;
    p1.y = starty + 1;
    p2.y = stopy - 1;
    drawline(p1, p2, axes.axis);

    p1.y = p2.y = 0;
    p1.x = startx + 1;
    p2.x = stopx - 1;
    drawline(p1, p2, axes.axis);

    // Positive X axis
    for (p1.x = axes.tickspacing; p1.x < stopx; p1.x += axes.tickspacing) {
        p2.x = p1.x;
        if (axes.showgrid) {
            p1.y = starty;
            p2.y = stopy;
            drawline(p1, p2, axes.grid);
        }
        p1.y = axes.ticksize;
        p2.y = -axes.ticksize;
        drawline(p1, p2, axes.tick);
    }

    // Negative X axis
    for (p1.x = -axes.tickspacing; p1.x > startx; p1.x -= axes.tickspacing) {
        p2.x = p1.x;
        if (axes.showgrid) {
            p1.y = starty;
            p2.y = stopy;
            drawline(p1, p2, axes.grid);
        }
        p1.y = axes.ticksize;
        p2.y = -axes.ticksize;
        drawline(p1, p2, axes.tick);
    }
    p1.x = p2.x = 0;
    drawline(p1, p2, axes.tick);


    // Positive Y axis
    for (p1.y = axes.tickspacing; p1.y < stopy; p1.y += axes.tickspacing) {
        p2.y = p1.y;
        if (axes.showgrid) {
            p1.x = startx;
            p2.x = stopx;
            drawline(p1, p2, axes.grid);
        }
        p1.x = axes.ticksize;
        p2.x = -axes.ticksize;
        drawline(p1, p2, axes.tick);
    }

    // Negative X axis
    for (p1.y = -axes.tickspacing; p1.y > starty; p1.y -= axes.tickspacing) {
        p2.y = p1.y;
        if (axes.showgrid) {
            p1.x = startx;
            p2.x = stopx;
            drawline(p1, p2, axes.grid);
        }
        p1.x = axes.ticksize;
        p2.x = -axes.ticksize;
        drawline(p1, p2, axes.tick);
    }
    p1.y = p2.y = 0;
    drawline(p1, p2, axes.tick);
}

void GUI::Status(std::string message, bool fixed, int size) {
    Screen* scr = GetScreen();
    RenderLine(message, scr->surface(), 0, 0,
               *GetFont(fixed), scr->forecolor(), scr->backcolor(),
               size);
}

void GUI::Messages::add(std::string message, RGBColor color) {
    messages.push(std::make_pair(message, color));
}

void GUI::Messages::add(RGBColor color) {
    messages.push(std::make_pair(stream.str(), color));
    stream.str("");
}

void GUI::Messages::print(bool fixed, int size) {
    Screen* scr = GetScreen();
    Font* font = GetFont(fixed);
    Uint16 cury = origin.y;
    std::pair<std::string, RGBColor> msg;
    while (messages.size() > 0) {
        msg = messages.front();
        cury = RenderLine(msg.first, scr->surface(), origin.x, cury,
                          *font, msg.second, scr->backcolor(),
                          size);
        messages.pop();
    }
}

std::vector<ColorEnum> GUI::ColorPicker::enums;
std::vector<std::string> GUI::ColorPicker::names;
std::map<std::string, ColorEnum> GUI::ColorPicker::namesmap;

GUI::ColorPicker::ColorPicker() {
    ColorPicker::initialize();
    left = 14;
    top = 14;
    width = 200;
    height = 200;
}

void GUI::ColorPicker::initialize() {
    static bool initialized = false;
    if (initialized) return;
    names.push_back("White"); enums.push_back(White);
    names.push_back("Red"); enums.push_back(Red);
    names.push_back("Lime"); enums.push_back(Lime);
    names.push_back("Blue"); enums.push_back(Blue);
    names.push_back("Cyan"); enums.push_back(Cyan);
    names.push_back("Magenta"); enums.push_back(Magenta);
    names.push_back("Yellow"); enums.push_back(Yellow);
    names.push_back("Gray10"); enums.push_back(Gray10);
    names.push_back("Gray20"); enums.push_back(Gray20);
    names.push_back("Gray30"); enums.push_back(Gray30);
    names.push_back("Gray40"); enums.push_back(Gray40);
    names.push_back("Gray50"); enums.push_back(Gray50);
    names.push_back("Gray60"); enums.push_back(Gray60);
    names.push_back("Gray70"); enums.push_back(Gray70);
    names.push_back("Gray80"); enums.push_back(Gray80);
    names.push_back("Gray90"); enums.push_back(Gray90);
    names.push_back("Black"); enums.push_back(Black);

    names.push_back("Snow"); enums.push_back(Snow);
    names.push_back("Honeydew"); enums.push_back(Honeydew);
    names.push_back("MintCream"); enums.push_back(MintCream);
    names.push_back("Azure"); enums.push_back(Azure);
    names.push_back("AliceBlue"); enums.push_back(AliceBlue);
    names.push_back("GhostWhite"); enums.push_back(GhostWhite);
    names.push_back("WhiteSmoke"); enums.push_back(WhiteSmoke);
    names.push_back("Seashell"); enums.push_back(Seashell);
    names.push_back("Beige"); enums.push_back(Beige);
    names.push_back("OldLace"); enums.push_back(OldLace);
    names.push_back("FloralWhite"); enums.push_back(FloralWhite);
    names.push_back("Ivory"); enums.push_back(Ivory);
    names.push_back("AntiqueWhite"); enums.push_back(AntiqueWhite);
    names.push_back("Linen"); enums.push_back(Linen);
    names.push_back("LavenderBlush"); enums.push_back(LavenderBlush);
    names.push_back("MistyRose"); enums.push_back(MistyRose);

    names.push_back("IndianRed"); enums.push_back(IndianRed);
    names.push_back("LightCoral"); enums.push_back(LightCoral);
    names.push_back("Salmon"); enums.push_back(Salmon);
    names.push_back("DarkSalmon"); enums.push_back(DarkSalmon);
    names.push_back("LightSalmon"); enums.push_back(LightSalmon);
    names.push_back("Crimson"); enums.push_back(Crimson);
    names.push_back("Red"); enums.push_back(Red);
    names.push_back("FireBrick"); enums.push_back(FireBrick);
    names.push_back("DarkRed"); enums.push_back(DarkRed);

    names.push_back("Pink"); enums.push_back(Pink);
    names.push_back("LightPink"); enums.push_back(LightPink);
    names.push_back("HotPink"); enums.push_back(HotPink);
    names.push_back("DeepPink"); enums.push_back(DeepPink);
    names.push_back("MediumVioletRed"); enums.push_back(MediumVioletRed);
    names.push_back("PaleVioletRed"); enums.push_back(PaleVioletRed);

    names.push_back("Coral"); enums.push_back(Coral);
    names.push_back("Tomato"); enums.push_back(Tomato);
    names.push_back("OrangeRed"); enums.push_back(OrangeRed);
    names.push_back("DarkOrange"); enums.push_back(DarkOrange);
    names.push_back("Orange"); enums.push_back(Orange);

    names.push_back("Gold"); enums.push_back(Gold);
    names.push_back("Yellow"); enums.push_back(Yellow);
    names.push_back("LightYellow"); enums.push_back(LightYellow);
    names.push_back("LemonChiffon"); enums.push_back(LemonChiffon);
    names.push_back("LightGoldenrodYellow"); enums.push_back(LightGoldenrodYellow);
    names.push_back("PapayaWhip"); enums.push_back(PapayaWhip);
    names.push_back("Moccasin"); enums.push_back(Moccasin);
    names.push_back("PeachPuff"); enums.push_back(PeachPuff);
    names.push_back("PaleGoldenrod"); enums.push_back(PaleGoldenrod);
    names.push_back("Khaki"); enums.push_back(Khaki);
    names.push_back("DarkKhaki"); enums.push_back(DarkKhaki);

    names.push_back("Lavender"); enums.push_back(Lavender);
    names.push_back("Thistle"); enums.push_back(Thistle);
    names.push_back("Plum"); enums.push_back(Plum);
    names.push_back("Violet"); enums.push_back(Violet);
    names.push_back("Orchid"); enums.push_back(Orchid);
    names.push_back("Fuchsia"); enums.push_back(Fuchsia);
    names.push_back("Magenta"); enums.push_back(Magenta);
    names.push_back("MediumOrchid"); enums.push_back(MediumOrchid);
    names.push_back("MediumPurple"); enums.push_back(MediumPurple);
    names.push_back("BlueViolet"); enums.push_back(BlueViolet);
    names.push_back("DarkViolet"); enums.push_back(DarkViolet);
    names.push_back("DarkOrchid"); enums.push_back(DarkOrchid);
    names.push_back("DarkMagenta"); enums.push_back(DarkMagenta);
    names.push_back("Purple"); enums.push_back(Purple);
    names.push_back("Indigo"); enums.push_back(Indigo);
    names.push_back("SlateBlue"); enums.push_back(SlateBlue);
    names.push_back("DarkSlateBlue"); enums.push_back(DarkSlateBlue);

    names.push_back("GreenYellow"); enums.push_back(GreenYellow);
    names.push_back("Chartreuse"); enums.push_back(Chartreuse);
    names.push_back("LawnGreen"); enums.push_back(LawnGreen);
    names.push_back("Lime"); enums.push_back(Lime);
    names.push_back("LimeGreen"); enums.push_back(LimeGreen);
    names.push_back("PaleGreen"); enums.push_back(PaleGreen);
    names.push_back("LightGreen"); enums.push_back(LightGreen);
    names.push_back("MediumSpringGreen"); enums.push_back(MediumSpringGreen);
    names.push_back("SpringGreen"); enums.push_back(SpringGreen);
    names.push_back("MediumSeaGreen"); enums.push_back(MediumSeaGreen);
    names.push_back("SeaGreen"); enums.push_back(SeaGreen);
    names.push_back("ForestGreen"); enums.push_back(ForestGreen);
    names.push_back("Green"); enums.push_back(Green);
    names.push_back("DarkGreen"); enums.push_back(DarkGreen);
    names.push_back("YellowGreen"); enums.push_back(YellowGreen);
    names.push_back("OliveDrab"); enums.push_back(OliveDrab);
    names.push_back("Olive"); enums.push_back(Olive);
    names.push_back("DarkOliveGreen"); enums.push_back(DarkOliveGreen);
    names.push_back("MediumAquamarine"); enums.push_back(MediumAquamarine);
    names.push_back("DarkSeaGreen"); enums.push_back(DarkSeaGreen);
    names.push_back("LightSeaGreen"); enums.push_back(LightSeaGreen);
    names.push_back("DarkCyan"); enums.push_back(DarkCyan);
    names.push_back("Teal"); enums.push_back(Teal);

    names.push_back("Aqua"); enums.push_back(Aqua);
    names.push_back("Cyan"); enums.push_back(Cyan);
    names.push_back("LightCyan"); enums.push_back(LightCyan);
    names.push_back("PaleTurquoise"); enums.push_back(PaleTurquoise);
    names.push_back("Aquamarine"); enums.push_back(Aquamarine);
    names.push_back("Turquoise"); enums.push_back(Turquoise);
    names.push_back("MediumTurquoise"); enums.push_back(MediumTurquoise);
    names.push_back("DarkTurquoise"); enums.push_back(DarkTurquoise);
    names.push_back("CadetBlue"); enums.push_back(CadetBlue);
    names.push_back("SteelBlue"); enums.push_back(SteelBlue);
    names.push_back("LightSteelBlue"); enums.push_back(LightSteelBlue);
    names.push_back("PowderBlue"); enums.push_back(PowderBlue);
    names.push_back("LightBlue"); enums.push_back(LightBlue);
    names.push_back("SkyBlue"); enums.push_back(SkyBlue);
    names.push_back("LightSkyBlue"); enums.push_back(LightSkyBlue);
    names.push_back("DeepSkyBlue"); enums.push_back(DeepSkyBlue);
    names.push_back("DodgerBlue"); enums.push_back(DodgerBlue);
    names.push_back("CornflowerBlue"); enums.push_back(CornflowerBlue);
    names.push_back("MediumSlateBlue"); enums.push_back(MediumSlateBlue);
    names.push_back("RoyalBlue"); enums.push_back(RoyalBlue);
    names.push_back("Blue"); enums.push_back(Blue);
    names.push_back("MediumBlue"); enums.push_back(MediumBlue);
    names.push_back("DarkBlue"); enums.push_back(DarkBlue);
    names.push_back("Navy"); enums.push_back(Navy);
    names.push_back("MidnightBlue"); enums.push_back(MidnightBlue);

    names.push_back("Cornsilk"); enums.push_back(Cornsilk);
    names.push_back("BlanchedAlmond"); enums.push_back(BlanchedAlmond);
    names.push_back("Bisque"); enums.push_back(Bisque);
    names.push_back("NavajoWhite"); enums.push_back(NavajoWhite);
    names.push_back("Wheat"); enums.push_back(Wheat);
    names.push_back("BurlyWood"); enums.push_back(BurlyWood);
    names.push_back("Tan"); enums.push_back(Tan);
    names.push_back("RosyBrown"); enums.push_back(RosyBrown);
    names.push_back("SandyBrown"); enums.push_back(SandyBrown);
    names.push_back("Goldenrod"); enums.push_back(Goldenrod);
    names.push_back("DarkGoldenrod"); enums.push_back(DarkGoldenrod);
    names.push_back("Peru"); enums.push_back(Peru);
    names.push_back("Chocolate"); enums.push_back(Chocolate);
    names.push_back("SaddleBrown"); enums.push_back(SaddleBrown);
    names.push_back("Sienna"); enums.push_back(Sienna);
    names.push_back("Brown"); enums.push_back(Brown);
    names.push_back("Maroon"); enums.push_back(Maroon);

    names.push_back("Gainsboro"); enums.push_back(Gainsboro);
    names.push_back("LightGray"); enums.push_back(LightGray);
    names.push_back("Silver"); enums.push_back(Silver);
    names.push_back("DarkGray"); enums.push_back(DarkGray);
    names.push_back("Gray"); enums.push_back(Gray);
    names.push_back("DimGray"); enums.push_back(DimGray);
    names.push_back("LightSlateGray"); enums.push_back(LightSlateGray);
    names.push_back("SlateGray"); enums.push_back(SlateGray);
    names.push_back("DarkSlateGray"); enums.push_back(DarkSlateGray);
    names.push_back("Black"); enums.push_back(Black);
    for(size_t i = 0; i < names.size(); ++i) {
        namesmap[tolower(names[i])] = enums[i];
    }
    initialized = true;
}

unsigned int GUI::ColorPicker::table_left() {
    return left;
}

void GUI::ColorPicker::table_left(unsigned int l) {
    if (l <= 14) left = 14;
}

unsigned int GUI::ColorPicker::table_top() {
    return top;
}

void GUI::ColorPicker::table_top(unsigned int t) {
    if (t <= 14) top = 14;
}

unsigned int GUI::ColorPicker::table_width() {
    return width;
}

void GUI::ColorPicker::table_width(unsigned int w) {
    if (w <= 200) width = 200; else width = w;
}

unsigned int GUI::ColorPicker::table_height() {
    return height;
}

void GUI::ColorPicker::table_height(unsigned int h) {
    if (h <= 200) height = 200; else height = h;
}

ColorEnum GUI::ColorPicker::getenum(std::string name) const {
    name = tolower(name);
    if (namesmap.find(name) != namesmap.end()) {
        return namesmap[name];
    } else {
        return White;
    }
}

std::string GUI::ColorPicker::getname(ColorEnum name) const {
    for(size_t i = 0; i < enums.size(); ++i) {
        if (name == enums[i]) return names[i];
    }
    return "White";
}


std::pair<ColorEnum, std::string>
GUI::ColorPicker::pick(std::string oldname, const std::string& message) {
    return pick(getenum(oldname), message);
}

std::pair<ColorEnum, std::string>
GUI::ColorPicker::pick(ColorEnum oldname, const std::string& message) {
    size_t oldindex = 0;
    for(size_t i = 0; i < enums.size(); ++i) {
        if (oldname == enums[i]) {
            oldindex = i;
            break;
        }
    }
    std::pair<ColorEnum, std::string> result = std::make_pair(enums[oldindex], names[oldindex]);
    Screen& scr = *GetScreen();
    scr.backcolor(RGB(Black));
    scr.clear();
    Rect rect = { left,top + 14*7, width, height };
    Steps s = drawtable(rect);
    bool update = true, done = false;
    size_t maxindex = names.size()-1;
    size_t curindex = 0;
    PointXY mousepos(0, 0);
    RGBColor oldc = RGB(enums[oldindex]);
    while (true) {
        Delay(50);
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_MOUSEMOTION) {
                mousepos.x = Sint16(event.motion.x);
                mousepos.y = Sint16(event.motion.y);
                if (FallsIn(mousepos, rect)) {
                    mousepos.x -= rect.x;
                    mousepos.y -= rect.y;
                    update = true;
                }
            } else if(event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == 1) {
                mousepos.x = Sint16(event.button.x);
                mousepos.y = Sint16(event.button.y);
                    if (FallsIn(mousepos, rect)) {
                        mousepos.x -= rect.x;
                        mousepos.y -= rect.y;
                        update = true;
                        done = true;
                    }
                }
            } else if (event.type == SDL_KEYUP) {
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    return result;
                }
            }
            if (update) {
                curindex = floor(double(mousepos.y) / s.rowh)*s.cols +
                           floor(double(mousepos.x) / s.colw);
                if ((curindex>=0) && (curindex<=maxindex)) {
                    RGBColor newc = RGB(enums[curindex]);
                    PointXY oldorigin = scr.status.origin;
                    scr.status.origin = PointXY(left + 66, top);
                    Rect prew = rect;
                    prew.y = scr.status.origin.y + 14;
                    prew.w = scr.status.origin.x - rect.x - 14;
                    prew.h = 70;
                    DrawRect(prew, RGB(Silver), 0, RGB(Silver), true, scr.surface());
                    prew.h = 33;
                    prew.x +=2;
                    prew.y +=2;
                    prew.w -=4;
                    DrawRect(prew, RGB(enums[oldindex]), 0, RGB(enums[oldindex]), true, scr.surface());
                    prew.y += 33;
                    DrawRect(prew, RGB(enums[curindex]), 0, RGB(enums[curindex]), true, scr.surface());

                    scr.status.clear();
                    if (message.size()==0) {
                        scr.status.stream << "Pick a new color";
                    } else {
                        scr.status.stream << "Setting new color for: " << message;
                    }
                    scr.status.add(RGB(Red));
                    scr.status.stream << "Old Color: " << names[oldindex];
                    scr.status.stream << "                               \n";
                    scr.status.stream << "RGB(" << int(oldc.r) << ", " ;
                    scr.status.stream << int(oldc.g) << ", " << int(oldc.b);
                    scr.status.stream << ")            ";
                    scr.status.add(RGB(White));

                    scr.status.stream << "Press ESC to cancel";
                    scr.status.add(RGB(Red));

                    scr.status.stream << "New Color: " << names[curindex];
                    scr.status.stream << "                               \n";
                    scr.status.stream << "RGB(" << int(newc.r) << ", " ;
                    scr.status.stream << int(newc.g) << ", " << int(newc.b);
                    scr.status.stream << ")            ";

                    scr.status.add(RGB(White));
                    scr.status.print();
                    scr.status.origin = oldorigin;
                    scr.flip();
                    if (done) {
                        result.first = enums[curindex];
                        result.second = names[curindex];
                        return result;
                    }
                }
                update = false;
            }
        }
    }
}

GUI::ColorPicker::Steps GUI::ColorPicker::drawtable(Rect& rect) {
    Screen& scr = *GetScreen();
    Steps s;
    size_t maxindex = names.size()-1;
    s.cols = s.rows = sqrt(maxindex);
    if (s.cols*s.rows < maxindex) ++s.rows;
    s.colw = s.rowh = rect.w/s.cols;
    rect.w = s.colw * s.cols;
    rect.h = s.rowh * s.rows;
    DrawRect(rect, RGB(Silver), 1, RGB(Silver), true, scr.surface());
    for(Uint16 ir = 0; ir < s.rows; ++ir) {
        for(Uint16 ic = 0; ic < s.cols; ++ic) {
            Uint16 i = ir * s.cols + ic;
            if (i>maxindex) continue;
            Rect r = { rect.x+ic*s.colw, rect.y+ir*s.rowh, s.colw+1, s.rowh+1 };
            DrawRect(r, RGB(Silver), 1, RGB(enums[i]), true, scr.surface());
        }
    }
    return s;
}
