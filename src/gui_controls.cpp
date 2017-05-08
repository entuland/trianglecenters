#include "gui_controls.h"

using namespace Controls;

/* ---------------------------------------------------------------------------------
 * BaseObj Members
 */


void Controls::BaseObj::init() {
    visible(true);
    parent = 0;
    local_rect.x = 0;
    local_rect.y = 0;
    local_rect.w = 10;
    local_rect.h = 10;
    bordersize = 2;
    border = RGB(White);
    background = RGB(Gray);
    foreground = RGB(Black);
    onclick = 0;
    onmouseup = 0;
    onmousedown = 0;
    onmousemove = 0;
    ongotfocus = 0;
    onlostfocus = 0;
    font = GetFont();
}

Controls::BaseObj::BaseObj() {
    init();
}

void Controls::BaseObj::move(int x, int y, int w, int h) {
    local_rect.x = x;
    local_rect.y = y;
    local_rect.w = w;
    local_rect.h = h;
}

void Controls::BaseObj::click(SDL_MouseButtonEvent Button) {
    if (onclick) onclick(this, Button);
}

/* ---------------------------------------------------------------------------------
 * Container Members
 */

void Controls::Container::mousedown(SDL_MouseButtonEvent Button) {

    int cX, cY, X1, X2, Y1, Y2;
    SDL_Rect Off = global();
    cX = Button.x - Off.x;
    cY = Button.y - Off.y;

    if ((screen->mouselock) && (screen->mouselock != this)){
        screen->mouselock->mousedown(Button);
        return;
    }

    if (screen->mouselock == this) return;

    if (children.size() > 0) {
        for(int child = 0; child < children.size(); child++) {
            SDL_Rect cRe = children.at(child)->local();
            if (FallsIn(cX, cY, cRe, -bordersize)) {
                children.at(child)->mousedown(Button);
                return;
            } else if (FallsIn(cX, cY, cRe, 0)) {
                return;
            }
        }
    }
    screen->mouselock = this;

    if (screen->focused != this) {
        if (screen->focused) screen->focused->lostfocus();
        screen->focused = this;
        gotfocus();
    }

    if(onmousedown) onmousedown(this, Button);
}

void Controls::Container::mousemove(SDL_MouseMotionEvent Motion) {
    int cX, cY;
    SDL_Rect Off = global();
    cX = Motion.x - Off.x;
    cY = Motion.y - Off.y;

    if (screen->mouselock == this) {
        if (onmousemove) onmousemove(this, Motion);
        return;
    }

    if (screen->mouselock) {
        screen->mouselock->mousemove(Motion);
        return;
    }

    if (children.size() > 0) {
        for(int child = 0; child < children.size(); child++) {
            SDL_Rect cRe = children.at(child)->local();
            if (FallsIn(cX, cY, cRe, -bordersize)) {
                children.at(child)->mousemove(Motion);
                return;
            } else if (FallsIn(cX, cY, cRe, 0)) {
                return;
            }
        }
    }
    if (onmousemove) onmousemove(this, Motion);
}


void Controls::Container::mouseup(SDL_MouseButtonEvent Button) {
    int cX, cY;
    SDL_Rect Off = global();
    cX = Button.x - Off.x;
    cY = Button.y - Off.y;

    if (screen->mouselock == this) {
        screen->mouselock = 0;
        if(onmouseup) onmouseup(this, Button);
        if ((screen->focused = this) && FallsIn(cX, cY, local(), -bordersize)) {
            this->click(Button);
        }
        return;
    }

    if (screen->mouselock) {
        screen->mouselock->mouseup(Button);
        return;
    }

    if (children.size() > 0) {
        for(int child = 0; child < children.size(); child++) {
            SDL_Rect cRe = children.at(child)->local();
            if (FallsIn(cX, cY, cRe, -bordersize)) {
                children.at(child)->mouseup(Button);
                return;
            } else if (FallsIn(cX, cY, cRe, 0)) {
                return;
            }
        }
    }
    if (onmouseup) onmouseup(this, Button);
    if (screen->focused = this) this->click(Button);
    screen->mouselock = 0;
}

