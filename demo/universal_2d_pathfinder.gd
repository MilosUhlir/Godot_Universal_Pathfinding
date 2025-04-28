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


func random_maze() -> void:
	var tile_amount: int = map_size.x * map_size.y
	for x in range(map_size.x):
		for y in range(map_size.y):
			set_cell(Vector2i(x,y), 0, Vector2i(1,0))
			var prob = randf()
			if prob > 0.80:
				set_cell(Vector2i(x,y), 0, Vector2i(0,0))
			pass
	
	
	
	pass
