#@tool
extends Universal_2D_Pathfinder


func _ready() -> void:
	pass


func _process(delta: float) -> void:
	pass


func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		Pathfinder([Vector2i(0,0)], [Vector2i(2,2)], true)
	pass
