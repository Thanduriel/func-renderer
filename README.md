# func-renderer
OpenGl 3.3 based visualiser for mathematical functions

# build
Grab source packages from the right version for the following libraries and but them into "dependencies/":
* [glfw-3.2.1](http://www.glfw.org/download.html)
* [glew-2.0.0](http://glew.sourceforge.net/)
* [glm](https://github.com/g-truc/glm/tags)
Use the CMakeLists.txt in the main folder to generate platform depend build files.

# usage
A variaty of useful functions can be found in functions1d.hpp, functions2d.hpp and modifiers.hpp.
Examples on how to combine and render them are located in scene.cpp.
To display a specific scene change the type in main.cpp.
If you want to display 1D functions make sure to #define MODE2D in config.hpp
otherwise remove it.

# further read
* http://staff.fh-hagenberg.at/burger/publications/reports/2008GradientNoise/Burger-GradientNoiseGerman-2008.pdf
A detailed explananation on creating 1 to N-dimensional gradient noise functions

* http://www.decarpentier.nl/scape-procedural-basics
* http://www.decarpentier.nl/scape-procedural-extensions
generating different terrains based on modified and combined perlin noise
