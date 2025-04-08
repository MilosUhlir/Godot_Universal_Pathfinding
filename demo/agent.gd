extends CharacterBody2D

@onready var map = $"../Universal_2D_Pathfinder"

var path: Array

var speed = 500

var current_tile: Vector2
var next_tile:Vector2
var epsilon = 5
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	path.clear()
	position = map.map_to_local(map.local_to_map(position))
	current_tile = position
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#print("current_tile: ", map.local_to_map(current_tile))
	#print("agent\'s path: ", path)
	var pos_delta = (current_tile - position)
	var pos_base_vect = pos_delta.normalized()
	if not path.is_empty():
		if pos_delta.length() < epsilon:
			#velocity = Vector2.ZERO
			next_tile = map.map_to_local(path.pop_at(-1))
			#print("next_tile: ", next_tile)
			current_tile = next_tile
		else:
			velocity = pos_base_vect * speed
	else:
		if pos_delta.length() < epsilon:
			velocity = Vector2.ZERO
		else:
			velocity = pos_base_vect * speed
			
			
			
	move_and_slide()
	pass
