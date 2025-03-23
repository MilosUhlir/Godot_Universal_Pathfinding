
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

    // Start_position
    ClassDB::bind_method(D_METHOD("set_Start_position"), &Universal_2D_Pathfinder::set_Start_position);
    ClassDB::bind_method(D_METHOD("get_Start_position"), &Universal_2D_Pathfinder::get_Start_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "Start_position"), "set_Start_position", "get_Start_position");

    // Map size
    ClassDB::bind_method(D_METHOD("set_map_size"), &Universal_2D_Pathfinder::set_map_size);
    ClassDB::bind_method(D_METHOD("get_map_size"), &Universal_2D_Pathfinder::get_map_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "map_size"), "set_map_size", "get_map_size");

    // Pathfinding algorithms
    ClassDB::bind_method(D_METHOD("set_Algorithm"), &Universal_2D_Pathfinder::set_Algorithm);
    ClassDB::bind_method(D_METHOD("get_Algorithm"), &Universal_2D_Pathfinder::get_Algorithm);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Algorithm", PROPERTY_HINT_ENUM, "A*,Dijkstra"), "set_Algorithm", "get_Algorithm");
    BIND_ENUM_CONSTANT(ASTAR);
    BIND_ENUM_CONSTANT(DIJKSTRA);

    // Heuristics
    ClassDB::bind_method(D_METHOD("set_Heuristic", "Heuristic"), &Universal_2D_Pathfinder::set_Heuristic);
    ClassDB::bind_method(D_METHOD("get_Heuristic"), &Universal_2D_Pathfinder::get_Heuristic);
    ADD_PROPERTY(
        PropertyInfo(Variant::INT, "Heuristic", PROPERTY_HINT_ENUM, "Euclid, Euclid (to power), Euclid (weighted), Euclid (exponential), Manhatan"),
        "set_Heuristic",
        "get_Heuristic"
    );
    BIND_ENUM_CONSTANT(EUCLID);
    BIND_ENUM_CONSTANT(EUCLID_POW);
    BIND_ENUM_CONSTANT(EUCLID_WGHT);
    BIND_ENUM_CONSTANT(EUCLID_EXP);
    BIND_ENUM_CONSTANT(MANHATAN);
    
    // map initializer
    ClassDB::bind_method(D_METHOD("map_initializer"), &Universal_2D_Pathfinder::map_initializer);
    ClassDB::bind_method(D_METHOD("set_button"), &Universal_2D_Pathfinder::set_button);
    ClassDB::bind_method(D_METHOD("get_button"), &Universal_2D_Pathfinder::get_button);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug button"), "set_button", "get_button");

    // storage array
    ClassDB::bind_method(D_METHOD("set_storage_array"), &Universal_2D_Pathfinder::set_storage_array);
    ClassDB::bind_method(D_METHOD("get_storage_array"), &Universal_2D_Pathfinder::get_storage_array);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "storage array"), "set_storage_array", "get_storage_array");

}

