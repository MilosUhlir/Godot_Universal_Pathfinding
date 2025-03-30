
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
    
    // map initializer
    ClassDB::bind_method(D_METHOD("map_initializer"), &Universal_2D_Pathfinder::map_initializer);
    ClassDB::bind_method(D_METHOD("set_button"), &Universal_2D_Pathfinder::set_button);
    ClassDB::bind_method(D_METHOD("get_button"), &Universal_2D_Pathfinder::get_button);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug button"), "set_button", "get_button");

    // storage array
    // ClassDB::bind_method(D_METHOD("set_storage_array"), &Universal_2D_Pathfinder::set_storage_array);
    // ClassDB::bind_method(D_METHOD("get_storage_array"), &Universal_2D_Pathfinder::get_storage_array);
    // ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "storage array"), "set_storage_array", "get_storage_array");

    ClassDB::bind_method(D_METHOD("set_diagonal"), &Universal_2D_Pathfinder::set_diagonal);
    ClassDB::bind_method(D_METHOD("get_diagonal"), &Universal_2D_Pathfinder::get_diagonal);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "diagonal movement"), "set_diagonal", "get_diagonal");

    ClassDB::bind_method(D_METHOD("set_weight"), &Universal_2D_Pathfinder::set_weight);
    ClassDB::bind_method(D_METHOD("get_weight"), &Universal_2D_Pathfinder::get_weight);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Weight"), "set_weight", "get_weight");
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
    UtilityFunctions::print("tile_data: ", tile_data);
    

    // map_size = Vector2i(2,2);

    // Universal_2D_Pathfinder::map_initializer();

    // UtilityFunctions::print("map size: ", Preprocessed_Map.size(),"x",Preprocessed_Map[0].size());
    
}


