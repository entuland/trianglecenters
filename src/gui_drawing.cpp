#include "gui_drawing.h"
#include <queue>

using namespace GUI;

inline PrecXY MaxX(PrecXY p1, PrecXY p2) {
    if (p1.x > p2.x) { return p1; } else { return p2; }
}

inline PrecXY MinX(PrecXY p1, PrecXY p2) {
    if (p1.x < p2.x) { return p1; } else { return p2; }
}

inline PrecXY MaxY(PrecXY p1, PrecXY p2) {
    if (p1.y > p2.y) { return p1; } else { return p2; }
}

inline PrecXY MinY(PrecXY p1, PrecXY p2) {
    if (p1.y < p2.y) { return p1; } else { return p2; }
}

void GUI::DrawPoint(const PointXY& point,
                    const RGBColor& color,
                    Surface* dest) {
    Uint32 pxcol = Map(color, dest);
    Rect rect = {0, 0, dest->w, dest->h};
    Uint32 offset;
    if(CoordToPos(point, rect, &offset)) {
        if (Lock(dest)==0) {
            Uint32* mp = static_cast<Uint32* >(dest->pixels);
            mp += offset;
            *mp = pxcol;
            Unlock(dest);
        }
    }
}


void GUI::DrawLine(PointXY p1,
                   PointXY p2,
                   RGBColor color,
                   Surface* dest) {
    // Implemented using Bresenham's incremental algorithm of the line
    Uint16 clip = 0;
    Rect r = {clip, clip, dest->w-clip*2, dest->h-clip*2};

    // If line is out of boundaries:
    if (    ( (p1.x < r.x)       && (p2.x < r.x)       ) || // out on the left
            ( (p1.x > r.w-1+r.x) && (p2.x > r.w-1+r.x) ) || // out on the right
            ( (p1.y < r.y)       && (p2.y < r.y)       ) || // out on the top
            ( (p1.y > r.h-1+r.y) && (p2.y > r.h-1+r.y) )    // out on the bottom
       ) {
        return;
    }

    PrecXY prec1(p1.x, p1.y);
    PrecXY prec2(p2.x, p2.y);
    ExplicitLine line(prec1, prec2);

    if (line.orient == horizontal) {
        if (p1.x > p2.x) std::swap(p1, p2);
        p1.x = Max(p1.x, r.x);
        p2.x = Min(p2.x, r.w + r.x -1);
        r.x = p1.x;
        r.y = p1.y;
        r.w = p2.x - p1.x + 1;
        r.h = 1;
        SDL_FillRect(dest, &r, Map(color, dest));
        return;
    }

    if (line.orient == vertical) {
        if (p1.y > p2.y) std::swap(p1, p2);
        p1.y = Max(p1.y, r.y);
        p2.y = Min(p2.y, r.h+r.y-1);
        r.x = p1.x;
        r.y = p1.y;
        r.w = 1;
        r.h = p2.y - p1.y + 1;
        SDL_FillRect(dest, &r, Map(color, dest));
        return;
    }

    PrecXY topleft(r.x, r.y);
    PrecXY topright(r.x+r.w-1, r.y);
    PrecXY bottomleft(r.x, r.y+r.h-1);
    PrecXY bottomright(r.x+r.w-1, r.y+r.h-1);

    ParametricLine left(topleft, bottomleft);
    ParametricLine right(topright, bottomright);
    ParametricLine top(topleft, topright);
    ParametricLine bottom(bottomleft, bottomright);

    // Intersection points between given line and
    // the lines of the visualization rectangle:
    PrecXY leftx, rightx, topx, bottomx;

    if (!FallsIn(p1, r) || !FallsIn(p2, r)) {
        Intersect(line, left, &leftx);
        Intersect(line, right, &rightx);
        Intersect(line, top, &topx);
        Intersect(line, bottom, &bottomx);
        if (!FallsIn(leftx, r) &&
                !FallsIn(rightx, r) &&
                !FallsIn(topx, r) &&
                !FallsIn(bottomx, r)) {
            return;
        }
    }

    Sint32 dx = abs(p2.x - p1.x);
    Sint32 dy = abs(p2.y - p1.y);
    Sint32 d, difneg, difpos;
    Sint16 change = (Sign(line.b) == Sign(line.a)) ? -1 : 1;

    // Horizontal slope:
    if (fabs(line.m) <= 1) {

        Uint32 mapcolor = Map(color, dest);

        if (p1.x > p2.x) std::swap(p1, p2);

        if (!FallsIn(p1, r))
            p1 = MaxX(leftx, ( line.m>0 ? topx : bottomx));
        if (!FallsIn(p2, r))
            p2 = MinX(rightx, ( line.m<0 ? topx : bottomx));
        if (!FallsIn(p1, r) || !FallsIn(p2, r)) {
            return;
        }

        // Printing Part
        if(Lock(dest)!=0){
            return;
        }
        d = dy * 2 - dx;
        difneg = 2 * dy;
        difpos = 2 * (dy - dx);
        Uint32* firstpix = static_cast<Uint32*>(dest->pixels);
        size_t maxoff = dest->w * dest->h -1;
        size_t curoff = (p1.y * dest->w) + p1.x;
        if (maxoff > curoff) *(firstpix + curoff) = mapcolor;
        while (p1.x < p2.x) {
            if (d < 1) { d += difneg;
                ++p1.x;
            }
            else { d += difpos;
                ++p1.x;
                p1.y += change;
            }
            curoff = (p1.y * dest->w) + p1.x;
            if (maxoff > curoff) *(firstpix + curoff) = mapcolor;
        }
        Unlock(dest);
    }

    // Vertical slope:
    else { // if (fabs(1/m) < 1)
        Uint32 mapcolor = Map(color, dest);

        if (p1.y > p2.y) std::swap(p1, p2);

        if (!FallsIn(p1, r))
            p1 = MaxY(topx, ( (1/line.m)>0 ? leftx : rightx));
        if (!FallsIn(p2, r))
            p2 = MinY(bottomx, ((1/line.m)<0 ? leftx : rightx));
        if (!FallsIn(p1, r) || !FallsIn(p2, r)) {
            return;
        }

        // Printing Part
        if(Lock(dest)!=0){
            return;
        }
        d = dx * 2 - dy;
        difneg = 2 * dx;
        difpos = 2 * (dx - dy);
        Uint32* firstpix = static_cast<Uint32*>(dest->pixels);
        size_t maxoff = dest->w * dest->h;
        size_t curoff = (p1.y * dest->w) + p1.x;
        if (maxoff > curoff) *(firstpix + curoff) = mapcolor;
        while (p1.y < p2.y) {
            if (d < 1) { d += difneg;
                ++p1.y;
            }
            else { d += difpos;
                ++p1.y;
                p1.x += change;
            }
            curoff = (p1.y * dest->w) + p1.x;
            if (maxoff > curoff) *(firstpix + curoff) = mapcolor;
        }
        Unlock(dest);
    }
}



