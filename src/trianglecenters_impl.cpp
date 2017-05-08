#include "trianglecenters_impl.h"

using namespace GUI;

typedef PrecXY Point;

FakeCoord fake;
Entities entities;
bool finished;

FakeCoord* get_fake() { return &fake; }
Entities* get_entities() { return &entities; }
bool* get_finished() { return &finished; }

Point operator/(Point p, double divisor) {
    return Point(p.x/divisor, p.y/divisor);
}


void DrawCircle(Point center, double radius) {
    Screen& scr = *GetScreen();
    Point p1(radius, 0);
    Point p2(radius, 0);
    int sides = abs(int(radius))+10;
    if (sides > 100) sides = 100;
    double angle = 360.0 / sides;
    for (int step = 0; step < sides; ++step) {
        p1.y = Rad(angle * step);
        p2.y = Rad(angle * (step+1));
        scr.drawline(PolToCart(p1)+center, PolToCart(p2)+center, scr.forecolor());
    }
}

void DrawVertexToPoint(const Triangle& triangle, const Point& point) {
    Screen& scr = *GetScreen();
    scr.drawline(point, triangle.p1);
    scr.drawline(point, triangle.p2);
    scr.drawline(point, triangle.p3);
}

void DrawVertexToVertex(const Triangle& t1, const Triangle& t2) {
    Screen& scr = *GetScreen();
    scr.drawline(t1.p1, t2.p1);
    scr.drawline(t1.p2, t2.p2);
    scr.drawline(t1.p3, t2.p3);
}


Point Centroid(const Triangle& triangle, Triangle* computed) {
    static bool firstuse = true;
    static Triangle oldin, oldout;
    static Point oldpoint;
    if (firstuse) {
        firstuse = false;
    } else if (triangle == oldin) {
        *computed = oldout;
        return oldpoint;
    }

    const Point& p1 = triangle.p1;
    const Point& p2 = triangle.p2;
    const Point& p3 = triangle.p3;

    Point m1 = MiddlePoint(p3, p2);
    Point m2 = MiddlePoint(p1, p3);
    Point m3 = MiddlePoint(p1, p2);

    Point centroid;
    Intersect(p1, m1, p2, m2, &centroid);
    computed->p1 = m1;
    computed->p2 = m2;
    computed->p3 = m3;
    oldout = *computed;
    oldpoint = centroid;
    return centroid;
}


Point Incenter(const Triangle& triangle, Triangle* computed) {
    static bool firstuse = true;
    static Triangle oldin, oldout;
    static Point oldpoint;
    if (firstuse) {
        firstuse = false;
    } else if (triangle == oldin) {
        *computed = oldout;
        return oldpoint;
    }

    const Point& p1 = triangle.p1;
    const Point& p2 = triangle.p2;
    const Point& p3 = triangle.p3;

    Point bis, ab1, ab2, ab3, incenter;

    bis = CartToPol(p2-p1);
    bis.y += CartToPol(p3-p1).y;
    bis.y /= 2;
    bis = PolToCart(bis)+p1;
    Intersect(p1, bis, p2, p3, &ab1);

    bis = CartToPol(p1-p2);
    bis.y += CartToPol(p3-p2).y;
    bis.y /= 2;
    bis = PolToCart(bis)+p2;
    Intersect(p2, bis, p1, p3, &ab2);

    bis = CartToPol(p1-p3);
    bis.y += CartToPol(p2-p3).y;
    bis.y /= 2;
    bis = PolToCart(bis)+p3;
    Intersect(p3, bis, p1, p2, &ab3);

    Intersect(p1, ab1, p2, ab2, &incenter);
    computed->p1 = ab1;
    computed->p2 = ab2;
    computed->p3 = ab3;
    oldout = *computed;
    oldpoint = incenter;
    return incenter;
}


Point Orthocenter(const Triangle& triangle, Triangle* computed) {
    static bool firstuse = true;
    static Triangle oldin, oldout;
    static Point oldpoint;
    if (firstuse) {
        firstuse = false;
    } else if (triangle == oldin) {
        *computed = oldout;
        return oldpoint;
    }

    const Point& p1 = triangle.p1;
    const Point& p2 = triangle.p2;
    const Point& p3 = triangle.p3;

    Point ortho1, ortho2, ortho3, orthocenter;

    ParametricLine line1(p2, p3);
    ParametricLine normal1 = Normal(line1, p1);
    Intersect(line1, normal1, &ortho1);

    ParametricLine line2(p1, p3);
    ParametricLine normal2 = Normal(line2, p2);
    Intersect(line2, normal2, &ortho2);

    ParametricLine line3(p1, p2);
    ParametricLine normal3 = Normal(line3, p3);
    Intersect(line3, normal3, &ortho3);

    Intersect(normal1, normal2, &orthocenter);
    computed->p1 = ortho1;
    computed->p2 = ortho2;
    computed->p3 = ortho3;
    oldout = *computed;
    oldpoint = orthocenter;
    return orthocenter;
}

