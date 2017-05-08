#ifndef TRIANGLECENTERS_IMPL_H
#define TRIANGLECENTERS_H

#include <iostream>
#include <queue>
#include <numeric>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "gui.h"

using namespace GUI;

typedef PrecXY Point;

inline bool operator<(const Point& p1, const Point& p2) {
    return (p1.x < p2.x) && (p1.y < p2.y);
}

inline Point abs(const Point& p) {
    return Point(fabs(p.x), fabs(p.y));
}

inline bool Nearby(Point p1, Point p2, double range = 1) {
    return abs(p2 - p1) < Point(range, range);
}

inline std::ostream& operator<<(std::ostream& s, PrecXY p) {
    std::ios::fmtflags oldflags = s.flags();
    s << std::setprecision(3) << std::showpoint;
    s << std::internal << std::fixed << std::setfill('0');
    s << "[" << std::setw(7) << p.x << ", " <<  std::setw(7) << p.y << "]";
    s.flags(oldflags);
    return s;
}

struct Triangle {
    Point p1, p2, p3;
    bool isclockwise;
    bool isvalid;
    Triangle() {};
    Triangle(const Point& point1,
             const Point& point2,
             const Point& point3)
            : p1(point1), p2(point2), p3(point3) {}
    void draw() const {
        Screen* scr = GetScreen();
        scr->drawline(p1, p2);
        scr->drawline(p2, p3);
        scr->drawline(p3, p1);
    }
    bool clockwise() {
        return (isclockwise = Clockwise(p1, p2, p3));
    }
    bool valid(int limit = 1) {
        return isvalid = (Distance(p1, p2, p3)>limit) &&
                         (Distance(p2, p1, p3)>limit) &&
                         (Distance(p3, p1, p2)>limit);
    }
    void operator+=(Point p) {
        p1 += p;
        p2 += p;
        p3 += p;
    }
    void operator-=(Point p) {
        p1 -= p;
        p2 -= p;
        p3 -= p;
    }
    void operator*=(Point p) {
        p1 *= p;
        p2 *= p;
        p3 *= p;
    }
    void operator/=(Point p) {
        p1 /= p;
        p2 /= p;
        p3 /= p;
    }
};

inline bool operator==(const Triangle& t1, const Triangle& t2) {
    return (t1.p1 == t2.p1) && (t1.p2 == t2.p2) && (t1.p3 == t2.p3);
}

Point operator/(Point p, double divisor);
void DrawCircle(Point center, double radius);
void DrawVertexToPoint(const Triangle& triangle, const Point& point);
void DrawVertexToVertex(const Triangle& t1, const Triangle& t2);

Point Centroid(const Triangle& triangle, Triangle* computed);
Point Incenter(const Triangle& triangle, Triangle* computed);
Point Orthocenter(const Triangle& triangle, Triangle* computed);
Point Circumcenter(const Triangle& triangle, Triangle* computed);
Point FermatsPoint(const Triangle& triangle, Triangle* computed);

void DisplayCentroid(const Triangle& triangle);
void DisplayMedians(const Triangle& triangle);
void DisplayIncenter(const Triangle& triangle);
void DisplayAnglebisectors(const Triangle& triangle);
void DisplayCircumcenter(const Triangle& triangle);
void DisplayIncircle(const Triangle& triangle);
void DisplayNormalbisectors(const Triangle& triangle);
void DisplayCircumcircle(const Triangle& triangle);
void DisplayOrthocenter(const Triangle& triangle);
void DisplayAltitudes(const Triangle& triangle);
void DisplayFermatspoint(const Triangle& triangle);
void DisplayFermatsequilaterals(const Triangle& triangle);
void DisplayFermatslines(const Triangle& triangle);
void DisplayNinepointscenter(const Triangle& triangle);
void DisplayNinepointscircle(const Triangle& triangle);
void DisplayNinepoints(const Triangle& triangle) ;
void DisplayEulersline(const Triangle& triangle);

inline void Negate(bool* value) {
    *value = !(*value);
}

enum EntityType { et_center, et_circle, et_line, et_point };

struct Entity {
    std::string name;
    std::string accel;
    SDLKey key;
    bool active;
    EntityType type;
    ColorEnum color;
    void (*display)(const Triangle& triangle);
};

struct Entities: public std::vector<Entity> {
    bool override;
    Entity* getbyname(std::string name) {
        for(int i = 0; i < size(); ++i) {
            if (operator[](i).name == name) return &(operator[](i));
        }
        return 0;
    }
};

struct FakeCoord {
    Point shift;
    Point fake(Point p) {
        p -= shift;
        p.y = -p.y;
        return p;
    }
};


void SwitchKey(SDLKey key, Entities* ents);
void SetDefaultActive(Entities* ents);

void ButtonClick(BaseObj* obj, SDL_MouseButtonEvent);

void ColorSwitchClick(BaseObj* obj, SDL_MouseButtonEvent);
void SaveAndExitProgram();
void LoadCustom();
void ExitProgram(BaseObj* obj, SDL_MouseButtonEvent);

void PrintInfo();
void PrintInformation(BaseObj* obj, SDL_MouseButtonEvent);

void OnlySelected(BaseObj* obj, SDL_MouseButtonEvent);
void InvertSelection(BaseObj* obj, SDL_MouseButtonEvent);
void SelectNone(BaseObj* obj, SDL_MouseButtonEvent);
void OnlyCenters(BaseObj* obj, SDL_MouseButtonEvent);
void OnlyCircles(BaseObj* obj, SDL_MouseButtonEvent);
void OnlyLines(BaseObj* obj, SDL_MouseButtonEvent);
void SelectAll(BaseObj* obj, SDL_MouseButtonEvent);

FakeCoord* get_fake();
Entities* get_entities();
bool* get_finished();

#endif // TRIANGLECENTERS_IMPL_H

