# tai-assignment1 #5

## How to run

First time:

Enter the /src directory and run the following commands

```bash
cmake .
```

Then to compile the Copy Model and the Generator:

```bash
make
```

And to Run the Copy Model Executable:

```bash
./CPM -f <filename> -k <k_value> -a <alpha_value> -t <threshold_value>
```

Example:

```bash

./CPM -f ../example/chry.txt -k 3 -a 0.5 -t 3

```

It can be run also with the following command:
The K, Alpha and Threshold values are optional, if not provided, the default values will be used.

```bash
./CPM -f <filename>
```

Example:

```bash
./CPM -f ../example/chry.txt
```


How to Run the Generator:

```bash
./CPM_GEN -f <file_name> -n <number_of_chars_to_predict> -k <number_of_chars_to_read>
```

Example:

```bash
./CPM_GEN -f ../example/chry.txt -n 100 -k 3
```
