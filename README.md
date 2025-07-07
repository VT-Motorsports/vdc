# Required Software
Before building & modifying C++ projects, you will need the prerequisites:
Git: For version control (GitHub)
CMake: Creates makefiles from CMakeLists.txt
MinGW: Build windows binaries (.exe) from makefiles
### Installing Git
Download from https://git-scm.com/downloads
Run executable to install Git
Check functionality by opening up Command Prompt and running:
```
git --help
```
### Installing CMake
Download from https://cmake.org/download/
Run executable to install CMake
Check functionality by opening up Command Prompt and running:
```
cmake --help
```
### Installing MinGW
Download MinGW (Win32) from winlibs (NOT the out-of-date sourceforge installer)
Make C:\MinGW folder
Dump the contents of .zip subfolder mingw32 into C:\MinGW
Make sure C:\MinGW\bin exists
Add C:\MinGW\bin to (USER, not SYSTEM) Path environment variable
Check functionality by opening up (or restarting) Command Prompt and running:
```
mingw32-make --help
```

# Required Libraries (for VDC)
Required libraries for this project (**install under vdc\lib**) are:
**Armadillo**: For Matlab-like numerical manipulation
**glad**: Complementary library for glfw
**GLFW**: For creating OpenGL window (Windows, macOS, or Linux Wayland/X11)
**ImGui**: For main (immediate-mode) GUI
**ImPlot**: Plotting extension for ImPlot
**json**: Read/write .json files
### Installing Armadillo
Into Command Prompt:
```
git clone https://gitlab.com/conradsnicta/armadillo-code.git
```
### Installing glad
Into Command Prompt:
```
git clone --single-branch --branch c https://github.com/Dav1dde/glad.git
```
### Installing GLFW
Into Command Prompt:
```
git clone https://github.com/glfw/glfw.git
```
Building (Windows):
```
cd wherever_youve_installed_this\vdc\lib\GLFW
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```
### Installing ImGui
```
git clone https://github.com/ocornut/imgui.git
```
### Installing ImPlot
```
git clone https://github.com/epezent/implot.git
```
### Installing json
```
git clone https://github.com/nlohmann/json.git
```