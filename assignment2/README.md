## assignment #2

### how to compile and run


1. lang.cpp

```bash
g++ -std=c++11 -Wall lang.cpp cpm.cpp -o lang

./lang <ri_file> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )

./lang english.txt portuguese.txt
```

2. findlang.cpp

```bash
g++ -std=c++11 -Wall findlang.cpp lang.cpp cpm.cpp -o findlang

./findlang <ri_folder> <target_file> (optional: -a <alpha: int> -t <threshold: float> -k <K: int> )

./findlang lang_files/short portuguese.txt
```

3. locatelang.cpp

```bash
g++ -std=c++11 -Wall locatelang.cpp lang.cpp cpm.cpp -o locatelang

./locatelang lang_files/short portuguese.txt
```