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

@onready var Drawing_node = $Drawing_node

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
	pathfinder.map_initializer(0)
	pathfinder.Preprocessor()
	pass


func _process(delta: float) -> void:
	# Get current FPS
	current_fps = Engine.get_frames_per_second()
	$CanvasLayer/UI/stat_display/FPS.text = str(current_fps) + " FPS"


func _unhandled_input(event: InputEvent) -> void:
	if tile_placement_mode == false:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
			#pathfinder.map_initializer(0);
			#pathfinder.Preprocessor()
			var mouse_pos = pathfinder.local_to_map(get_local_mouse_position())
			target_marker.set_position(pathfinder.map_to_local(mouse_pos))
			print(mouse_pos)
			
			var starts: Array
			var ends: Array
			if pathfinder.Algorithm == pathfinder.DYNAMIC_PROG:
				Drawing_node.Open.clear()
				Drawing_node.Closed.clear()
				var target = Vector2i(pathfinder.map_size.x-2,pathfinder.map_size.y-2)
				pathfinder.set_cell(target, 0, Vector2i(1,0))
				starts = [mouse_pos]
				ends = [target]
				agent.velocity = Vector2(0,0)
				agent.current_tile = pathfinder.map_to_local(mouse_pos)
				agent.position = pathfinder.map_to_local(mouse_pos)
				agent._ready()
			else:
				starts = [pathfinder.local_to_map(agent.position)]
				ends = [mouse_pos]
			Paths.clear()
			var time_start: int
			var time_end: int
			if pathfinder.Algorithm == pathfinder.DYNAMIC_PROG:
				if FileAccess.file_exists("user://DP_data"):
					pathfinder.load_from_file("user://DP_data")
				else:
					time_start = Time.get_ticks_usec()
					Paths = pathfinder.Pathfinder(starts, ends, false)
					time_end = Time.get_ticks_usec()
					for p in range(0, Paths.size()):
						var pth: Array = Paths[p]
						pth.reverse()
					pathfinder.save_to_file("", "DP_data")
			else:
				time_start = Time.get_ticks_usec()
				Paths = pathfinder.Pathfinder(starts, ends, false)
				time_end = Time.get_ticks_usec()
			if Paths.size() > 0:
				Drawing_node.Paths = Paths
				var path = Paths[0]
				var time_delta: float
				time_delta = time_end-time_start
				var unit: String
				if time_delta < 1000:
					unit = " µs"
				elif time_delta >= 1000 and time_delta < 1000000:
					time_delta = time_delta/1000
					unit = " ms"
				else:
					time_delta = time_delta/1000000
					unit = "s"
				agent.path = path
				$CanvasLayer/UI/stat_display/Pathfinder_time.text = "last path search took " + str(time_delta) + unit
				$CanvasLayer/UI/stat_display/Path_length.text = "last path length is " + str(path.size()) + " tiles"
			#Drawing_node.Open.clear()
			#Drawing_node.Closed.clear()
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
	pathfinder.map_initializer(0)
	pathfinder.Preprocessor()
	

func switch_map_layout() -> void:
	var selected_layout:int = $CanvasLayer/UI/Tile_placement_UI/map_type.selected
	match selected_layout:
		0: pathfinder.tile_set = tileset_sqr
		1: pathfinder.tile_set = tileset_hex
		2: pathfinder.tile_set = tileset_iso
	pathfinder.map_initializer(0)
	pathfinder.Preprocessor()


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


func _on_algorithm_item_selected(index: int) -> void:
	pathfinder.Algorithm = index


func _on_heuristic_item_selected(index: int) -> void:
	pathfinder.Heuristic = index


func _on_diagonal_movement_toggled(toggled_on: bool) -> void:
	pathfinder.diagonal_movement = toggled_on
	pathfinder.map_initializer(2)
	pathfinder.Preprocessor()


func _on_clear_map_pressed() -> void:
	for x in range(0, pathfinder.map_size.x):
		for y in range(0, pathfinder.map_size.y):
			pathfinder.set_cell(Vector2i(x,y), 0, Vector2i(1,0))
	pass # Replace with function body.


func _on_debug_pressed() -> void:
	var runs:int = 1000
	var maps:Array = [
		Vector2i(20,10),
		Vector2i(50,25), 
		Vector2i(100,50), 
		Vector2i(200,100), 
		Vector2i(500,250)
		]
	var shapes:Array = [
		"sqr", 
		#"iso"
		]
	
	var godot_astar = AStarGrid2D.new()
	godot_astar.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	
	pathfinder.map_size = Vector2i(500, 250)
	seed(221208)
	pathfinder.random_maze()
	
	godot_astar.region = Rect2i(0, 0, 499, 249)
	godot_astar.update()
	
	for x in range(0, maps[-1].x-1):
		for y in range(0, maps[-1].y-1):
			if pathfinder.get_cell_atlas_coords(Vector2i(x,y)) == Vector2i(0,0):
				godot_astar.set_point_solid(Vector2i(x,y), true)
			else:
				godot_astar.set_point_solid(Vector2i(x,y), false)
	
	
	var time_delta:int
	for map in maps:
		print("-------------------------------------------")
		print("map size: ", map)
		for shape in shapes:
			time_delta = 0
			var path: Array = Array()
			for r in range(0, runs):
				match shape:
					"sqr": 
						godot_astar.cell_shape = AStarGrid2D.CELL_SHAPE_SQUARE
						godot_astar.cell_size = Vector2i(64, 64)
					"iso": 
						godot_astar.cell_shape = AStarGrid2D.CELL_SHAPE_ISOMETRIC_RIGHT
						godot_astar.cell_size = Vector2i(128, 64)
						
				#godot_astar.region = Rect2i(0, 0, map.x, map.y)
				godot_astar.update()
				var TS = Time.get_ticks_usec()
				path = godot_astar.get_id_path(Vector2i(1,1), Vector2i(map.x-2,map.y-2))
				var TE = Time.get_ticks_usec()
				time_delta += TE-TS
			time_delta /= runs
			var time_str:String
			var unit: String
			if time_delta < 1000:
				unit = " µs"
			elif time_delta >= 1000 and time_delta < 1000000:
				time_delta = time_delta/1000
				unit = " ms"
			elif time_delta >= 1000000:
				time_delta = time_delta/1000000
				unit = " s"
			time_str = str(time_delta) + unit
			print("path on ", shape," map found in ", time_str, " on avg, length: ", path.size())
			#print("path: ", path)
			
			


