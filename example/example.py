import math

def read_route(filename):
	"""Returns a list of 2d points"""

	#Get route data
	file = open(filename,"rb");
	data = file.read()
	file.close()

	#Convert to signed bytes
	raw = list(data)
	for i,n in enumerate(raw):
		if n > 127:
			raw[i] = n-256

	#Get Points
	points = [[0 for i in range(2)] for j in range(len(raw)//2)]
	for i in range(len(raw)//2):
		points[i][0] = raw[2*i]
		points[i][1] = raw[2*i+1]

	return points

def write_route(filename, points):
	"""Returns a list of 2d points"""

	#Concatenate into one list
	raw = [0 for i in range(2*len(points))]
	for i,p in enumerate(points):
		raw[2*i] = p[0]
		raw[2*i+1] = p[1]

	#Convert to unsigned
	for i,n in enumerate(raw):
		if n < 0:
			raw[i] = n+256
	data = bytearray(raw)

	#Write to file
	file = open(filename,"wb");
	file.write(data)
	file.close()

def point_distance(p1, p2):
	"""Returns the distance between two points"""
	dx = p1[0]-p2[0]
	dy = p1[1]-p2[1]
	return math.sqrt(dx*dx+dy*dy)

def route_distance(points):
	"""Returns the distance of the entire tour"""
	total = 0
	for i in range(len(points)-1):
		total += point_distance(points[i],points[i+1])
	#Return path
	total += point_distance(points[len(points)-1],points[0])
	return total

def distance_matrix(points):
	"""Returns the distance matrix"""
	dm = [[0 for i in points] for j in points]
	for i,p1 in enumerate(points):
		for j,p2 in enumerate(points):
			dm[i][j] = point_distance(p1,p2)
	return dm

def optimize_route(points):
	"""Shortest Distance"""
	#Allocate space
	route = [[0 for i in range(2)] for j in points]
	valid = [True for i in points]

	#Get the distance matrix
	dm = distance_matrix(points)

	#Loop through all points
	pos = 0
	route[pos] = points[pos]
	valid[pos] = False
	for i in range(len(points)-1):

		#Find the next minimum distance
		min_dist = None
		min_pos = None
		for j in range(len(points)):
			if valid[j]:
				if min_dist is None:
					min_dist = dm[pos][j]
					min_pos = j
				elif dm[pos][j] < min_dist:
					min_dist = dm[pos][j]
					min_pos = j

		#Move to the next point
		pos = min_pos

		#Record the current position
		route[i+1] = points[pos]
		valid[pos] = False

	return route

def plot_route(points):
	#Plot results
	try:
		import matplotlib.pyplot as plt
	except:
		print("Unable to import matplotlib! No plotting allowed :(")
		return

	x = [points[i][0] for i in range(len(points))]
	y = [points[i][1] for i in range(len(points))]
	plt.scatter(x,y)
	plt.plot(x,y)
	plt.show()

if __name__ == "__main__":

	#Read file
	filename = "../routes/route25"
	points = read_route(filename)
	plot_route(points)

	#Show current distance
	old_distance = route_distance(points)
	print(f"Route {filename} is length {old_distance}")

	#Attempt to improve
	route = optimize_route(points)
	new_distance = route_distance(route)
	print(f"The new distance is {new_distance}")

	#Write file
	write_route("../results/example_route25",route)

	#Plot results
	plot_route(route)

