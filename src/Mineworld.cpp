#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <clipp.h>

#include "Mineworld.h"
#include "event/event/KeyboardEvent.h"
#include "event/listener/KeyboardListener.h"
#include "exception/IOException.h"

namespace Mineworld {
    std::unique_ptr<Mineworld> Mineworld::theMineworld = nullptr;
    std::unique_ptr<Logger> Mineworld::logger = nullptr;

    const std::string Mineworld::NAME = "Mineworld";
    const std::string Mineworld::VERSION = "0.0.1";

    auto Mineworld::getInstance() -> Mineworld& {
        if (Mineworld::theMineworld == nullptr) {
            Mineworld::theMineworld.reset(new Mineworld());
        }
        return *(Mineworld::theMineworld);
    }

    auto Mineworld::getLogger() -> Logger& {
        if (Mineworld::logger == nullptr) {
            Mineworld::logger.reset(new Logger("Mineworld.log"));
        }
        return *(Mineworld::logger);
    }

    auto Mineworld::getResourceManager() -> ResourceManager& {
        return *resourceManager;
    }

    auto Mineworld::getWindow() -> Window& {
        return *window;
    }

    auto Mineworld::getRenderer() -> Renderer& {
        return *renderer;
    }

    auto Mineworld::init() -> void {
        running = true;
        try {
            SDL_Init(SDL_INIT_AUDIO
                    | SDL_INIT_EVENTS
                    | SDL_INIT_VIDEO);
            eventQueue.reset(new EventQueue());
            window.reset(new Window());
            window->setTitle(Mineworld::NAME + " " + Mineworld::VERSION);
            resourceManager.reset(new ResourceManager());
            resourceManager->loadResourcePack(std::filesystem::path("assets.zip"));
            renderer.reset(new Renderer(*window));
        } catch (const IOException& e) {
            Mineworld::getLogger().error(e.what());
            running = false;
            return;
        } catch (...) {
            running = false;
            return;
        }
        
        /// Install event listeners
        eventQueue->addListener("mineworld.event.keyboard", std::shared_ptr<EventListener>(new KeyboardListener("mineworld.event.listener.keyboard")));

        Mineworld::getLogger().info("Mineworld initialized.");
    }

    auto Mineworld::eventLoop() -> void{
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    
                    break;
                default:
                    break;
            }
        }
        while (!eventQueue->empty()) {
            auto event = eventQueue->popEvent();
            eventQueue->dispatchEvent(event);
        }
    }

    auto Mineworld::run() -> int {
        if (running) {
            /// To avoid running this method multiple times
            return 0;
        }
        init();

        auto start = std::chrono::steady_clock::now();
        auto lastFrame = std::chrono::steady_clock::now();
        auto msPerFrame = 1000 / 60;
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        /// Conditonal compilation for FPS counter
        /// Reserved for future use
        #if MINEWORLD_FPS_COUNTER
            auto fpsTimer = std::chrono::steady_clock::now();
            auto frames = 0;
        #endif

        while (running) {
            start = std::chrono::steady_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - lastFrame);
            
            if (elapsedTime.count() < msPerFrame) {
                std::this_thread::sleep_for(std::chrono::milliseconds(msPerFrame - elapsedTime.count()));
                start = std::chrono::steady_clock::now();
            }

            lastFrame = start;
            eventLoop();
            renderer->render();
        #if MINEWORLD_FPS_COUNTER
            frames++;

            if (std::chrono::steady_clock::now() - fpsTimer >= std::chrono::seconds(1)) {
                Mineworld::getLogger().debug("FPS: " + std::to_string(frames));
                frames = 0;
                fpsTimer = std::chrono::steady_clock::now();
            }
        #endif
        }

        Mineworld::getLogger().info("Mineworld exited.");
        return EXIT_SUCCESS;
    }

    Mineworld::~Mineworld() {
        SDL_Quit();
    }
}

auto main(int argc, char *argv[]) -> int {
    auto cli = (
        clipp::option("-v", "--version").doc("Prints the version of Mineworld"),
        clipp::option("-h", "--help").doc("Prints this help message")
    );
    if (!clipp::parse(argc, argv, cli)) {
        std::cout<<clipp::make_man_page(cli, argv[0])<<std::endl;
        return EXIT_FAILURE;
    }
    return Mineworld::Mineworld::getInstance().run();
}
