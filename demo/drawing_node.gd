extends Node2D

@onready var pathfinder = $"../Universal_2D_Pathfinder"

var Paths: Array
var Open: Array
var Closed: Array


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _draw() -> void:
	
	var line_width: int = 3
	var circle_radius: int = 8
	
	#Open.clear()
	Open = pathfinder.OPEN_list
	print("Drawing Open and Closed lists")
	if Open.is_empty() == false:
		for coord in Open:
			coord = pathfinder.map_to_local(coord)
			draw_circle(coord, circle_radius, Color.ORANGE_RED)

	#Closed.clear()
	Closed = pathfinder.CLOSED_list
	if Closed.is_empty() == false:
		for coord in Closed:
			coord = pathfinder.map_to_local(coord)
			draw_circle(coord, circle_radius, Color.BLUE)

		
	if Paths.is_empty() == false:
		for path in Paths:
			var prev_tile: Vector2
			var curr_tile: Vector2
			var i = 0
			for coord in path:
				coord = pathfinder.map_to_local(coord)
				curr_tile = coord
				draw_circle(coord, circle_radius, Color.YELLOW)
				if i != 0:
					draw_line(prev_tile, curr_tile, Color.YELLOW, line_width)
					pass
				
				prev_tile = curr_tile
				i += 1
	
