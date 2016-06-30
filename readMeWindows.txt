-----------------------------------------------------------------------
| TSGL INSTALLATION INSTRUCTIONS FOR WINDOWS (Last updated: 06/30/16) |
-----------------------------------------------------------------------

Hello, and thank you for downloading TSGL!

The installation process is very fast and should only take but a few minutes.

This process affects the following Windows versions: Windows 7, 8, 10.

Make sure that you are installing TSGL on one of those operating systems during this process!

-------------------------------
| NOTES (PLEASE READ FIRST!!) |
-------------------------------

* Should you ever run into a situation which says "Download failed", always click "Retry".

* For both Visual Studio 2012 & 2015, we used the Windows 8.1 SDK.
  This does NOT mean that the TSGL solutions will not work in Windows 10, this just means that we used the 
  Windows 8.1 SDK which ships with both versions. 
  For this reason, you may have to install the Windows 8.1 SDK.
 
  In fact, you will need these tools (which can be installed by running Visual Studio's setup program):
  Programming Languages -> Visual C++
  Windows 8.1 -> Tools and Windows SDKs 

* The Visual Studio solutions work with the Windows 10 SDK, but this does not ship with the Visual Studio versions.
  You may install the Windows 10 SDK, and then set the "Target Platform Version" Property in each of the project properties for each solution 
  in order to use the Windows 10 SDK. However, as of summer 2016, we will use the Windows 8.1 SDK until the Windows 10 SDK ships with Visual Studio.

* If, for whatever reason, Visual Studio wants to install missing components, do so. 
  (This may occur if you have not installed the Windows 8.1 SDK, or the Visual C++ tools).
  (When you open the solution, Visual Studio may tell you that the solution needs missing components.
   These are the Windows 8.1 SDK and Visual C++ tools.)

-------------------------------
| Easy Automatic Installation |
-------------------------------
(Visual Studio 2015)
  Run install-windowsVS2015.exe, installing to this directory (which should be the default location).
  ***WINDOWS 10*** Click "More Info" when Windows 10 prevents you from running the installer, and click "Run anyway" or something to that effect.
  Wait for the installer to finish, then open tsgl.sln in Visual Studio 2015.
  A pop-up box should appear. Uncheck the box that says "Ask me for every project" or something to that effect.
  Make sure that the Debug configuration is set to x86.
     * There should be a box somewhere in the Visual Studio toolbar that says "Debug".
     * Make sure that the number next to that box says "x86".
  Right click on tsgl -> Build.
  Right click on "Solution 'tsgl'" -> Build Solution.
  Done! Right click on any of the test projects -> Debug -> Start new instance to get started.

 (Visual Studio 2012):
  Run install-windows.exe, installing to this directory (which should be the default location)
  ***WINDOWS 10 ONLY***: Click "More Info" when you get a blue window popping up saying that Windows protected your PC. Click "I understand and want to run anyway" and then "Run anyway". Afterwards, click "Yes" in the window that pops up. 
  Open tsgl.sln in Visual Studio 2012
  ***VISUAL STUDIO 2013 USERS!*** If you are trying to open TSGL in Visual Studio 2013, do so, but uncheck testBallroom, testSeaUrchin, and testSmartSort when you have to update the projects. After the tests have been updated, right click on each of those three tests individually and click "Properties". In "Platform Toolset", change "Visual Studio 2012 (v110)" to "Visual Studio 2013 (v120)". If any other projects fail to work, make sure that "Platform Toolset" is set to "Visual Studio 2013 (V120)"!  
  Right click on tsgl -> Build
  Right click on solution -> Build Solution
  Done! Right click on any of the test projects -> Debug -> Start New Instance to get started!

-------------------------------------------------------
| Creating a new project in the same solution as TSGL |
-------------------------------------------------------
  Right click on "Solution 'tsgl'" -> Add -> New Project, and create an Empty Project with the name of your choice
  Right click on your project -> Project Dependencies, and check "tsgl"
  On the Property Manager, right click on your project -> Add Existing Property Sheet
    Navigate to tsgl's root folder and import testProperties.props
  You're all set up! Just "#include <tsgl.h>" at the top of all files using TSGL's functions.

-----------------------
| Manual Installation |
-----------------------
  (NOTE: This has not been updated for Visual Studio 2015. Proceed with caution if using Visual Studio 2015!)
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
