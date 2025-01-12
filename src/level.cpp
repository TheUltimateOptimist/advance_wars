#include "level.hpp"
#include "building.hpp"
#include "unit.hpp"
#include <string>
#include <iostream>
#include <SDL.h>

namespace advanced_wars
{
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
        {{MovementType::SHIPENGINE, TerrainType::WATER}, 1}
    };

    Level::Level(const std::vector<std::vector<TerrainType>>& map, string name, int width, int height, std::vector<Tile> tiles, std::vector<Building> buildings, std::vector<Unit> units) : name(name), width(width), height(height), buildings(buildings), units(units), vertex_map(map.size(), std::vector<Vertex>(map[0].size())
    {
        initializeGraph(map);
    };

    Level::initializeGraph(const std::vector<std::vector<TerrainType>>& map) {
        // Füge alle Vertices hinzu
        for (int y = 0; y < map.size(); ++y)
        {
            for (int x = 0; x < map[0].size(); ++x)
            {
                Vertex v = add_vertex({x, y, map[y][x]}, g);
                vertex_map[y][x] = v;
            }
        }

        // Füge Kanten hinzu
        for (int y = 0; y < map.size(); ++y)
        {
            for (int x = 0; x < map[0].size(); ++x)
            {
                // Horizontale und vertikale Kanten
                if (x < map[0].size() - 1)
                { // Rechts
                    addEdge(vertex_map[y][x], vertex_map[y][x + 1]);
                }
                if (y < map.size() - 1)
                { // Unten
                    addEdge(vertex_map[y][x], vertex_map[y + 1][x]);
                }
            }
        }
    }

    Level::addEdge(Vertex v1, Vertex v2) {
        boost::add_edge(v1, v2, EdgeProperties{}, g);
    }

    /*Bin dafür den Graph aus der Level Klasse zu nehmen*/
    std::vector<Position> Level::getMovementRange(Position start, MovementType type, int movementPoints) {
        EdgeWeightCalculator weight_calc(type, g);

        std::vector<int> distances(num_vertices(g));
        std::vector<Vertex> predecessors(num_vertices(g));

        Vertex start_vertex = vertex_map[start.y][start.x];

        boost::dijkstra_shortest_paths(g, start_vertex,
                                       boost::weight_map(boost::make_function_property_map<Edge>(weight_calc))
                                           .distance_map(boost::make_iterator_property_map(
                                               distances.begin(),
                                               boost::get(boost::vertex_index, g)))
                                           .predecessor_map(boost::make_iterator_property_map(
                                               predecessors.begin(),
                                               boost::get(boost::vertex_index, g))));

        // Sammle erreichbare Positionen
        std::vector<Position> reachable;
        for (int y = 0; y < vertex_map.size(); ++y)
        {
            for (int x = 0; x < vertex_map[0].size(); ++x)
            {
                if (distances[boost::get(boost::vertex_index, g, vertex_map[y][x])] <= movementPoints)
                {
                    reachable.push_back({x, y});
                }
            }
        }
        return reachable;
    }

    Level::render(SDL_Renderer *renderer, std::vector<SDL_Event> &events)
    {
        // Iterate over all events
        while (!events.empty())
        {
            events.erase(events.begin());
        }

        // Set background color for renderer
        if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0))
        {
            std::cout << "Could not set render draw color: " << SDL_GetError() << std::endl;
        }
    }

    int EdgeWeightCalculator::operator()(const Edge& e) const {
        Vertex target_vertex = boost::target(e, graph);
        TerrainType destTerrain = graph[target_vertex].terrain;
        return MovementGraph::MOVEMENT_COSTS.at({currentType, destTerrain});
    }

}
