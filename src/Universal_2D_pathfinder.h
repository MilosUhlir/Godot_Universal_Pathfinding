#ifndef UNIVERSAL_2D_PATHFINDER_H
#define UNIVERSAL_2D_PATHFINDER_H

// Godot includes
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>


// my includes
#include <vector>
#include <map>


namespace godot {

class Universal_2D_Pathfinder : public TileMapLayer {
	GDCLASS(Universal_2D_Pathfinder, TileMapLayer)

private:

	/**/

	// accessible variables
	

	

	// enum WaypointOrder {							// enum for selecting the order of use for waypoints
	// 	No_Waypoints,
	// 	All_In_Order,
	// 	All_Random,
	// 	Single_random,
	// 	Single_In_Order
	// };

	int MAX_PATH_LENGTH;							// maximum possible length of path before pathfinder termination
	
	// PathDiversion;								// How far will the actual path be from the shortest path (in tiles?)
	
	// TileMapLayer Map;								// TileMapLayer object to read Map Data like tileset type and tile atlas
	
	std::vector<Vector2i> Waypoints;				// an array of user defined waypoints which will be used to generate the path
	
	bool UseWaypoints;								// whether to use waypoints or not
	
	Vector2i HardEnd;								// an override of the pathfinding target location (e.g. for tower defense where you only need paths to one set point)
	

	
	Dictionary costs_dictionary;					// dictionary extracted from .JSON file containing all costs for each tile variant
	
	// internal variables
	std::vector<std::pair<int, int>> Path;			// array (vector) of tile coordinates
	std::vector<std::pair<int, int>> SearchVector;	// array of neighbor tiles
	TileSet* Map_tileset;							// TileSet object
	Vector2i square_search_array[8];				// search array for square(and isometrtic) tile grids
	Vector2i hex_search_array[6];					// search array for hexagonal tile grid


	struct Node_Data {								// Structure to store all needed node data in one spot
		Vector2i Node_coordinates;					// map coordinates of current node
		int Node_cost;								// cost of movement onto this node
		Array Node_Neighbors;						// array of coordinates of neighboring nodes. Maximum of 8  for normal maps, +2 for custom neighbouring tiles (i.e. teleports/tunels etc.)
		// std::vector<std::pair<int, int>> Node_Neighbors;
		double Node_Label;							// the total cost to reach this node from start point
		int Node_state;								// custom user defined tile state (0 always obstacle, 1 and above are all custom i.e. concrete, mud, bog, water, etc.)
	
		// Convert Node_Data to Variant
		operator Variant() const {
			Dictionary dict;
			dict["Node_coordinates"] = Variant(Node_coordinates);
			dict["Node_cost"] = Node_cost;
			dict["Node_Neighbors"] = Node_Neighbors;
			dict["Node_Label"] = Node_Label;
			dict["Node_state"] = Node_state;
			return dict;
		}
		
		// Convert Variant to Node_Data
		static Node_Data from_variant(const Variant& variant) {
			Dictionary dict = variant;
			Node_Data data;
			data.Node_coordinates = dict["Node_coordinates"];
			data.Node_cost =  dict["Node_cost"];
			data.Node_Neighbors = dict["Node_Neighbors"];
			data.Node_Label = dict["Node_Label"];
			data.Node_state = dict["Node_state"];
			return data;
		}

	};

	Array Preprocessed_Map;

	Array OPEN_list;
	Array CLOSED_list;

	// Methods

	// Algorithms
		// A*
		Array AStar_Pathfinder(Vector2i start_node, Vector2i end_node);

		// Dynamic Programming
		Array DP_Pathfinder(Vector2i start_node, Vector2i end_node);
		
		// Dijkstra
		Array Disjkstra_Pathfinder(Vector2i start_node, Vector2i end_node);
		
		// other...

	// Helper methods
		// label calculation
		double Label_Calculator(struct Node_data node_parent, struct Node_data node);
		
		// neighbor search


		// find minimal label
		Node_Data find_minimum_label(Array& open_list, Vector2i end_node);



protected:
	static void _bind_methods();

	// accessible parameters

public:

	// Constructor / Destructor
	Universal_2D_Pathfinder();
	~Universal_2D_Pathfinder();

	void save_handler();
	
	// void _process(double delta) override;

	Array (Universal_2D_Pathfinder::*algorithm)(Vector2i, Vector2i);

	double time_passed;
	
	// accessible parameters
	Vector2i Start_position;
	void set_Start_position(const Vector2i new_start);
	Vector2i get_Start_position() const;

	Vector2i map_size = Vector2i(0,0);				// Coordinates of furthest tile from origin (0,0)
	void set_map_size(const Vector2i new_mapsize);
	Vector2i get_map_size() const;
	
	enum Heuristic {
		EUCLID ,// h = sqrt(x^2 + y^2)
		EUCLID_POW ,// h^weight
		EUCLID_WGHT ,// h*weight
		EUCLID_EXP ,//= exp(h)
		MANHATAN // h = x + y
	};
	Heuristic Heuristic;

	enum Algorithm_Type {							// enum for selection of Pathfinding algorithm
		AStar,										// Standard A* algorithm (mainly for realtime pathfinding)
		// Astar_Exponential,						// exponential heuristic with f = g + h*e^h	//potentially (fills Double numeric cappacity 1.7*10^308 with h>200)	have in-script logic to use this on small distances?
		Dynamic_Programing,							// DP only for many starts to one target
		Dijkstra									// standard Dijkstra algorithm (good for one start to many ends and preprocessing paths for later use)
	};
	Algorithm_Type Algorithm;

	// Methods
	
		// Main methods

			// Pathifinder
			Array Pathfinder(Array Start_points_array, Array End_points_array, const bool debug = false);


			

			// Preprocessor
			Array Preprocessor();


		// Setters / Getters

			// Preprocessed_Map
			void set_Preprocessed_Map();
			Array get_Preprocessed_Map() const;


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
		
};

}

#endif