## assignment #2

### how to compile and run 

1. lang.cpp

```bash
g++ -std=c++11 -Wall lang.cpp cpm.cpp fcm.cpp -o lang

# applying our copy model
./lang lang_files/short/Portuguese.utf8 target_files/Dutch.utf8

# applying the finite-context model
./lang lang_files/short/Portuguese.utf8 target_files/Dutch.utf8 -f
```

2. findlang.cpp

```bash
g++ -std=c++11 -Wall findlang.cpp lang.cpp cpm.cpp fcm.cpp -o findlang

# applying our copy model
./findlang lang_files/short target_files/Dutch.utf8

# applying the finite-context model
./findlang lang_files/short target_files/Dutch.utf8 -f
```

3. locatelang.cpp

```bash
g++ -std=c++11 -Wall locatelang.cpp lang.cpp cpm.cpp fcm.cpp -o locatelang

# applying our copy model
./locatelang lang_files/short target_files/Dutch.utf8

# applying the finite-context model
./locatelang lang_files/short target_files/Dutch.utf8 -f
```

