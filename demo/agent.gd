extends CharacterBody2D

@onready var map = $"../Universal_2D_Pathfinder"

var path: Array

var speed = 10

var current_tile: Vector2
var next_tile:Vector2
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
	if not path.is_empty():
		if position.is_equal_approx(current_tile):
			#velocity = Vector2.ZERO
			next_tile = map.map_to_local(path.pop_at(-1))
			#print("next_tile: ", next_tile)
			current_tile = next_tile
		else:
			velocity = (current_tile - position) * speed
	else:
		if position.is_equal_approx(current_tile):
			velocity = Vector2.ZERO
		else:
			velocity = (current_tile - position) * speed
			
			
			
	move_and_slide()
	pass
