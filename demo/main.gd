extends Node2D

@onready var pathfinder = $Universal_2D_Pathfinder
@onready var target_marker = $target_marker
@onready var agent = $agent
@onready var tileset_sqr:TileSet = preload("res://tile_sets/set/square_tile_set.tres")
@onready var tileset_hex:TileSet = preload("res://tile_sets/set/hex_tile_set.tres")
@onready var tileset_iso:TileSet = preload("res://tile_sets/set/iso_tile_set.tres")

@onready var green_tile = $CanvasLayer/UI/Tile_placement_UI/GridContainer/green_tile
@onready var blue_tile = $CanvasLayer/UI/Tile_placement_UI/GridContainer/blue_tile
@onready var red_tile = $CanvasLayer/UI/Tile_placement_UI/GridContainer/red_tile

# Variables to track FPS
var current_fps:float = 0
var min_fps:float = INF
var max_fps:float = 0

var Paths: Array
var Open: Array
var Closed: Array
var tile_placement_mode:bool = false
var tile: Vector2i

enum tile_type {green, blue, red, none}

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


func _process(delta: float) -> void:
	# Get current FPS
	current_fps = Engine.get_frames_per_second()
	$CanvasLayer/UI/stat_display/FPS.text = str(current_fps) + " FPS"


func _unhandled_input(event: InputEvent) -> void:
	if tile_placement_mode == false:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
			pathfinder.map_initializer(0);
			pathfinder.Preprocessor()
			var mouse_pos = pathfinder.local_to_map(get_local_mouse_position())
			target_marker.set_position(pathfinder.map_to_local(mouse_pos))
			print(mouse_pos)
			var time_start = Time.get_ticks_usec()
			var starts = [pathfinder.local_to_map(agent.position)]
			var ends = [mouse_pos]
			Paths.clear()
			Paths = pathfinder.Pathfinder(starts, ends, false)
			if Paths.size() > 0:
				$Drawing_node.Paths = Paths
				var path = Paths[0]
				var time_end = Time.get_ticks_usec()
				var time_delta = time_end-time_start
				var unit: String
				if time_delta > 1000:
					unit = " ms"
				else:
					unit = " µs"
				agent.path = path
				$CanvasLayer/UI/stat_display/Pathfinder_time.text = "last path search took " + str(time_delta) + unit
				$CanvasLayer/UI/stat_display/Path_length.text = "last path length is " + str(path.size()) + " tiles"
			$Drawing_node.queue_redraw()
		elif event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_RIGHT and event.is_released():
			var mouse_pos = pathfinder.local_to_map(get_local_mouse_position())
			agent.velocity = Vector2(0,0)
			agent.current_tile = pathfinder.map_to_local(mouse_pos)
			agent.position = pathfinder.map_to_local(mouse_pos)
			agent._ready()
	else:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
			var mouse_pos:Vector2i = pathfinder.local_to_map(get_local_mouse_position())
			if tile != Vector2i(-1,-1):
				pathfinder.set_cell(mouse_pos, 0, tile)
				pathfinder.map_initializer(0)


func Save_data_to_file() -> void:
	
	print("saving...")
	
	pathfinder.save_to_file("", "map_1")
	print("data saved")
	pass


func tile_placer_mode_switch() -> void:
	tile_placement_mode = !tile_placement_mode
	$CanvasLayer/UI/Main_UI.visible = !$CanvasLayer/UI/Main_UI.visible
	$CanvasLayer/UI/Tile_placement_UI.visible = !$CanvasLayer/UI/Tile_placement_UI.visible
	pass
	

func switch_map_layout() -> void:
	var selected_layout:int = $CanvasLayer/UI/Main_UI/map_type.selected
	match selected_layout:
		0: pathfinder.tile_set = tileset_sqr
		1: pathfinder.tile_set = tileset_hex
		2: pathfinder.tile_set = tileset_iso


func UI_visibility() -> void:
	$CanvasLayer/UI.visible = !($CanvasLayer/UI.visible)
	#pathfinder.load_tileset_cfg("res://tile_sets/config/square_config.cfg")
	#pathfinder.load_from_file("user://map_1.cfg")
	pass


func _input(event: InputEvent) -> void:
	if event.is_action_released("UI_hide_show"):
		UI_visibility()


func _on_green_tile_pressed() -> void:
	if green_tile.button_pressed == true:
		tile = Vector2i(1,0)
		blue_tile.button_pressed = false
		red_tile.button_pressed = false
	else:
		tile = Vector2i(-1,-1)
	pass # Replace with function body.


func _on_blue_tile_pressed() -> void:
	if blue_tile.button_pressed == true:
		tile = Vector2i(2,0)
		green_tile.button_pressed = false
		red_tile.button_pressed = false
	else:
		tile = Vector2i(-1,-1)
	pass # Replace with function body.


func _on_red_tile_pressed() -> void:
	if red_tile.button_pressed == true:
		tile = Vector2i(0,0)
		blue_tile.button_pressed = false
		green_tile.button_pressed = false
	else:
		tile = Vector2i(-1,-1)
	pass # Replace with function body.


func _on_load_tile_cfg_pressed() -> void:
	pathfinder.load_tileset_cfg("res://tile_sets/config/square_config.cfg")
