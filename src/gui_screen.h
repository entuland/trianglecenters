#ifndef SCREEN_H
#define SCREEN_H

#include "gui_common.h"
#include "gui_drawing.h"
#include <queue>
#include <sstream>

namespace GUI {

struct Axes {
    double tickspacing;
    Sint16 ticksize;
    bool showgrid;
    RGBColor axis;
    RGBColor tick;
    RGBColor grid;
};


class Messages
{
        std::queue< std::pair< std::string, RGBColor> > messages;
    public:
        PointXY origin;
        std::stringstream stream;
        Messages() {};
        void add(std::string, RGBColor);
        void add(RGBColor);
        void print(bool fixed = false, int size = 1);
        void clear() {
            stream.str("");
            while(!messages.empty()) messages.pop();
        }
};

class ColorPicker
{
    public:
        ColorPicker();
        ColorEnum getenum(std::string name) const;
        std::string getname(ColorEnum name) const;
        std::pair<ColorEnum, std::string>
            pick(std::string oldcolor, const std::string& message = "");
        std::pair<ColorEnum, std::string>
            pick(ColorEnum oldcolor, const std::string& message = "");
        unsigned int table_left();
        void table_left(unsigned int l);
        unsigned int table_top();
        void table_top(unsigned int t);
        unsigned int table_width();
        void table_width(unsigned int w);
        unsigned int table_height();
        void table_height(unsigned int h);
    private:
        unsigned int left;
        unsigned int top;
        unsigned int width;
        unsigned int height;
        static std::vector<ColorEnum> enums;
        static std::vector<std::string> names;
        static std::map<std::string, ColorEnum> namesmap;
        static void initialize();
        struct Steps {
            unsigned short colw, rowh, cols, rows;
        };
        Steps drawtable(Rect&);
};

class Canvas {
    protected:
        Surface* srf;
        Rect utile;
    public:
        Canvas() {};
        Canvas(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Surface* surface);
        RGBColor backcolor();
        void backcolor(RGBColor newcolor);
        RGBColor forecolor();
        void forecolor(RGBColor newcolor);
        void drawline(PointXY point1, PointXY point2);
        void drawline(PointXY point1, PointXY point2, RGBColor color);
        void drawpoint(PointXY point);
        void drawpoint(PointXY point, RGBColor color);
        void crosshair(PointXY point);
        void crosshair(PointXY point, RGBColor color);
        void crossbox(PointXY point);
        void crossbox(PointXY point, RGBColor color);
        void drawaxes();
        Uint32 width();
        Uint32 height();
        Axes axes;
        Messages status;
        RGBColor bgcol;
        RGBColor fgcol;
};

inline Canvas::Canvas(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Surface* surface) {
    utile.x = x;
    utile.y = y;
    utile.w = w;
    utile.h = h;
    srf = surface;
}


class Screen: public Canvas {
    public:
        class double_screen {}; // Used as exception
        Screen(Uint32 heightval = 640,
               Uint32 widthval = 480,
               Uint32 bitsperpixel = 32);
        ~Screen();
        void flip();
        void clear();
        Surface* surface();
        ColorPicker picker;
           Controls::BaseObj* focused;
        Controls::BaseObj* mouselock;
    private:
        Screen(Screen&) {}
        Screen& operator=(Screen&) { return *this; }
};

inline Surface* Screen::surface() {
    return srf;
}

inline void Screen::flip() {
    SDL_Flip(srf);
}

inline void Screen::clear() {
    SDL_FillRect(srf, 0, Map(bgcol, srf));
}

inline Uint32 Canvas::width() {
    return srf->w;
}

inline Uint32 Canvas::height() {
    return srf->h;
}

inline RGBColor Canvas::backcolor() {
    return bgcol;
}

inline void Canvas::backcolor(RGBColor newcolor) {
    bgcol = newcolor;
}

inline RGBColor Canvas::forecolor() {
    return fgcol;
}

inline void Canvas::forecolor(RGBColor newcolor) {
    fgcol = newcolor;
}

inline void Canvas::drawline(PointXY point1, PointXY point2, RGBColor color) {
    GUI::DrawLine(point1, point2, color, srf);
}

inline void Canvas::drawline(PointXY point1, PointXY point2) {
    GUI::DrawLine(point1, point2, fgcol, srf);
}

inline void Canvas::drawpoint(PointXY point, RGBColor color) {
    GUI::DrawPoint(point, color, srf);
}

inline void Canvas::drawpoint(PointXY point) {
    GUI::DrawPoint(point, fgcol, srf);
}

inline void Canvas::crosshair(PointXY point, RGBColor color) {
    GUI::DrawCrossHair(point, color, srf);
}

inline void Canvas::crosshair(PointXY point) {
    GUI::DrawCrossHair(point, fgcol, srf);
}

inline void Canvas::crossbox(PointXY point, RGBColor color) {
    GUI::DrawCrossBox(point, color, srf);
}

inline void Canvas::crossbox(PointXY point) {
    GUI::DrawCrossBox(point, fgcol, srf);
}

void Status(std::string message, bool fixed = false, int size = 1);

} // namespace GUI

#endif //SCREEN_H
