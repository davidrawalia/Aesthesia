# Aesthesia

Aesthesia is an OpenGL based platform to generate real-time 3D visuals from audio and midi data for live electronic music performers.

## Getting Started

To get started, install all prerequisite libraries and Clone or download the repository.

### Prerequisites

Aesthesia currently uses a Max/MSP patch (router/m4l/aesthesia-router.amxd) to route audio amplitude data from Ableton or directly from Max to the visualizer. You will need a copy Max or Max4Live as a data source. Future plans include implementation of a VST and VSTi to send audio or midi data from any DAW.

Libraries used:
* GLEW 2.1.0 (https://sourceforge.net/projects/glew/files/glew/2.1.0/)
* GLFW 3.3 (https://sourceforge.net/projects/glfw/)
* SOIL 2.0 (https://bitbucket.org/SpartanJ/soil2/src/default/)
* Boost 1.71.0 (https://www.boost.org/users/download/#live)

Make sure to link the libraries in your IDE before trying to compile.

## Authors

* **David Rawalia** - *Initial work* - (https://github.com/davidrawalia)

## License

This project is licensed under the GPL-3.0 License - see the [LICENSE.md](LICENSE.md) file for details
