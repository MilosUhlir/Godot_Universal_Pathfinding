#ifndef UNIVERSAL_2D_PATHFINDER_H
#define UNIVERSAL_2D_PATHFINDER_H

// Godot includes
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/file_access.hpp>


// my includes
// #include <vector>
// #include <map>


namespace godot {

class Universal_2D_Pathfinder : public TileMapLayer {
	GDCLASS(Universal_2D_Pathfinder, TileMapLayer)

private:


protected:
	static void _bind_methods();
	

public:
	// Constructor / Destructor
	Universal_2D_Pathfinder();
	~Universal_2D_Pathfinder();


	// Variables

	// enum WaypointOrder {							// enum for selecting the order of use for waypoints
	// 	No_Waypoints,
	// 	All_In_Order,
	// 	All_Random,
	// 	Single_random,
	// 	Single_In_Order
	// };

	bool DP_prev_used;

	int max_iter;

	bool diagonal;
	void set_diagonal(const bool diag);
	bool get_diagonal() const;

	int MAX_PATH_LENGTH = 10000;							// maximum possible length of path before pathfinder termination
	
	// PathDiversion;								// How far will the actual path be from the shortest path (in tiles?)

	Array Waypoints;								// an array of user defined waypoints which will be used to generate the path
	
	bool UseWaypoints;								// whether to use waypoints or not
	
	Vector2i HardEnd;								// an override of the pathfinding target location (e.g. for tower defense where you only need paths to one set point)
	
	// String TileData;
	Dictionary tile_data;							// dictionary extracted from .cfg file containing all costs for each tile variant
	
	Array Path;										// array of tile coordinates
	Array Paths;									// array of all calculated paths
	Array SearchVector;								// array of neighbor tiles
	TileSet* Map_tileset;							// TileSet object
	Array square_search_array;						// search array for square(and isometrtic) tile grids
	Array hex_search_array_y_even;							// search array for hexagonal tile grid
	Array hex_search_array_y_odd;

	Array OPEN_list;
	Array CLOSED_list;

	ConfigFile* file_manager;
	
	struct Node_Data {
		Vector2i Node_coordinates = Vector2i(0,0);		// map coordinates of current node
		Vector2i Node_parent = Vector2i(0,0);			// parent node of the current one
		Vector<Vector2i> Node_neighbors;				// array of coordinates of neighboring nodes
		float Node_cost = 0;							// cost of movement onto this node - user defined from cfg file
		float Distance_to = 0;							// current distance from start to this point g(n)
		double Node_label = 1e10;						// the total cost to reach this node from start point f(n) = g(n) = h(n)
		bool Reachable = false;							// if node can be reached - user defined from cfg file
		Node_Data();
	};

	Dictionary storage;		// Node_Data counterpart for out of extension export
	Array storage_array;	// storage for out of extension export of Preprocessed_Map
	// void set_storage_array(Array sta);
	// Array get_storage_array();

	Vector<Vector<Node_Data>> Preprocessed_Map;



	// void _process(double delta) override;

	

	double time_passed;
	
	// accessible parameters
	Vector2i Start_position;
	void set_Start_position(const Vector2i new_start);
	Vector2i get_Start_position() const;

	Vector2i map_size = Vector2i(0,0);				// Size of the map in tiles (x,y)
	void set_map_size(const Vector2i new_mapsize);
	Vector2i get_map_size() const;
	


	Array (Universal_2D_Pathfinder::*algorithm)(Vector2i, Vector2i);
	enum Algorithm_Type {							// enum for selection of Pathfinding algorithm
		ASTAR = 0,										// Standard A* algorithm (mainly for realtime pathfinding)
		// Astar_Exponential,						// exponential heuristic with f = g + h*e^h	//potentially (fills Double numeric cappacity 1.7*10^308 with h>200)	have in-script logic to use this on small distances?
		DIJKSTRA = 1,									// standard Dijkstra algorithm (good for one start to many ends and preprocessing paths for later use)
		// Dynamic_Programing,							// DP only for many starts to one target
		DYNAMIC_PROG = 2,
	} Algorithm;
	
	void set_Algorithm(Universal_2D_Pathfinder::Algorithm_Type alg);
	Universal_2D_Pathfinder::Algorithm_Type get_Algorithm() const;

	enum Heuristic_Type {
		EUCLID = 0,// h = sqrt(x^2 + y^2)
		EUCLID_POW = 1 ,// h^weight
		EUCLID_WGHT = 2,// h*weight
		EUCLID_EXP = 3,// h * exp(h)
		MANHATAN = 4,// h = x + y
		CHEBYSHEV = 5, // h = max(x, y)
		OCTILE = 6, // h = (min(x, y) * sqrt(2)) + (max(x, y) - min(x, y))
	} Heuristic;
	void set_Heuristic(Universal_2D_Pathfinder::Heuristic_Type heur);
	Universal_2D_Pathfinder::Heuristic_Type get_Heuristic() const;

	int weight = 1;
	void set_weight(float w);
	int get_weight() const;

	// Methods
	
		// Main methods

			// init
			void init();

			// Pathifinder
			Array Pathfinder(Array Start_points_array, Array End_points_array, const bool debug = false);

			// Preprocessor
			void Preprocessor();


		// Algorithms
			// A*
			Array AStar_Pathfinder(Vector2i start_node, Vector2i end_node);
			
			// Dijkstra
			Array Disjkstra_Pathfinder(Vector2i start_node, Vector2i end_node);
			
			// Dynamic Programming
			Array DP_Pathfinder(Vector2i start_node, Vector2i end_node);

		// Helper methods
			// label calculation
			double Label_Calculator(Vector2i node, Vector2i end, bool is_tile_diagonal, bool overwrite);

			// Map initializer
			void map_initializer(int type);
			bool button;
			void set_button(bool but);
			bool get_button();

			// find minimal label
			Array find_minimum_label(Array& open_list);

		// file management
			// map savers
				// to godot
				Array save_to_code();
				// to .cfg file
				void save_to_file(String path_to_file, String file_name);

			// map loaders
				// from godot
				void load_from_code(Array data);
				// from .cfg file
				void load_from_file(String path_to_file);

			// load tileset cfg
			void load_tileset_cfg(String path_to_file);

		// Setters / Getters


			// MAX_PATH_LENGTH
			void set_MAX_PATH_LENGTH(const int new_max_length);
			int get_MAX_PATH_LENGTH() const;


			// OPEN_list
			void set_open_list(Array _open);
			Array get_open_list();

			// CLOSED_list
			void set_closed_list(Array _closed);
			Array get_closed_list();
};

}

VARIANT_ENUM_CAST(Universal_2D_Pathfinder::Algorithm_Type);
VARIANT_ENUM_CAST(Universal_2D_Pathfinder::Heuristic_Type);

#endif