Point Circumcenter(const Triangle& triangle, Triangle* computed) {
    static bool firstuse = true;
    static Triangle oldin, oldout;
    static Point oldpoint;
    if (firstuse) {
        firstuse = false;
    } else if (triangle == oldin) {
        *computed = oldout;
        return oldpoint;
    }

    const Point& p1 = triangle.p1;
    const Point& p2 = triangle.p2;
    const Point& p3 = triangle.p3;

    Point m1 = MiddlePoint(p3, p2);
    Point m2 = MiddlePoint(p1, p3);
    Point m3 = MiddlePoint(p1, p2);

    ParametricLine line1(p2, p3);
    ParametricLine normal1 = Normal(line1, m1);

    ParametricLine line2(p1, p3);
    ParametricLine normal2 = Normal(line2, m2);

    ParametricLine line3(p1, p2);
    ParametricLine normal3 = Normal(line3, m3);

    Point circumcenter;

    Intersect(normal2, normal3, &circumcenter);
    computed->p1 = m1;
    computed->p2 = m2;
    computed->p3 = m3;
    oldout = *computed;
    oldpoint = circumcenter;
    return circumcenter;

}

Point FermatsPoint(const Triangle& triangle, Triangle* computed) {
    static bool firstuse = true;
    static Triangle oldin, oldout;
    static Point oldpoint;
    if (firstuse) {
        firstuse = false;
    } else if (triangle == oldin) {
        *computed = oldout;
        return oldpoint;
    }

    const Point& p1 = triangle.p1;
    const Point& p2 = triangle.p2;
    const Point& p3 = triangle.p3;

    Point f1, f2, f3;
    double rot = triangle.isclockwise ? Rad(60) : -Rad(60);

    f1 = CartToPol(p3-p2);
    f1.y += rot;
    f1 = PolToCart(f1)+p2;

    f2 = CartToPol(p1-p3);
    f2.y += rot;
    f2 = PolToCart(f2)+p3;

    f3 = CartToPol(p2-p1);
    f3.y += rot;
    f3 = PolToCart(f3)+p1;

    Point fermatspoint;
    Intersect(f1, p1, f2, p2, &fermatspoint);
    computed->p1 = f1;
    computed->p2 = f2;
    computed->p3 = f3;
    oldout = *computed;
    oldpoint = fermatspoint;
    return fermatspoint;
}



void DisplayCentroid(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle medians;
    Point centroid = Centroid(triangle, &medians);
    scr.crosshair(centroid);
}
void DisplayMedians(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle medians;
    Centroid(triangle, &medians);
    DrawVertexToVertex(triangle, medians);
}

void DisplayIncenter(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle angle_bisectors;
    Point incenter = Incenter(triangle, &angle_bisectors);
    scr.crosshair(incenter);
}

void DisplayAnglebisectors(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle angle_bisectors;
    Point incenter = Incenter(triangle, &angle_bisectors);
    DrawVertexToPoint(triangle, incenter);
}

void DisplayCircumcenter(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    scr.crosshair(circumcenter);
}

void DisplayIncircle(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle angle_bisectors;
    Point incenter = Incenter(triangle, &angle_bisectors);
    DrawCircle(incenter, Distance(incenter, triangle.p1, triangle.p2));
}

void DisplayNormalbisectors(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    DrawVertexToPoint(normal_bisectors, circumcenter);
}

void DisplayCircumcircle(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    DrawCircle(circumcenter, Distance(circumcenter, triangle.p1));
}

void DisplayOrthocenter(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle altitudes;
    Point orthocenter = Orthocenter(triangle, &altitudes);
    GetScreen()->crosshair(orthocenter);
}

void DisplayAltitudes(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle altitudes;
    Orthocenter(triangle, &altitudes);
    DrawVertexToVertex(triangle, altitudes);
}

void DisplayFermatspoint(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle equilaterals;
    Point fermatspoint = FermatsPoint(triangle, &equilaterals);
    scr.crosshair(fermatspoint);
}

