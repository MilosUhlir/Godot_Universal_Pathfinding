
#include "Universal_2D_Pathfinder.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/json.hpp>


using namespace godot;


void Universal_2D_Pathfinder::_bind_methods() {
    // add methods
    ClassDB::bind_method(D_METHOD("Pathfinder", "Start_points_array", "End_points_array", "debug"), &Universal_2D_Pathfinder::Pathfinder);

    // add propperties
    // ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_passed"),,);

    ClassDB::bind_method(D_METHOD("set_Start_position"), &Universal_2D_Pathfinder::set_Start_position);
    ClassDB::bind_method(D_METHOD("get_Start_position"), &Universal_2D_Pathfinder::get_Start_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "Start_position"), "set_Start_position", "get_Start_position");

    ClassDB::bind_method(D_METHOD("set_map_size"), &Universal_2D_Pathfinder::set_map_size);
    ClassDB::bind_method(D_METHOD("get_map_size"), &Universal_2D_Pathfinder::get_map_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "map_size"), "set_map_size", "get_map_size");

    ClassDB::bind_method(D_METHOD("set_Preprocessed_Map"), &Universal_2D_Pathfinder::set_Preprocessed_Map);
    ClassDB::bind_method(D_METHOD("get_Preprocessed_Map"), &Universal_2D_Pathfinder::get_Preprocessed_Map);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "Preprocessed_Map"),"set_Preprocessed_Map","get_Preprocessed_Map");

}

Universal_2D_Pathfinder::Universal_2D_Pathfinder() {
    // initialization of variables
    // Path;
    // MAX_PATH_LENGTH = 100000;
    // PathDiversion
    // Map_tileset = Map.get_tile_set().operator->();

    map_size = Vector2i(2,2);

    // Node_Data = Dictionary  {
    //     {"Node_coordinates", Vector2i()},				// map coordinates of current node
	// 	{"Node_parent", Vector2i()},
	// 	{"Node_Neighbors", Array{}},					// array of coordinates of neighboring nodes. Maximum of 8  for normal maps, +2 for custom neighbouring tiles (i.e. teleports/tunels etc.)
	// 	{"Node_cost", 0},								// cost of movement onto this node
	// 	{"Node_state", 0},								// custom user defined tile state (0 always obstacle, 1 and above are all custom i.e. concrete, mud, bog, water, etc.)
	// 	{"Node_Label", 0.0} 							// the total cost to reach this node from start point
	// };
    Node_Data["Node_coordinates"] = Vector2i(0,0);
    Node_Data["Node_parent"] = Vector2i(0,0);
    Node_Data["Node_neighbors"] = Array{};
    Node_Data["Node_cost"] = 0;
    Node_Data["Node_state"] = 0;
    Node_Data["Node_label"] = 0.0;

    Dictionary default_node = Node_Data;
    Array default_array;
    default_array.push_back(default_node);
    default_array.push_back(default_node);
    Preprocessed_Map.push_back(default_array);
    Preprocessed_Map.push_back(default_array);
    
}


Universal_2D_Pathfinder::~Universal_2D_Pathfinder() {
    // cleanup
    if (Engine::get_singleton()->is_editor_hint()) {

        Array test = Preprocessed_Map[0];
        Dictionary test1 = test[0];
        test1;

        // UtilityFunctions::print("test: \n", test);
        // UtilityFunctions::print("test1: \n", test1);

        UtilityFunctions::print("Clean up message");
        // run preprocessor on scene close in editor
        Preprocessor();
    }
}


// void Universal_2D_Pathfinder::_process(double delta) {
//     time_passed += delta;
//     Vector2 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)));
//     set_position(new_position);

// }


