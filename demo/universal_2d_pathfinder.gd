#@tool
extends Universal_2D_Pathfinder

@onready var target_marker = $"../target_marker"
func _ready() -> void:
	pass


func _process(delta: float) -> void:
	pass


func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
		#Pathfinder([Vector2i(0,0)], [Vector2i(1,1)], true)
		var mouse_pos = local_to_map(get_local_mouse_position())
		target_marker.set_position(map_to_local(mouse_pos))
		print(mouse_pos)
	pass
