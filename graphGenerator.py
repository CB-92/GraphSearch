import sys
import numpy as np
# generate random integer values
from random import seed
from random import randint

print(sys.argv)

if len(sys.argv)<5:
    print("Usage: python graphGenerator.py node_num edge_num max_value output_file")
    exit

node_num = int(sys.argv[1])
edge_num = int(sys.argv[2])
max_value = int(sys.argv[3])
filename = sys.argv[4]

file = open(filename, 'w+')
# seed random number generator
seed(1)

for id in range(node_num):
    val = randint(0, max_value)
    file.write(str(id) + ' ' + str(val) + '\n')

file.write('#\n')

i=0

while (node_num-i)>edge_num:
    file.write(str(i) + ' ')
    a = np.random.choice(range(i, node_num), edge_num, replace=False)
    #print(a)
    for x in a:
        file.write( str(x)+',')
    file.write('\n')
    i+=1
