spark-cerial - Cloud Serial adapter
=============================

**Cerial** is a library giving you an easy way to debug your [Spark Core](http://spark.io) over the cloud.

It is a **drop-in replacement** for [Serial](http://docs.spark.io/firmware/#communication-serial) usage, offering all its functionalities with identical function calls.

### Key features

- Debug your code as if your Spark Core would be connected locally
- 100% compatible with all Serial functions (printing, formatting, writing/reading, conversion...)
- Communicate in **both** ways, read the output and send your input
- [FIFO](https://en.wikipedia.org/wiki/FIFO) input & output buffers, circular buffering for the (debug) win!
- Cerial Monitor as a serial monitor replacement, available as
  - HTML5 based web interface 
  - Node.js based console version

## Example
```c++
  #include "Cerial/Cerial.h"
  Cerialize Cerial;
  
  void setup() {
    Cerial.begin();
    
    Cerial.println("Waiting for you... send me a key if you read me!");
    while(!Cerial.available()) SPARK_WLAN_Loop();
    
    Cerial.print("Dude.. I'm already running since ");
    Cerial.print(millis()/1000);
    Cerial.println(" seconds and here you are!");
  }
  
  void loop() {
    // Echo back all we get
    if(Cerial.available()) {
      Cerial.print("Oh, you sent me: ");
      while(Cerial.available()) Cerial.write(Cerial.read());
      Cerial.println();
    }
      
    // Do funky conversions
    Cerial.println(Time.now(), HEX);
    
    // Or whatever comes into your magical mind!
  }
```
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

Call `Cerial.begin()` from your setup function and you are ready to go!

In all cases, just use `Cerial.*` instead of `Serial.*`.

## How does it work?

Like the `Serial` class, the library extends  [`Stream`](https://github.com/spark/firmware/blob/master/src/spark_wiring_stream.cpp) and provides all mandatory functions to it. Doing so allows the full usage of the toolchain `Stream` and  [`Print`](https://github.com/spark/firmware/blob/master/src/spark_wiring_print.cpp) provides.

By calling `Cerial.begin()` following Spark accessors are being set up:
- Output: `cerialBuffer` - holding the circular output buffer and a pointer to the current circular position
- Input: `cerial` - feed data to the Cerial device

The first 3 bytes of the `cerialBuffer` contain a stringified version of the current circular position followed by spaces. From byte 4 to 622 the circular buffer is allocated.

The Cerial Monitor constantly pulls this buffer and outputs the changed bytes to the user, simulating a real Serial experience.


## Limitations
* Output buffer is limited to 618 bytes (Spark::variable's 622 bytes - 4 bytes for pointer storage)
* Zero-bytes are not supported - based on how chars are transferred from and to the Spark Cloud, a string gets terminated when \0 occurs. So don't send them! :)
