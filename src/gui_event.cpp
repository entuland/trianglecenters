#include "gui_event.h"
#include <map>

const Sint16 GUI::Action::joyaxis_tolerance = 5000;
const Sint16 GUI::Action::min_mouse_motion = 3;

bool GUI::Action::operator==( const SDL_Event& compare ) const
{
    if ( event.type == compare.type ) {
        switch ( event.type ) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if ( event.key.keysym.sym == compare.key.keysym.sym ) {
                    return true;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if ( event.button.button == compare.button.button ) {
                    return true;
                }
                break;
            case SDL_MOUSEMOTION:
                if (
                    ( ( event.motion.xrel > min_mouse_motion ) &&
                      ( event.motion.xrel < compare.motion.xrel ) )
                    ||
                    ( ( event.motion.xrel < -min_mouse_motion ) &&
                      ( event.motion.xrel > compare.motion.xrel ) )
                    ||
                    ( ( event.motion.yrel > min_mouse_motion ) &&
                      ( event.motion.yrel < compare.motion.yrel ) )
                    ||
                    ( ( event.motion.yrel < -min_mouse_motion ) &&
                      ( event.motion.yrel > compare.motion.yrel ) )
                    ||
                    ( Inside( Sint16(-min_mouse_motion),
                              event.motion.xrel,
                              min_mouse_motion ) &&
                      Inside( Sint16(-min_mouse_motion),
                              compare.motion.xrel,
                              min_mouse_motion ) )
                    ||
                    ( Inside( Sint16(-min_mouse_motion),
                              event.motion.yrel,
                              min_mouse_motion ) &&
                      Inside( Sint16(-min_mouse_motion),
                              compare.motion.yrel,
                              min_mouse_motion ) )
                ) {
                    return true;
                }
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                if ( event.jbutton.button == compare.jbutton.button ) {
                    return true;
                }
                break;
            case SDL_JOYAXISMOTION:
                if ( event.jaxis.axis == compare.jaxis.axis ) {
                    long lbound = event.jaxis.value - joyaxis_tolerance;
                    long curval = compare.jaxis.value;
                    long ubound = event.jaxis.value + joyaxis_tolerance;
                    if ( Inside( lbound, curval, ubound ) ) {
                        return true;
                    }
                }
                break;
            default:
                return false;
        }
    }
    else {
        return false;
    }
    return false;
}
bool GUI::Action::operator==( const Action& compare ) const
{
    return operator==( compare.event );
}

bool GUI::Actions::execute( const Action& action ) const
{
    for ( int act = 0; act < size(); ++act ) {
        if ( this->at( act ) == action ) {
            this->at( act ).execute();
            return true;
        }
    }
    return false;
}

void GUI::Actions::poll()
{
    static SDL_Joystick * joy = 0;
    if ( !joy && ( SDL_NumJoysticks() ) ) {
        joy = SDL_JoystickOpen( 0 );
    }
    static Action action;
    while ( SDL_PollEvent( &action.event ) ) {
        push_back( action );
    }
}

std::string GUI::GetEventName( Uint8 event_type )
{
    static std::map<Uint8, std::string> eventmap;
    if ( eventmap.size() == 0 ) {
        eventmap[ SDL_ACTIVEEVENT ] = "SDL_ACTIVEEVENT";
        eventmap[ SDL_KEYDOWN ] = "SDL_KEYDOWN";
        eventmap[ SDL_KEYUP ] = "SDL_KEYUP";
        eventmap[ SDL_MOUSEMOTION ] = "SDL_MOUSEMOTION";
        eventmap[ SDL_MOUSEBUTTONDOWN ] = "SDL_MOUSEBUTTONDOWN";
        eventmap[ SDL_MOUSEBUTTONUP ] = "SDL_MOUSEBUTTONUP";
        eventmap[ SDL_JOYAXISMOTION ] = "SDL_JOYAXISMOTION";
        eventmap[ SDL_JOYBALLMOTION ] = "SDL_JOYBALLMOTION";
        eventmap[ SDL_JOYHATMOTION ] = "SDL_JOYHATMOTION";
        eventmap[ SDL_JOYBUTTONDOWN ] = "SDL_JOYBUTTONDOWN";
        eventmap[ SDL_JOYBUTTONUP ] = "SDL_JOYBUTTONUP";
        eventmap[ SDL_QUIT ] = "SDL_QUIT";
        eventmap[ SDL_SYSWMEVENT ] = "SDL_SYSWMEVENT";
        eventmap[ SDL_VIDEORESIZE ] = "SDL_VIDEORESIZE";
        eventmap[ SDL_VIDEOEXPOSE ] = "SDL_VIDEOEXPOSE";
        eventmap[ SDL_USEREVENT ] = "SDL_USEREVENT";
    }
    return eventmap[ event_type ];
}

