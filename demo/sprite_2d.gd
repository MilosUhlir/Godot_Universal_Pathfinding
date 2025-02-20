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
	# Replace with function body.
var test2 = float(INF)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	time_passed += delta
	#rotation += PI * delta
	position = Vector2(-200,-200) + Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)))
	#set_position(new_position)
	
	print(pathfinder.Pathfinder())
	
	test[0][0] = "test"
	#print(test)
