#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "scene.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "building.hpp"
#include <string>
#include <vector>
#include <SDL.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace advanced_wars
{

    /**
     * @brief The main window of the game
     */

    struct VertexProperties
    {
        int x;
        int y;
        TerrainType terrain;
    }

    struct EdgeProperties
    {
        int weight;
    }

    using Graph = boost::adjacency_list<
        boost::vecS,        // OutEdgeList
        boost::vecS,        // VertexList
        boost::undirectedS, // Directed
        VertexProperties,   // Vertex properties
        EdgeProperties      // Edge properties
        >;

    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

    const std::map<std::pair<MovementType, TerrainType>, int> MOVEMENT_COSTS = {
        {{MovementType::INFANTRY, TerrainType::PLAIN}, 1},
        {{MovementType::INFANTRY, TerrainType::FOREST}, 2},
        {{MovementType::INFANTRY, TerrainType::MOUNTAIN}, 2},
        {{MovementType::INFANTRY, TerrainType::STREET}, 1},
        {{MovementType::INFANTRY, TerrainType::WATER}, 999},

        {{MovementType::TRACKED, TerrainType::PLAIN}, 1},
        {{MovementType::TRACKED, TerrainType::FOREST}, 2},
        {{MovementType::TRACKED, TerrainType::MOUNTAIN}, 999},
        {{MovementType::TRACKED, TerrainType::STREET}, 1},
        {{MovementType::TRACKED, TerrainType::WATER}, 999},

        {{MovementType::WHEELED, TerrainType::PLAIN}, 2},
        {{MovementType::WHEELED, TerrainType::FOREST}, 3},
        {{MovementType::WHEELED, TerrainType::MOUNTAIN}, 999},
        {{MovementType::WHEELED, TerrainType::STREET}, 1},
        {{MovementType::WHEELED, TerrainType::WATER}, 999},

        {{MovementType::AIR, TerrainType::PLAIN}, 1},
        {{MovementType::AIR, TerrainType::FOREST}, 1},
        {{MovementType::AIR, TerrainType::MOUNTAIN}, 1},
        {{MovementType::AIR, TerrainType::STREET}, 1},
        {{MovementType::AIR, TerrainType::WATER}, 999},

        {{MovementType::SHIPENGINE, TerrainType::PLAIN}, 999},
        {{MovementType::SHIPENGINE, TerrainType::FOREST}, 999},
        {{MovementType::SHIPENGINE, TerrainType::MOUNTAIN}, 999},
        {{MovementType::SHIPENGINE, TerrainType::STREET}, 999},
        {{MovementType::SHIPENGINE, TerrainType::WATER}, 1}};

    class Level : public Scene
    {
    public:
        Level(std::string name, int width, int height, std::vector<Tile> tiles, std::vector<Building> buildings, std::vector<Unit> units);

        void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events);

    private:
        std::string name;
        int width;
        int height;
        std::vector<Building> buildings;
        std::vector<Unit> units;
    };

}

#endif
