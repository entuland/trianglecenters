#include "trianglecenters.h"
#include "trianglecenters_impl.h"

using namespace GUI;

void TriangleCenters::Main() {
    Screen scr(800, 614, 32);

    FakeCoord& fake = *get_fake();
    Entities& entities = *get_entities();
    bool& finished = *get_finished();

    finished = false;

    scr.axes.showgrid = true;
    const int minleft = 214;
    scr.status.origin = PointXY(minleft + 14, 24);
    fake.shift(minleft, 614);
    scr.picker.table_width(400);
    scr.picker.table_height(400);

    SDL_WM_SetCaption("\"The Centers of a Triangle\" by entuland@gmail.com", 0);

    Entity ent;

    ent.type    = et_center;
    ent.name    = "Centroid";
    ent.accel   = "T";
    ent.key     = SDLK_t;
    ent.color   = Red;
    ent.display = &DisplayCentroid;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Medians";
    ent.accel   = "M";
    ent.key     = SDLK_m;
    ent.color   = Crimson;
    ent.display = &DisplayMedians;
    entities.push_back(ent);

    ent.type    = et_center;
    ent.name    = "Incenter";
    ent.accel   = "I";
    ent.key     = SDLK_i;
    ent.color   = Lime;
    ent.display = &DisplayIncenter;
    entities.push_back(ent);

    ent.type    = et_circle;
    ent.name    = "Incircle";
    ent.accel   = "J";
    ent.key     = SDLK_j;
    ent.color   = LimeGreen;
    ent.display = &DisplayIncircle;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Angle bisectors";
    ent.accel   = "B";
    ent.key     = SDLK_b;
    ent.color   = ForestGreen;
    ent.display = &DisplayAnglebisectors;
    entities.push_back(ent);

    ent.type    = et_center;
    ent.name    = "Circumcenter";
    ent.accel   = "C";
    ent.key     = SDLK_c;
    ent.color   = Aqua;
    ent.display = &DisplayCircumcenter;
    entities.push_back(ent);

    ent.type    = et_circle;
    ent.name    = "Circumcircle";
    ent.accel   = "D";
    ent.key     = SDLK_d;
    ent.color   = DarkTurquoise;
    ent.display = &DisplayCircumcircle;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Normal bisectors";
    ent.accel   = "N";
    ent.key     = SDLK_n;
    ent.color   = DodgerBlue;
    ent.display = &DisplayNormalbisectors;
    entities.push_back(ent);

    ent.type    = et_center;
    ent.name    = "Orthocenter";
    ent.accel   = "O";
    ent.key     = SDLK_o;
    ent.color   = Orange;
    ent.display = &DisplayOrthocenter;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Altitudes";
    ent.accel   = "A";
    ent.key     = SDLK_a;
    ent.color   = DarkOrange;
    ent.display = &DisplayAltitudes;
    entities.push_back(ent);

    ent.type    = et_center;
    ent.name    = "Fermat's point";
    ent.accel   = "F";
    ent.key     = SDLK_f;
    ent.color   = MediumOrchid;
    ent.display = &DisplayFermatspoint;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Fermat's equilaterals";
    ent.accel   = "G";
    ent.key     = SDLK_g;
    ent.color   = DarkOrchid;
    ent.display = &DisplayFermatsequilaterals;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Fermat's lines";
    ent.accel   = "H";
    ent.key     = SDLK_h;
    ent.color   = BlueViolet;
    ent.display = &DisplayFermatslines;
    entities.push_back(ent);

    ent.type    = et_center;
    ent.name    = "Nine points center";
    ent.accel   = "P";
    ent.key     = SDLK_p;
    ent.color   = Wheat;
    ent.display = &DisplayNinepointscenter;
    entities.push_back(ent);

    ent.type    = et_circle;
    ent.name    = "Nine points circle";
    ent.accel   = "Q";
    ent.key     = SDLK_q;
    ent.color   = BurlyWood;
    ent.display = &DisplayNinepointscircle;
    entities.push_back(ent);

    ent.type    = et_point;
    ent.name    = "Nine points";
    ent.accel   = "R";
    ent.key     = SDLK_r;
    ent.color   = Sienna;
    ent.display = &DisplayNinepoints;
    entities.push_back(ent);

    ent.type    = et_line;
    ent.name    = "Euler's line";
    ent.accel   = "E";
    ent.key     = SDLK_e;
    ent.color   = DarkKhaki;
    ent.display = &DisplayEulersline;
    entities.push_back(ent);

    Triangle triangle;
    Triangle computed;

    triangle.p1(250, 420);
    triangle.p2(765, 420);
    triangle.p3(290, 150);

    SetDefaultActive(&entities);

    RGBColor inactive = RGB(DimGray);

    LoadCustom();

    Frame buttons(&scr);
    Frame activeswitch;
    Frame colorswitch;
    Frame otherswitch;
    buttons.add(&activeswitch);
    buttons.add(&colorswitch);
    buttons.add(&otherswitch);
    buttons.background = RGB(Silver);
    activeswitch.background = RGB(Silver);
    colorswitch.background = RGB(Silver);
    otherswitch.background = RGB(Silver);
    CommandButton* button;
    for (size_t i = 0; i < entities.size(); ++i) {
        button = new CommandButton();
        button->align = bottomleft;
        button->caption = entities[i].accel + " - " + entities[i].name;
        switch(entities[i].type) {
            case et_center:
                button->caption += " +";
                break;
            case et_circle:
                button->caption += " O";
                break;
            case et_line:
                button->caption += " /";
                break;
            case et_point:
                button->caption += " ò";
                break;
        }
        button->tag = entities[i].name;
        button->onclick = &ButtonClick;
        button->background = RGB(Silver);
        activeswitch.add(button);

        button = new CommandButton();
        button->background = RGB(entities[i].color);
        button->border = RGB(entities[i].color);
        button->tag = entities[i].name;
        button->onclick = &ColorSwitchClick;
        colorswitch.add(button);
    }

    button = new CommandButton();
    button->align = bottomleft;
    button->caption = "ESC - Exit program";
    button->onclick = &ExitProgram;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "TAB - Program information";
    button->onclick = &PrintInformation;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "SPACE - Selected";
    button->onclick = &OnlySelected;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "X - Invert selection";
    button->onclick = &InvertSelection;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "0 - Select none";
    button->onclick = &SelectNone;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "1 - Only centers";
    button->onclick = &OnlyCenters;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "2 - Only circles";
    button->onclick = &OnlyCircles;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "3 - Only lines";
    button->onclick = &OnlyLines;
    button->background = RGB(Silver);
    otherswitch.add(button);

    button = new CommandButton();
    button->align = left;
    button->caption = "8 - Select all";
    button->onclick = &SelectAll;
    button->background = RGB(Silver);
    otherswitch.add(button);

    buttons.move(0, 0, minleft, scr.height());

    colorswitch.autosizecontent = true;
    colorswitch.rows = 17;
    colorswitch.cols = 1;
    colorswitch.margin = 2;
    colorswitch.move(0, 0, 28, 24*17);

    activeswitch.autosizecontent = true;
    activeswitch.rows = 17;
    activeswitch.cols = 1;
    activeswitch.margin = 2;
    activeswitch.move(26, 0, minleft - 26, 24*17);

    otherswitch.autosizecontent = true;
    otherswitch.rows = 9;
    otherswitch.cols = 1;
    otherswitch.margin = 2;
    otherswitch.move(0, 24*17 - 10, minleft, 24*9);

    Point p;

    bool moving = false;
    bool jumpit = false;

    const int snapdist = 10;
    Point* cursnap = 0;

    Point zoomp(1.2, 1.2);

    bool update = true;
    buttons.repaint();
    scr.clear();
    while (!finished) {
        Delay(50);
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_MOUSEBUTTONUP) {
                p(event.button.x, event.button.y);
                if (FallsIn(p,buttons.global()) || scr.mouselock) {
                    buttons.mouseup(event.button);
                    update = true;
                } else if (moving && event.button.button == 1) {
                    moving = false;
                    cursnap = 0;
                    update = true;
                } else if (event.button.button == 4) {
                    triangle -= p;
                    triangle /= zoomp;
                    triangle += p;
                    update = true;
                } else if (event.button.button == 5) {
                    triangle -= p;
                    triangle *= zoomp;
                    triangle += p;
                    update = true;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                p(event.button.x, event.button.y);
                if (FallsIn(p,buttons.global()) || scr.mouselock) {
                    buttons.mousedown(event.button);
                    update = true;
                } else if (cursnap && event.button.button == 1) {
                    moving = true;
                    *cursnap = p;
                    if (cursnap->x < minleft) cursnap->x = minleft;
                    update = true;
                } else {
                    if (FallsIn(p,buttons.global())) buttons.mousedown(event.button);
                    update = true;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                p(event.motion.x, event.motion.y);
                if (moving) {
                    *cursnap = p;
                    if (cursnap->x < minleft) cursnap->x = minleft;
                    update = true;
                } else if (scr.mouselock) {
                    cursnap = 0;
                    buttons.mousemove(event.motion);
                    update = true;
                } else if(Distance(p, triangle.p1) < snapdist) {
                    cursnap = &triangle.p1;
                    update = true;
                } else if (Distance(p, triangle.p2) < snapdist) {
                    cursnap = &triangle.p2;
                    update = true;
                } else if (Distance(p, triangle.p3) < snapdist) {
                    cursnap = &triangle.p3;
                    update = true;
                } else if (event.motion.state == SDL_BUTTON(1)) {
                    p(event.motion.xrel, event.motion.yrel);
                    triangle.p1 += p;
                    triangle.p2 += p;
                    triangle.p3 += p;
                    update = true;
                } else {
                    cursnap = 0;
                    update = true;
                }
            } else if (event.type == SDL_QUIT) {
                    SaveAndExitProgram();
            } else if (event.type == SDL_KEYUP) {
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    SaveAndExitProgram();
                } else if(event.key.keysym.sym == SDLK_TAB) {
                    PrintInfo();
                    update = true;
                } else {
                    SwitchKey(event.key.keysym.sym, &entities);
                    update = true;
                }
            }
            if (!jumpit && update && moving) {
                jumpit = true;
            }
            if (update && !moving && jumpit) {
                jumpit = false;
                update = false;
            }
            if (update) {

                scr.backcolor(RGB(Black));
                scr.forecolor(RGB(cursnap?Red:White));
                scr.clear();
                triangle.draw();

                triangle.valid(1);
                triangle.clockwise();

                if (!triangle.isvalid) {
                    scr.status.stream << "Triangle too narrow or too small\n";
                    scr.status.add(RGB(Red));
                }

                for(int i = 0; i < entities.size(); ++i) {
                    const std::vector<BaseObj*>& children = activeswitch.getchildren();
                    Entity& e = entities[i];
                    if (triangle.isvalid && (entities.override || e.active)) {
                        scr.forecolor(RGB(e.color));
                        e.display(triangle);
                        children[i]->background = RGB(Gray90);
                        children[i]->foreground = RGB(e.color);
                    } else {
                        children[i]->background = RGB(Silver);
                        children[i]->foreground = RGB(Black);
                    }
                }

                {
                    const std::vector<BaseObj*>& children = otherswitch.getchildren();
                    children[2]->caption = (entities.override ? "SPACE - Draw only selected" : "SPACE - Draw everything");
                    for (int i = 0; i < 6; ++i) {
                        children[i+3]->visible(!entities.override);
                    }
                }

                scr.forecolor(RGB(Yellow));
                if(cursnap) scr.crosshair(*cursnap);
                scr.forecolor(RGB(Red));
                if(cursnap) scr.crossbox(*cursnap);

                scr.status.print();
                buttons.repaint();
                scr.flip();
                update = false;
            }
        }
    }
}
