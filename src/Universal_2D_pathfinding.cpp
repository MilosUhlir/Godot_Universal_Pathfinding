
#include "Universal_2D_pathfinding.h"
#include <godot_cpp/core/class_db.hpp>


using namespace godot;


void Universal_2D_Pathinding::_bind_methods() {
}

Universal_2D_Pathinding::Universal_2D_Pathinding() {
    // initialization of variables
    
}


Universal_2D_Pathinding::~Universal_2D_Pathinding() {
    // cleanup
}

// Main methods









// getters and setters
    // MAX_PATH_LENGTH
    void Universal_2D_Pathinding::set_MAX_PATH_LENGTH(const int new_max_length) {
        MAX_PATH_LENGTH = new_max_length;
    }
    int Universal_2D_Pathinding::get_MAX_PATH_LENGTH() const {
        return MAX_PATH_LENGTH;
    }

    //
