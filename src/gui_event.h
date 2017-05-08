#ifndef EVENT_H
#define EVENT_H

#include "gui_common.h"
#include "gui_screen.h"
#include <deque>

namespace GUI
{
    class Action
    {
        public:
            static const Sint16 joyaxis_tolerance;
            static const Sint16 min_mouse_motion;
            SDL_Event event;
            void ( *execute ) ();
            bool operator==( const SDL_Event& compare ) const;
            bool operator==( const Action& compare ) const;
    };

    class Actions: public std::deque<Action>
    {
        public:
            bool execute( const Action& action ) const;
            void poll();
    };

    std::string GetEventName( Uint8 event_type );
    std::string DescribeKey( SDLKey key );
    std::string DescribeEvent( const SDL_Event& event );

} // namespace GUI

#endif // EVENT_H
