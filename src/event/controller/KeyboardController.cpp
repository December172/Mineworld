#include "KeyboardController.hpp"

namespace Mineworld {
    auto Key::isPressed() const -> bool {
        return pressed;
    }

    auto Key::isReleased() const -> bool {
        return released;
    }

    auto Key::update(bool isPressed) -> void {
        pressed = isPressed;
        released = !isPressed;
    }

    void KeyboardController::handle(const Event& event) {
        K
}