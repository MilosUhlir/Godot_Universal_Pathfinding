extends Node2D

@onready var pathfinder = $Universal_2D_Pathfinder
@onready var target_marker = $target_marker
@onready var agent = $agent
@onready var tileset_sqr:TileSet = preload("res://tile_sets/set/square_tile_set.tres")
@onready var tileset_hex:TileSet = preload("res://tile_sets/set/hex_tile_set.tres")
@onready var tileset_iso:TileSet = preload("res://tile_sets/set/iso_tile_set.tres")

@onready var green_tile = $UI/Tile_placement_UI/GridContainer/green_tile
@onready var blue_tile = $UI/Tile_placement_UI/GridContainer/blue_tile
@onready var red_tile = $UI/Tile_placement_UI/GridContainer/red_tile

# Variables to track FPS
var current_fps:float = 0
var min_fps:float = INF
var max_fps:float = 0

var Paths: Array
var tile_placement_mode:bool = false

enum tile_type {green, blue, red, none}

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
		# Get current FPS
	current_fps = Engine.get_frames_per_second()
	
	# Update min/max FPS
	min_fps = min(min_fps, current_fps)
	max_fps = max(max_fps, current_fps)
	$UI/stat_display/FPS.text = str(current_fps) + " FPS"


func _unhandled_input(event: InputEvent) -> void:
	if tile_placement_mode == false:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
			#Pathfinder([Vector2i(0,0)], [Vector2i(1,1)], true)
			var mouse_pos = pathfinder.local_to_map(get_local_mouse_position())
			target_marker.set_position(pathfinder.map_to_local(mouse_pos))
			print(mouse_pos)
			var time_start = Time.get_ticks_usec()
			var starts = [pathfinder.local_to_map(agent.position)]
			var ends = [mouse_pos]
			Paths.clear()
			Paths = pathfinder.Pathfinder(starts, ends, false)
			if Paths.size() > 0:
				var path = Paths[0]
				var time_end = Time.get_ticks_usec()
				var time_delta = time_end-time_start
				var unit: String
				if time_delta > 1000:
					unit = " ms"
				else:
					unit = " µs"
				print("Pathfinder finished in ", time_delta, unit) #" ms")
				print("Path length: ", path.size())
				print("Path: ", path)
				agent.path = path
				#storage_array = save_to_code()
				$UI/stat_display/Pathfinder_time.text = "last path search took " + str(time_delta) + unit
				$UI/stat_display/Path_length.text = "last path length is " + str(path.size()) + " tiles"
				
	else:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
			var mouse_pos:Vector2i = pathfinder.local_to_map(get_local_mouse_position())
			var tile: Vector2i
			match tile_type:
				0: tile = Vector2i(1,0)
				1: tile = Vector2i(2,0)
				2: tile = Vector2i(0,0)
				3: return
			
			if green_tile.toggled == true:
				tile = Vector2i(1,0)
			elif blue_tile.toggled == true:
				tile = Vector2i(2,0)
			elif red_tile.toggled == true:
				tile = Vector2i(0,0)
			
			#print("tile: ", tile)
			pathfinder.set_cell(mouse_pos, 0, tile_type)
			pass
		pass


func Save_data_to_file() -> void:
	
	print("saving...")
	
	pathfinder.save_to_file("", "map_1")
	pass


func tile_placer_mode_switch() -> void:
	tile_placement_mode = !tile_placement_mode
	$UI/Main_UI.visible = !$UI/Main_UI.visible
	$UI/Tile_placement_UI.visible = !$UI/Tile_placement_UI.visible
	pass
	

func switch_map_layout() -> void:
	var selected_layout:int = $UI/Main_UI/map_type.selected
	match selected_layout:
		0: pathfinder.tile_set = tileset_sqr
		1: pathfinder.tile_set = tileset_hex
		2: pathfinder.tile_set = tileset_iso


func UI_visibility() -> void:
	$UI.visible = !($UI.visible)
	#pathfinder.load_tileset_cfg("res://tile_sets/config/square_config.cfg")
	#pathfinder.load_from_file("user://map_1.cfg")
	pass


func _input(event: InputEvent) -> void:
	if event.is_action_released("UI_hide_show"):
		UI_visibility()


func _notification(what):
	if what == NOTIFICATION_PREDELETE:
		# Save FPS data when node is destroyed
		var config = ConfigFile.new()
		config.set_value("fps", "min", min_fps)
		config.set_value("fps", "max", max_fps)
		var err = config.save("user://fps_config.cfg")


func _on_green_tile_pressed() -> void:
	if green_tile.toggled == false:
		green_tile.toggled = true
		blue_tile.toggled = false
		red_tile.toggled = false
	else:
		green_tile.toggled = false
		blue_tile.toggled = true
		red_tile.toggled = true
	pass # Replace with function body.


func _on_blue_tile_pressed() -> void:
	if green_tile.toggled == false:
		green_tile.toggled = false
		blue_tile.toggled = true
		red_tile.toggled = false
	else:
		green_tile.toggled = true
		blue_tile.toggled = false
		red_tile.toggled = true
	pass # Replace with function body.


func _on_red_tile_pressed() -> void:
	if green_tile.toggled == false:
		green_tile.toggled = false
		blue_tile.toggled = false
		red_tile.toggled = true
	else:
		green_tile.toggled = true
		blue_tile.toggled = true
		red_tile.toggled = false
	pass # Replace with function body.
