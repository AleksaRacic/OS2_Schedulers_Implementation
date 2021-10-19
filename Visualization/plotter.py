import numpy as np
import matplotlib.pyplot as plt

FCFS = open(r"../Logs/PRF.txt", "r")
Jobs = {}

def inside(a, times):
    for b in times:
        start = int(b.split('-')[0])
        end = int(b.split('-')[1])
        if a <= end and a >= start:
            return True

    return False

def make_y(value, rang, times):
    y = np.zeros(rang, dtype=int)
    for i in range(rang):
        if inside(i, times):
            y[i] = value
    return y


for line in FCFS:
    data = line.strip().split(' ')
    if data[0] not in Jobs:
        Jobs[data[0]] = []
    Jobs[data[0]].append(data[1])
Jobs_plot = {}
end = 0
for key in Jobs.keys():
    times = Jobs[key]
    tmp = int(times[-1].split('-')[1])
    if end < tmp:
        end = tmp
x = np.arange(0, end,1)
y1 = make_y(1, end, Jobs['M:'])
y2 = make_y(1, end, Jobs['SJ:'])
y3 = make_y(1, end, Jobs['LJ:'])

plt.plot(x, y1, 'r') # plotting t, a separately
plt.plot(x, y2, 'b') # plotting t, b separately
plt.plot(x, y3, 'g') # plotting t, c separately
plt.show()

