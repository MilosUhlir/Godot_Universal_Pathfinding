#ifndef UNIVERSAL_PATHINDER_H
#define UNIVERSAL_PATHINDER_H

// Godot includes
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>

// my includes
#include <vector>
#include <map>


// #define MAX_PATH_LENGTH 10000


namespace godot {

class Universal_2D_Pathinder : public Node {
	GDCLASS(Universal_2D_Pathinder, Node)

private:



protected:
	static void _bind_methods();


	// internal variables
	std::vector<std::pair<int, int>> Path;			// array (vector) of tile coordinates
	std::vector<std::pair<int, int>> SearchVector;	// array of neighbor tiles
	TileSet* Map_tileset;							// TileSet object
	Vector2i square_search_array[8];				// search array for square(and isometrtic) tile grids
	Vector2i hex_search_array[6];					// search array for hexagonal tile grid

	struct Node_Data {								// Structure to store all needed node data in one spot
		Vector2i Node_coordinates;					// map coordinates of current node
		int Node_cost;								// cost of movement onto this node
		std::vector<Vector2i> Node_Neighbors; 		// array of coordinates of neighboring nodes
		float Node_Label;							// the total cost to reach this node from start point
		enum Node_state {
			
		};
	};
	
	std::vector<struct Node_Data> OPEN_list;
	std::vector<struct Node_Data> CLOSED_list;

	std::vector<std::vector<struct Node_Data>> Preprocessed_Map;
	
	

	// accessible parameters
	
	
	
	// Methods
		// Helper methods
			// label calculation
			float Label_Calculator(struct Node_parent, struct Node);
			// neighbor search

			// find minimal label
			Node_Data find_minimum_label();


public:
	// Variables

	Vector2i Start_position;

	enum Pathfinder {							// enum for selection of Pathfinding algorithm
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
	int MAX_PATH_LENGTH;							// maximum possible length of path before pathfinder termination
	// PathDiversion;								// How far will the actual path be from the shortest path (in tiles?)
	TileMapLayer Map;								// TileMapLayer object to read Map Data like tileset type and tile atlas
	std::vector<Vector2i> Waypoints;				// an array of user defined waypoints which will be used to generate the path
	bool UseWaypoints;								// whether to use waypoints or not
	Vector2i HardEnd;								// an override of the pathfinding target location (e.g. for tower defense where you only need paths to one set point)
	int Pathfinder;
	Vector2i map_size = Vector2i(0,0);								// Coordinates of furthest tile from origin (0,0)
	int map_size_x;									// X axis size of map
	int map_size_y;									// Y axis size of map
	Dictionary costs_dictionary;					// dictionary extracted from .JSON file containing all costs for each tile variant
	

	// Methods

		// Constructor / Destructor
		Universal_2D_Pathinder();
		~Universal_2D_Pathinder();
	
		// Main methods

			// Pathifinder
			std::vector<std::vector<Vector2i>> Pathfinder(std::vector<Vector2i>& Start_points_array, std::vector<Vector2i>& End_points_array, std::vector<Vector2i>& Waypoints_array);

			// Algorithms
				// A*
				std::vector<Vector2i> AStar_Pathfinder(Vector2i& start_node, Vector2i& end_node);
				
				// Dynamic Programming
				std::vector<Vector2i> DP_Pathfinder(Vector2i& start_node, Vector2i& end_node);
				
				// Dijkstra

				
				// other...
			

			// Preprocessor
			std::vector<std::vector<struct Node_Data>> Preprocessor();




		



		/*// Setters / Getters
			// MAX_PATH_LENGTH
			void set_MAX_PATH_LENGTH(const int new_max_length);
			int get_MAX_PATH_LENGTH() const;

			// path diversion


			// Map
			void set_Map(const TileMapLayer new_Map);
			TileMapLayer get_Map() const;

			// Waypoints
			void set_Waypoints(const std::vector<Vector2i> new_waypoints);
			std::vector<Vector2i> get_Waypoints() const;

			// useWaypoints
			void set_UseWaypoints(const bool new_UseWaypoints);
			bool get_UseWaypoints() const;

			// HardEnd
			void set_HardEnd(const Vector2i new_HardEnd);
			Vector2i get_HardEnd() const;

			// pathfinder
			void set_Pathfinder(const int new_pathfinder);
			int get_Pathfinder() const;
		*/
};

}

#endif