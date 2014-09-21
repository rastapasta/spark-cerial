spark-cerial - Cloud Serial adapter
=============================

**Cerial** is a library giving you an easy way to debug your [Spark Core](http://spark.io) over the cloud.

It is a **drop-in replacement** for [Serial](http://docs.spark.io/firmware/#communication-serial) usage, offering all its functionalities with identical function calls.

### Key features

- Debug your code as if your core is connected locally
- Communicate in **both** ways
- 100% compatible with all Serial functions (printing, formatting, writing/reading, conversion...)
- Huge output&input buffers
- Use the HTML5 based Cerial Monitor or use the node serial-monitor-cli to do same in any console

## Getting started

- If your use the online IDE, click on "libraries", then "Cerial", then "Include in app", and finally select the app you want to use the library with. This will automatically add the include directive to bring in the library header file.

- If you're compiling locally using the firmware makefile, clone the github repo to the same folder that contains
`firmware`.
 - edit `firmware/src/build.mk` and add these two lines:

 ```
    INCLUDE_DIRS += ../spark-cerial/firmware
    CPPSRC += ../spark-cerial/firmware/Cerial.cpp
 ```


## Using the library
To use the library in application code, include the header file and create a **Cerial** instance:

In the online IDE:
```c++
    #include "Cerial/Cerial.h"
    Cerialize Cerial;
```

Local build:
```c++
    #include "Cerial.h"
    Cerialize Cerial;
```

From here on, everything is identical to the usage of the classical Serial interface.
Just use `Cerial.*` instead of `Serial.*`.


## How does it work?



## Limitations
* Output buffer is limited to 618 bytes (Spark::variable's 622 bytes - 4 bytes for pointer storage)
* Zero-bytes are not supported - based on how chars are transferred from and to the Spark Cloud, a string gets terminated when \0 occurs. So don't send them! :)