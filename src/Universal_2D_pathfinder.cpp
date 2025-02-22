
#include "Universal_2D_Pathfinder.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>


using namespace godot;


void Universal_2D_Pathfinder::_bind_methods() {
    // add methods
    ClassDB::bind_method(D_METHOD("Pathfinder", "Start_points_array", "End_points_array", "debug"), &Universal_2D_Pathfinder::Pathfinder);

    // add propperties
    // ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_passed"),,);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "Start_position"), "set_Start_position", "get_Start_position");

}

Universal_2D_Pathfinder::Universal_2D_Pathfinder() {
    // initialization of variables
    // Path;
    // MAX_PATH_LENGTH = 100000;
    // PathDiversion
    // Map;
    // Map_tileset = Map.get_tile_set().operator->();



    time_passed = 0.0;

}


Universal_2D_Pathfinder::~Universal_2D_Pathfinder() {
    // cleanup
    if (Engine::get_singleton()->is_editor_hint()) {
        UtilityFunctions::print("Clean up message");
        // run preprocessor on scene close in editor
    }
}


// void Universal_2D_Pathfinder::_process(double delta) {
//     time_passed += delta;
//     Vector2 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)));
//     set_position(new_position);
    
// }


// Main methods

    // Pathfinder
    Array Universal_2D_Pathfinder::Pathfinder(Array Start_points_array, Array End_points_array, const bool& debug) {
        Array Paths;
        Array Path;
        Path.append(Vector2i(1,5));
        Path.append(Vector2i(5,48));
        Paths.append(Path);
        Path.clear();
        Path.append(Vector2i(10,25));
        Path.append(Vector2i(45,26));
        Paths.append(Path);

        // one to one path
        if (Start_points_array.size() == 1 & End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("one to one path") : nullptr;

        }

        // many to one paths
        else if (Start_points_array.size() > 1 & End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("many to one paths") : nullptr;

        }

        // one to many paths
        else if (Start_points_array.size() == 1 & End_points_array.size() > 1) {
            (debug == true) ? UtilityFunctions::print("one to many paths") : nullptr;
            
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

                
            }
            
            
            
            
            
        }
        

        
        
        

        return Paths;
    }

/*
    // Algorithms
        // A*
        std::vector<Vector2i> Universal_2D_Pathfinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {

            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            OPEN_list.push_back(Preprocessed_Map[start_x][start_y]);

            Astar_while:
            while (!OPEN_list.empty()) {
                // if OPEN list is empty there is no solution -> terminate search
                if (OPEN_list.empty()) {
                    break;
                };
                
                // pull node with the smallest f(i) value in OPEN, if multiple check if any is end node
                
                    
            
            }

            std::vector<Vector2i> placeholder = {Vector2i(0,0)};
            return placeholder;
        }


        // Dynamic Programming


        // Dijkstra


        // other...

*/

    // preprocessor
    Array Universal_2D_Pathfinder::Preprocessor() {
        // Vector2i map_size = Vector2i(0,0), TileMapLayer Map
        Vector2i start_coords(0,0);
        int map_size_x = map_size[0];
        int map_size_y = map_size[1];
        for (int x = 0; x <= map_size_x; x++) {
            for (int y = 0; y <= map_size_y; y++) {
                Vector2i node_atlas_coords = Map.get_cell_atlas_coords(Vector2i(x,y));
                Preprocessed_Map[x][y].Node_coordinates = {x, y};
                Preprocessed_Map[x][y].Node_cost;
                Preprocessed_Map[x][y].Node_Label = 0;
                Preprocessed_Map[x][y].Node_Neighbors;
            }
        }


        Array placeholder = {};
        return placeholder;
    }

/*

    // Helper methods
        // label calculation
        double Label_Calculator(struct Node_Data Node_Parent, struct Node_Data Node);
        // neighbor search

        // find minimal label
        Universal_2D_Pathfinder::Node_Data Universal_2D_Pathfinder::find_minimum_label(std::vector<Universal_2D_Pathfinder::Node_Data> open_list, std::pair<int, int> end_node) {
            
            // find first node with minimal f(i)
            auto min_fi_node = std::min_element(OPEN_list.begin(), OPEN_list.end(),
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

            return current_node;
        };


*/


// getters and setters

    // Start_position
    void Universal_2D_Pathfinder::set_Start_position(const Vector2i new_start) {
        Start_position = new_start;
    }
    Vector2i Universal_2D_Pathfinder::get_Start_position() {
        return Start_position;
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


	// tilemap
	void Universal_2D_Pathfinder::set_Map(const TileMapLayer new_Map) {
        Map = new_Map;
    }
	TileMapLayer Universal_2D_Pathfinder::get_Map() const {
        return Map;
    }

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

    