Universal_2D_Pathfinder::Universal_2D_Pathfinder() {
    // permanent part of constructor
    // Universal_2D_Pathfinder::map_initializer();


    // search arrays initialization
    square_search_array.append(Vector2i( 1, 0));
    square_search_array.append(Vector2i( 1, 1));
    square_search_array.append(Vector2i( 0, 1));
    square_search_array.append(Vector2i(-1, 1));
    square_search_array.append(Vector2i(-1, 0));
    square_search_array.append(Vector2i(-1,-1));
    square_search_array.append(Vector2i( 0,-1));
    square_search_array.append(Vector2i( 1,-1));

    hex_search_array_y_even.append(Vector2i( 1, 0));
    hex_search_array_y_even.append(Vector2i( 0, 1));
    hex_search_array_y_even.append(Vector2i(-1, 1));
    hex_search_array_y_even.append(Vector2i(-1, 0));
    hex_search_array_y_even.append(Vector2i(-1,-1));
    hex_search_array_y_even.append(Vector2i( 0,-1));

    hex_search_array_y_odd.append(Vector2i( 1, 0));
    hex_search_array_y_odd.append(Vector2i( 1, 1));
    hex_search_array_y_odd.append(Vector2i( 0, 1));
    hex_search_array_y_odd.append(Vector2i(-1, 0));
    hex_search_array_y_odd.append(Vector2i( 0,-1));
    hex_search_array_y_odd.append(Vector2i( 1,-1));

    Algorithm = ASTAR;
    Heuristic = EUCLID;

    storage["coordinates"]  = Vector2i(0,0);
    storage["parent"] = Vector2i(0,0);
    storage["neighbors"] = Array{};
    storage["cost"] = 0;
    storage["state"] = 0;
    storage["label"] = 0.0;
    // UtilityFunctions::print(storage);

    // Testing portion of constructor

    // map_size = Vector2i(2,2);

    // Universal_2D_Pathfinder::map_initializer();

    // UtilityFunctions::print("map size: ", Preprocessed_Map.size(),"x",Preprocessed_Map[0].size());


    // // // Preprocessed_Map.write[0].write[0].Node_coordinates = Vector2i(2,5);
    // // Preprocessed_Map.write[0].write[0].Node_cost = 15;
    // // Preprocessed_Map.write[0].write[0].Node_label = 6.5;
    // // Preprocessed_Map.write[0].write[0].Node_neighbors.append(Vector2i(0,0));
    // // Preprocessed_Map.write[0].write[0].Node_parent = Vector2i(2,2);
    // // Preprocessed_Map.write[0].write[0].Node_state = 5;

    // UtilityFunctions::print("Preprocessed_Map[0][0]");
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_coordinates);
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_cost);
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_label);
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_neighbors);
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_parent);
    // UtilityFunctions::print(Preprocessed_Map[0][0].Node_state);
    // UtilityFunctions::print("Preprocessed_Map[0][1]");
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_coordinates);
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_cost);
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_label);
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_neighbors);
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_parent);
    // UtilityFunctions::print(Preprocessed_Map[0][1].Node_state);


    // UtilityFunctions::print("test_array:");
    // Array test_array;
    // Array test_row;
    // test_row.append("test1");
    // test_row.append("test2");
    // test_array.append(test_row);
    // test_row[0] = "test3";
    // test_row[1] = "test4";
    // test_array.append(test_row);
    // UtilityFunctions::print(test_array);
    // // UtilityFunctions::print(test_array[0][0].Node_coordinates);
    // // UtilityFunctions::print(test_array[0][0].Node_cost);
    // // UtilityFunctions::print(test_array[0][0].Node_label);
    // // UtilityFunctions::print(test_array[0][0].Node_neighbors);
    // // UtilityFunctions::print(test_array[0][0].Node_parent);
    // // UtilityFunctions::print(test_array[0][0].Node_state);
    
}


