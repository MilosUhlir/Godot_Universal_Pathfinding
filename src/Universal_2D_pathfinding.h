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
	std::vector<std::pair<int, int>> Path;

	int MAX_PATH_LENGTH;	// maximum possible length of path before pathfinder termination
	// PathDiversion; // How far will the actual path be from the shortest path (in tiles?)
	TileMapLayer TileMap;
	enum SelectAlgorithm {
		AStar,
		DynamicPrograming,
		Dijkstra
	};
	std::vector<Vector2i> Waypoints;
	bool UseWaypoints;
	enum WaypointOrder {
		All_In_Order,
		All_Random,
		Single_random,
		Single_In_Order
	};
	Vector2i HardEnd;


protected:
	static void _bind_methods();


public:
	Universal_2D_Pathinding();
	~Universal_2D_Pathinding();

	//void _process(double delta) override;
	void _set(const property);
	
};

}

#endif