void DisplayFermatsequilaterals(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle equilaterals;
    Point fermatspoint = FermatsPoint(triangle, &equilaterals);
    scr.drawline(equilaterals.p1, triangle.p2);
    scr.drawline(equilaterals.p1, triangle.p3);
    scr.drawline(equilaterals.p2, triangle.p1);
    scr.drawline(equilaterals.p2, triangle.p3);
    scr.drawline(equilaterals.p3, triangle.p1);
    scr.drawline(equilaterals.p3, triangle.p2);
}

void DisplayFermatslines(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle equilaterals;
    Point fermatspoint = FermatsPoint(triangle, &equilaterals);
    DrawVertexToVertex(triangle, equilaterals);
}

void DisplayNinepointscenter(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    Triangle altitudes;
    Point orthocenter = Orthocenter(triangle, &altitudes);
    Point ninecenter = MiddlePoint(circumcenter, orthocenter);
    scr.crosshair(ninecenter);
}

void DisplayNinepointscircle(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    Triangle altitudes;
    Point orthocenter = Orthocenter(triangle, &altitudes);
    Point ninecenter = MiddlePoint(circumcenter, orthocenter);
    DrawCircle(ninecenter, Distance(ninecenter, MiddlePoint(triangle.p1, triangle.p2)));
}

void DisplayNinepoints(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle altitudes;
    Point orthocenter = Orthocenter(triangle, &altitudes);
    Triangle medians;
    Centroid(triangle, &medians);
    scr.crossbox(altitudes.p1);
    scr.crossbox(altitudes.p2);
    scr.crossbox(altitudes.p3);
    scr.crossbox(medians.p1);
    scr.crossbox(medians.p2);
    scr.crossbox(medians.p3);
    scr.crossbox(MiddlePoint(orthocenter, triangle.p1));
    scr.crossbox(MiddlePoint(orthocenter, triangle.p2));
    scr.crossbox(MiddlePoint(orthocenter, triangle.p3));
}

void DisplayEulersline(const Triangle& triangle) {
    Screen& scr = *GetScreen();
    Triangle altitudes;
    Point orthocenter = Orthocenter(triangle, &altitudes);
    Triangle normal_bisectors;
    Point circumcenter = Circumcenter(triangle, &normal_bisectors);
    GetScreen()->drawline(orthocenter, circumcenter);
}

void ButtonClick(BaseObj* obj, SDL_MouseButtonEvent) {
    CommandButton* button = static_cast<CommandButton*>(obj);
    Screen& scr = *GetScreen();
    for(int i = 0; i < entities.size(); ++i) {
        if (button->tag == entities[i].name) {
            if (!entities.override) Negate(&entities[i].active);
            return;
        }
    }
}

void ColorSwitchClick(BaseObj* obj, SDL_MouseButtonEvent) {
    CommandButton* button = static_cast<CommandButton*>(obj);
    Screen& scr = *GetScreen();
    for(int i = 0; i < entities.size(); ++i) {
        if (button->tag == entities[i].name) {
            entities[i].color = scr.picker.pick(entities[i].color, entities[i].name).first;
            button->background = RGB(entities[i].color);
            button->border = button->background;
            return;
        }
    }
}

void SaveAndExitProgram() {
    Screen& scr = *GetScreen();
    try {
        std::ofstream custom;
        custom.open("trianglecenters.dat");
        custom << "Override\n";
        custom << entities.override << "\n";
        for (int i = 0; i < entities.size(); ++i) {
            custom << entities[i].name << "\n";
            custom << scr.picker.getname(entities[i].color) << "\n";
            custom << entities[i].active << "\n";
        }
        custom << ",";
    } catch (...) {
    }
    finished = true;
}

void LoadCustom() {
    Screen& scr = *GetScreen();
    Entities temp = entities;
    std::string pass;
    std::ifstream custom;
    char ch;
    try {
        custom.open("trianglecenters.dat");
        std::getline(custom, pass);
        custom >> temp.override >> ch;
        for (int i = 0; i < entities.size(); ++i) {
            std::getline(custom, pass);
            custom >> pass;
            temp[i].color = scr.picker.getenum(pass);
            custom >> temp[i].active >> ch >> ch;
        }
    } catch (...) {
        std::cerr << "Unable to read custom settings file, using defaults\n";
        return;
    }
    if (custom.eof()) {
        entities = temp;
    } else {
        std::cerr << "Unable to read custom settings file, using defaults\n";
    }
}

void ExitProgram(BaseObj* obj, SDL_MouseButtonEvent) {
    SaveAndExitProgram();
}

