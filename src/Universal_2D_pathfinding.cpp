
#include "Universal_2D_pathfinding.h"
#include <godot_cpp/core/class_db.hpp>


using namespace godot;


void Universal_2D_Pathinder::_bind_methods() {
}

Universal_2D_Pathinder::Universal_2D_Pathinder() {
    // initialization of variables
    Path;
    MAX_PATH_LENGTH = 100000;
    // PathDiversion
    Map;
    Map_tileset = Map.get_tile_set().operator->();
    


}


Universal_2D_Pathinder::~Universal_2D_Pathinder() {
    // cleanup
}

// Main methods

    // Algorithms
        // A*
        std::vector<std::pair<int, int>> Universal_2D_Pathinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {

            // 

        }


        // Dynamic Programming


        // Dijkstra


        // other...



    // preprocessor
    std::vector<std::vector<Universal_2D_Pathinder::Node_Data>> Universal_2D_Pathinder::Preprocessor() {
        // Vector2i map_size = Vector2i(0,0), TileMapLayer Map
        Vector2i start_coords(0,0);
        int map_size_x = map_size[0];
        int map_size_y = map_size[1];
        for (int x = 0; x <= map_size_x; x++) {
            for (int y = 0; y <= map_size_y; y++) {
                Vector2i node_atlas_coords = Map.get_cell_atlas_coords(Vector2i(x,y));
                Preprocessed_Map[x][y].Node_coordinates = Vector2i(x,y);
                Preprocessed_Map[x][y].Node_cost;
                Preprocessed_Map[x][y].Node_Label = 0;
                Preprocessed_Map[x][y].Node_Neighbors;
            }
        }
    }




/*// getters and setters

    // MAX_PATH_LENGTH
    void Universal_2D_Pathinder::set_MAX_PATH_LENGTH(const int new_max_length) {
        MAX_PATH_LENGTH = new_max_length;
    }
    int Universal_2D_Pathinder::get_MAX_PATH_LENGTH() const {
        return MAX_PATH_LENGTH;
    }

    // path diversion


	// tilemap
	void Universal_2D_Pathinder::set_Map(const TileMapLayer new_Map) {
        // Map = new_Map;
    }
	TileMapLayer Universal_2D_Pathinder::get_Map() const {
        return Map;
    }

	// Waypoints
	void Universal_2D_Pathinder::set_Waypoints(const std::vector<Vector2i> new_waypoints) {
        Waypoints = new_waypoints;
    }
	std::vector<Vector2i> Universal_2D_Pathinder::get_Waypoints() const {
        return Waypoints;
    }

	// useWaypoints
	void Universal_2D_Pathinder::set_UseWaypoints(const bool new_UseWaypoints) {
        UseWaypoints = new_UseWaypoints;
    }
	bool Universal_2D_Pathinder::get_UseWaypoints() const {
        return UseWaypoints;
    }

	// HardEnd
	void Universal_2D_Pathinder::set_HardEnd(const Vector2i new_HardEnd) {
        HardEnd = new_HardEnd;
    }
	Vector2i Universal_2D_Pathinder::get_HardEnd() const {
        return HardEnd;
    }

	// pathfinder
	void Universal_2D_Pathinder::set_Pathfinder(const int new_pathfinder) {
        Pathfinder = new_pathfinder;
    }
	int Universal_2D_Pathinder::get_Pathfinder() const {
        return Pathfinder;
    }
    */