Universal_2D_Pathfinder::~Universal_2D_Pathfinder() {
    // cleanup
    if (Engine::get_singleton()->is_editor_hint()) {

        UtilityFunctions::print("Clean up message");
        // // run preprocessor on scene close in editor
        // UtilityFunctions::print("storage_array.size(): ", storage_array.size());
        // UtilityFunctions::print("Preprocessed_Map.size(): ", Preprocessed_Map.size());
        if (Preprocessed_Map.size() <= map_size.x) {
            Preprocessor();
        }

        // file_manager = memnew(ConfigFile());
        // file_manager->set_value("section1", "map_1", storage_array);
        // file_manager->save("res://test_array.cfg");
        // ERR_FAIL_COND(file_manager->save("res://test_array.cfg") != godot::Error::OK);

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
        Paths = {};
        Path = {};
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
                algorithm = &Universal_2D_Pathfinder::Disjkstra_Pathfinder;
                break;
            // case 2:
            //     algorithm = &Universal_2D_Pathfinder::DP_Pathfinder;
            //     break;
        }

        // one to one path
        if (Start_points_array.size() == 1 && End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("one to one path") : nullptr;     // print message if debug is true
            Path = (this->*algorithm)(Start_points_array[0], End_points_array[0]);      // finds path
            Paths.append(Path);                                                         // appends path to the list of paths
        }

        // many to one paths
        else if (Start_points_array.size() > 1 && End_points_array.size() == 1) {
            (debug == true) ? UtilityFunctions::print("many to one paths") : nullptr;
            for (int i = 0; i < Start_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[i], End_points_array[0]);
                Paths.append(Path);
            }
        }

        // one to many paths
        else if (Start_points_array.size() == 1 && End_points_array.size() > 1) {
            (debug == true) ? UtilityFunctions::print("one to many paths") : nullptr;
            for (int i = 0; i < End_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[0], End_points_array[i]);
                Paths.append(Path);
            }
        }

        // many to many paths (MUST HAVE the same number of starts and ends!!!)
        else if (Start_points_array.size() > 1 && End_points_array.size() > 1) {
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
            
            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            // Vector<Node_Data> temp;

            // temp = Preprocessed_Map[start_x];
            // OPEN_list.append(temp[start_y]);

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
    void Universal_2D_Pathfinder::Preprocessor() {
        if (map_size == Vector2i(0,0)) {
            // UtilityFunctions::print("Preprocessor finished");
            // return Preprocessed_Map;
            return;
        }
        else {

            if (Preprocessed_Map.size() != map_size.x) {
                map_initializer();
            }

        //     int map_size_x = map_size.x;
        //     int map_size_y = map_size.y;
            for (int32_t x = 0; x < map_size.x; x++) {
                // UtilityFunctions::print("x: ", x);
                for (int32_t y = 0; y < map_size.y; y++) {
                    // UtilityFunctions::print("y: ", y);
                    UtilityFunctions::print("coordinates: ", Vector2i(x,y));
                    Vector2i node_atlas_coords = get_cell_atlas_coords(Vector2i(x,y));
                    int node_tile_id = get_cell_source_id(Vector2i(x,y));
                    UtilityFunctions::print("node_tile_id: ", node_tile_id);
                    UtilityFunctions::print("node_atlas_coords: ", node_atlas_coords);
                    // Node_Data map_node = map_x[y];
                    // Preprocessed_Map.write[x].write[y].Node_coordinates = Vector2i(x,y);
                    // Preprocessed_Map.write[x].write[y].Node_parent;
                    // Preprocessed_Map.write[x].write[y].Node_neighbors = Array{};
                    Preprocessed_Map.write[x].write[y].Node_cost = 0;
                    Preprocessed_Map.write[x].write[y].Node_state = 0;
                    // Preprocessed_Map.write[x].write[y].Node_label = 0.0;
                    
                }
            }
            // neighbors processing
            for (int32_t x = 0; x < map_size.x; x++) {
                for (int32_t y = 0; y < map_size.y; y++) {







                    Preprocessed_Map.write[x].write[y].Node_neighbors;
                }
            }

            UtilityFunctions::print("Preprocessor finished");
            // return Preprocessed_Map;
            return;
        }
    }



    // Helper methods
        // label calculation
        double Universal_2D_Pathfinder::Label_Calculator(Universal_2D_Pathfinder::Node_Data Node_Parent, Universal_2D_Pathfinder::Node_Data Node) {
            return 0;
        };


        // neighbor search


        // Map initializer
        void Universal_2D_Pathfinder::set_button(bool but) {
            // if (button == true) {button = false;}
            // if (button == false) {button = true;}
            button = but;
            map_initializer();
            Preprocessor();
        }
        bool Universal_2D_Pathfinder::get_button() {
            return button;
        }
        void Universal_2D_Pathfinder::map_initializer() {
            int size_x = map_size.x;
            int size_y = map_size.y;

            Node_Data init_data;
            Vector<Node_Data> y_row;
            Vector<Vector<Node_Data>> x_row;
            Array y_storage;
            Array x_storage;

            if (size_x > 0 && size_y > 0) {
                for (int x = 0; x < size_x; x++) {
                    y_row.clear();
                    y_storage.clear();
                    for (int y = 0; y < size_y; y++) {
                        init_data.Node_coordinates = Vector2i(x,y);
                        // UtilityFunctions::print("x: ", x, "|y: ", y, "|coordinates:", init_data.Node_coordinates);
                        y_row.append(init_data);
                        y_storage.append(storage);
                    }
                    x_row.append(y_row);
                    x_storage.append(y_storage);
                }
            }
            Preprocessed_Map = x_row;
            storage_array = x_storage;
            UtilityFunctions::print("Map initialized");
        }

        // map savers
        // to godot
        Array Universal_2D_Pathfinder::Map_to_code() {
            return Array{};
        }
        // to .cfg file
        void Universal_2D_Pathfinder::Map_to_file(String path_to_file) {
            return;
        }

        // map loaders
        // from godot
        void Universal_2D_Pathfinder::Code_to_map(Array data) {
            return;
        }
        // from .cfg file
        void Universal_2D_Pathfinder::File_to_map(String path_to_file) {
            return;
        }

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

    // set algorithm
    void Universal_2D_Pathfinder::set_Algorithm(Algorithm_Type alg) {
        Algorithm = alg;
    }
    Universal_2D_Pathfinder::Algorithm_Type Universal_2D_Pathfinder::get_Algorithm() const {
        return Algorithm;
    }

    // set Heuristic
    void Universal_2D_Pathfinder::set_Heuristic(Heuristic_Type heur) {
        Heuristic = heur;
    }
    Universal_2D_Pathfinder::Heuristic_Type Universal_2D_Pathfinder::get_Heuristic() const {
        return Heuristic;
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


    // storage array
    void Universal_2D_Pathfinder::set_storage_array(Array sta) {
        storage_array = sta;
    }
    Array Universal_2D_Pathfinder::get_storage_array() {
        return storage_array;
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