std::string GUI::DescribeKey( SDLKey key )
{
    static std::map<SDLKey, std::string> keymap;
    if ( keymap.size() == 0 ) {
        keymap[ SDLK_BACKSPACE ] = "backspace";
        keymap[ SDLK_TAB ] = "tab";
        keymap[ SDLK_CLEAR ] = "clear";
        keymap[ SDLK_RETURN ] = "return";
        keymap[ SDLK_PAUSE ] = "pause";
        keymap[ SDLK_ESCAPE ] = "escape";
        keymap[ SDLK_SPACE ] = "space";
        keymap[ SDLK_EXCLAIM ] = "exclaim";
        keymap[ SDLK_QUOTEDBL ] = "double quotes";
        keymap[ SDLK_HASH ] = "hash";
        keymap[ SDLK_DOLLAR ] = "dollar";
        keymap[ SDLK_AMPERSAND ] = "ampersand";
        keymap[ SDLK_QUOTE ] = "quote";
        keymap[ SDLK_LEFTPAREN ] = "left parenthesis";
        keymap[ SDLK_RIGHTPAREN ] = "right parenthesis";
        keymap[ SDLK_ASTERISK ] = "asterisk";
        keymap[ SDLK_PLUS ] = "plus sign";
        keymap[ SDLK_COMMA ] = "comma";
        keymap[ SDLK_MINUS ] = "minus sign";
        keymap[ SDLK_PERIOD ] = "period";
        keymap[ SDLK_SLASH ] = "forward slash";
        keymap[ SDLK_0 ] = "0";
        keymap[ SDLK_1 ] = "1";
        keymap[ SDLK_2 ] = "2";
        keymap[ SDLK_3 ] = "3";
        keymap[ SDLK_4 ] = "4";
        keymap[ SDLK_5 ] = "5";
        keymap[ SDLK_6 ] = "6";
        keymap[ SDLK_7 ] = "7";
        keymap[ SDLK_8 ] = "8";
        keymap[ SDLK_9 ] = "9";
        keymap[ SDLK_COLON ] = "colon";
        keymap[ SDLK_SEMICOLON ] = "semicolon";
        keymap[ SDLK_LESS ] = "less-than sign";
        keymap[ SDLK_EQUALS ] = "equals sign";
        keymap[ SDLK_GREATER ] = "greater-than sign";
        keymap[ SDLK_QUESTION ] = "question mark";
        keymap[ SDLK_AT ] = "at";
        keymap[ SDLK_LEFTBRACKET ] = "left bracket";
        keymap[ SDLK_BACKSLASH ] = "backslash";
        keymap[ SDLK_RIGHTBRACKET ] = "right bracket";
        keymap[ SDLK_CARET ] = "caret";
        keymap[ SDLK_UNDERSCORE ] = "underscore";
        keymap[ SDLK_BACKQUOTE ] = "back quote";
        keymap[ SDLK_a ] = "a";
        keymap[ SDLK_b ] = "b";
        keymap[ SDLK_c ] = "c";
        keymap[ SDLK_d ] = "d";
        keymap[ SDLK_e ] = "e";
        keymap[ SDLK_f ] = "f";
        keymap[ SDLK_g ] = "g";
        keymap[ SDLK_h ] = "h";
        keymap[ SDLK_i ] = "i";
        keymap[ SDLK_j ] = "j";
        keymap[ SDLK_k ] = "k";
        keymap[ SDLK_l ] = "l";
        keymap[ SDLK_m ] = "m";
        keymap[ SDLK_n ] = "n";
        keymap[ SDLK_o ] = "o";
        keymap[ SDLK_p ] = "p";
        keymap[ SDLK_q ] = "q";
        keymap[ SDLK_r ] = "r";
        keymap[ SDLK_s ] = "s";
        keymap[ SDLK_t ] = "t";
        keymap[ SDLK_u ] = "u";
        keymap[ SDLK_v ] = "v";
        keymap[ SDLK_w ] = "w";
        keymap[ SDLK_x ] = "x";
        keymap[ SDLK_y ] = "y";
        keymap[ SDLK_z ] = "z";
        keymap[ SDLK_DELETE ] = "delete";
        keymap[ SDLK_KP0 ] = "keypad 0";
        keymap[ SDLK_KP1 ] = "keypad 1";
        keymap[ SDLK_KP2 ] = "keypad 2";
        keymap[ SDLK_KP3 ] = "keypad 3";
        keymap[ SDLK_KP4 ] = "keypad 4";
        keymap[ SDLK_KP5 ] = "keypad 5";
        keymap[ SDLK_KP6 ] = "keypad 6";
        keymap[ SDLK_KP7 ] = "keypad 7";
        keymap[ SDLK_KP8 ] = "keypad 8";
        keymap[ SDLK_KP9 ] = "keypad 9";
        keymap[ SDLK_KP_PERIOD ] = "keypad period";
        keymap[ SDLK_KP_DIVIDE ] = "keypad divide";
        keymap[ SDLK_KP_MULTIPLY ] = "keypad multiply";
        keymap[ SDLK_KP_MINUS ] = "keypad minus";
        keymap[ SDLK_KP_PLUS ] = "keypad plus";
        keymap[ SDLK_KP_ENTER ] = "keypad enter";
        keymap[ SDLK_KP_EQUALS ] = "keypad equals";
        keymap[ SDLK_UP ] = "up arrow";
        keymap[ SDLK_DOWN ] = "down arrow";
        keymap[ SDLK_RIGHT ] = "right arrow";
        keymap[ SDLK_LEFT ] = "left arrow";
        keymap[ SDLK_INSERT ] = "insert";
        keymap[ SDLK_HOME ] = "home";
        keymap[ SDLK_END ] = "end";
        keymap[ SDLK_PAGEUP ] = "page up";
        keymap[ SDLK_PAGEDOWN ] = "page down";
        keymap[ SDLK_F1 ] = "F1";
        keymap[ SDLK_F2 ] = "F2";
        keymap[ SDLK_F3 ] = "F3";
        keymap[ SDLK_F4 ] = "F4";
        keymap[ SDLK_F5 ] = "F5";
        keymap[ SDLK_F6 ] = "F6";
        keymap[ SDLK_F7 ] = "F7";
        keymap[ SDLK_F8 ] = "F8";
        keymap[ SDLK_F9 ] = "F9";
        keymap[ SDLK_F10 ] = "F10";
        keymap[ SDLK_F11 ] = "F11";
        keymap[ SDLK_F12 ] = "F12";
        keymap[ SDLK_F13 ] = "F13";
        keymap[ SDLK_F14 ] = "F14";
        keymap[ SDLK_F15 ] = "F15";
        keymap[ SDLK_NUMLOCK ] = "numlock";
        keymap[ SDLK_CAPSLOCK ] = "capslock";
        keymap[ SDLK_SCROLLOCK ] = "scrollock";
        keymap[ SDLK_RSHIFT ] = "right shift";
        keymap[ SDLK_LSHIFT ] = "left shift";
        keymap[ SDLK_RCTRL ] = "right ctrl";
        keymap[ SDLK_LCTRL ] = "left ctrl";
        keymap[ SDLK_RALT ] = "right alt";
        keymap[ SDLK_LALT ] = "left alt";
        keymap[ SDLK_RMETA ] = "right meta";
        keymap[ SDLK_LMETA ] = "left meta";
        keymap[ SDLK_LSUPER ] = "left windows key";
        keymap[ SDLK_RSUPER ] = "right windows key";
        keymap[ SDLK_MODE ] = "mode shift";
        keymap[ SDLK_HELP ] = "help";
        keymap[ SDLK_PRINT ] = "print-screen";
        keymap[ SDLK_SYSREQ ] = "SysRq";
        keymap[ SDLK_BREAK ] = "break";
        keymap[ SDLK_MENU ] = "menu";
        keymap[ SDLK_POWER ] = "power";
        keymap[ SDLK_EURO ] = "euro";
    }
    return keymap[ key ];
}