void ScanPolyShowit(const std::vector<PointXY>& points,
                    std::map<Sint16, std::multiset<Sint16> >* scan,
                    Surface* dest) {
    // Implemented using Bresenham's incremental algorithm of the line
    Sint16 maxx = dest->w-1;
    Sint16 maxy = dest->h-1;

    for (size_t i = 1; i < points.size(); ++i) {

        PointXY p1 = points[i-1];
        PointXY p2 = points[i];

        // If line is not out of boundaries:
        if (!(
                    ( (p1.x < 0)    && (p2.x < 0)    ) || // out on the left
                    ( (p1.x > maxx) && (p2.x > maxx) ) || // out on the right
                    ( (p1.y < 0)    && (p2.y < 0)    ) || // out on the top
                    ( (p1.y > maxy) && (p2.y > maxy) )    // out on the bottom
                ) )
        {
            // Computes the differences between equal coordinates:
            Sint16 Yd = p2.y - p1.y;
            Sint16 Xd = p2.x - p1.x;
            Sint16 change = (Sign(Xd)==Sign(Yd)) ? 1 : -1;
            // Explicit formula: [y = mx + q] -> [m = (y2-y1)/(x2-x1)]
            double m = double(Yd) / Xd;
            // If line is an horizontal slope:
            if (fabs(m) <= 1) {
                if (p1.x > p2.x) std::swap(p1, p2);

                Sint16 dx = abs(p2.x - p1.x);
                Sint16 dy = abs(p2.y - p1.y);

                Sint16 d = dy * 2 - dx;
                Sint16 difneg = 2 * dy;
                Sint16 difpos = 2 * (dy - dx);

                (*scan)[p1.y].insert(p1.x);
                while (p1.x < p2.x) {
                    if (d <= 0) {
                        d += difneg;
                        ++p1.x;
                    }
                    else {
                        d += difpos;
                        ++p1.x;
                        p1.y += change;
                    }
                    (*scan)[p1.y].insert(p1.x);
                }
            }
            // Here the line can't be anything else than a vertical slope:
            else { // if (fabs(1/m) < 1)
                if (p1.y > p2.y) std::swap(p1, p2);

                Sint16 dx = abs(p2.x - p1.x);
                Sint16 dy = abs(p2.y - p1.y);

                Sint16 d = dx * 2 - dy;
                Sint16 difneg = 2 * dx;
                Sint16 difpos = 2 * (dx - dy);

                (*scan)[p1.y].insert(p1.x);
                while (p1.y < p2.y) {
                    if (d <= 0) {
                        d += difneg;
                        ++p1.y;
                    }
                    else {
                        d += difpos;
                        ++p1.y;
                        p1.x+=change;
                    }
                    (*scan)[p1.y].insert(p1.x);
                }
            }
        } // If inside of boundaries
    } // For loop
} // End of function


