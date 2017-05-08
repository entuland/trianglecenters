#ifndef GUI_DRAWING_H
#define GUI_DRAWING_H

#include "gui_common.h"
#include <vector>
#include <map>
#include <set>

namespace GUI {

/**********************************************************************************/
// Primitive functions
/**********************************************************************************/

void DrawPoint(const PointXY& point,
               const RGBColor& color,
               Surface* dest);

void DrawLine(PointXY p1,
              PointXY p2,
              RGBColor color,
              Surface* dest);

inline void DrawLine(Uint16 x1, Uint16 y1,
                     Uint16 x2, Uint16 y2,
                     const RGBColor& color,
                     Surface* dest) {
    DrawLine(PointXY(x1, y1), PointXY(x2, y2), color, dest);
}

void ScanLine(PointXY p1,
              PointXY p2,
              std::map<Sint16, std::multiset<Sint16> >* scan,
              Surface* dest);

void ScanPoly(const std::vector<PointXY>& points,
              std::map<Sint16, std::multiset<Sint16> >* scan,
              Surface* dest);

void DrawScan(const std::map<Sint16, std::multiset<Sint16> >& scan,
              const RGBColor& color,
              Surface* dest);

void DrawRect(const Rect& rect,
              const RGBColor& bordercolor,
              const Uint32& bordersize,
              const RGBColor& fillcolor,
              const bool& fill,
              Surface* dest);

void DrawBorder(Surface* dest,
                const Rect& utile,
                const BorderStyle& style,
                const RGBColor& col);

void Fill(PointXY point,
          RGBColor newcolor,
          Surface* dest);

void FillToEdge(PointXY point,
                RGBColor fillcolor,
                RGBColor edgecolor,
                Surface* dest);

/**********************************************************************************/
// Various pointers
/**********************************************************************************/

void DrawCrossHair(const PointXY& point,
                   const RGBColor& color,
                   Surface* dest);

void DrawArrow(const PointXY& point,
               const RGBColor& color,
               Surface* dest);

void DrawCrossBox(const PointXY& point,
                  const RGBColor& color,
                  Surface* dest);

} // namespace GUI

#endif // GUI_DRAWING_H