func _on_run_tests_pressed() -> void:
	var runs:int = 100
	var test_start_time = Time.get_ticks_usec()
	var maps:Array = [
		Vector2i(20,10),
		Vector2i(50,25), 
		Vector2i(100,50), 
		Vector2i(200,100), 
		Vector2i(500,250)
		]
	
	var shapes:Array = [
		"sqr", 
		#"iso", 
		#"hex"
		]
	var algs:Array = [
		"A*", 
		#"Dijkstra", 
		"DP"
		]
		
		
	var test_results:ConfigFile = ConfigFile.new()
	var section:String
	
	var starts:Array
	var ends:Array
	
	var time:String
	
	pathfinder.diagonal_movement = false
	
	pathfinder.map_size = Vector2i(500, 250)
	seed(221208)
	pathfinder.random_maze()
	
	for shape:String in shapes:
		for alg:String in algs:
			#pathfinder.map_initializer(1)
			for map:Vector2i in maps:
				pathfinder.map_size = map
				pathfinder.map_initializer(0)
				pathfinder.Preprocessor()
				starts = [Vector2i(1,1)]
				ends = [Vector2i(map.x-2, map.y-2)]
				
				match alg:
					"A*":
						pathfinder.Algorithm = pathfinder.ASTAR
					"Dijkstra":
						pathfinder.Algorithm = pathfinder.DIJKSTRA
					"DP":
						pathfinder.Algorithm = pathfinder.DYNAMIC_PROG
						pathfinder.map_initializer(1)
				
				match shape:
					"sqr":pathfinder.tile_set = tileset_sqr
					"hex": pathfinder.tile_set = tileset_hex
					"iso": pathfinder.tile_set = tileset_iso
				
				
				var time_start: int
				var time_end: int
				var time1:int
				var time2:int
				var times:Array
				times.clear()
				if pathfinder.Algorithm == pathfinder.DYNAMIC_PROG:
					#if FileAccess.file_exists("user://DP_data"):
						#pathfinder.load_from_file("user://DP_data")
					#else:
					for r in range(0, runs):
						time_start = Time.get_ticks_usec()
						Paths = pathfinder.Pathfinder(starts, ends, false)
						time_end = Time.get_ticks_usec()
						time1 += time_end-time_start
						for p in range(0, Paths.size()):
							var pth: Array = Paths[p]
							pth.reverse()
						pathfinder.save_to_file("", "DP_data")
						time_start = Time.get_ticks_usec()
						Paths = pathfinder.Pathfinder(starts, ends, false)
						time_end = Time.get_ticks_usec()
						time2 += time_end-time_start
					times.append(time1/runs)
					times.append(time2/runs)
					
					var unit: String
					var time_strings: Array
					for t in times:
						var time_delta: float
						time_delta = t
						if time_delta < 1000:
							unit = " µs"
						elif time_delta >= 1000 and time_delta < 1000000:
							time_delta = time_delta/1000
							unit = " ms"
						elif time_delta >= 1000000:
							time_delta = time_delta/1000000
							unit = " s"
						time_strings.append(str(time_delta)+unit)
					time = time_strings[0]+" / "+time_strings[1]
					
				else:
					for r in range(0, runs):
						time_start = Time.get_ticks_usec()
						Paths = pathfinder.Pathfinder(starts, ends, false)
						time_end = Time.get_ticks_usec()
					
					var time_delta: float
					time_delta = time_end-time_start
					var unit: String
					if time_delta < 1000:
						unit = " µs"
					elif time_delta >= 1000 and time_delta < 1000000:
						time_delta = time_delta/1000
						unit = " ms"
					elif time_delta >= 1000000:
						time_delta = time_delta/1000000
						unit = " s"
					time = str(time_delta) + unit
				
				section = shape+", "+alg+", "+str(map)
				
				var start_end:String = str(starts[0])+" / "+str(ends[0])
				test_results.set_value(section, "start/end", start_end)
				
				test_results.set_value(section, "time", time)
				
				var path1:Array = Paths[0]
				test_results.set_value(section, "path length", path1.size())
				
				test_results.save("user://test_results_A.cfg")

				
				var test_end_time = Time.get_ticks_usec()
				var time_delta: float
				time_delta = test_end_time-test_start_time
				var unit: String
				if time_delta < 1000:
					unit = " µs"
				elif time_delta >= 1000 and time_delta < 1000000:
					time_delta = time_delta/1000
					unit = " ms"
				elif time_delta >= 1000000:
					time_delta = time_delta/1000000
					unit = " s"
				time = str(time_delta) + unit
				$CanvasLayer/UI/stat_display/Pathfinder_time.text = "testování proběhlo za "+time+" µs"
				pass