void GUI::ScanPoly(const std::vector<PointXY>& points,
                   std::map<Sint16, std::multiset<Sint16> >* scan,
                   Surface* dest) {
    // Implemented using Bresenham's incremental algorithm of the line
    Sint16 maxx = dest->w-1;
    Sint16 maxy = dest->h-1;
    for (size_t i = 1; i < points.size(); ++i) {
        PointXY p1 = points[i-1];
        PointXY p2 = points[i];
        // If line is inside of boundaries:
        if (!(
                    ( (p1.x < 0)    && (p2.x < 0)    ) || // out on the left
                    ( (p1.x > maxx) && (p2.x > maxx) ) || // out on the right
                    ( (p1.y < 0)    && (p2.y < 0)    ) || // out on the top
                    ( (p1.y > maxy) && (p2.y > maxy) )    // out on the bottom
                ) )
        {
            // Computes the differences between equal coordinates:
            Sint16 Yd = p2.y - p1.y;
            Sint16 Xd = p2.x - p1.x;
            // Explicit formula: [y = mx + q] -> [m = (y2-y1)/(x2-x1)]
            double m = double(Yd) / Xd;
            // If line is an horizontal slope:
            if (fabs(m) <= 1) {
                if (p1.x > p2.x) std::swap(p1, p2);
                Sint16 dx = abs(p2.x - p1.x);
                Sint16 dy = abs(p2.y - p1.y);
                Sint16 d = dy * 2 - dx;
                Sint16 difneg = 2 * dy;
                Sint16 difpos = 2 * (dy - dx);
                (*scan)[p1.y].insert(p1.x);
                if (Sign(Xd) == Sign(Yd)) {
                    while (p1.x < p2.x) {
                        if (d <= 0) {
                            d += difneg;
                            ++p1.x;
                        }
                        else {
                            d += difpos;
                            ++p1.x;
                            ++p1.y;
                        }
                        (*scan)[p1.y].insert(p1.x);
                    }
                } else {
                    while (p1.x < p2.x) {
                        if (d <= 0) {
                            d += difneg;
                            ++p1.x;
                        }
                        else {
                            d += difpos;
                            ++p1.x;
                            --p1.y;
                        }
                        (*scan)[p1.y].insert(p1.x);
                    }
                }
            }
            // Line is a vertical slope:
            else {
                if (p1.y > p2.y) std::swap(p1, p2);
                Sint16 dx = abs(p2.x - p1.x);
                Sint16 dy = abs(p2.y - p1.y);
                Sint16 d = dx * 2 - dy;
                Sint16 difneg = 2 * dx;
                Sint16 difpos = 2 * (dx - dy);
                (*scan)[p1.y].insert(p1.x);
                if (Sign(Xd) == Sign(Yd)) {
                    while (p1.y < p2.y) {
                        if (d <= 0) {
                            d += difneg;
                            ++p1.y;
                        }
                        else {
                            d += difpos;
                            ++p1.y;
                            ++p1.x;
                        }
                        (*scan)[p1.y].insert(p1.x);
                    }
                } else {
                    while (p1.y < p2.y) {
                        if (d <= 0) {
                            d += difneg;
                            ++p1.y;
                        }
                        else {
                            d += difpos;
                            ++p1.y;
                            --p1.x;
                        }
                        (*scan)[p1.y].insert(p1.x);
                    }
                }
            }
        } // If inside of boundaries
    } // For loop
} // End of function


