# MyJQL

*This is the repo for FDU 2022 - Introduction to Database - Project 2 - MyJQL*

## Introduction

MyJQL is a simplified redis-like key-value database, which supports three kinds of operations: `get key`, `set key value`, and `del key`. B-Tree index is automatically created for `key`.

- [`include`](include) and [`src`](src): Core implementation of MyJQL.
- [`driver`](driver): Header and source for MyJQL driver. After compilation, you can include [myjqlclient.h](driver/myjqlclient.h) and link myjqlclient.lib (in Windows) or libmyjqlclient.a (in Linux). In this way, your own program will be able to store data to and retrieve data from MyJQL.
- [`main`](main): [`myjqlserver.c`](main/myjqlserver.c) is the server. [`myjqlshell.cpp`](main/myjqlshell.cpp) is the client, which also serves as an example utilizing the driver.
- [`test`](test): Tests for core implementation. [`test_myjql.cpp`](test/test_myjql.cpp) is used to examine the full functionality.

## Build

MyJQL supports Windows and Linux (Ubuntu).

Shell/CMD/PowerShell (in the root directory of this repo):
```
mkdir build
cd build
cmake ..
cmake --build .
```

### Release Build

MSVC:
```
cmake --build . --config Release
```

gcc:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Run Tests

Change directory into `build/test` (if you are currently in `build`):
```
cd test
```

List tests:
```
ctest -N
```

Run tests:

- MSVC:
  ```
  ctest -C Debug
  ctest -C Release
  ```

- gcc:
  ```
  ctest
  ```


## Usage

- Start the server: `myjqlserver`.
- Start several clients: `myjqlshell`.
- Input commands to the clients:
  - `set key value`: set `key` to `value`.
  - `get key`: get value based on `key`.
  - `del key`: delete value based on `key`.
  - `exit`: exit the client shell.
- To **safely** stop the server:
  - Use `ctrl + c` in Windows and Linux.
  - In Windows, you also can close it directly.
