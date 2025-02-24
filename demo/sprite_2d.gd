@tool
extends Sprite2D



@export var test3 = PackedVector2Array()

@onready var pathfinder =  $"../Universal_2D_Pathfinder"
var time_passed
var test = [[String()]]


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	print(position)
	time_passed = 0.0
	
	test = pathfinder.Pathfinder([Vector2i(1,1), Vector2i(2,2)], [Vector2i(5,5), Vector2i(1,1), Vector2i(2,2)], false)
	print("sprite script print")
	print(test, "\n \n")
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	time_passed += delta
	#rotation += PI * delta
	position = Vector2(50,20) + Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)))
	#set_position(new_position)
	#var test5 = pathfinder.get_Preprocessed_Map()
	#print(test5[0][0].Node_neighbors)
	

func _exit_tree() -> void:
	test = pathfinder.Pathfinder([Vector2i(1,1), Vector2i(2,2)], [Vector2i(5,5), Vector2i(1,1), Vector2i(2,2)], true)
	print("test: ", test)
	