void GUI::DrawScan(const std::map<Sint16, std::multiset<Sint16> >& scan,
                   const RGBColor& color,
                   Surface* dest) {
    if (Lock(dest)==0) {
        Uint32 pxcol = Map(color, dest);
        Uint32* mp = static_cast<Uint32* >(dest->pixels);
        Sint16 maxx = dest->w - 1;
        Sint16 maxy = dest->h - 1;
        PointXY point;
        std::map<Sint16, std::multiset<Sint16> >::const_iterator mapi;
        std::multiset<Sint16>::const_iterator seti;
        for (mapi = scan.begin(); mapi != scan.end(); ++mapi) {
            for (seti = (*mapi).second.begin(); seti != (*mapi).second.end(); ++seti) {
                point.y = (*mapi).first;
                point.x = (*seti);
                if(
                    (0 <= point.x) && (point.x <= maxx ) &&
                    (0 <= point.y) && (point.y <= maxy )
                ) {
                    *(mp + (point.y * dest->w  + point.x)) = pxcol;
                }
            }
        }
        Unlock(dest);
    }
}



void GUI::DrawRect(const Rect& orig_rect,
                   const RGBColor& bordercolor,
                   const Uint32& bordersize,
                   const RGBColor& fillcolor,
                   const bool& fill,
                   Surface* dest) {
    Rect line = { 0, 0, dest->w, dest->h };
    Rect rect = Intersect(orig_rect, line);
    line = rect;
    Uint32 borderc = Map(bordercolor, dest);
    Uint32 fillc = Map(fillcolor, dest);
    if (fill && (borderc == fillc)) {
        SDL_FillRect(dest, &line, borderc);
    } else if (fill && (bordersize < Min(rect.w, rect.h)/2)) {
        SDL_FillRect(dest, &line, borderc);
        line = rect;
        line.x += bordersize;
        line.y += bordersize;
        line.w -= bordersize*2;
        line.h -= bordersize*2;
        SDL_FillRect(dest, &line, fillc);
    } else {
        static Uint32 bdsize;
        bdsize = Min(rect.w, rect.h);
        bdsize = Min(bdsize, bordersize);
        line.h = bdsize;
        SDL_FillRect(dest, &line, borderc);
        line.w = bdsize;
        line.h = rect.h;
        SDL_FillRect(dest, &line, borderc);
        line.x = rect.x + rect.w - bdsize;
        line.y = rect.y;
        line.w = bdsize;
        line.h = rect.h;
        SDL_FillRect(dest, &line, borderc);
        line.x = rect.x;
        line.y = rect.y + rect.h - bdsize;
        line.w = rect.w;
        line.h = bdsize;
        SDL_FillRect(dest, &line, borderc);
    }
}

