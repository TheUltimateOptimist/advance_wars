#include "window.hpp"
#include "engine.hpp"
#include "level.hpp"
#include <vector>

using namespace advanced_wars;

int main() {

    Window window("Advanced Wars", 800, 600);

    Level level("Osnabrück",20,20, std::vector<Tile>(), std::vector<Building>(),std::vector<Unit>());

    Engine engine(window, level);

    while(!engine.exited()) {
        engine.pump();
        engine.render();
    }

    return 0;
}