void PrintInformation(BaseObj* obj, SDL_MouseButtonEvent) {
    PrintInfo();
}

void OnlySelected(BaseObj* obj, SDL_MouseButtonEvent) {
    Negate(&entities.override);
}

void InvertSelection(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_x, &entities);
}

void SelectNone(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_0, &entities);
}

void OnlyCenters(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_1, &entities);
}

void OnlyCircles(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_2, &entities);
}

void OnlyLines(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_3, &entities);
}

void SelectAll(BaseObj* obj, SDL_MouseButtonEvent) {
    SwitchKey(SDLK_8, &entities);
}

void SwitchKey(SDLKey key, Entities* ent) {
    Entities& entities = *ent;
    switch (key) {
        case SDLK_SPACE:
            Negate(&entities.override);
            break;
        case SDLK_x:
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    Negate(&e.active);
                }
            }
            break;
        case SDLK_0: // Select none
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    e.active = false;
                }
            }
            break;
        case SDLK_1: // Only centers
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    e.active = (e.type == et_center);
                }
            }
            break;
        case SDLK_2: // Only circles
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    e.active = (e.type == et_circle);
                }
            }
            break;
        case SDLK_3: // Only lines
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    e.active = (e.type == et_line);
                }
            }
            break;
        case SDLK_8: // All
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    e.active = true;
                }
            }
            break;
        default:
            if(!entities.override) {
                for(int i = 0; i < entities.size(); ++i) {
                    Entity& e = entities[i];
                    if (e.key == key) {
                        Negate(&e.active);
                        return;
                    }
                }
            }
    }
}

void SetDefaultActive(Entities* ent) {
    ent->override = false;
    SwitchKey(SDLK_3, ent);
    SwitchKey(SDLK_x, ent);
}

void PrintInfo() {
    Screen& scr = *GetScreen();
    RGBColor normal = RGB(PaleTurquoise);
    RGBColor highlight = RGB(Gold);
    RGBColor warning = RGB(Red);
    scr.clear();
    scr.status.stream << "Thank you for using the program";
    scr.status.add(normal);
    scr.status.stream << "\"The Centers of a Triangle\"\n";
    scr.status.add(highlight);
    scr.status.stream << "This program has been written in C++, using the\n";
    scr.status.stream << "SDL (Simple DirectMedia Layer) Library, by";
    scr.status.add(normal);
    scr.status.stream << "Francesco S. Carta, entuland@gmail.com";
    scr.status.add(highlight);
    scr.status.stream << "Your comments, questions and suggestions are welcome\n";
    scr.status.add(normal);
    scr.status.stream << "This is a freeware program, feel free to use it\n";
    scr.status.stream << "and to distribute it at your will, you're welcome\n";
    scr.status.add(highlight);
    scr.status.stream << "Several informations needed to develop this\n";
    scr.status.stream << "program have been taken from Jim Loy's geometry\n";
    scr.status.stream << "pages, which can be found at www.jimloy.com\n";
    scr.status.add(normal);
    scr.status.stream << "Thank you Jim for all of your writings.\n";
    scr.status.add(highlight);
    scr.status.stream << "Program information:\n";
    scr.status.add(warning);
    scr.status.stream << "- Keyboard shortcuts are shown at the beginning\n";
    scr.status.stream << " of each button - e.g. \"SPACE - Draw only selected\"\n\n";
    scr.status.stream << "- Click the colored buttons on the left\n";
    scr.status.stream << " to change the color of each entity\n\n";
    scr.status.stream << "- Click and drag anywhere on the screen to move\n";
    scr.status.stream << " the triangle and all the related entities\n\n";
    scr.status.stream << "- Click and drag close to a triangle's corner\n";
    scr.status.stream << " to move that corner - it will be highlighted\n\n";
    scr.status.stream << "- Use the mouse wheel to zoom in and out\n\n";
    scr.status.stream << "The drawing will be messed up when zoomed in very much,\n";
    scr.status.stream << "also, this program could crash if zoomed in *too* much\n\n";
    scr.status.stream << "Finally, this program stores all customization values\n";
    scr.status.stream << "in a file called \"trianglecenters.dat\", in the program's\n";
    scr.status.stream << "folder - delete that file to restore default settings\n";
    scr.status.add(normal);
    scr.status.stream << "CLICK OR PRESS ANY KEY TO RETURN TO THE PROGRAM";
    scr.status.add(warning);
    scr.status.print();
    scr.flip();
    WaitForKeyClick();
}

