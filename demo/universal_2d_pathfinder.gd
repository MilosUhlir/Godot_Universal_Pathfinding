#@tool
extends Universal_2D_Pathfinder

@onready var target_marker = $"../target_marker"
@onready var agent = $"../agent"
var timer = Time
var Paths: Array

var storage_array: Array

func _ready() -> void:
	pass


func _process(_delta: float) -> void:
	pass


#func _unhandled_input(event: InputEvent) -> void:
	#
	#if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
		##Pathfinder([Vector2i(0,0)], [Vector2i(1,1)], true)
		#var mouse_pos = local_to_map(get_local_mouse_position())
		#target_marker.set_position(map_to_local(mouse_pos))
		#print(mouse_pos)
		#var time_start = Time.get_ticks_usec()
		#var starts = [local_to_map(agent.position)]
		#var ends = [mouse_pos]
		#Paths = Pathfinder(starts, ends, false)
		#if Paths.size() > 0:
			#var path = Paths[0]
			#var time_end = Time.get_ticks_usec()
			#var time_delta = time_end-time_start
			#var unit: String
			#if time_delta > 1000:
				#unit = " ms"
			#else:
				#unit = " µs"
			#print("Pathfinder finished in ", time_delta, unit) #" ms")
			#print("Path length: ", path.size())
			##print("Path: ", Paths[0])
			#agent.path = path
			##storage_array = save_to_code()
		#
	#var test :Dictionary
	#test = OS.get_memory_info()
	#test
	#pass
