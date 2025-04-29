
#include "Universal_2D_Pathfinder.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
// #include <godot_cpp/classes/editor_interface.hpp>
// #include <godot_cpp/classes/json.hpp>


using namespace godot;


void Universal_2D_Pathfinder::_bind_methods() {
    // add methods
    ClassDB::bind_method(D_METHOD("Pathfinder", "Start_points_array", "End_points_array", "debug"), &Universal_2D_Pathfinder::Pathfinder);
    
    ClassDB::bind_method(D_METHOD("save_to_code"), &Universal_2D_Pathfinder::save_to_code);
    ClassDB::bind_method(D_METHOD("save_to_file", "path_to_file", "file_name"), &Universal_2D_Pathfinder::save_to_file);
    
    ClassDB::bind_method(D_METHOD("load_from_code", "data"), &Universal_2D_Pathfinder::load_from_code);
    ClassDB::bind_method(D_METHOD("load_from_file", "path_to_file"), &Universal_2D_Pathfinder::load_from_file);
    
    ClassDB::bind_method(D_METHOD("load_tileset_cfg", "path_to_file"), &Universal_2D_Pathfinder::load_tileset_cfg);
    
    // map initializer and preprocessor
    ClassDB::bind_method(D_METHOD("map_initializer"), &Universal_2D_Pathfinder::map_initializer);
    ClassDB::bind_method(D_METHOD("Preprocessor"), &Universal_2D_Pathfinder::Preprocessor);

    // add propperties

    // // Start_position
    // ClassDB::bind_method(D_METHOD("set_Start_position"), &Universal_2D_Pathfinder::set_Start_position);
    // ClassDB::bind_method(D_METHOD("get_Start_position"), &Universal_2D_Pathfinder::get_Start_position);
    // ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "Start_position"), "set_Start_position", "get_Start_position");

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
        PropertyInfo(Variant::INT, "Heuristic", PROPERTY_HINT_ENUM, "Euclid, Euclid (to power), Euclid (weighted), Euclid (exponential), Manhatan, Chebyshev, Octile"),
        "set_Heuristic",
        "get_Heuristic"
    );
    BIND_ENUM_CONSTANT(EUCLID);
    BIND_ENUM_CONSTANT(EUCLID_POW);
    BIND_ENUM_CONSTANT(EUCLID_WGHT);
    BIND_ENUM_CONSTANT(EUCLID_EXP);
    BIND_ENUM_CONSTANT(MANHATAN);
    BIND_ENUM_CONSTANT(CHEBYSHEV);
    BIND_ENUM_CONSTANT(OCTILE);
    
    ClassDB::bind_method(D_METHOD("set_button"), &Universal_2D_Pathfinder::set_button);
    ClassDB::bind_method(D_METHOD("get_button"), &Universal_2D_Pathfinder::get_button);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug button"), "set_button", "get_button");

    ClassDB::bind_method(D_METHOD("set_diagonal"), &Universal_2D_Pathfinder::set_diagonal);
    ClassDB::bind_method(D_METHOD("get_diagonal"), &Universal_2D_Pathfinder::get_diagonal);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "diagonal movement"), "set_diagonal", "get_diagonal");

    ClassDB::bind_method(D_METHOD("set_weight"), &Universal_2D_Pathfinder::set_weight);
    ClassDB::bind_method(D_METHOD("get_weight"), &Universal_2D_Pathfinder::get_weight);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Weight"), "set_weight", "get_weight");

    ClassDB::bind_method(D_METHOD("set_open_list"), &Universal_2D_Pathfinder::set_open_list);
    ClassDB::bind_method(D_METHOD("get_open_list"), &Universal_2D_Pathfinder::get_open_list);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "OPEN_list"), "set_open_list", "get_open_list");

    ClassDB::bind_method(D_METHOD("set_closed_list"), &Universal_2D_Pathfinder::set_closed_list);
    ClassDB::bind_method(D_METHOD("get_closed_list"), &Universal_2D_Pathfinder::get_closed_list);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "CLOSED_list"), "set_closed_list", "get_closed_list");
}

Universal_2D_Pathfinder::Universal_2D_Pathfinder() {
    // permanent part of constructor
    // Universal_2D_Pathfinder::map_initializer();

    // search arrays initialization
    square_search_array.append(Vector2i( 1, 0));
    square_search_array.append(Vector2i( 0, 1));
    square_search_array.append(Vector2i(-1, 0));
    square_search_array.append(Vector2i( 0,-1));
    square_search_array.append(Vector2i( 1, 1));
    square_search_array.append(Vector2i( 1,-1));
    square_search_array.append(Vector2i(-1, 1));
    square_search_array.append(Vector2i(-1,-1));

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

    init();

    


    // UtilityFunctions::print(storage);

    //%%%%%%% Testing portion of constructor

    Array tile_red;
    tile_red.append(0);
    tile_red.append(false);
    tile_data[Vector2i(0,0)] = tile_red;
    Array tile_green;
    tile_green.append(1);
    tile_green.append(true);
    tile_data[Vector2i(1,0)] = tile_green;
    Array tile_blue;
    tile_blue.append(5);
    tile_blue.append(true);
    tile_data[Vector2i(2,0)] = tile_blue;
    map_initializer(0);
    Preprocessor();

}


