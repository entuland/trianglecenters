#ifndef GUI_COMMON_H
#define GUI_COMMON_H

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <SDL/SDL.h>


namespace Controls {
    class BaseObj; // Forward declaration, see "gui_controls.h" for definition
}
namespace GUI {

class Screen; // Forward declaration, see "gui_screen.h" for definition
Screen* GetScreen(); // Returns current Screen
/**********************************************************************************/
// Typedefs and SDL functions wrappers
/**********************************************************************************/

typedef SDL_Rect Rect;
typedef SDL_Surface Surface;
typedef SDL_Color RGBColor;

//const double Pi = 3.1415926535897932;
const double Pi = (4*atan(1.0/5)-atan(1.0/239))*4;

inline int Lock(Surface* Srf) {
    if (SDL_MUSTLOCK(Srf)) return SDL_LockSurface(Srf);
    return 0;
}

inline void Unlock(Surface* Srf) {
    if (SDL_MUSTLOCK(Srf)) SDL_UnlockSurface(Srf);
}

inline void Delay(Uint32 ms) {
    SDL_Delay(ms);
}

inline void Delays(Uint32 sec) {
    SDL_Delay(sec*1000);
}

SDLKey WaitForKey(int delay = 10);

void WaitForKeyClick(int delay = 10);


/**********************************************************************************/
// Math helpers
/**********************************************************************************/

template<class T> inline int Sign(const T& number) {
    return (number < 0) ? -1 : 1;
}

inline double Rad(double Degree) {
    return (Pi * (Degree / 180));
}

template<class T> inline T Min(T first, T second) {
    return (first < second) ? first : second;
}

template<class T> inline T Max(T first, T second) {
    return (first > second) ? first : second;
}

template<class T, class U> inline T Min(T first, U second) {
    return (first < T(second) ? first : T(second));
}

template<class T, class U> inline T Max(T first, U second) {
    return (first > T(second) ? first : T(second));
}

template <class T> inline bool Inside( T lbound, T value, T ubound )
{
    return ( lbound <= value ) && ( value <= ubound );
}

/**********************************************************************************/
// String helpers
/**********************************************************************************/

inline std::string tolower(const std::string& s) {
    std::stringstream result;
    for(size_t i = 0; i < s.size(); ++i) {
        result << (isalpha(s[i]) ? std::tolower(s[i]) : s[i]);
    }
    return result.str();
}

inline std::string toupper(const std::string& s) {
    std::stringstream result;
    for(size_t i = 0; i < s.size(); ++i) {
        result << (isalpha(s[i]) ? std::toupper(s[i]) : s[i]);
    }
    return result.str();
}

/**********************************************************************************/
// Point structures and related functions
/**********************************************************************************/

struct PointXY {
    Sint16 x;
    Sint16 y;
    PointXY(Sint16 xval = 0, Sint16 yval = 0): x(xval), y(yval) {}
    bool operator==(const PointXY& comp) const {
        return (x==comp.x) && (y==comp.y);
    }
    bool operator!=(const PointXY& comp) const {
        return !(*this==comp);
    }
    PointXY& operator()(const Sint16& xval, const Sint16& yval) {
        x = xval;
        y = yval;
        return *this;
    }
    PointXY& operator+=(const PointXY& summon) {
        x += summon.x;
        y += summon.y;
        return *this;
    }
    PointXY& operator-=(const PointXY& subtract) {
        x -= subtract.x;
        y -= subtract.y;
        return *this;
    }
    PointXY& operator*=(const PointXY& multiply) {
        x *= multiply.x;
        y *= multiply.y;
        return *this;
    }
    PointXY& operator/=(const PointXY& divide) {
        x /= divide.x;
        y /= divide.y;
        return *this;
    }
};

inline PointXY operator+(const PointXY& term1, const PointXY& term2) {
    PointXY result(term1);
    result += term2;
    return result;
}

inline PointXY operator-(const PointXY& term1, const PointXY& term2) {
    PointXY result(term1);
    result -= term2;
    return result;
}

inline PointXY operator*(const PointXY& term1, const PointXY& term2) {
    PointXY result(term1);
    result *= term2;
    return result;
}

inline PointXY operator/(const PointXY& term1, const PointXY& term2) {
    PointXY result(term1);
    result /= term2;
    return result;
}

inline std::ostream& operator<<(std::ostream& s, const PointXY& p) {
    return s << "(" << p.x << ", " << p.y << ")";
}


inline int Distance(const PointXY& p1, const PointXY& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


struct PrecXY {
    double x;
    double y;
    PrecXY(double xval = 0, double yval = 0): x(xval), y(yval) {}
    PrecXY(const PrecXY& p): x(p.x), y(p.y) {};
    operator PointXY() const {
        PointXY point;
        point.x = Sint16(floor(x+0.5));
        point.y = Sint16(floor(y+0.5));
        return point;
    }
    PrecXY& operator()(const double& xval, const double& yval) {
        x = xval;
        y = yval;
        return *this;
    }
    PrecXY& operator+=(const PrecXY& summon) {
        x += summon.x;
        y += summon.y;
        return *this;
    }
    PrecXY& operator-=(const PrecXY& subtract) {
        x -= subtract.x;
        y -= subtract.y;
        return *this;
    }
    PrecXY& operator*=(const PrecXY& multiply) {
        x *= multiply.x;
        y *= multiply.y;
        return *this;
    }
    PrecXY& operator/=(const PrecXY& divide) {
        x /= divide.x;
        y /= divide.y;
        return *this;
    }

    bool operator==(const PrecXY& comp) const {
        return (x == comp.x) && (y == comp.y);
    }

    bool operator!=(const PrecXY& comp) const {
        return !(*this==comp);
    }

    void Scale(double xfact, double yfact) {
        x *= xfact;
        y *= yfact;
    }
};

inline PrecXY operator+(const PrecXY& term1, const PrecXY& term2) {
    PrecXY result(term1);
    result += term2;
    return result;
}

inline PrecXY operator-(const PrecXY& term1, const PrecXY& term2) {
    PrecXY result(term1);
    result -= term2;
    return result;
}

inline PrecXY operator*(const PrecXY& term1, const PrecXY& term2) {
    PrecXY result(term1);
    result *= term2;
    return result;
}

inline PrecXY operator/(const PrecXY& term1, const PrecXY& term2) {
    PrecXY result(term1);
    result /= term2;
    return result;
}

inline std::ostream& operator<<(std::ostream& s, const PrecXY& p) {
    return s << "(" << p.x << ", " << p.y << ")";
}

inline bool FallsIn(const PointXY& point, const Rect& rect) {
    return ((rect.x <= point.x) && (point.x <= rect.x + rect.w - 1) &&
            (rect.y <= point.y) && (point.y <= rect.y + rect.h - 1 ));
}

inline bool FallsIn(const PointXY& point, const Rect& rect, int correction) {
    return ((rect.x - correction <= point.x) &&
            (point.x <= rect.x + rect.w - 1 + correction) &&
            (rect.y - correction <= point.y) &&
            (point.y <= rect.y + rect.h - 1 + correction ));
}

inline bool FallsIn(int x, int y, const Rect& rect, int correction) {
    return FallsIn(PointXY(x,y), rect, correction);
}


inline PointXY PosToCoord(const Uint32& position, const Uint16& xsize) {
    PointXY point;
    point.x = Uint16(position % xsize);
    point.y = Uint16(std::div(position, xsize).quot);
    return point;
}

inline bool CoordToPos(const PointXY& point, const Rect& rect, Uint32* position) {
    if (!FallsIn(point, rect)) {
        return false;
    }
    *position = Uint32(point.y * rect.w  + point.x);
    return true;
}

inline PrecXY PolToCart(PrecXY polar) {
    PrecXY result;
    result.x = polar.x * cos(polar.y);
    result.y = polar.x * sin(polar.y);
    return result;
}

inline PrecXY CartToPol(PrecXY point) {
    PrecXY polar;
    polar.x = sqrt(pow(point.x, 2) + pow(point.y, 2));
    polar.y = atan2(point.y, point.x);
    return polar;
}

enum OrientationEnum {
    slope = 0,
    horizontal,
    vertical,
    pointregressed
};

struct ParametricLine {
    double a, b, c, k;
    OrientationEnum orient;
    ParametricLine() {};
    ParametricLine(const PrecXY&, const PrecXY&);
};

inline ParametricLine::ParametricLine(const PrecXY& p1, const PrecXY& p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = p2.x*p1.y - p1.x*p2.y;
    if(a == 0 && b == 0) {
        orient = pointregressed;
    } else if(a == 0) {
        orient = horizontal;
        k = p1.y;
    } else if(b == 0) {
        orient = vertical;
        k = p1.x;
    } else {
        orient = slope;
    }
}

struct ExplicitLine: public ParametricLine{
    double m, q;
    ExplicitLine() {}
    ExplicitLine(const PrecXY&, const PrecXY&);
};

inline ExplicitLine::ExplicitLine(const PrecXY& p1, const PrecXY& p2)
        : ParametricLine(p1, p2) {
    if (orient == slope) {
        m = -a/b;
        q = -c/b;
    }
}

inline PrecXY MiddlePoint(const PrecXY& p1, const PrecXY& p2) {
    return PrecXY((p2.x+p1.x)/2,(p2.y+p1.y)/2);
}

inline bool Intersect(const ParametricLine& r, const ParametricLine& s, PrecXY* result) {
    double denom = r.a*s.b - s.a*r.b;
    if (denom==0) return false;
    result->x = (r.b*s.c - s.b*r.c)/denom;
    result->y = (s.a*r.c - r.a*s.c)/denom;
    return true;
}

inline bool Intersect(const PrecXY& r1,
                      const PrecXY& r2,
                      const PrecXY& s1,
                      const PrecXY& s2,
                      PrecXY* result) {
    ParametricLine r(r1, r2);
    ParametricLine s(s1, s2);
    return Intersect(r, s, result);
}

inline Rect Intersect(const Rect& R1,
                        const Rect& R2) {

    Rect RR;
    RR.x = std::max(R1.x, R2.x); //R1.x > R2.x ? R1.x : R2.x;
    RR.y = std::max(R1.y, R2.y); //R1.y > R2.y ? R1.y : R2.y;
    int R1x2 = R1.x + R1.w;
    int R2x2 = R2.x + R2.w;
    int R1y2 = R1.y + R1.h;
    int R2y2 = R2.y + R2.h;
    if ((R2x2 < R1.x) || (R2y2 < R1.y) || (R2.x > R1x2) || (R2.y > R1y2)) {
        RR.w = 0;
        RR.h = 0;
    } else {
        RR.w = -RR.x + (R1x2 < R2x2 ? R1x2 : R2x2);
        RR.h = -RR.y + (R1y2 < R2y2 ? R1y2 : R2y2);
    }
    return RR;
}


inline ParametricLine Normal(const ParametricLine& r, const PrecXY& p) {
    PrecXY ortho;
    double m;
    double q;
    switch (r.orient) {
        case horizontal:
            ortho(p.x, p.y + 10);
            break;
        case vertical:
            ortho(p.x + 10, p.y);
            break;
        case slope:
            m = -1/(-r.a/r.b);
            q = p.y - m * p.x;
            ortho(p.x + 10, m * (p.x + 10) + q);
            break;
    case pointregressed: default:
            return r;
    }
    return ParametricLine(p, ortho);
}

inline double Distance(const PrecXY& p1, const PrecXY& p2) {
    ParametricLine r(p1, p2);
    switch (r.orient) {
        case pointregressed:
            return 0;
        case horizontal:
            return fabs(r.b);
        case vertical:
            return fabs(r.a);
    case slope: default:
            return sqrt(pow(r.b, 2) + pow(r.a, 2));
    }
}


inline double Distance(const PrecXY& p, const ParametricLine& r) {
    double denom = sqrt(r.a*r.a + r.b*r.b);
    return (denom!=0) ? fabs((r.a*p.x + r.b*p.y + r.c) / denom) : 0;
}

inline double Distance(const PrecXY& p, const PrecXY& r1, const PrecXY& r2) {
    return Distance(p, ParametricLine(r1, r2));
}

inline bool Clockwise(const PrecXY& p1, const PrecXY& p2,const PrecXY& p3) {
    // as taken from
    // The Win95 Game Programmer's Encyclopedia by Mark Feldman
    double E1x = p1.x - p2.x;
    double E1y = p1.y - p2.y;
    double E2x = p3.x - p2.x;
    double E2y = p3.y - p2.y;
    if ((E1x * E2y - E1y * E2x) >= 0) {
        return true;
    } else {
        return false;
    }
}


/**********************************************************************************/
// Color constants and functions
/**********************************************************************************/

enum ColorEnum {
    Black = 0,
    AliceBlue,
    AntiqueWhite,
    Aqua,
    Aquamarine,
    Azure,
    Beige,
    Bisque,
    BlanchedAlmond,
    Blue,
    BlueViolet,
    Brown,
    BurlyWood,
    CadetBlue,
    Chartreuse,
    Chocolate,
    Coral,
    CornflowerBlue,
    Cornsilk,
    Crimson,
    Cyan,
    DarkBlue,
    DarkCyan,
    DarkGoldenrod,
    DarkGray,
    DarkGreen,
    DarkKhaki,
    DarkMagenta,
    DarkOliveGreen,
    DarkOrange,
    DarkOrchid,
    DarkRed,
    DarkSalmon,
    DarkSeaGreen,
    DarkSlateBlue,
    DarkSlateGray,
    DarkTurquoise,
    DarkViolet,
    DeepPink,
    DeepSkyBlue,
    DimGray,
    DodgerBlue,
    FireBrick,
    FloralWhite,
    ForestGreen,
    Fuchsia,
    Gainsboro,
    GhostWhite,
    Gold,
    Goldenrod,
    Gray,
    Gray10,
    Gray20,
    Gray30,
    Gray40,
    Gray50,
    Gray60,
    Gray70,
    Gray80,
    Gray90,
    Green,
    GreenYellow,
    Honeydew,
    HotPink,
    IndianRed,
    Indigo,
    Ivory,
    Khaki,
    Lavender,
    LavenderBlush,
    LawnGreen,
    LemonChiffon,
    LightBlue,
    LightCoral,
    LightCyan,
    LightGoldenrodYellow,
    LightGreen,
    LightGray,
    LightPink,
    LightSalmon,
    LightSeaGreen,
    LightSkyBlue,
    LightSlateGray,
    LightSteelBlue,
    LightYellow,
    Lime,
    LimeGreen,
    Linen,
    Magenta,
    Maroon,
    MediumAquamarine,
    MediumBlue,
    MediumOrchid,
    MediumPurple,
    MediumSeaGreen,
    MediumSlateBlue,
    MediumSpringGreen,
    MediumTurquoise,
    MediumVioletRed,
    MidnightBlue,
    MintCream,
    MistyRose,
    Moccasin,
    NavajoWhite,
    Navy,
    OldLace,
    Olive,
    OliveDrab,
    Orange,
    OrangeRed,
    Orchid,
    PaleGoldenrod,
    PaleGreen,
    PaleTurquoise,
    PaleVioletRed,
    PapayaWhip,
    PeachPuff,
    Peru,
    Pink,
    Plum,
    PowderBlue,
    Purple,
    Red,
    RosyBrown,
    RoyalBlue,
    SaddleBrown,
    Salmon,
    SandyBrown,
    SeaGreen,
    Seashell,
    Sienna,
    Silver,
    SkyBlue,
    SlateBlue,
    SlateGray,
    Snow,
    SpringGreen,
    SteelBlue,
    Tan,
    Teal,
    Thistle,
    Tomato,
    Turquoise,
    Violet,
    Wheat,
    WhiteSmoke,
    Yellow,
    YellowGreen,
    White,
};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 rmask = 0xff000000;
const Uint32 gmask = 0x00ff0000;
const Uint32 bmask = 0x0000ff00;
const Uint32 amask = 0x000000ff;
const int rshift = 24;
const int gshift = 16;
const int bshift = 8;
const int ashift = 0;
#else
const Uint32 rmask = 0x000000ff;
const Uint32 gmask = 0x0000ff00;
const Uint32 bmask = 0x00ff0000;
const Uint32 amask = 0xff000000;
const int rshift = 0;
const int gshift = 8;
const int bshift = 16;
const int ashift = 24;
#endif

inline std::ostream& operator<<(std::ostream& stream, RGBColor& color) {
    return stream << "RGB(" << int(color.r) << ", "
           << int(color.g) << ", " << int(color.b) << ")";
}

inline RGBColor RGB(Uint8 red, Uint8 green, Uint8 blue) {
    RGBColor color;
    color.r = red;
    color.g = green;
    color.b = blue;
    return color;
}

inline void Invert(RGBColor* color) {
    color->r = 0xFF - color->r;
    color->g = 0xFF - color->g;
    color->b = 0xFF - color->b;
}


inline RGBColor Greys(Uint8 intensity) {
    RGBColor result = { intensity, intensity, intensity };
    return result;
}

RGBColor RGB(ColorEnum colorkey);

RGBColor GetRGB(const PointXY& point, Surface* orig);

inline Uint32 Map(RGBColor color, Surface* dest) {
    return SDL_MapRGB(dest->format, color.r, color.g, color.b);
}

inline Uint32 Map(ColorEnum colorkey, Surface* dest) {
    RGBColor color = RGB(colorkey);
    return SDL_MapRGB(dest->format, color.r, color.g, color.b);
}

inline Surface* CreateSurface(Uint16 width, Uint16 height) {
    return SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
}

/**********************************************************************************/
// Other
/**********************************************************************************/

enum Alignment {
    left, center, right,
    topleft, topcenter, topright,
    bottomleft, bottomcenter, bottomright
};

enum BorderStyle {
    solid, risen, sunken, inset, outset, dots, dashes
};

} // namespace GUI

#endif // GUI_COMMON_H