void Controls::Container::add(BaseObj * NewChild) {
    NewChild->parent = this;
    NewChild->screen = screen;
    NewChild->font = font;
    children.push_back(NewChild);
}

/* ---------------------------------------------------------------------------------
 * Frame Members
 */


void Controls::Frame::repaint() {
    if (!visible()) return;

    SDL_Rect inside, brush, clip;
    inside = global();
    brush = inside;

    if (inside.h == 0 || inside.w == 0) return;

    if (parent) {
        SDL_Rect must = parent->clip_rect;
        if (must.h == 0 || must.w == 0) return;
        clip = Intersect(inside, must);
    } else {
        clip = inside;
    }

    clip_rect = clip;

    if (clip.h == 0 || clip.w == 0) return;

    SDL_SetClipRect(screen->surface(), &clip);

    SDL_FillRect(screen->surface(), &brush, SDL_MapRGB(screen->surface()->format,
                 background.r, background.g, background.b));
    brush = inside;

    DrawBorder(screen->surface(), inside, inset, border);

    brush.x += bordersize;
    brush.y += bordersize;
    brush.w -= bordersize*2;
    brush.h -= bordersize*2;

    clip = Intersect(clip, brush);

    clip_rect = clip;

    if (clip.h == 0 || clip.w == 0) {
        SDL_SetClipRect(screen->surface(), 0);
        return;
    }

    SDL_SetClipRect(screen->surface(), &clip);

    int fx = inside.x + bordersize*2;
    int fy = inside.y + bordersize*2;


    RenderLine(caption, screen->surface(), fx, fy, *font, foreground, background, 1);

    SDL_SetClipRect(screen->surface(), 0);

    int CaptH = font->GetChar(std::string(" "), border, border, 1)->h;
    if (caption == "") CaptH = 0;
    int ix, iy, iw, ih;

    if (children.size() > 0) {
        for(int child = 0; child < children.size(); child++) {
            if (autosizecontent) {
                iw = (w()- bordersize*2 - margin) / cols - margin ;
                ih = (h() - CaptH - bordersize*2 - margin) / rows - margin;
                ix = child % cols * (iw + margin) + bordersize + margin;
                iy = child / cols * (ih + margin) + CaptH + (bordersize * 2) + margin;
                children.at(child)->move(ix, iy, iw, ih);
            }
            children.at(child)->repaint();
        }
    }
}

/* ---------------------------------------------------------------------------------
 * TextBox Members
 */

void Controls::TextBox::repaint() {
    if (!visible()) return;

    SDL_Rect inside, brush, clip;
    inside = global();
    brush = inside;

    if (inside.h == 0 || inside.w == 0) return;

    if (parent) {
        SDL_Rect must = parent->clip_rect;
        if (must.h == 0 || must.w == 0) return;
        clip = Intersect(inside, must);
    } else {
        clip = inside;
    }

    if (clip.h == 0 || clip.w == 0) return;

    SDL_SetClipRect(screen->surface(), &clip);

    SDL_FillRect(screen->surface(), &brush, SDL_MapRGB(screen->surface()->format,
                 background.r, background.g, background.b));

    DrawBorder(screen->surface(), inside, inset, border);

    brush = inside;

    brush.x += bordersize;
    brush.y += bordersize;
    brush.w -= bordersize*2;
    brush.h -= bordersize*2;

    clip = Intersect(clip, brush);

    clip_rect = clip;

    if (clip.h == 0 || clip.w == 0) {
        SDL_SetClipRect(screen->surface(), 0);
        return;
    }

    SDL_SetClipRect(screen->surface(), &clip);


    int fx = inside.x + bordersize*2;
    int fy = inside.y + bordersize*2;

    RenderLine(text, screen->surface(), fx, fy, *font, foreground, background, 1);

    SDL_SetClipRect(screen->surface(), 0);
}


/* ---------------------------------------------------------------------------------
 * CommandButton Members
 */