void GUI::DrawBorder(Surface* dest,
                     const Rect& utile,
                     const BorderStyle& style,
                     const RGBColor& col) {
    RGBColor HL, ML, LL, DK; // Highlight / Midlight / Lowlight / Darkness
    RGBColor Top1, Top2, Left1, Left2, Right1, Right2, Bottom1, Bottom2;
    int HLF = 100, MLF = 80, LLF = 50, DKF = 20; // Factors
    int L, R, T, B; // Rect's angles

    L = utile.x;
    R = L + utile.w-1;
    T = utile.y;
    B = T + utile.h-1;

    HL = RGB(col.r * HLF / 100, col.g * HLF / 100, col.b * HLF / 100);
    ML = RGB(col.r * MLF / 100, col.g * MLF / 100, col.b * MLF / 100);
    LL = RGB(col.r * LLF / 100, col.g * LLF / 100, col.b * LLF / 100);
    DK = RGB(col.r * DKF / 100, col.g * DKF / 100, col.b * DKF / 100);

    switch (style) {
        case risen:
            Left1 = Top1 = ML;
            Left2 = Top2 = HL;
            Right1 = Bottom1 = DK;
            Right2 = Bottom2 = LL;
            break;
        case sunken:
            Left1 = Top1 = DK;
            Left2 = Top2 = LL;
            Right1 = Bottom1 = ML;
            Right2 = Bottom2 = HL;
            break;
        case inset:
            Left1 = Top1 = LL;
            Left2 = Top2 = HL;
            Right1 = Bottom1 = HL;
            Right2 = Bottom2 = LL;
            break;
        case outset:
            Left1 = Top1 = HL;
            Left2 = Top2 = LL;
            Right1 = Bottom1 = LL;
            Right2 = Bottom2 = HL;
            break;
    }
    DrawLine(L, T, L, B, Left1, dest);
    DrawLine(L, T, R, T, Top1, dest);
    DrawLine(L+1, T+1, L+1, B-1, Left2, dest);
    DrawLine(L+1, T+1, R-1, T+1, Top2, dest);
    DrawLine(R-1, T+1, R-1, B-1, Right2, dest);
    DrawLine(L+1, B-1, R-1, B-1, Bottom2, dest);
    DrawLine(R, T, R, B, Right1, dest);
    DrawLine(L, B, R, B, Bottom1, dest);
}


