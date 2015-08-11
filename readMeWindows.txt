Easy Automatic Installation (Requires Visual Studio 2012):
  Run tsgl-setup.exe, installing to this directory (which should be the default location)
  Open tsgl.sln in Visual Studio 2012
  Right click on tsgl -> Build
  Right click on solution -> Build Solution
  Done! Right click on any of the test projects -> Debug -> Start New Instance to get started!


Creating a new project in the same solution as TSGL:
  Right click on "Solution 'tsgl'" -> Add -> New Project, and create an Empty Project with the name of your choice
  Right click on your project -> Project Dependencies, and check "tsgl"
  On the Property Manager, right click on your project -> Add Existing Property Sheet
    Navigate to tsgl's root folder and import testProperties.props
  You're all set up! Just "#include <tsgl.h>" at the top of all files using TSGL's functions.


Creating a new project in a separate solution from TSGL:
  Click on the View Menu -> Property Manager
  In the Property Manager Pane, navigate to tsgl -> Debug |Win 32 -> testProperties
  Right click on testProperties -> Properties, and navigate to User Macros
  Verify that the TSGL directories are pointing to the appropriate directories, and, if not, change them accordingly:
    TsglDllDir should point to your earlier lib directory
    TsglExternalLibDir should also point to your earlier lib directory
    TsglIncludeDir should point to your earlier include directory
  Follow the steps under "Creating a new project in the same solution as TSGL"


Manual Installation:
  Install the Windows 32 version of CMake from the official website if it's not already installed
  Clone the repository from https://github.com/Calvin-CS/TSGL into a convenient place; e.g. "workspace/tsgl"
  Create a folder in a convenient place for includes; e.g. "workspace/tsgl/include"
  Create a folder in a convenient place for libraries; e.g. "workspace/tsgl/lib"
  Open Visual Studio 2012 (Ultimate)
    File -> New > Project From Existing Code
      Set the project type to Visual C++ and click "Next"
      Set the project location to the root of the cloned repo
      Set the name to "tsgl" and click "Finish"
    (Optional) Click on the Project Menu -> Show All Files (displays physical folder layout)
    Right Click on "tsgl" in the solution explorer -> Properties -> Configuration Properties -> VC++ Directories
      Add your project's own src/TSGL directory to "Include Directories"
      Add the path to your include directory to "Include Directories"
      Add the path to your library directory to "Library Directories"
    Navigate to Configuration Properties -> Linker -> System
      Switch the SubSystem to "Console (/SUBSYSTEM:CONSOLE)"
    Navigate to Configuration Properties -> C/C++ -> Language
      Switch on Open MP Support
    Navigate to Configuration Properties - General
      Set the Configuration Type to "Static library (.lib)"
    (Optional) Right click on Project -> Properties -> Configuration Properties -> C/C++ -> Advanced
      Enter error code "4244" (int to float data loss) under "Disable Specific Warnings"
      Enter error code "4005" (macro redefinition) under "Disable Specific Warnings"
    (Optional) Right click on Project -> Properties -> Configuration Properties -> Linker -> Command Line
      Enter /ignore:4049 [re-imported definitions] to "Additional Options"
  Download the 32-bit source .zip for GLEW from the official site
    Extract the contents of glew-1.12.0.zip
    Navigate to build/vc12 and open glew_static.vcxproj in VS2010
    Right click on glew_satic in the Solution Explorer and click "Build"
    If you get a "Specified platform toolset is not installed" error in the last step:
      Right click on the project -> Properties -> Configuration Properties -> General -> Platform Toolset
      Set the Platform Toolset to v100
      Right click on glew_static again and click "Build"
    Copy GLEW's lib/Debug/Win32/glew32sd.lib into your earlier lib directory
    Copy the contents of GLEW's "include" directory into your earlier include directory
  Download the Windows source .zip for GLFW from the official site
    Extract the contents of the glew-3.1.1.zip
    Run the CMake GUI with the extracted folder's root directory as both the source code / binary location
      Click Configure
        Use "Visual Studio 10 2010" as the generator, using default native compilers
      Click Generate
    Open ALL_BUILD.vcxproj in VS 2010 and run "Build Solution" from the Solution Explorer
    Copy src/Debug/glfw3.lib into your earlier lib directory
    Copy the contents of GLFW's "include" directory into your earlier include directory
  Download the GnuWin32 .zip bin and dep packages for FreeType2 from Sourceforge
    Extract the contents of each freetype-2.3.5-1-bin.zip
      Copy bin/freetype6.dll into your earlier dll directory
      Copy lib/freetype.lib into your earlier lib directory
      Copy the contents of include/ into your earlier include directory
        Rename the include's "freetype2" to "freetype", and copy the inner "freetype" folder back to the root level (e.g., there should be a path to "workspace/includes/freetype/freetype.h")
    Extract the contents of each freetype-2.3.5-1-dep.zip
      Copy bin/zlib1.dll into your earlier dll directory
  Download the tarball source for Freeglut from the official site
    Extract the contents of freeglut-3.0.0.tar.gz
    Run the CMake GUI with the extracted folder's root directory as both the source code / binary location
      Click Configure
        Use "Visual Studio 10 2010" as the generator, using default native compilers
      Click Generate
    Open ALL_BUILD.vcxproj in VS 2010 and run "Build Solution" from the Solution Explorer
    Copy lib/Debug/freeglutd.lib into your earlier lib directory
    Copy the contents of Freeglut's "include" directory into your earlier include directory
  Download STB (Sean's Tool Box) from Github
    Extract stb-master.zip
    Rename the top-level directory to "stb"
    Copy the top-level stb directory into the your earlier include directory
  Return to tsgl.vcxproj
    Right click on Project -> Properties -> Configuration Properties -> Linker -> Input
      Add the names of all of the libaries in your earlier lib directory under "Additional Dependencies"
        glfw3.lib, glew32sd.lib, freetype.lib
        (NOTE: freeglutd.lib need not be listed here, but must be present in the includes/ directory)
    Right click on Project -> Properties -> Configuration Properties -> Debugging
      Under Environment, add "PATH=<path to your dll directory>;%PATH%"
    Copy your dlls from your earlier dlls directory into the project's Debug directory (for later)
    Right click on tsgl in the Solution Exploer, and click "Build"
    Run!