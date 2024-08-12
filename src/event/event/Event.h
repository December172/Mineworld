#ifndef FILE_EVENT_EVENT_H
#define FILE_EVENT_EVENT_H

#include <string>

namespace Mineworld {
    /// @brief Basic event class
    class Event {
    public:
        /// @brief Constructor
        /// @param id Event ID for identifying the event type
        Event(std::string typeId) noexcept;
        /// @brief  Get the event ID
        /// @return Event ID
        auto getTypeId() const noexcept -> const std::string&;
        virtual ~Event() = default;
    protected:
    private:
        const std::string typeId;
    };
}

#endif // FILE_EVENT_EVENT_H