Universal_2D_Pathfinder::~Universal_2D_Pathfinder() {
    // cleanup
    if (Engine::get_singleton()->is_editor_hint()) {

        // UtilityFunctions::print("Clean up message");
        // // run preprocessor on scene close in editor
        // UtilityFunctions::print("storage_array.size(): ", storage_array.size());
        // UtilityFunctions::print("Preprocessed_Map.size(): ", Preprocessed_Map.size());
        // if (Preprocessed_Map.size() <= map_size.x) {
        //     Preprocessor();
        // }

        // file_manager = memnew(ConfigFile());
        // file_manager->set_value("section1", "map_1", storage_array);
        // file_manager->save("res://test_array.cfg");
        // ERR_FAIL_COND(file_manager->save("res://test_array.cfg") != godot::Error::OK);

    }

}



// Main methods
    void Universal_2D_Pathfinder::init() {

        button = false;

        Algorithm = ASTAR;
        Heuristic = EUCLID;

        storage["coordinates"] = Vector2i(0,0);
        storage["parent"] = Vector2i(0,0);
        storage["neighbors"] = Array();
        storage["cost"] = float(0);
        storage["distance_to"] = float(0);
        storage["state"] = bool(false);
        storage["label"] = float(0);
        return;
    }


    // Pathfinder
    Array Universal_2D_Pathfinder::Pathfinder(Array Start_points_array, Array End_points_array, const bool debug) {
        for (int i =0; i < Start_points_array.size(); i++) {
            Vector2i pos = Start_points_array[i];
            if (pos.x < 0 || pos.y < 0 || pos.x >= map_size.x || pos.y >= map_size.y) {
                UtilityFunctions::print("Start position is out of bounds!");
                return Array();
            }
        }
        Map_tileset = *get_tile_set();
        TileSet::TileShape current_tileset = Map_tileset->get_tile_shape();
        TileSet::TileShape last_used_tileset;
        if (Preprocessed_Map.size() < 1) {
            last_used_tileset = Map_tileset->get_tile_shape();
            map_initializer(0);
            Preprocessor();
        } else if (Preprocessed_Map[0].size() < 1) {
            last_used_tileset = Map_tileset->get_tile_shape();
            map_initializer(0);
            Preprocessor();
        }
        
        if (current_tileset != last_used_tileset) {
            last_used_tileset = Map_tileset->get_tile_shape();
            map_initializer(0);
            Preprocessor();
        }
        
        Paths = Array();
        Path = Array();
        switch (Algorithm) {
            case 0:
                algorithm = &Universal_2D_Pathfinder::AStar_Pathfinder;
                break;
            case 1:
                algorithm = &Universal_2D_Pathfinder::Disjkstra_Pathfinder;
                break;
            case 2:
                algorithm = &Universal_2D_Pathfinder::DP_Pathfinder;
                break;
        }

        // one to one path
        if (Start_points_array.size() == 1 && End_points_array.size() == 1) {
            if (debug == true) { 
                UtilityFunctions::print("one to one path");     // print message if debug is true
            }
            Path = (this->*algorithm)(Start_points_array[0], End_points_array[0]);      // finds path
            Paths.append(Path);                                                         // appends path to the list of paths
        }

        // many to one paths
        else if (Start_points_array.size() > 1 && End_points_array.size() == 1) {
            if (debug == true) {
                UtilityFunctions::print("many to one paths");
            }
            for (int i = 0; i < Start_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[i], End_points_array[0]);
                Paths.append(Path);
            }
        }

        // one to many paths
        else if (Start_points_array.size() == 1 && End_points_array.size() > 1) {
            if (debug == true) {
                UtilityFunctions::print("one to many paths");
            }
            for (int i = 0; i < End_points_array.size(); i++)
            {
                /* code */
                Path = (this->*algorithm)(Start_points_array[0], End_points_array[i]);
                Paths.append(Path);
            }
        }

        // many to many paths (MUST HAVE the same number of starts and ends!!!)
        else if (Start_points_array.size() > 1 && End_points_array.size() > 1) {
            if (debug == true) {
                UtilityFunctions::print("many to many paths");
            }
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
        last_used_tileset = Map_tileset->get_tile_shape();
        return Paths;
    }


    // Algorithms
        // A*
        Array Universal_2D_Pathfinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {
            if (Preprocessed_Map.size() > 0) {
                if (Preprocessed_Map[0].size() > 0) {
                    map_initializer(2);
                }
            }

            OPEN_list.clear();
            CLOSED_list.clear();

            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            OPEN_list.append(start_node);

            if (OPEN_list.is_empty()) {
                UtilityFunctions::print("A* exited with: OPEN array is empty");
                return Array();
            };

            Label_Calculator(OPEN_list[0], end_node, false, true);
            bool exit_flag = false;
            Vector2i previous_node;
            Vector2i current_node;
            Vector2i min;
            int iter = 1;
            while (exit_flag == false) {
                // UtilityFunctions::print("A* iteration: ", iter);
                // if OPEN list is empty there is no solution -> terminate search
                // if (OPEN_list.is_empty()) {
                //     UtilityFunctions::print("A* exited with: OPEN array is empty");
                //     return Array();
                // };

                // UtilityFunctions::print("Open list: ", OPEN_list);

                // pull node with the smallest f(i) value in OPEN, if multiple check if any is end node
                Array mins = find_minimum_label(OPEN_list);
                // UtilityFunctions::print("minimums: ", mins);
                if (mins.size() > 1) {
                    for (int i = 0; i < mins.size(); i++) {
                        if (mins[i] == end_node) {
                            // exit_flag = true;
                            min = mins[i];
                            break;
                        } else {
                            min = mins.back();
                        }
                    }
                } else if (mins.size() == 1) {
                    min = mins[0];
                } else {
                    UtilityFunctions::print("A* exited with: No minimum found! After ", iter, " iterations");
                    break;
                }
                int idx = OPEN_list.find(min, 0);
                current_node = OPEN_list.pop_at(idx);
                if (current_node == end_node) {
                    exit_flag = true;
                }

                CLOSED_list.append(current_node);

                // expanding neighbors
                // Array curr_node_data;
                // // UtilityFunctions::print("current node: ", current_node);
                // curr_node_data.clear();
                // curr_node_data.append(Preprocessed_Map[current_node.x][current_node.y].Node_parent);
                // curr_node_data.append(Preprocessed_Map[current_node.x][current_node.y].Node_cost);
                // curr_node_data.append(Preprocessed_Map[current_node.x][current_node.y].Distance_to);
                // curr_node_data.append(Preprocessed_Map[current_node.x][current_node.y].Node_label);
                // curr_node_data.append(Preprocessed_Map[current_node.x][current_node.y].Reachable);
                // UtilityFunctions::print(curr_node_data);
                // for (int i = 0; i < Preprocessed_Map[current_node.x][current_node.y].Node_neighbors.size(); i++) {
                //     UtilityFunctions::print("current node's neighbors: ", Preprocessed_Map[current_node.x][current_node.y].Node_neighbors[i]);
                // }


                for (int i = 0; i < Preprocessed_Map[current_node.x][current_node.y].Node_neighbors.size(); i++) {
                    Vector2i curr_nbr = Preprocessed_Map[current_node.x][current_node.y].Node_neighbors[i];
                    bool open_has = OPEN_list.has(curr_nbr);
                    bool closed_has = CLOSED_list.has(curr_nbr);
                    
                    bool diag = false;
                    if (i > 3) {
                        diag = true;
                    } else {
                        diag = false;
                    }
                    double f_n = Label_Calculator(curr_nbr, end_node, diag, false);
                    // double f_n;
                    if (open_has == true || closed_has == true) {
                        if (f_n < Preprocessed_Map[curr_nbr.x][curr_nbr.y].Node_label) {
                            // Label_Calculator(curr_nbr, end_node, true);
                            f_n = Label_Calculator(curr_nbr, end_node, diag, false);
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_label = f_n;
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                            if (closed_has && !open_has) {
                                OPEN_list.append(CLOSED_list.pop_at(CLOSED_list.find(curr_nbr, 0)));
                            }
                        }
                    } else if (open_has == false && closed_has == false) {
                    //     double f_n = Label_Calculator(curr_nbr, end_node, true);
                        f_n = Label_Calculator(curr_nbr, end_node, diag, false);
                        Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_label = f_n;
                        Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                        OPEN_list.append(curr_nbr);
                    }
                }

                if (exit_flag == true) {
                    break;
                }

                if (OPEN_list.is_empty()) {
                    UtilityFunctions::print("A* exited with: OPEN array is empty! After ", iter, " iterations");
                    return Array();
                }
                
                if (iter > 10000) {
                    UtilityFunctions::print("A* exited with: Iteration limit reached");
                    break;
                }

                iter++;
            }

            if (CLOSED_list.has(end_node)) {
                current_node = end_node;
            }
            Path.append(current_node);
            while (!(current_node == start_node)) {
                current_node = Preprocessed_Map[current_node.x][current_node.y].Node_parent;
                Path.append(current_node);
                if (Path.size() >= MAX_PATH_LENGTH) {
                    break;
                }
            }

            // // if (CLOSED_list.has(end_node)) {
            // //     current_node = end_node;
            // // }
            // // Path.append(current_node);
            // iter = 1;
            // Vector2i new_node;
            // while (!(current_node == start_node) || iter < MAX_PATH_LENGTH) {
            //     if (iter = 1) {
            //         if (CLOSED_list.has(end_node)) {
            //             current_node = end_node;
            //         }
            //     } else {
            //         new_node = Preprocessed_Map[current_node.x][current_node.y].Node_parent;
            //         if (new_node == current_node) {
            //             UtilityFunctions::print("A* path looped on itself!");
            //             return Array();
            //         } else {
            //             current_node = new_node;
            //         }
            //     }
            //     if (!(Path.has(current_node))) {
            //         Path.append(current_node);
            //         if (Path.size() >= MAX_PATH_LENGTH) {
            //             break;
            //         }
            //     } //else {
            //     //     UtilityFunctions::print("A* path looped on itself!");
            //     //     break;
            //     // }
            //     iter++;
            // }

            UtilityFunctions::print("A* finished and found path in ", iter, " iterations");

            // UtilityFunctions::print("OPEN_list: ", OPEN_list);
            // UtilityFunctions::print("CLOSED_list: ", CLOSED_list);
            Array placeholder;
            placeholder.append(Vector2i(0,0));
            // return placeholder;
            return Path;
        }

        
        // Dijkstra
        Array Universal_2D_Pathfinder::Disjkstra_Pathfinder(Vector2i start_node, Vector2i end_node) {
            if (Preprocessed_Map.size() > 0) {
                if (Preprocessed_Map[0].size() > 0) {
                    map_initializer(2);
                }
            }
            
            OPEN_list.clear();
            CLOSED_list.clear();

            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            OPEN_list.append(start_node);

            if (OPEN_list.is_empty()) {
                UtilityFunctions::print("A* exited with: OPEN array is empty");
                return Array();
            };

            Label_Calculator(OPEN_list[0], end_node, false, true);
            bool exit_flag = false;
            Vector2i previous_node;
            Vector2i current_node;
            Vector2i min;
            int iter = 1;
            while (exit_flag == false) {
                // UtilityFunctions::print("A* iteration: ", iter);
                // if OPEN list is empty there is no solution -> terminate search
                // if (OPEN_list.is_empty()) {
                //     UtilityFunctions::print("A* exited with: OPEN array is empty");
                //     return Array();
                // };

                // UtilityFunctions::print("Open list: ", OPEN_list);

                current_node = OPEN_list.pop_front();
                if (current_node == end_node) {
                    exit_flag = true;
                }

                CLOSED_list.append(current_node);

                // expanding neighbors


                for (int i = 0; i < Preprocessed_Map[current_node.x][current_node.y].Node_neighbors.size(); i++) {
                    Vector2i curr_nbr = Preprocessed_Map[current_node.x][current_node.y].Node_neighbors[i];
                    bool open_has = OPEN_list.has(curr_nbr);
                    bool closed_has = CLOSED_list.has(curr_nbr);
                    bool diag;
                    if (i >= 4) {
                        diag = true;
                    } else {
                        diag = false;
                    }
                    double f_n = Label_Calculator(curr_nbr, end_node, diag, false);
                    if (open_has == true || closed_has == true) {
                        if (f_n < Preprocessed_Map[curr_nbr.x][curr_nbr.y].Distance_to) {
                            // Label_Calculator(curr_nbr, end_node, true);
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Distance_to = f_n;
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                            // if (closed_has && !open_has) {
                            //     OPEN_list.append(CLOSED_list.pop_at(CLOSED_list.find(curr_nbr, 0)));
                            // }
                        }
                    } else if (open_has == false && closed_has == false) {
                    //     double f_n = Label_Calculator(curr_nbr, end_node, true);
                        Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Distance_to = f_n;
                        Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                        OPEN_list.append(curr_nbr);
                    }
                }

                if (exit_flag == true) {
                    break;
                }

                if (OPEN_list.is_empty()) {
                    UtilityFunctions::print("A* exited with: OPEN array is empty! After ", iter, " iterations");
                    return Array();
                }
                
                if (iter > 10000) {
                    UtilityFunctions::print("A* exited with: Iteration limit reached");
                    break;
                }

                iter++;
            }

            if (CLOSED_list.has(end_node)) {
                current_node = end_node;
            }
            Path.append(current_node);
            while (!(current_node == start_node)) {
                current_node = Preprocessed_Map[current_node.x][current_node.y].Node_parent;
                Path.append(current_node);
                if (Path.size() >= MAX_PATH_LENGTH) {
                    break;
                }
            }

            

            UtilityFunctions::print("A* finished and found path in ", iter, " iterations");

            
            Array placeholder;
            placeholder.append(Vector2i(0,0));
            // return placeholder;
            return Path;


            return Array();
        }


        // Dynamic Programming
        Array Universal_2D_Pathfinder::DP_Pathfinder(Vector2i start_node, Vector2i end_node) {
            // For DP Nodes parent is going to be its child
            
            bool map_ready;
            
            if (Preprocessed_Map.size() > 0) {
                if (Preprocessed_Map[0].size() > 0) {
                    map_initializer(2);
                }
            }

            if (!map_ready) {
                bool update;
                for (int i = 0; i < 100000; i++) {
                    if (update == false) {
                        break;
                    }

                    for (int x = 0; x < Preprocessed_Map.size(); x++) {
                        for (int y = 0; y < Preprocessed_Map[0].size(); y++) {
                            
                        }
                    }

                }


                map_ready = true;
            }


            


            
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
            if (Preprocessed_Map.size() != map_size.x) { map_initializer(0); }
            for (int32_t x = 0; x < map_size.x; x++) {
                // UtilityFunctions::print("x: ", x);
                for (int32_t y = 0; y < map_size.y; y++) {
                    int tile_cost;
                    bool reachable;
                    Vector2i node_atlas_coords = get_cell_atlas_coords(Vector2i(x,y));
                    int node_tile_id = get_cell_source_id(Vector2i(x,y));
                    Array data = tile_data[node_atlas_coords];
                    if (node_atlas_coords == Vector2i(-1,-1)) {
                        tile_cost = 0;
                        reachable = false;
                    } else {
                        tile_cost = data[0];
                        reachable = data[1];
                    }
                    Preprocessed_Map.write[x].write[y].Node_cost = tile_cost;
                    Preprocessed_Map.write[x].write[y].Reachable = reachable;
                }
            }
            // neighbors processing
            Map_tileset = *get_tile_set();
            Array neighbors;
            TileSet::TileShape Map_shape = Map_tileset->get_tile_shape();
            for (int32_t x = 0; x < map_size.x; x++) {
                for (int32_t y = 0; y < map_size.y; y++) {
                    SearchVector.clear();
                    neighbors.clear();
                    switch (Map_shape)
                    {
                    case 0:
                        neighbors.clear();
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_RIGHT_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_LEFT_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_SIDE));
                        if (diagonal == true) {
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_LEFT_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_LEFT_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_RIGHT_CORNER));
                        }
                        break;
                    case 1:
                        neighbors.clear();
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_LEFT_SIDE));
                        neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
                        if (diagonal == true) {
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_RIGHT_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_LEFT_CORNER));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_CORNER));
                        }
                        break;
                    case 3:
                        neighbors.clear();
                        if (Map_tileset->get_tile_offset_axis() == TileSet::TileOffsetAxis::TILE_OFFSET_AXIS_HORIZONTAL) {
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_RIGHT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_LEFT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_LEFT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
                        } else {
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_RIGHT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_RIGHT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_BOTTOM_LEFT_SIDE));
                            neighbors.append(get_neighbor_cell(Vector2i(x,y), TileSet::CellNeighbor::CELL_NEIGHBOR_TOP_LEFT_SIDE));
                        }
                    }
                    Preprocessed_Map.write[x].write[y].Node_neighbors.clear();
                    for (int i = 0; i < neighbors.size(); i++) {
                        Vector2i n = neighbors[i];
                        if (!(n.x < 0) && !(n.y < 0) && !(n.x > map_size.x-1) && !(n.y > map_size.y-1)) {
                            if (Preprocessed_Map[n.x][n.y].Reachable == true && Preprocessed_Map[x][y].Reachable == true) {
                                Preprocessed_Map.write[x].write[y].Node_neighbors.append(neighbors[i]);
                            }
                        }
                    }

                    // Array list;
                    // list.clear();
                    // for (int i = 0; i < Preprocessed_Map[x][y].Node_neighbors.size(); i++) {
                    //     list.append(Preprocessed_Map[x][y].Node_neighbors[i]);
                    // }
                    // UtilityFunctions::print("neighbors of ", Vector2i(x,y), ": ", list);
                }
            }
            UtilityFunctions::print("Preprocessor finished");
            return;
        }
    }



    // Helper methods
        // label calculation
        double Universal_2D_Pathfinder::Label_Calculator(Vector2i _node, Vector2i end, bool is_tile_diagonal, bool overwrite) {
            Vector2i coords = Preprocessed_Map[_node.x][_node.y].Node_coordinates;
            double cost = Preprocessed_Map[_node.x][_node.y].Node_cost;
            double cost_mod = 0;
            if (is_tile_diagonal == true) {
                cost = cost + sqrt(2);
            } else {
                cost += 0;
            }
            Vector2i parent = Preprocessed_Map[_node.x][_node.y].Node_parent;
            double parent_distance;
            if (parent == coords) {
                parent_distance = 0;
            } else {
                parent_distance = Preprocessed_Map[parent.x][parent.y].Distance_to;
            }
            double h;
            int x = abs(coords.x - end.x);
            int y = abs(coords.y - end.y);
            double x_pow = pow(x, 2);;
            double y_pow = pow(y, 2);;
            switch (Heuristic)
            {
            case 0:
                h = sqrt(x_pow + y_pow);
                break;
            case 1:
                h = pow(sqrt(x_pow + y_pow), weight);
                break;
            case 2:
                h = sqrt(x_pow + y_pow) * weight;
                break;
            case 3:
                h = sqrt(x_pow + y_pow);
                h = h * exp(h);
                break;
            case 4:
                h = x + y;
                break;
            case 5:
                h = (x > y) ? x : y; // if x > y -> x else y
                break;
            case 6:
                int max = (x > y) ? x : y; // if x > y -> x else y
                int min = (x < y) ? x : y; // if x < y -> x else y
                h = (min * (sqrt(2) - 1)) + max;
                break;
            }
            if (overwrite == true) {
                Preprocessed_Map.write[_node.x].write[_node.y].Distance_to = parent_distance + cost + cost_mod;
                Preprocessed_Map.write[_node.x].write[_node.y].Node_label = parent_distance + cost + cost_mod + h;
                return 0.0;
            } else {
                double f_n;
                if (Algorithm == Algorithm_Type::ASTAR) {
                    f_n = parent_distance + cost + cost_mod + h;
                } else if (Algorithm == Algorithm_Type::DIJKSTRA)
                {
                    f_n = parent_distance + cost + cost_mod;
                }
                return f_n;
            }
        }

        // Map initializer
        void Universal_2D_Pathfinder::set_button(bool but) {
            button = but;
            if (button == true) {
                // map_initializer(0);
                Preprocessor();
                Array t_s;
                t_s.append(Vector2i(0,0));
                Array t_e;
                t_e.append(Vector2i(2,2));
                Array path = Pathfinder(t_s, t_e, true);
                UtilityFunctions::print("path: ", path);

                // TypedArray<Vector2i> test;
                // test.append(Vector2i(0,0));
                // test.append(Vector2i(0,1));
                // test.append(Vector2i(1,0));
                // test.append(Vector2i(1,1));
                // test.append(Vector2i(1,2));
                // UtilityFunctions::print("TypedArray test: ", test);
                

                // AStar_Pathfinder(Vector2i(0,0), Vector2i(1,1));
                // Label_Calculator(Vector2i(0,0), Vector2i(1,1), true);
                
                // Array test;
                // test.append(Vector2i(0,0));
                // test.append(Vector2i(1,1));
                // Preprocessed_Map.write[0].write[0].Node_label = 2.1;
                // Preprocessed_Map.write[1].write[1].Node_label = 3.0;

                // Array min_label = find_minimum_label(test);
                // UtilityFunctions::print(min_label);
                button = false;
            }
        }
        bool Universal_2D_Pathfinder::get_button() {
            return button;
        }
        void Universal_2D_Pathfinder::map_initializer(int type) {
            
            if (!(type == 2)) {
                Preprocessed_Map.clear();
            }
            
            int size_x = map_size.x;
            int size_y = map_size.y;

            Node_Data init_data;
            Vector<Node_Data> y_row;
            Vector<Vector<Node_Data>> x_row;

            Array y_storage;
            Array x_storage;

            switch (type)
            {
            case 0:
                if (size_x > 0 && size_y > 0) {
                    for (int x = 0; x < size_x; x++) {
                        y_row.clear();
                        for (int y = 0; y < size_y; y++) {
                            init_data.Node_coordinates = Vector2i(x,y);
                            init_data.Node_parent = Vector2i(x,y);
                            y_row.append(init_data);
                        }
                        x_row.append(y_row);
                    }
                }
                Preprocessed_Map = x_row;
                UtilityFunctions::print("Map initialized");
                break;
            case 1:
                if (size_x > 0 && size_y > 0) {
                    for (int x = 0; x < size_x; x++) {
                        y_storage.clear();
                        for (int y = 0; y < size_y; y++) {
                            storage["coordinates"] = Vector2i(x,y);
                            y_storage.append(storage.duplicate());
                        }
                        x_storage.append(y_storage.duplicate());
                    }
                }
                storage_array = x_storage.duplicate();
                x_storage.clear();
                y_storage.clear();
                break;
            case 2:
                if (Preprocessed_Map.size() > 0) {
                    if (Preprocessed_Map[0].size() > 0) {
                        for (int x = 0; x < Preprocessed_Map.size(); x++) {
                            for (int y = 0; y < Preprocessed_Map[0].size(); y++) {
                                // Preprocessed_Map.write[x].write[y].Node_neighbors.clear();
                                Preprocessed_Map.write[x].write[y].Distance_to = 0;
                                Preprocessed_Map.write[x].write[y].Node_label = 1e5;
                                Preprocessed_Map.write[x].write[y].Node_parent = Vector2i(x,y);
                            }
                        }
                    }
                }
                break;
            }
        }



        // find minimal label
        Array Universal_2D_Pathfinder::find_minimum_label(Array& open_list) {
            Array minimums = Array();
            float min = 1.7e10;
            for (int i = 0; i < open_list.size(); i++) {
                Vector2i coords = open_list[i];
                float l =  Preprocessed_Map[coords.x][coords.y].Node_label;
                if (l < min) {
                    min = l;
                } else {
                    continue;
                }
            }
            for (int i = 0; i < open_list.size(); i++) {
                Vector2i coords = open_list[i];
                float l =  Preprocessed_Map[coords.x][coords.y].Node_label;
                if (l - min < 1e-10) {
                    minimums.append(open_list[i]);
                }
            }
            return minimums;
        };

    // File management
        // map savers
        // to godot
        Array Universal_2D_Pathfinder::save_to_code() {
            // map_initializer(1);


            storage_array.clear();
            Array storage_array_y;
            Array neighbors;
            if (Preprocessed_Map.size() > 0) {
                if (Preprocessed_Map[0].size() > 0) {
                    for (int x = 0; x < Preprocessed_Map.size(); x++) {
                        storage_array_y.clear();
                        for (int y = 0; y < Preprocessed_Map[0].size(); y++){
                            storage["coordinates"] = Vector2i(x,y);
                            storage["parent"] = Preprocessed_Map[x][y].Node_parent;
                            neighbors.clear();
                            for (Vector2i n : Preprocessed_Map[x][y].Node_neighbors) {
                                neighbors.append(n);
                            }
                            storage["neighbors"] = neighbors.duplicate();
                            storage["cost"] = Preprocessed_Map[x][y].Node_cost;
                            storage["distance_to"] = Preprocessed_Map[x][y].Distance_to;
                            storage["state"] = Preprocessed_Map[x][y].Reachable;
                            storage["label"] = Preprocessed_Map[x][y].Node_label;
                            storage_array_y.append(storage.duplicate());
                        }
                        storage_array.append(storage_array_y.duplicate());
                    }
                }
            }
            // int64_t mem_size = sizeof(&storage_array);
            // UtilityFunctions::print("memory size of storage array: ", mem_size, " Bytes");

            return storage_array;
        }
        // to .cfg file
        void Universal_2D_Pathfinder::save_to_file(String path_to_file, String file_name) {
            // map_initializer(1);

            
            storage_array.clear();
            storage.clear();
            Array storage_array_y;
            Array neighbors;
            if (Preprocessed_Map.size() > 0) {
                if (Preprocessed_Map[0].size() > 0) {
                    for (int x = 0; x < Preprocessed_Map.size(); x++) {
                        storage_array_y.clear();
                        for (int y = 0; y < Preprocessed_Map[0].size(); y++){
                            storage.clear();
                            storage["coordinates"] = Vector2i(x,y);
                            storage["parent"] = Preprocessed_Map[x][y].Node_parent;
                            neighbors.clear();
                            for (Vector2i n : Preprocessed_Map[x][y].Node_neighbors) {
                                neighbors.append(n);
                            }
                            storage["neighbors"] = neighbors.duplicate();
                            storage["cost"] = Preprocessed_Map[x][y].Node_cost;
                            storage["distance_to"] = Preprocessed_Map[x][y].Distance_to;
                            storage["state"] = Preprocessed_Map[x][y].Reachable;
                            storage["label"] = Preprocessed_Map[x][y].Node_label;
                            storage_array_y.append(storage.duplicate());
                        }
                        storage_array.append(storage_array_y.duplicate());
                    }
                }
            }
            
            Map_tileset = *get_tile_set();
            TileSet::TileShape Map_shape = Map_tileset->get_tile_shape();
            String section;
            switch (Map_shape)
            {
                case 0:
                section = "square";
                break;
                case 1:
                section = "iso";
                break;
                case 3:
                section = "hex";
                break;
            }
            
            // UtilityFunctions::print("selected section: ", section);
            
            String name = get_name();
            // UtilityFunctions::print("node name: ", name);
            
            // UtilityFunctions::print(storage_array);
            
            ConfigFile* saver = memnew(ConfigFile);
            if (storage_array.size() > 0 && !(storage.is_empty())) {
                saver->set_value(section, name, storage_array);
            } else {
                UtilityFunctions::print("No data to save!");
                return;
            }
            String full_path = "user://" + path_to_file + "//" + file_name + ".cfg";
            // UtilityFunctions::print("file path: ", full_path);
            // saver->save(full_path);
            Error save_state;
            save_state = saver->save(full_path);
            // ERR_FAIL_COND(saver->save(full_path) != godot::Error::OK);
            if (save_state == Error::OK) {
                UtilityFunctions::print("Seccesfully saved");
            } else {
                UtilityFunctions::print("Error saving data to file!");
            }
            return;
        }

        // map loaders
        // from godot
        void Universal_2D_Pathfinder::load_from_code(Array data) {
            int size_x = data.size();
            int size_y;
            if (size_x > 0) {
                Array d_y = data.front();
                size_y = d_y.size();
                if (size_y > 0) {
                    map_size.x = size_x;
                    map_size.y = size_y;
                }
            }
            map_initializer(0);

            for (int x = 0; x < size_x; x++) {
                for (int y = 0; y < size_y; y++) {
                    TypedArray<Dictionary> data_x = data.pop_front();
                    Dictionary data_x_y = data_x.pop_front();
                    Preprocessed_Map.write[x].write[y].Node_coordinates = data_x_y["coordinates"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_parent = data_x_y["parent"].duplicate();
                    Array nbr = data_x_y["neighbors"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_neighbors.clear();
                    for (int i = 0; i < nbr.size(); i++) {
                        Preprocessed_Map.write[x].write[y].Node_neighbors.append(nbr[i].duplicate());
                    }
                    Preprocessed_Map.write[x].write[y].Node_cost = data_x_y["cost"].duplicate();
                    Preprocessed_Map.write[x].write[y].Distance_to = data_x_y["distance_to"].duplicate();
                    Preprocessed_Map.write[x].write[y].Reachable = data_x_y["state"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_label = data_x_y["label"].duplicate();
                }
            }

            return;
        }
        // from .cfg file
        void Universal_2D_Pathfinder::load_from_file(String path_to_file) {
            
            file_manager = memnew(ConfigFile);
            Error err = file_manager->load(path_to_file);
            if (err != OK) {
                UtilityFunctions::print("Error loading ", path_to_file);
                return;
            }
            
            Map_tileset = *get_tile_set();
            TileSet::TileShape Map_shape = Map_tileset->get_tile_shape();
            String section;
            switch (Map_shape)
            {
                case 0:
                section = "square";
                break;
                case 1:
                section = "iso";
                break;
                case 3:
                section = "hex";
                break;
            }
            String name = get_name();

            Array data = file_manager->get_value(section, name);

            UtilityFunctions::print("data: ", data);

            int size_x = data.size();
            int size_y;
            if (size_x > 0) {
                Array d_y = data.front();
                size_y = d_y.size();
                if (size_y > 0) {
                    map_size.x = size_x;
                    map_size.y = size_y;
                }
            }
            map_initializer(0);

            for (int x = 0; x < size_x; x++) {
                for (int y = 0; y < size_y; y++) {
                    TypedArray<Dictionary> data_x = data.pop_front();
                    Dictionary data_x_y = data_x.pop_front();
                    Preprocessed_Map.write[x].write[y].Node_coordinates = data_x_y["coordinates"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_parent = data_x_y["parent"].duplicate();
                    Array nbr = data_x_y["neighbors"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_neighbors.clear();
                    for (int i = 0; i < nbr.size(); i++) {
                        Preprocessed_Map.write[x].write[y].Node_neighbors.append(nbr[i].duplicate());
                    }
                    Preprocessed_Map.write[x].write[y].Node_cost = data_x_y["cost"].duplicate();
                    Preprocessed_Map.write[x].write[y].Distance_to = data_x_y["distance_to"].duplicate();
                    Preprocessed_Map.write[x].write[y].Reachable = data_x_y["state"].duplicate();
                    Preprocessed_Map.write[x].write[y].Node_label = data_x_y["label"].duplicate();
                }
            }

            return;
        }

        // tileset .cfg
        void Universal_2D_Pathfinder::load_tileset_cfg(String path_to_file) {
            file_manager = memnew(ConfigFile);
            Error err = file_manager->load(path_to_file);
            if (err != OK) {
                UtilityFunctions::print("Error loading ", path_to_file);
                return;
            }
            Array sections = file_manager->get_sections();
            UtilityFunctions::print(sections);

            Array tile;
            tile_data.clear();
            for (int i = 0; i < sections.size(); i++) {
                if (sections[i] != "tileset_data") {
                    tile.clear();
                    String value = sections[i];
                    Array parts = value.split(",");
                    Vector2i tile_key = Vector2i(int(parts[0]),int(parts[1]));
                    String name = file_manager->get_value(sections[i],"tile_name");
                    tile.append(name);
                    int cost = file_manager->get_value(sections[i],"tile_cost");
                    tile.append(cost);
                    bool reach = file_manager->get_value(sections[i],"reachable_state");
                    tile.append(reach);
                    tile_data[tile_key] = tile.duplicate();
                }
            }
            UtilityFunctions::print(tile_data);



            return;
        }


// getters and setters

    // weight
    void Universal_2D_Pathfinder::set_weight(float w) {
        weight = w;
    }
    int Universal_2D_Pathfinder::get_weight() const {
        return weight;
    }

    // diagonal
    void Universal_2D_Pathfinder::set_diagonal(const bool diag) {
        diagonal = diag;
    }
    bool Universal_2D_Pathfinder::get_diagonal() const {
        return diagonal;
    }


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

/*
    // MAX_PATH_LENGTH
    void Universal_2D_Pathfinder::set_MAX_PATH_LENGTH(const int new_max_length) {
        MAX_PATH_LENGTH = new_max_length;
    }
    int Universal_2D_Pathfinder::get_MAX_PATH_LENGTH() const {
        return MAX_PATH_LENGTH;
    }

    */

    // OPEN_list
    void Universal_2D_Pathfinder::set_open_list(Array _open) {
        OPEN_list = _open;
    }
    Array Universal_2D_Pathfinder::get_open_list() {
        return OPEN_list;
    }

    // CLOSED_list
    void Universal_2D_Pathfinder::set_closed_list(Array _closed) {
        CLOSED_list = _closed;
    }
    Array Universal_2D_Pathfinder::get_closed_list() {
        return CLOSED_list;
    }


Universal_2D_Pathfinder::Node_Data::Node_Data() {
    Node_neighbors = Vector<Vector2i>();
}