Universal_2D_Pathfinder::~Universal_2D_Pathfinder() {
    // cleanup
    if (Engine::get_singleton()->is_editor_hint()) {

        UtilityFunctions::print("Clean up message");
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


// void Universal_2D_Pathfinder::_process(double delta) {
//     time_passed += delta;
//     Vector2 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)));
//     set_position(new_position);

// }





// Main methods
    void Universal_2D_Pathfinder::init() {

        button = false;

        Algorithm = ASTAR;
        Heuristic = EUCLID;

        storage["coordinates"] = Vector2i(0,0);
        storage["parent"] = Vector2i(0,0);
        storage["neighbors"] = Array{};
        storage["cost"] = 0;
        storage["state"] = 0;
        storage["label"] = 0.0;
        return;
    }


    // Pathfinder
    Array Universal_2D_Pathfinder::Pathfinder(Array Start_points_array, Array End_points_array, const bool debug) {
        if (Preprocessed_Map.size() < 1) {
            map_initializer(0);
        } else if (Preprocessed_Map[0].size() < 1) {
            map_initializer(0);
        }
        Paths = Array();
        Path = Array();
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
        return Paths;
    }


    // Algorithms
        // A*
        Array Universal_2D_Pathfinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {
            // put first node into OPEN, CLOSED is empty
            int start_x = start_node.x;
            int start_y = start_node.y;
            // Vector<Node_Data> temp;

            // const Node_Data* first_node;
            // first_node = &Preprocessed_Map[start_x][start_y];
            OPEN_list.append(start_node);
            UtilityFunctions::print("OPEN_list: ", OPEN_list);

            if (OPEN_list.is_empty()) {
                return Array();
            };
            Label_Calculator(OPEN_list[0], end_node, true);
            // UtilityFunctions::print("Node_coordinates: ", Preprocessed_Map[start_x][start_y].Node_coordinates);
            // UtilityFunctions::print("Node_cost: ", Preprocessed_Map[start_x][start_y].Node_cost);
            // UtilityFunctions::print("Node_label: ", Preprocessed_Map[start_x][start_y].Node_label);
            // UtilityFunctions::print("Node_parent: ", Preprocessed_Map[start_x][start_y].Node_parent);
            // UtilityFunctions::print("Reachable: ", Preprocessed_Map[start_x][start_y].Reachable);
            bool exit_flag = false;
            Vector2i previous_node;
            Vector2i current_node;
            Vector2i min;
            while (exit_flag == false) {
                // if OPEN list is empty there is no solution -> terminate search
                if (OPEN_list.is_empty()) {
                    return Array();
                };

                // pull node with the smallest f(i) value in OPEN, if multiple check if any is end node
                Array mins = find_minimum_label(OPEN_list);
                if (mins.size() > 1) {
                    for (int i = 0; i < mins.size(); i++) {
                        if (mins[i] == end_node) {
                            // exit_flag = true;
                            min = mins[i];
                            break;
                        } else {
                            min = mins[0];
                        }
                    }
                } else if (mins.size() == 1) {
                    min = mins[0];
                } else {
                    break;
                }
                int idx = OPEN_list.find(min, 0);
                current_node = OPEN_list.pop_at(idx);
                UtilityFunctions::print("current_node: ", current_node, " end_node: ", end_node);
                if (current_node == end_node) {
                    break;
                }

                CLOSED_list.append(current_node);

                // expanding neighbors
                for (int i = 0; i < Preprocessed_Map[current_node.x][current_node.y].Node_neighbors.size(); i++) {
                    Vector2i curr_nbr = Preprocessed_Map[current_node.x][current_node.y].Node_neighbors[i];
                    bool open_has = OPEN_list.has(curr_nbr);
                    bool closed_has = CLOSED_list.has(curr_nbr);
                    if (open_has == true || closed_has == true) {
                        double f_n = Label_Calculator(curr_nbr, end_node, false);
                        if (f_n < Preprocessed_Map[curr_nbr.x][curr_nbr.y].Node_label) {
                            Label_Calculator(curr_nbr, end_node, true);
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                            if (closed_has && !open_has) {
                                OPEN_list.append(CLOSED_list.pop_at(CLOSED_list.find(curr_nbr, 0)));
                            }
                        }
                    } else {
                        OPEN_list.append(curr_nbr);
                    }
                }

                if (OPEN_list.is_empty()) {
                    return Array();
                };

                if (exit_flag == true) {
                    break;
                }
            }

            current_node = end_node;
            Path.append(current_node);
            while (!(current_node == start_node)) {
                current_node = Preprocessed_Map[current_node.x][current_node.y].Node_parent;
                Path.append(current_node);
                if (Path.size() >= MAX_PATH_LENGTH) {
                    break;
                }
            }
            


            UtilityFunctions::print("A* finished");
            UtilityFunctions::print("OPEN_list: ", OPEN_list);
            UtilityFunctions::print("CLOSED_list: ", CLOSED_list);
            Array placeholder;
            placeholder.append(Vector2i(0,0));
            // return placeholder;
            return Path;
        }

        
        // Dijkstra
        Array Universal_2D_Pathfinder::Disjkstra_Pathfinder(Vector2i start_node, Vector2i end_node) {
            return Array{};
        }


        // Dynamic Programming
        Array Universal_2D_Pathfinder::DP_Pathfinder(Vector2i start_node, Vector2i end_node) {
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
                        if (y%2 == 0) {
                            SearchVector = hex_search_array_y_even;
                        } else {
                            SearchVector = hex_search_array_y_odd;
                        }
                    }
                    for (int i = 0; i < neighbors.size(); i++) {
                        Vector2i n = neighbors[i];
                        if (!(n.x < 0) && !(n.y < 0) && !(n.x > map_size.x-1) && !(n.y > map_size.y-1))
                            if (Preprocessed_Map[n.x][n.y].Reachable == true && Preprocessed_Map[x][y].Reachable == true) {
                                Preprocessed_Map.write[x].write[y].Node_neighbors.append(neighbors[i]);
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
        double Universal_2D_Pathfinder::Label_Calculator(Vector2i _node, Vector2i end, bool overwrite) {
            Vector2i coords = Preprocessed_Map[_node.x][_node.y].Node_coordinates;
            int cost = Preprocessed_Map[_node.x][_node.y].Node_cost;
            Vector2i parent = Preprocessed_Map[_node.x][_node.y].Node_parent;
            float parent_distance = Preprocessed_Map[parent.x][parent.y].Distance_to;
            double h;
            int x = abs(coords.x - end.x);
            int y = abs(coords.y - end.y);
            double x_pow = pow(x, 2);;
            double y_pow = pow(y, 2);;
            // UtilityFunctions::print("this is heuristic test of x^2\n", x, "^2 = ", pow(x, 2));
            // UtilityFunctions::print("this is heuristic test of y^2\n", y, "^2 = ", pow(y, 2));
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
            UtilityFunctions::print("heur: ", Heuristic, " -> h = ", h);

            if (overwrite) {
                Preprocessed_Map.write[_node.x].write[_node.y].Distance_to = parent_distance + cost;
                Preprocessed_Map.write[_node.x].write[_node.y].Node_label = parent_distance + cost + h;
                // UtilityFunctions::print("Node_coordinates: ", Preprocessed_Map[_node.x][_node.y].Node_coordinates);
                // UtilityFunctions::print("Node_cost: ", Preprocessed_Map[_node.x][_node.y].Node_cost);
                // UtilityFunctions::print("Node_label: ", Preprocessed_Map[_node.x][_node.y].Node_label);
                // UtilityFunctions::print("Node_parent: ", Preprocessed_Map[_node.x][_node.y].Node_parent);
                // UtilityFunctions::print("Reachable: ", Preprocessed_Map[_node.x][_node.y].Reachable);
                return 0.0;
            } else {
                double f_n;
                f_n = parent_distance + cost + h;
                return f_n;
            }
        }


        // neighbor search


        // Map initializer
        void Universal_2D_Pathfinder::set_button(bool but) {
            // if (button == true) {button = false;}
            // if (button == false) {button = true;}
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
            }
        }



        // find minimal label
        Array Universal_2D_Pathfinder::find_minimum_label(Array& open_list) {
            Array minimums = Array();
            float min = 1.7e100;
            for (int i = 0; i < open_list.size(); i++) {
                Vector2i coords = open_list[i];
                float l =  Preprocessed_Map[coords.x][coords.y].Node_label;
                (l < min) ? min = l : min;
            }
            for (int i = 0; i < open_list.size(); i++) {
                Vector2i coords = open_list[i];
                float l =  Preprocessed_Map[coords.x][coords.y].Node_label;
                if (l - min < 1e-6) {
                    minimums.append(open_list[i]);
                }
            }

            // Array placeholder;

            return minimums;
        };

            




    // File management
        // map savers
        // to godot
        Array Universal_2D_Pathfinder::save_to_code() {
            map_initializer(1);
            return Array{};
        }
        // to .cfg file
        void Universal_2D_Pathfinder::save_to_file(String path_to_file) {
            map_initializer(1);
            return;
        }

        // map loaders
        // from godot
        void Universal_2D_Pathfinder::load_from_code(Array data) {
            map_initializer(0);
            return;
        }
        // from .cfg file
        void Universal_2D_Pathfinder::load_from_file(String path_to_file) {
            map_initializer(0);
            return;
        }

        // tileset .cfg
        void Universal_2D_Pathfinder::load_tileset_cfg(String path_to_file) {
            
            
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


    // storage array
    // void Universal_2D_Pathfinder::set_storage_array(Array sta) {
    //     storage_array = sta;
    // }
    // Array Universal_2D_Pathfinder::get_storage_array() {
    //     return storage_array;
    // }

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



Universal_2D_Pathfinder::Node_Data::Node_Data() {
    Node_neighbors = Array();
}