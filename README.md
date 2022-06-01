# Plugins hanler example

This plugins handler examples are provided as example about how to create a plugins system, which allow to add functionalities to your program without to have to recompile the entire solution.

## Disclaimer

I am not an expert programmer and this is the first time I play with libraries, so maybe there are better ways to do it... It works and for me is enought. Any improvement which help me to learn is welcome.

This examples are provided without any kind of warranty and I will not be responsible of any wrong use of it or the injuries that can derivative of it use (the dinosaur extinction was not my fault).

## License

This examples are provided under the Apache license, which will allow you to do almost everything with it. It would be nice that if you improve any of them, share it under the same license.

## Versions

There are two versions for now of the plugins manager.

### Method 1

This method uses the C ABI interface to get the basic information like the plugin name, version... and creates a class object in one of those C functions.

**Pros:**

 * Easy to implement
 * One function to rule them all. In that function you will export a class object address which will allow you to use it
 * Adding methods or variables to the class will be easy and fast
 * You can free all the resources automatically with the class destructor
 * Working on Windows and Linux


 **Cons:**

  * ABI dependant which is a big deal. g++ and other GNU tools have more stable ABI and will you provide more compatibility, allowing even to set the ABI version. Visual Studio on the other hand will change the ABI in every version of the compiler, which will break the compatibility of the libraries.
  * You have to include the same base class in the main program and all the plugins. Every change on it will probably break the compatibility with already compiled plugins.
  * On the plugin you will have to override all the class methods (maybe can be easily fixed, but I have not tried to do it yet).


### Method 2

This method will use the C interface for all the functions.

**Pros:**
 
 * C ABI is stable and doesn't suffer of changes, so the compatibility is high. You can compile a plugin on Visual Studio and use it in a base program compiled with MinGW.
 * The main program doesn't care about the class you are using in the library, so you don't need to include it.
 * The plugin can be fully independent of the main program. If you update the main program API, the plugins will continue working (of course if the functions exists and the change doesn't affects to an existing function).


 **Cons:**

  * Is a bit tedious to manage. If you create a class inside, every method and variable that you want to export must be done through a C function.
  * The resources must be freed on the plugin before to close it, or there will be memory leaks.
  * Only basic variables types can be used (C variables).
  * Every function on the library is independent, so you cannot share a class object directly between them. The best way to do it will be using a global variable or passing the pointer to the class object as argument. To pass the pointer as argument is recomended for libraries that will be used by more than one program, because global variables will be shared and will conflict.