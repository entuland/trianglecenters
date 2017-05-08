#ifndef CONTROLS_H
#define CONTROLS_H
#include "gui_common.h"
#include "gui_screen.h"
#include "gui_font.h"

namespace Controls {

using namespace GUI;

class BaseObj {
    private:
        BaseObj& operator=(BaseObj&) {}
        BaseObj(BaseObj&) {}
    protected:
        ~BaseObj() {}
        Screen* screen;
        Rect global_rect;
        Rect local_rect;
        Rect clip_rect;
        void init();
        BaseObj();
        bool isvisible;
        bool isenabled;
        bool islocked;
    public:
        Font* font;
        BaseObj* parent;
        Rect clip() { return clip_rect; }
        Rect local() { return local_rect; }
        Rect global() {
            if (parent) {
                global_rect = parent->global();
                global_rect.x += local_rect.x;
                global_rect.y += local_rect.y;
                global_rect.w = local_rect.w;
                global_rect.h = local_rect.h;
            } else {
                global_rect = local_rect;
            }
            return global_rect;
        };

        int gx()     { return global().x; }
        int gy()     { return global().y; }
        int gx2()     { return global().x + local_rect.w -1; }
        int gy2()     { return global().y + local_rect.h -1; }
        int x()     { return local_rect.x; }
        int y()     { return local_rect.y; }
        int x2()     { return local_rect.x + local_rect.w -1; }
        int y2()     { return local_rect.y + local_rect.h -1; }
        int w()     { return local_rect.w; }
        int h()     { return local_rect.h; }

        void x(int val) { local_rect.x = val; }
        void y(int val) { local_rect.y = val; }
        void w(int val) { local_rect.w = val; }
        void h(int val) { local_rect.h = val; }
        void move(int x, int y, int w, int h);


        std::string caption;
        std::string tag;
        RGBColor border;
        RGBColor background;
        RGBColor foreground;
        unsigned int bordersize;

        virtual bool visible() { return isvisible; }
        virtual bool enabled() { return isenabled; }
        virtual bool locked() { return islocked; }

        virtual void visible(bool NewVal) { isvisible = NewVal; }
        virtual void enabled(bool NewVal) { isenabled = NewVal; }
        virtual void locked(bool NewVal) { islocked = NewVal; }

        virtual void repaint() {}

        void (*onclick)(BaseObj* Obj, SDL_MouseButtonEvent Button);

        void (*onmouseup)(BaseObj* Obj, SDL_MouseButtonEvent Button);
        void (*onmousedown)(BaseObj* Obj, SDL_MouseButtonEvent Button);
        void (*onmousemove)(BaseObj* Obj, SDL_MouseMotionEvent Motion);

        void (*ongotfocus)(BaseObj* Obj);
        void (*onlostfocus)(BaseObj* Obj);

        virtual void click(SDL_MouseButtonEvent Button);

        virtual void mouseup(SDL_MouseButtonEvent Button) {}
        virtual void mousedown(SDL_MouseButtonEvent Button) {}
        virtual void mousemove(SDL_MouseMotionEvent Motion) {}

        virtual void gotfocus() {}
        virtual void lostfocus() {}

        friend class Container;
        friend class Frame;
        friend class TextBox;
        friend class CommandButton;
};

class Container: public BaseObj {
    protected:
        std::vector<BaseObj*> children;
    public:
        void mouseup(SDL_MouseButtonEvent Button);
        void mousedown(SDL_MouseButtonEvent Button);
        void mousemove(SDL_MouseMotionEvent Motion);
        void add(BaseObj*);
        const std::vector<BaseObj*>& getchildren() { return children; }
};


class Frame : public Container {
    private:
        Frame operator=(Frame) {}
        Frame(Frame&) {}
    public:
        bool autosizecontent;
        unsigned int cols;
        unsigned int rows;
        unsigned int margin;

        Frame() { autosizecontent = false; cols = 1; rows = 1; margin = 15; }
        Frame(Screen* ScreenRef) {
            screen = ScreenRef; autosizecontent = false; cols = 1; rows = 1; margin = 15;}

        Alignment align;

        void repaint();
};

class TextBox : public BaseObj {
    public:
        Alignment align;
        std::string text;
        void repaint();
        TextBox() {    background = RGB(White); border = RGB(Silver);}
        TextBox(std::string NewText) { text = NewText;
            background = RGB(White); border = RGB(Silver); }
};

class CommandButton : public BaseObj {
    public:
        Alignment align;
        void repaint();
        bool pressed;
        CommandButton() { pressed = false; align = center; }
        CommandButton(std::string NewText) { caption = NewText; pressed = false; align = center;}
        void mouseup(SDL_MouseButtonEvent Button);
        void mousemove(SDL_MouseMotionEvent Motion);
        void mousedown(SDL_MouseButtonEvent Button);
};

}

#endif
