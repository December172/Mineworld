#ifndef FILE_CLIENT_RESOURCE_RESOURCE_H
#define FILE_CLIENT_RESOURCE_RESOURCE_H

#include <string>

namespace Mineworld {
    class Resource {
        public:
            Resource(std::string id);
            virtual ~Resource() = default;

            /// Access uniform ID of the resource
            /// @return The current value of uniformID
            auto getUniformID() const noexcept -> const std::string&;
        protected:
            const std::string uniformID;
            /** Unload resource */
            virtual void unload() = 0;
        private:
    };
}
#endif // FILE_CLIENT_RESOURCE_RESOURCE_H