// Main methods

    // Pathfinder
    Array Universal_2D_Pathfinder::Pathfinder(Array Start_points_array, Array End_points_array, const bool debug) {
        Array Paths = {};
        Array Path = {};
        // Path.append(Vector2i(1,5));
        // Path.append(Vector2i(5,48));
        // Paths.append(Path);
        // Path.clear();
        // Path.append(Vector2i(10,25));
        // Path.append(Vector2i(45,26));
        // Paths.append(Path);

        switch (Algorithm) {
            case 0:
                algorithm = &Universal_2D_Pathfinder::AStar_Pathfinder;
                break;
            case 1:
                algorithm = &Universal_2D_Pathfinder::DP_Pathfinder;
                break;
            case 2:
                algorithm = &Universal_2D_Pathfinder::Disjkstra_Pathfinder;
                break;
        }

        // one to one path
        if (Start_points_array.size() == 1 & End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("one to one path") : nullptr;     // print message if debug is true
            Path = (this->*algorithm)(Start_points_array[0], End_points_array[0]);      // finds path
            Paths.append(Path);                                                         // appends path to the list of paths
        }

        // many to one paths
        else if (Start_points_array.size() > 1 & End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("many to one paths") : nullptr;
            for (int i = 0; i < Start_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[i], End_points_array[0]);
                Paths.append(Path);
            }
        }

        // one to many paths
        else if (Start_points_array.size() == 1 & End_points_array.size() > 1) {
            (debug == true) ? UtilityFunctions::print("one to many paths") : nullptr;
            for (int i = 0; i < End_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[0], End_points_array[i]);
                Paths.append(Path);
            }
        }

        // many to many paths (MUST HAVE the same number of starts and ends!!!)
        else if (Start_points_array.size() > 1 & End_points_array.size() > 1) {
            (debug == true) ? UtilityFunctions::print("many to many paths") : nullptr;

            // error handling of arrays are not the same size
            Array err = {};
            ERR_FAIL_COND_V_EDMSG(Start_points_array.size() != End_points_array.size(), err, "Start and End point arrays are not the same size");

            for (int i = 0; i < Start_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[i], End_points_array[i]);
                Paths.append(Path);
            }
        }
        return Paths;
    }


    // Algorithms
        // A*
        Array Universal_2D_Pathfinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {

            Array test = Preprocessed_Map[0];
            // UtilityFunctions::print("test: \n", test);

            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            Array temp;

            temp = Preprocessed_Map[start_x];
            OPEN_list.append(temp[start_y]);

            Astar_while:
            while (!OPEN_list.is_empty()) {
                // if OPEN list is empty there is no solution -> terminate search
                if (OPEN_list.is_empty()) {
                    break;
                };

                // pull node with the smallest f(i) value in OPEN, if multiple check if any is end node
            }

            Array placeholder;
            placeholder.append(Vector2i(0,0));
            return placeholder;
        }


        // Dynamic Programming
        Array Universal_2D_Pathfinder::DP_Pathfinder(Vector2i start_node, Vector2i end_node) {
            return Array{};
        }

        // Dijkstra
        Array Universal_2D_Pathfinder::Disjkstra_Pathfinder(Vector2i start_node, Vector2i end_node) {
            return Array{};
        }

        // other...



    // preprocessor
    Array Universal_2D_Pathfinder::Preprocessor() {
        // Vector2i map_size = Vector2i(0,0), TileMapLayer Map
        if (map_size == Vector2i(0,0)) {
            UtilityFunctions::print("Preprocessor finished");
            return Array{};
        }
        else {
            int map_size_x = map_size.x-1;
            int map_size_y = map_size.y-1;
            for (int x = 0; x <= map_size_x; x++) {
                UtilityFunctions::print("x: ", x);
                Array map_x = Preprocessed_Map[x];
                for (int y = 0; y <= map_size_y; y++) {
                    UtilityFunctions::print("y: ", y);
                    Vector2i node_atlas_coords = get_cell_atlas_coords(Vector2i(x,y));
                    UtilityFunctions::print(node_atlas_coords);
                    Dictionary map_node = map_x[y];
                    map_node["Node_coordinates"] = Vector2i(x,y);
                    map_node["Node_parent"] = Vector2i(x,y);
                    map_node["Node_neighbors"] = Array{};
                    map_node["Node_cost"] = 0;
                    map_node["Node_state"] = 0;
                    map_node["Node_label"] = 0.0;
                }
            }


            // JSON json_parser;
            // String map_parsed = json_parser.stringify(Preprocessed_Map);
            // UtilityFunctions::print(map_parsed);


            UtilityFunctions::print("Preprocessor finished");
            return Array{};
        }
    }



    // Helper methods
        // label calculation
        double Label_Calculator(Dictionary Node_Parent, Dictionary Node);
        // neighbor search

        // find minimal label
        Dictionary Universal_2D_Pathfinder::find_minimum_label(Array& open_list, Vector2i end_node) {








            /*
            // find first node with minimal f(i)
            auto min_fi_node = std::min_element(open_list.min(), open_list.end(),
                [](const Node_Data& a, const Node_Data& b) {
                    return a.Node_Label < b.Node_Label;
                });

            double min_fi = min_fi_node->Node_Label;
            std::vector<Node_Data> min_nodes;

            // find all nodes with minimal f(i)
            for (const auto& minima : OPEN_list) {
                if (minima.Node_Label == min_fi){
                    min_nodes.push_back(minima);
                }
            }

            // search for if any of the found nodes are end nodes
            Node_Data current_node = min_nodes[0];
            for (const auto& node : min_nodes) {            // loops through the list of found nodes with minimal label value
                if (node.Node_coordinates == end_node) {    // if any node is the end node it is selected
                    current_node = node;
                    break;
                }
                else {
                    current_node = min_nodes[0];
                }
            }
            */

            Dictionary placeholder;

            return placeholder;
        };





// getters and setters

    // preprocessed map
    void Universal_2D_Pathfinder::set_Preprocessed_Map() {};
    Array Universal_2D_Pathfinder::get_Preprocessed_Map() const {
        return Preprocessed_Map;
    }

    // Start_position
    void Universal_2D_Pathfinder::set_Start_position(const Vector2i new_start) {
        Start_position = new_start;
    }
    Vector2i Universal_2D_Pathfinder::get_Start_position() const {
        return Start_position;
    }

    // map size
    void Universal_2D_Pathfinder::set_map_size(const Vector2i new_mapsize) {
        map_size = new_mapsize;
    }
    Vector2i Universal_2D_Pathfinder::get_map_size() const {
        return map_size;
    }

/*
    // MAX_PATH_LENGTH
    void Universal_2D_Pathfinder::set_MAX_PATH_LENGTH(const int new_max_length) {
        MAX_PATH_LENGTH = new_max_length;
    }
    int Universal_2D_Pathfinder::get_MAX_PATH_LENGTH() const {
        return MAX_PATH_LENGTH;
    }

    // path diversion


	// Waypoints
	void Universal_2D_Pathfinder::set_Waypoints(const std::vector<Vector2i> new_waypoints) {
        Waypoints = new_waypoints;
    }
	std::vector<Vector2i> Universal_2D_Pathfinder::get_Waypoints() const {
        return Waypoints;
    }

	// useWaypoints
	void Universal_2D_Pathfinder::set_UseWaypoints(const bool new_UseWaypoints) {
        UseWaypoints = new_UseWaypoints;
    }
	bool Universal_2D_Pathfinder::get_UseWaypoints() const {
        return UseWaypoints;
    }

	// HardEnd
	void Universal_2D_Pathfinder::set_HardEnd(const Vector2i new_HardEnd) {
        HardEnd = new_HardEnd;
    }
	Vector2i Universal_2D_Pathfinder::get_HardEnd() const {
        return HardEnd;
    }
*/


