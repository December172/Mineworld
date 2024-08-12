#ifndef FILE_EVENT_CONTROLLER_CONTROLLER_HPP
#define FILE_EVENT_CONTROLLER_CONTROLLER_HPP

#include "event/event/Event.h"

namespace Mineworld {
    /// @brief  Low-level controller for Mineworld
    /// @details A controller only handles events, and does not have any knowledge of the game state.
    class Controller {
    public:
        Controller() = default;
        virtual void handle(const Event& event) = 0;
        virtual ~Controller() = default;
    protected:
    private:
    };
}

#endif // FILE_EVENT_CONTROLLER_CONTROLLER_HPP