std::string GUI::DescribeEvent( const SDL_Event& event )
{
    if ( !SDL_EnableUNICODE( -1 ) ) SDL_EnableUNICODE( 1 );
    static char ch;
    std::stringstream result;
    result << GetEventName( event.type ) << "\n";
    switch ( event.type ) {
        case SDL_ACTIVEEVENT:
            result << "gain: " << int( event.active.gain ) << "\n";
            result << "state: " << int( event.active.state ) << "\n";
            break;
        case SDL_KEYDOWN:
            result << "scancode: " << int( event.key.keysym.scancode ) << "\n";
            result << "sym: " << int( event.key.keysym.sym ) << "\n";
            result << "key: " << DescribeKey( event.key.keysym.sym ) << "\n";
            result << "mod: " << int( event.key.keysym.mod ) << "\n";
            if ( ( event.key.keysym.unicode & 0xFF80 ) == 0 ) {
                result << "CHAR: " << ( ch = event.key.keysym.unicode & 0x7F ) << "\n";
            }
            else {
                result << "I11l Ch nr ";
                result << event.key.keysym.unicode << "\n";
            }
            break;
        case SDL_KEYUP:
            result << "scancode: " << int( event.key.keysym.scancode ) << "\n";
            result << "sym: " << int( event.key.keysym.sym ) << "\n";
            result << "key: " << DescribeKey( event.key.keysym.sym ) << "\n";
            result << "mod: " << int( event.key.keysym.mod ) << "\n";
            break;
        case SDL_MOUSEMOTION:
            result << "state: " << int( event.motion.state ) << "\n";
            result << "x: " << event.motion.x << "\n";
            result << "y: " << event.motion.y << "\n";
            result << "xrel: " << event.motion.xrel << "\n";
            result << "xrel: " << event.motion.yrel << "\n";
            break;
        case SDL_MOUSEBUTTONDOWN:
            result << "button: " << int( event.button.button ) << "\n";
            result << "state: " << int( event.button.state ) << "\n";
            result << "x: " << event.button.x << "\n";
            result << "y: " << event.button.y << "\n";
            break;
        case SDL_MOUSEBUTTONUP:
            result << "button: " << int( event.button.button ) << "\n";
            result << "state: " << int( event.button.state ) << "\n";
            result << "x: " << event.button.x << "\n";
            result << "y: " << event.button.y << "\n";
            break;
        case SDL_JOYAXISMOTION:
            result << "which: " << int( event.jaxis.which ) << "\n";
            result << "axis: " << int( event.jaxis.axis ) << "\n";
            result << "value: " << event.jaxis.value << "\n";
            break;
        case SDL_JOYBALLMOTION:
            result << "which: " << int( event.jball.which ) << "\n";
            result << "ball: " << int( event.jball.ball ) << "\n";
            result << "xrel: " << event.jball.xrel << "\n";
            result << "yrel: " << event.jball.yrel << "\n";
            break;
        case SDL_JOYHATMOTION:
            result << "which: " << int( event.jhat.which ) << "\n";
            result << "hat: " << int( event.jhat.hat ) << "\n";
            result << "value: " << int( event.jhat.value ) << "\n";
            break;
        case SDL_JOYBUTTONDOWN:
            result << "which: " << int( event.jbutton.which ) << "\n";
            result << "button: " << int( event.jbutton.button ) << "\n";
            result << "state: " << int( event.jbutton.state ) << "\n";
            break;
        case SDL_JOYBUTTONUP:
            result << "which: " << int( event.jbutton.which ) << "\n";
            result << "button: " << int( event.jbutton.button ) << "\n";
            result << "state: " << int( event.jbutton.state ) << "\n";
            break;
        case SDL_QUIT:
        case SDL_SYSWMEVENT:
        case SDL_VIDEORESIZE:
        case SDL_VIDEOEXPOSE:
        case SDL_USEREVENT:
        default:
            break;
    }
    return result.str();
}
