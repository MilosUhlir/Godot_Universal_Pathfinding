#ifndef UNIVERSAL_PATHINDER_H
#define UNIVERSAL_PATHINDER_H

// Godot includes
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>

// my includes
#include <vector>


// #define MAX_PATH_LENGTH 10000


namespace godot {

class Universal_2D_Pathinding : public Node {
	GDCLASS(Universal_2D_Pathinding, Node)

private:
	// Variables
	std::vector<std::pair<int, int>> Path;			// array (vector) of tile coordinates
	int MAX_PATH_LENGTH;							// maximum possible length of path before pathfinder termination
	// PathDiversion;								// How far will the actual path be from the shortest path (in tiles?)
	TileMapLayer TileMap;							// TileMapLayer object to read Tilemap Data like tileset type and tile atlas
	std::vector<Vector2i> Waypoints;				// an array of user defined waypoints which will be used to generate the path
	bool UseWaypoints;								// whether to use waypoints or not
	Vector2i HardEnd;								// an override of the pathfinding target location (e.g. for tower defense where you only need paths to one set point)

	// Methods



protected:
	static void _bind_methods();


public:
	// Variables
	enum SelectAlgorithm {							// enum for selection of Pathfinding algorithm
		AStar,
		DynamicPrograming,
		Dijkstra
	};
	enum WaypointOrder {							// enum for selecting the order of use for waypoints
		All_In_Order,
		All_Random,
		Single_random,
		Single_In_Order
	};


	// Methods

		// Constructor / Destructor
		Universal_2D_Pathinding();
		~Universal_2D_Pathinding();
	
		// Setters / Getters
			// MAX_PATH_LENGTH
			void set_MAX_PATH_LENGTH(const int new_max_length);
			int get_MAX_PATH_LENGTH() const;
	
};

}

#endif