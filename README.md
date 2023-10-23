# Build System for literally anything you can do in the command line

simple thing to run terminal commands so i dont have to type them out everytime

Runs commands from the BUILD file



## Quick Start

Compile the program

```console
g++ main.cpp -o build
./build
```



## Build Features

Just running any terminal commands 

```shell
g++ main.cpp -o build 
echo Hi Mom!
```

Variables to store things

```shell
Flags = "-Wextra -Werror -Wall"

g++ main.cpp -o build Flags
``` 

Ability to run commands while user is on a specific operating system

```shell

[WINDOWS]

echo this will only run on windows :D

[LINUX]

echo this will only run on linux :D

[MAC]

echo this will only work on Mac Os :|
```


