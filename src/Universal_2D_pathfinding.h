#ifndef UNIVERSAL_PATHINDER_H
#define UNIVERSAL_PATHINDER_H

// Godot includes
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>

// my includes
#include <vector>


#define MAX_PATH_LENGTH 10000


namespace godot {

class Universal_2D_Pathinding : public Node {
	GDCLASS(Universal_2D_Pathinding, Node)

private:
	std::vector<std::pair<int, int>> Path;

    

    TileMapLayer Tilemap;

protected:
	static void _bind_methods();

public:
	Universal_2D_Pathinding();
	~Universal_2D_Pathinding();

	//void _process(double delta) override;
};

}

#endif