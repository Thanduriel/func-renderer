# func-renderer
This is OpenGl 3.3 based renderer for noise functions to experiment with terrain generation.
It uses expression templates to compose multiple height functions in compile time.

# build
* clone repo
* update submodules
* Use the CMakeLists.txt in the main folder to generate platform depend build files
* build with toolchain of your choice
tested: msvc2019, gcc 8.1.0

# usage
A variaty of useful functions can be found in functions1d.hpp, functions2d.hpp and modifiers.hpp.
Examples on how to combine and render them are located in scene.cpp.
To display a specific scene change the type in main.cpp.

If you want to display 1D functions make sure to #define MODE2D in config.hpp,
otherwise remove it.

# further read
* http://staff.fh-hagenberg.at/burger/publications/reports/2008GradientNoise/Burger-GradientNoiseGerman-2008.pdf
A detailed explananation on creating 1 to N-dimensional gradient noise functions

* http://www.decarpentier.nl/scape-procedural-basics
* http://www.decarpentier.nl/scape-procedural-extensions
generating different terrains based on modified and combined perlin noise

* http://www.planetside.co.uk/forums/index.php?action=dlattach;topic=18400.0;attach=50831
creating dunes to form deserts
