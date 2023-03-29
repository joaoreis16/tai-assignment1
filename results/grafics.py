import matplotlib.pyplot as plt 
import glob
import re

alpha = []
threshold = []
k = []
total_bits = []
average_number_of_bits = []
elapsed_time = []


# Define the regular expression pattern to match filenames starting with "file"
pattern = re.compile(r'^file.*')

# Loop through all files in the current directory that match the pattern
for filename in glob.glob('*'):
    if pattern.match(filename):

        with open(filename, 'r') as f:
            for line in f:
                line_splited = line.strip().split("-")
                if line_splited[0] == 'alpha':
                    alpha.append(int(line_splited[1]))
                elif line_splited[0] == 'threshold':
                    threshold.append(int(line_splited[1]))
                elif line_splited[0] == 'K':
                    k.append(int(line_splited[1]))
                elif line_splited[0] == 'total_bits':
                    total_bits.append(int(line_splited[1]))
                elif line_splited[0] == 'Average_number_of_bits':
                    average_number_of_bits.append(float(line_splited[1]))
                elif line_splited[0] == 'Elapsed_time':
                    elapsed_time.append(float(line_splited[1]))


# K vs Total bits
plt.plot(k, total_bits, '-ro')
plt.xlabel('K')
plt.ylabel('Total bits')
plt.title('Total bits vs K')
plt.show()

#k vs Average number of bits
plt.plot(k, average_number_of_bits, '-bo')
plt.xlabel('K')
plt.ylabel('Average number of bits')
plt.title('Average number of bits vs K')
plt.show()

#k vs Elapsed time
plt.plot(k, elapsed_time, '-go')
plt.xlabel('K')
plt.ylabel('Elapsed time')
plt.title('Elapsed time vs K')
plt.show()

#alpha vs Total bits
plt.plot(alpha, total_bits, '-ro')
plt.xlabel('alpha')
plt.ylabel('Total bits')
plt.title('Total bits vs alpha')
plt.show()

#alpha vs Average number of bits
plt.plot(alpha, average_number_of_bits, '-bo')
plt.xlabel('alpha')
plt.ylabel('Average number of bits')
plt.title('Average number of bits vs alpha')
plt.show()

#threshold vs Total bits
plt.plot(threshold, total_bits, '-ro')
plt.xlabel('threshold')
plt.ylabel('Total bits')
plt.title('Total bits vs threshold')
plt.show()

#threshold vs Average number of bits
plt.plot(threshold, average_number_of_bits, '-bo')
plt.xlabel('threshold')
plt.ylabel('Average number of bits')
plt.title('Average number of bits vs threshold')
plt.show()