void GUI::Fill(PointXY point,
               RGBColor NewColor,
               Surface* dest) {

    Uint32 offset;
    Rect rect = {0, 0, dest->w, dest->h};

    if (!CoordToPos(point, rect, &offset)) return;

    RGBColor OldColor = GetRGB(point, dest);

    Uint32 oldcol = Map(OldColor, dest);
    Uint32 newcol = Map(NewColor, dest);

    if (oldcol == newcol) return;

    std::queue<Uint32> tofill;
    tofill.push(offset);

    Uint32 xcut = dest->w;
    Uint32 pLimit = dest->w * dest->h - 1;

    if (Lock(dest)!=0) return;

    std::vector<bool> flag(pLimit, false);

    Uint32* pixone = static_cast<Uint32*>(dest->pixels);

    Uint32 newoff, newpix;

    while (tofill.size() > 0) {
        offset = tofill.front();

        if (offset >= xcut) {
            newoff = offset - xcut;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (oldcol == newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if (offset < (pLimit - xcut)) {
            newoff = offset + xcut;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (oldcol == newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if ((offset%xcut)>0) {
            newoff = offset - 1;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (oldcol == newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if ((offset%xcut)<(xcut-1)) {
            newoff = offset + 1;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (oldcol == newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        *(pixone + offset) = newcol;
        tofill.pop();
    }
    Unlock(dest);
}


void GUI::FillToEdge(PointXY point,
                     RGBColor fillcolor,
                     RGBColor edgecolor,
                     Surface* dest) {

    Uint32 offset;
    Rect rect = {0, 0, dest->w, dest->h};

    if (!CoordToPos(point, rect, &offset)) return;

    Uint32 edge = Map(edgecolor, dest);
    Uint32 fill = Map(fillcolor, dest);

    std::queue<Uint32> tofill;
    tofill.push(offset);

    Uint32 xcut = dest->w;
    Uint32 pLimit = dest->w * dest->h - 1;

    if (Lock(dest)!=0) return;

    std::vector<bool> flag(pLimit, false);

    Uint32* pixone = static_cast<Uint32*>(dest->pixels);

    Uint32 newoff, newpix;

    while (tofill.size() > 0) {
        offset = tofill.front();

        if (offset >= xcut) {
            newoff = offset - xcut;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (edge != newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if (offset < (pLimit - xcut)) {
            newoff = offset + xcut;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (edge != newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if ((offset%xcut)>0) {
            newoff = offset - 1;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (edge != newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        if ((offset%xcut)<xcut) {
            newoff = offset + 1;
            if (!flag[newoff]) {
                newpix = *(pixone + newoff);
                if (edge != newpix) {
                    tofill.push(newoff);
                    flag[newoff]=true;
                }
            }
        }

        *(pixone + offset) = fill;

        tofill.pop();
    }
    Unlock(dest);
}


void GUI::DrawCrossHair(const GUI::PointXY& point,
                        const RGBColor& color,
                        Surface* dest) {
    Rect rect;
    rect.x = point.x - 2;
    rect.y = point.y;
    rect.w = 5;
    rect.h = 1;
    Uint32 c = GUI::Map(color, dest);
    SDL_FillRect(dest, &rect, c);
    rect.x = point.x;
    rect.y = point.y - 2;
    rect.w = 1;
    rect.h = 5;
    SDL_FillRect(dest, &rect, c);
}

void GUI::DrawArrow(const GUI::PointXY& p,
                    const RGBColor& color,
                    Surface* dest) {
    Rect rects[10] =
        {
            /* 0 */ { p.x+0, p.y+0, 1, 1 },
            /* 1 */ { p.x+0, p.y+1, 2, 1 },
            /* 2 */ { p.x+0, p.y+2, 3, 1 },
            /* 3 */ { p.x+0, p.y+3, 4, 1 },
            /* 4 */ { p.x+0, p.y+4, 5, 1 },
            /* 5 */ { p.x+0, p.y+5, 6, 1 },
            /* 6 */ { p.x+3, p.y+6, 1, 1 },
            /* 7 */ { p.x+3, p.y+7, 1, 1 },
            /* 8 */ { p.x+3, p.y+8, 1, 1 },
            /* 9 */ { p.x+3, p.y+9, 1, 1 }
        } ;
    Uint32 c = GUI::Map(color, dest);
    for (int i = 0; i < 10; SDL_FillRect(dest, &rects[i++], c));
}

void GUI::DrawCrossBox(const GUI::PointXY& p,
                       const RGBColor& color,
                       Surface* dest) {
    int s = 4;
    Rect rects[4] =
        {
            /* 0 */ { p.x-s, p.y-s, s*2, 1 },
            /* 1 */ { p.x-s, p.y+s, s*2, 1 },
            /* 2 */ { p.x-s, p.y-s, 1, s*2 },
            /* 3 */ { p.x+s, p.y-s, 1, s*2+1 },
        } ;
    Uint32 c = GUI::Map(color, dest);
    for (int i = 0; i < 4; SDL_FillRect(dest, &rects[i++], c));
}
