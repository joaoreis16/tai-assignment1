import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Load data from the text file
filename = 'results1.txt'

with open(filename, 'r') as file:
    lines = file.read().splitlines()

# Extract the header and values from the data
header = lines[0].strip().split()
data = [line.strip().split() for line in lines[1:] if line.strip()]

# Convert data values to appropriate types
data = [[float(value) for value in row] for row in data]

# Assign columns to variables
variables = {}  # Dictionary to store the variables

for i, column in enumerate(header):
    variables[column] = [row[i] for row in data]

alpha = variables['alpha']
threshold = variables['threshold']
k = variables['K']
bits = variables['bits']
fcmodel = variables['fcmodel']
time = variables['time']
result = variables['result']


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(len(alpha)):
    if result[i] == 1:
        ax.scatter(alpha[i], k[i], bits[i], c='green', marker='o')
    else:
        ax.scatter(alpha[i], k[i], bits[i], c='red', marker='o')

ax.set_xlabel('Alpha')
ax.set_ylabel('k')
ax.set_zlabel('Bits')
ax.set_title('Alpha vs k vs Bits')
plt.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(len(alpha)):
    if result[i] == 1:
        ax.scatter(alpha[i], k[i], time[i], c='green', marker='o')
    else:
        ax.scatter(alpha[i], k[i], time[i], c='red', marker='o')

ax.set_xlabel('Alpha')
ax.set_ylabel('k')
ax.set_zlabel('Execution Time')
ax.set_title('Alpha vs k vs Execution Time')
plt.show()


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(len(alpha)):
    if result[i] == 1:
        ax.scatter(alpha[i], threshold[i], bits[i], c='green', marker='o')
    else:
        ax.scatter(alpha[i], threshold[i], bits[i], c='red', marker='o')

ax.set_xlabel('Alpha')
ax.set_ylabel('Threshold')
ax.set_zlabel('Bits (s)')
ax.set_title('Alpha vs Threshold vs Bits')
plt.show()


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(len(alpha)):
    if result[i] == 1:
        ax.scatter(alpha[i], threshold[i], time[i], c='green', marker='o')
    else:
        ax.scatter(alpha[i], threshold[i], time[i], c='red', marker='o')

ax.set_xlabel('Alpha')
ax.set_ylabel('Threshold')
ax.set_zlabel('Execution Time (s)')
ax.set_title('Alpha vs Threshold vs Execution Time')
plt.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

for i in range(len(alpha)):
    if result[i] == 1:
        ax.scatter(bits[i], fcmodel[i], k[i], c='green', marker='o')
    else:
        ax.scatter(bits[i], fcmodel[i], k[i], c='red', marker='o')

ax.set_xlabel('Bits')
ax.set_ylabel('fcmodel')
ax.set_zlabel('k')
ax.set_title('Bits vs FC Model vs k')
plt.show()

"""

# Initialize lists to store execution times
execution_times_1 = []
execution_times_0 = []

# Collect execution times based on 'fcmodel' value
for i in range(len(alpha)):
    if result[i] == 1:
        execution_times_1.append(time[i])
    else:
        execution_times_0.append(time[i])

# Create a bar plot for execution times comparison
plt.figure()

plt.bar(['fcmodel=1', 'fcmodel=0'], [sum(execution_times_1), sum(execution_times_0)])

# Set axis labels
plt.xlabel('fcmodel')
plt.ylabel('Total Execution Time (s)')

# Set plot title
plt.title('Execution Time Comparison')

plt.show()