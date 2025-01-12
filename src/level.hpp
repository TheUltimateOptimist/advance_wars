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

    struct Position
    {
        int x, y;
        bool operator==(const Position &other) const
        {
            return x == other.x && y == other.y;
        }
    }

    using Graph = boost::adjacency_list<
        boost::vecS,        // OutEdgeList
        boost::vecS,        // VertexList
        boost::undirectedS, // Directed
        VertexProperties,   // Vertex properties
        EdgeProperties      // Edge properties
        >;

    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

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
