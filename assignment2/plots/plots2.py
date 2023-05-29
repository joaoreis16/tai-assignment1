import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Load data from results1.txt
filename1 = 'results1.txt'

with open(filename1, 'r') as file1:
    lines1 = file1.read().splitlines()

# Extract the header and values from the data
header1 = lines1[0].strip().split()
data1 = [line.strip().split() for line in lines1[1:] if line.strip()]

# Convert data values to appropriate types
data1 = [[float(value) for value in row] for row in data1]

# Assign columns to variables
variables1 = {}  # Dictionary to store the variables

for i, column in enumerate(header1):
    variables1[column] = [row[i] for row in data1]

alpha1 = variables1['alpha']
k1 = variables1['K']
threshold1 = variables1['threshold']
time1 = variables1['time']
bits1 = variables1['bits']


# Load data from results2.txt
filename2 = 'results2.txt'

with open(filename2, 'r') as file2:
    lines2 = file2.read().splitlines()

# Extract the header and values from the data
header2 = lines2[0].strip().split()
data2 = [line.strip().split() for line in lines2[1:] if line.strip()]

# Convert data values to appropriate types
data2 = [[float(value) for value in row] for row in data2]

# Assign columns to variables
variables2 = {}  # Dictionary to store the variables

for i, column in enumerate(header2):
    variables2[column] = [row[i] for row in data2]

alpha2 = variables2['alpha']
k2 = variables2['K']
threshold2 = variables2['threshold']
time2 = variables2['time']
bits2 = variables2['bits']

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(k1, threshold1, time1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(k2, threshold2, time2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Threshold')
ax.set_zlabel('Execution Time (s)')

# Set plot title
ax.set_title('Execution Time Comparison')

# Set legend
ax.legend()

plt.show()

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(k1, threshold1, bits1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(k2, threshold2, bits2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Threshold')
ax.set_zlabel('Bits')

# Set legend
ax.legend()

plt.show()

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(k1, alpha1, time1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(k2, alpha2, time2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Alpha')
ax.set_zlabel('Execution Time (s)')

# Set legend
ax.legend()

plt.show()

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(k1, alpha1, bits1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(k2, alpha2, bits2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Alpha')
ax.set_zlabel('Bits')

# Set legend
ax.legend()

plt.show()

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(threshold1, alpha1, time1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(threshold2, alpha2, time2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('Threshold')
ax.set_ylabel('Alpha')
ax.set_zlabel('Execution Time (s)')

# Set legend
ax.legend()

plt.show()

# Create a 3D plot to compare execution times between results1.txt and results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the execution times from results1.txt
ax.scatter(threshold1, alpha1, bits1, c='red', marker='o', label='fcmodel = 0')

# Plot the execution times from results2.txt
ax.scatter(threshold2, alpha2, bits2, c='green', marker='o', label='fcmodel = 1')

# Set axis labels
ax.set_xlabel('Threshold')
ax.set_ylabel('Alpha')
ax.set_zlabel('Bits')

# Set legend
ax.legend()

plt.show()