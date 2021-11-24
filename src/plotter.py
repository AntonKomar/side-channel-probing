import matplotlib.pyplot as plt
import csv
import sys

labels=['number[1]','number[5]','number[7]','placenum']
cols = [[],[],[],[]]

with open('measurements.csv', 'r') as file:
    content = file.readlines()
    x = range(0, len(content))

    for line in content:
        s = line.split(",")
        for i in range(0, 4):
            cols[i].append(int(s[i]))

# create plot
plt.figure(figsize=(20,8))

for i in range(0, 4):
    plt.plot(x, cols[i], linewidth=0.5, label=labels[i])

plt.xlabel('iteration counter')
plt.ylabel('measured access time')
plt.ylim(ymax = 800, ymin=0)
plt.xlim(xmin = 0)
plt.legend()
plt.savefig('../results/graph' + (sys.argv[1] if len(sys.argv) > 1 else '')+ '.jpg')
plt.show()
