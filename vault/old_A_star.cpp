Array Universal_2D_Pathfinder::AStar_Pathfinder(Vector2i start_node, Vector2i end_node) {
            DP_prev_used = false;
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

            // Label_Calculator(OPEN_list[0], end_node, false, true);
            Vector2i ct = OPEN_list[0];
            Array labels = Label_Calculator(ct, end_node, false);
            Preprocessed_Map.write[start_node.x].write[start_node.y].Distance_to = labels[0];
            Preprocessed_Map.write[start_node.x].write[start_node.y].Node_label = labels[1];
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
                        if (f_n < Preprocessed_Map[curr_nbr.x][curr_nbr.y].Distance_to) {
                            // Label_Calculator(curr_nbr, end_node, true);
                            f_n = Label_Calculator(curr_nbr, end_node, diag, true);
                            // Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_label = f_n;
                            Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_parent = current_node;
                            if (closed_has && !open_has) {
                                OPEN_list.append(CLOSED_list.pop_at(CLOSED_list.find(curr_nbr, 0)));
                            }
                        }
                    } else if (open_has == false && closed_has == false) {
                    //     double f_n = Label_Calculator(curr_nbr, end_node, true);
                        f_n = Label_Calculator(curr_nbr, end_node, diag, true);
                        // Preprocessed_Map.write[curr_nbr.x].write[curr_nbr.y].Node_label = f_n;
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