void Controls::CommandButton::repaint() {
    if (!visible()) return;

    SDL_Rect inside, brush, clip;
    inside = global();
    brush = inside;

    if (inside.h == 0 || inside.w == 0) return;

    if (parent) {
        SDL_Rect must = parent->clip_rect;
        if (must.h == 0 || must.w == 0) return;
        clip = Intersect(inside, must);
    } else {
        clip = inside;
    }

    if (clip.h == 0 || clip.w == 0) return;

    SDL_SetClipRect(screen->surface(), &clip);

    SDL_FillRect(screen->surface(), &brush, SDL_MapRGB(screen->surface()->format,
                 background.r, background.g, background.b));

    if (pressed) {
        DrawBorder(screen->surface(), inside, sunken, border);
    } else {
        DrawBorder(screen->surface(), inside, risen, border);
    }
    brush = inside;

    brush.x += bordersize;
    brush.y += bordersize;
    brush.w -= bordersize*2;
    brush.h -= bordersize*2;

    clip = Intersect(clip, brush);

    clip_rect = clip;

    if (clip.h == 0 || clip.w == 0) {
        SDL_SetClipRect(screen->surface(), 0);
        return;
    }

    SDL_SetClipRect(screen->surface(), &clip);

    int fx;
    int fy;
    int ll = LineLen(caption, *font, 1);
    int lh = font->GetChar(std::string("A"), foreground, background, 1)->h;
    switch (align) {
        case left:
        case topleft:
        case bottomleft:
            fx = inside.x + bordersize + 2;
            break;
        case right:
        case topright:
        case bottomright:
            fx = inside.x + inside.w - ll - bordersize - 2;
            break;
        case center:
        default:
            fx = inside.x + (inside.w - ll) / 2;
    }
    switch (align) {
        case topleft:
        case topcenter:
        case topright:
            fy = inside.y + bordersize + 2;
            break;
        case bottomleft:
        case bottomcenter:
        case bottomright:
            fy = inside.y + inside.h - lh - bordersize - 2;
            break;
        case center:
        default:
            fy = inside.y + (inside.h - lh) / 2;
    }
    fy += pressed ? 1 : 0;
    RenderLine(caption, screen->surface(), fx, fy, *font, foreground, background, 1);

    SDL_SetClipRect(screen->surface(), 0);
}

void Controls::CommandButton::mousedown(SDL_MouseButtonEvent Button) {
    if (screen->mouselock) {
        if (screen->mouselock!=this) {
            screen->mouselock->mousedown(Button);
            return;
        } else return;
    } else {
        screen->mouselock = this;
        if (Button.button == SDL_BUTTON_LEFT) {
            pressed = true;
            screen->focused = this;
            repaint();
        }
    }
    if(onmousedown) onmousedown(this, Button);
}

void Controls::CommandButton::mousemove(SDL_MouseMotionEvent Motion) {

    int cX, cY;
    global();
    cX = Motion.x;
    cY = Motion.y;

    if (screen->mouselock) {
        if (screen->mouselock == this) {
            if (FallsIn(cX, cY, global_rect, 0) && (SDL_GetMouseState(0,0) & SDL_BUTTON(1))) {
                if (!pressed) {
                    pressed = true;
                    repaint();
                }
            } else {
                if (pressed) {
                    pressed = false;
                    repaint();
                }
            }
            if (onmousemove) onmousemove(this, Motion);
            return;
        }
        screen->mouselock->mousemove(Motion);
        if (onmousemove) onmousemove(this, Motion);
    }
}


void Controls::CommandButton::mouseup(SDL_MouseButtonEvent Button) {
    int cX, cY;
    SDL_Rect Off = global();
    cX = Button.x;
    cY = Button.y;

    if (screen->mouselock == this) {
        if (SDL_GetMouseState(0,0)) return;
        if(onmouseup) onmouseup(this, Button);
        if(Button.button == SDL_BUTTON_LEFT) {
            screen->mouselock = 0;
            if ((screen->focused == this) && FallsIn(cX, cY, Off, -bordersize)) {
                this->click(Button);
            }
            pressed = false;
            repaint();
        }
        screen->mouselock=0;
    } else if (screen->mouselock) {
        screen->mouselock->mouseup(Button);
    }
}
