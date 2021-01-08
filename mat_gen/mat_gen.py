import sys
import random

if __name__ == '__main__':
	height = int(sys.argv[1])
	width = int(sys.argv[2])

	f = open('matrix.txt', 'w')

	for i in range(height):
		for j in range(width):
			if j < width-1:
				f.write("%d " % random.randrange(0, 10))
			else:
				f.write("%d\n" % random.randrange(0, 10))

	f.close()
