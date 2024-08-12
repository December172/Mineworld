#ifndef FILE_EVENT_CONTROLLER_KEYBOARDCONTROLLER_HPP
#define FILE_EVENT_CONTROLLER_KEYBOARDCONTROLLER_HPP

#include <unordered_map>
#include <filesystem>

#include "Controller.hpp"

namespace Mineworld {
    class Key {
    public:
        Key() : pressed(false), released(true) {}
        auto update(bool isPressed) -> void;
        
        auto isPressed() const -> bool;
        auto isReleased() const -> bool;
    protected:
    private:
        bool pressed;
        bool released;
    };

    class KeyboardController : public Controller {
    public:
        void handle(const Event& event) override;
        
        /// @brief Loads a lua key map file.
        /// @param mapFile Content of the key map file.
        auto loadKeyMap(const std::string& mapFile) -> void;
        auto pressed(const std::string& keyId) -> bool;
        auto released(const std::string& keyId) -> bool;
    protected:
    private:
        std::unordered_map<std::string, Key> keyMap;
    };
}

#endif // FILE_EVENT_CONTROLLER_KEYBOARDCONTROLLER_HPP