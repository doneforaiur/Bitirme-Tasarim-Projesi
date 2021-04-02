import matplotlib.pyplot as plt

while True:
	y = []
	
	file = open('fourier.txt', 'r')
	lines = file.readlines()
	
	for line in lines:
		y.append(float(line))
	x = range(0,len(y))
	
	plt.plot(x, y)
	plt.draw()
	plt.pause(0.001)
	plt.clf()
