#ifndef FILE_EVENT_KEYBOARDEVENT_HPP
#define FILE_EVENT_KEYBOARDEVENT_HPP

#include <SDL2/SDL.h>

#include "Event.h"

namespace Mineworld {
    /// @brief Low-level keyboard event
    /// @note This class is not meant to be used directly, but rather through KeyboardController
    ///       which provides key mapping and dispatching functionality
    /// @see KeyboardController
    class KeyboardEvent : public Event {
    public:
    KeyboardEvent(SDL_Event* event);

    auto getKeyCode() const -> SDL_Keycode;
    auto getKeyState() const -> SDL_Keymod;
    protected:
    private:
        SDL_Event* m_event;
    };
}

#endif // FILE_EVENT_KEYBOARDEVENT_HPP