import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

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
threshold2 = variables2['alpha']
time2 = variables2['time']
result2 = variables2['result']
bits2 = variables2['bits']

# Set plot title

# Add colorbar

plt.show()

# Create a 3D plot to compare result for different variables from results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the data points with execution time as the z-axis
sc = ax.scatter(k2, alpha2, result2, marker='o')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Alpha')
ax.set_zlabel('Result')

# Set plot title

# Add colorbar

plt.show()

# Create a 3D plot to compare execution times and alpha from results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the data points with result as the z-axis
sc = ax.scatter(k2, time2, result2, marker='o')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Execution Time (s)')
ax.set_zlabel('Result')

# Set plot title

# Add colorbar

plt.show()

# Create a 3D plot to compare result and alpha from results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the data points with execution time as the z-axis
sc = ax.scatter(k2, alpha2, result2, marker='o')

# Set axis labels
ax.set_xlabel('K')
ax.set_ylabel('Alpha')
ax.set_zlabel('Result')

# Set plot title

# Add colorbar

plt.show()

# Create a 3D plot to compare result and alpha from results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the data points with execution time as the z-axis
sc = ax.scatter(bits2, time2, result2, marker='o')

# Set axis labels
ax.set_xlabel('Bits')
ax.set_ylabel('Execution Time (s)')
ax.set_zlabel('Result')

# Set plot title

# Add colorbar

plt.show()

# Create a 3D plot to compare result and alpha from results2.txt
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the data points with execution time as the z-axis
sc = ax.scatter(bits2, alpha2, result2, marker='o')

# Set axis labels
ax.set_xlabel('Bits')
ax.set_ylabel('Alpha')
ax.set_zlabel('Result')

# Set plot title

# Add colorbar

plt.show()
