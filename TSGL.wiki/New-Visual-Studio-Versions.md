***NOTE:*** This page contains information which has been acquired to the best of our abilities. There may be some things that the documentation fails to report, or things may have changed. If this is the case, PLEASE update this page so that others may benefit from your findings! (See [[Doxygen and Wiki]] for more information on how to updated the wiki pages). 

Hello!

A new Visual Studio version has come out, and the TSGL Solution must be updated.

The process is long and tedious, but this page will guide you.

Let's begin.

---------------------------------
| Starting out: tsgl VS project |
--------------------------------- 

1). Get the TSGL zip folder from GitHub.

2). Install TSGL (using an earlier version of VS).

3). Make a clone of TSGL in a different place using Git Bash.

4). Copy over the ```lib``` and ```include``` directories from the zip file location to the cloned TSGL directory.

5). Create a new ```VS****``` folder inside of the cloned TSGL directory (replacing ```****``` with the Visual Studio version number).

6). Open up the new version of Visual Studio, and create a new ```tsgl``` project inside of the cloned TSGL folder. (```File -> New -> Project```, ```Visual C++```, ```Empty Project```, then change the name to ```tsgl``` and the ```Location``` to wherever you put the cloned TSGL folder).

You should now have a ```tsgl``` folder inside of the cloned TSGL folder. 

7). Via File Manager, go into the cloned TSGL folder, and into the ```tsgl``` VS project folder. Copy over all files from the project folder (and from the ```tsgl``` folder that is inside of the ```tsgl``` VS project folder) over to the cloned TSGL folder.

You should have the solution file, and the project files for the ```tsgl``` VS project in the cloned TSGL workspace. (Plus a few extra).

8). Close VS****, and delete the ```tsgl``` folder from the cloned TSGL workspace.

9). Reopen VS****, and open up the TSGL solution file that's in the cloned TSGL workspace. (```File -> Open -> Project/Solution```, then choose the solution file that is in the cloned TSGL folder).

It will say that it can't find the ```tsgl``` project (or that it can't load it). 

You'll have to re-add it back into the solution.

Remove the failed ```tsgl``` project (```right click on tsgl -> Remove```).

10). Right click on ```Solution 'tsgl'```, ```Add```, then ```Existing Project```. 

Get the ```tsgl``` project file that's in the cloned TSGL workspace (it should say ```tsgl``` and have a ```.vcxproj``` suffix).

11). MAKE SURE THAT YOU HAVE DEBUG SET TO x86 OR WIN32!

12). Add the ```.cpp``` and ```.h``` files to the ```tsgl``` project.

```Right-click on tsgl -> Add -> Existing Item```, then go to the ```src/TSGL``` folder inside of the cloned TSGL workspace folder, and add ALL of the files to the project

Once that is done, and now that ```tsgl``` is added back into the appropriate solution file, right-click on it. Choose ```Properties```.

Here is where the magic happens. We'll have to edit a few of these in order for ```tsgl``` to build correctly.

Here are the ones that *MUST* be edited (for ```tsgl```):

###General:

+ ```Target Platform Version:``` 10.0.10586.0 (If that doesn't work, do 8.1. You'll have to install the Windows 10 SDK then).
+ ```Output Directory:``` $(SolutionDir)lib\
+ ```Intermediate Directory:``` $(Configuration)\$(TargetName)\
+ ```Target Extension:``` .lib
+ ```Build Log File:``` $(IntDir)\$(MSBuildProjectName).log
+ ```Platform Toolset:``` Whatever version of Visual Studio that you are currently working in.
+ ```Configuration Type:``` Static library (.lib)

Click ```Apply```.

###Debugging:

+ ```Environment:``` PATH=$(TsglDllDir);%PATH%;$(LocalDebuggerEnvironment)

Click ```Apply```.

###VC++ Directories:

+ ```Include Directories:``` $(SolutionDir)\src\TSGL;$(SolutionDir)\include;$(IncludePath)
+ ```Library Directories:``` $(SolutionDir)lib;$(LibraryPath)

Click ```Apply```.

###C/C++:

Under ```General```:

  + ```Debug Information Format:``` Program Database (/Zi)
  + ```Warning Level:``` Turn Off All Warnings (/W0)
  + ```SDL checks:``` No (/sdl-)

Under ```Preprocessor```:

  + ```Preprocessor Definitions:``` WIN32;_DEBUG;_WINDOWS;%(PreprocessorDefinitions)

Under ```Language```:

  + ```Open MP Support:``` Yes (/openmp)

Click ```Apply```. 

Now, click ```Ok```. 

Moment of truth. Right click on ```tsgl```, and click ```Build```.

If it builds successively, then congrats! You have the new VS**** ```tsgl``` project inside of the main TSGL repo!

Before we forget, we need to add a ```Property Sheet``` to the ```tsgl``` project.

We have to get it from the zip folder, so go back to the downloaded TSGL zip file from earlier, copy the ```testProperties``` file, and stick that in the cloned TSGL workspace. 

Now, add it to ```tsgl``` through VS****:

* Click ```View -> Other Windows -> Property Manager```.
* ```Right-click on the project -> Add Existing Property Sheet```, and add ```testProperties``` 
	  (which should be inside of the cloned TSGL workspace folder).
* Click the save icon (should look like a floppy disk, somewhere above in the ```Property Manager``` toolbar).
* Click ```View -> Solution Explorer```.

It should be added to the project. 

Now, let's move onto the tests....

--------------------------------------------------
| VS TSGL Solution: tests, tests, and more tests |
--------------------------------------------------
 
These are a bit more tricky to add. 

For starters, they have different properties than the ```tsgl``` project.

Also, they belong within the ```VS****``` folder that we made earlier. 

Some also have extra files that need to be added, like ```testLangton``` and ```testPhilosophers```.

And to top it off, there are ***41*** of them. (At the time of this writing).

Which means, you'll have to repeat this process ***41 times***. 

Yes, ***41 times***. 

On the bright side, we can use the ```testProperties``` sheet on them.

Let's get started with ```testAlphaRectangle```.

-----------------------------------------
| testAlphaRectangle: fancy rectangles! |
-----------------------------------------

Right-click on ```Solution 'tsgl'``` -> ```Add``` -> ```New Project```, choose ```Visual C++```, ```Empty Project```,
and name it ```testAlphaRectangle```. Change the ```Location``` to point at the ```VS****``` folder inside of the cloned TSGL workspace.

Go into the ```VS****``` folder, and you'll see a folder named ```testAlphaRectangle```.

Just like we did with the ```tsgl``` project, we'll have to take out the files from this folder. 

Go into the ```testAlphaRectangle``` folder, copy the files over to the ```VS****``` directory that's in the cloned TSGL workspace (one directory up), and delete the ```testAlphaRectangle``` folder.

Remove the ```testAlphaRectangle``` project from the ```tsgl``` solution, and add the copied ```testAlphaRectangle``` project that's in the ```VS****``` folder.

Now, add the ```testAlphaRectangle.cpp``` file to the project.
(```Right-click -> Add -> Existing Item```, then go to ```src/tests``` inside of the cloned TSGL folder, and add ```testAlphaRectangle.cpp```).

Now, click on ```View``` -> ```Other Windows``` -> ```Property Manager```.

This is the place where we'll be adding the ```testProperties``` sheet to the test projects.

Right-click on ```testAlphaRectangle``` -> ```Add Existing Property Sheet```, and choose the ```testProperties``` sheet that's in the cloned TSGL workspace.

Click on the save icon (the one that looks like a floppy disk), and that saves the sheet for ``testAlphaRectangle```.

Go back to the ```Solution Explorer``` (```View``` -> ```Solution Explorer```), and right-click on ```testAlphaRectangle```. Choose ```Properties```.

Like we did for the ```tsgl``` project, we'll have to edit the properties for each test.

Luckily, this applies to all tests. The properties are generally the same for all tests.

Here are the ones that you have to change:

###General:

+ ```Target Platform:``` 10.0.10586.0 (or 8.1 if you don't have the Windows 10 SDK).
+ ```Intermediate Directory:``` $(Configuration)\$(TargetName)\
+ ```Platform Toolset:``` Whatever version of Visual Studio that you are currently working in.

###VC++ Directories:

+ ```Include Directories:``` $(TsglSourceDir);$(TsglIncludeDir);$(IncludePath)
+ ```Library Directories:``` $(TsglLibDir);$(TsglExternalLibDir);$(LibraryPath)

###C/C++:

* Under ```Language```:

  + ```Open MP Support:``` Yes (/openmp)

###Linker:

* Under ```Input```:

  + ```Additional Dependencies:``` tsgl.lib;freeglutd.lib;freetype.lib;glew32sd.lib;glfw3.lib;%(AdditionalDependencies)

Once that is all done, ```Right-click -> Build Dependencies -> Project Dependencies```, and check the box for ```tsgl```. 

This means that ```testAlphaRectangle``` depends on the ```tsgl``` project and won't build until after the ```tsgl``` project has been built.

Now, go into the ```lib``` folder inside of the cloned TSGL folder.

Copy over the ```freetype6.dll``` and ```zlib1.dll``` files over to the ```Debug``` folder inside of the cloned TSGL workspace.

```testAlphaRectangle``` won't work without those files inside of that folder.

Moment of truth again. Clean the solution (```Right-click on Solution 'tsgl' -> Clean Solution```).

Now, build it. (```Right-click on Solution 'tsgl' -> Build Solution```).

To run ```testAlphaRectangle```, right-click on it -> ```Debug``` -> ```Start new instance```.

If you see a screen that says ```Fancy Rectangles``` pop up, then congrats! You integrated the first test project into the main GitHub repo.

Now, you have to repeat this process ***40*** (at the time of this writing) times. 

The general pattern for adding a new project is:

1). Add a new project to the ```tsgl``` solution.
	
   * ```Right-click -> Add -> New Project```.
   * Choose ```Visual C++```, then ```Empty Project```.
   * Call it ```test*``` (replacing ```*``` with the name of the ```.cpp``` file associated with the TSGL test) and place it in the ```VS****``` folder inside of the cloned TSGL workspace.

2). Go to the ```VS****``` folder, and into the newly added project folder.

3). Copy all files to the ```VS****``` directory in the cloned TSGL workspace, and delete the project folder from the ```VS****``` folder.

4). Remove the deleted project from the ```tsgl``` solution, and add the copied one that's in the ```VS****``` folder.
	
   * ```Right-click -> Add -> Existing Project```.
   * Choose the project that's in the ```VS****``` folder inside of the cloned TSGL folder.

5). Add the necessary ```.cpp``` and ```.h``` files to the project.
  
   * ```Right-click -> Add -> Existing Item```.
   * The files should be in the ```src/tests/``` folder inside of the cloned TSGL workspace.

6). Add the ```testProperties``` sheet to the project.
   
   * ```View -> Other Windows -> Property Manager```.
   * ```Right-click on the project -> Add Existing Property Sheet```, and add ```testProperties``` 
        (which should be inside of the cloned TSGL workspace folder).
   * ```View -> Solution Explorer```.

7). Make sure the ```Properties``` are correct.
  
###General: 	

   + ```Target Platform:``` 10.0.10586.0 (or 8.1 if you don't have the Windows 10 SDK).
   + ```Intermediate Directory:``` $(Configuration)\$(TargetName)\
   + ```Platform Toolset:``` Whatever version of Visual Studio that you are currently working in.
	
   Click ```Apply```.
    
###VC++ Directories:

   + ```Include Directories:``` $(TsglSourceDir);$(TsglIncludeDir);$(IncludePath)
   + ```Library Directories:``` $(TsglLibDir);$(TsglExternalLibDir);$(LibraryPath)

   Click ```Apply```.

###C/C++:
	
   * Under ```Language:```
      + ```Open MP Support:``` Yes (/openmp)

   Click ```Apply```.
	
###Linker:
	
   * Under ```Input:```
      + ```Additional Dependencies:``` tsgl.lib;freeglutd.lib;freetype.lib;glew32sd.lib;glfw3.lib;%(AdditionalDependencies)

   Click ```Apply```, then ```Ok```.

8). Add ```tsgl``` as a ```Project Dependency```.

   * ```Right-click on the project -> Build Dependencies -> Project Dependencies```
   * Check the ```tsgl``` box and click ```Ok```.

9). Rebuild the solution. 

10). Run the new project!

   * ```Right-click on the project -> Debug -> Start new instance```

11). Repeat until no more projects need to be added.

List of projects that have to be added (at the time of this writing):

```testAlphaRectangle```, ```testAura```, ```testBallroom```, ```testBlurImage```, ```testCalcPi```, ```testColorPoints```, ```testColorWheel```, ```testConcavePolygon```, ```testConway```, ```testCosineIntegral```, ```testDumbSort```, ```testFireworks```, ```testForestFire```, ```testFunction```, ```testGetPixels```, ```testGradientWheel```, ```testGraydient```, ```testGreyscale```, ```testHighData```, ```testImage```, ```testImageCart```, ```testInverter```, ```testLangton```, ```testLineChain```, ```testLineFan```, ```testMandelbrot``` , ```testMouse```, ```testNewtonPendulum```, ```testPhilosophers```, ```testPong```, ```testProjectiles```, ```testScreenshot```, ```testSeaUrchin```, ```testSmartSort```, ```testSpectrogram```, ```testSpectrum```, ```testText```, ```testTextCart```, ```testTextTwo```, ```testUnits```, ```testVoronoi```.

As mentioned, some project require extra files. These are:

+ ```testConway``` -> ```LifeFarm.h```, ```LifeFarm.cpp```

+ ```testInverter``` -> ```ImageInverter.h```, ```ImageInverter.cpp```

+ ```testLangton``` -> ```AntFarm.h```, ```LangtonAnt.h```, ```AntFarm.cpp```, ```LangtonAnt.cpp```

+ ```testMandelbrot``` -> ```Buddhabrot.h```, ```GradientMandelbrot.h```, ```Julia.h```, ```Mandelbrot.h```, ```Nova.h```, ```Buddhabrot.cpp```, ```GradientMandelbrot.cpp```, ```Julia.cpp```, ```Mandelbrot.cpp```, ```Nova.cpp```

+ ```testPong``` -> ```Ball.h```, ```Paddle.h```, ```Pong.h```, ```Ball.cpp```, ```Paddle.cpp```, ```Pong.cpp```

+ ```testSeaUrchin``` -> ```SeaUrchin.h```, ```SeaUrchin.cpp```

+ ```testVoronoi``` -> ```ShadedVoronoi.h```, ```Voronoi.h```, ```ShadedVoronoi.cpp```, ```Voronoi.cpp```

They are located within the ```src/tests``` folder, inside of the directory that has the name of the test. 

(Example: ```testMandelbrot``` -> ```*.h``` && ````*.cpp``` are inside of ```Mandelbrot```).

When you get to those tests, just add those ```.h``` and ```.cpp``` files as you would add the ```test*.cpp``` file. 
(```Right-click on the project -> Add -> Existing Item```, then add the files.)


Once you have all the tests added, then you are finished!

----------------
| Finishing up |
----------------

1). Create a new ```winVS****``` folder (replacing ```****``` with the VS version number) inside of the cloned TSGL workspace folder.

2). Move the ```vs****``` folder into the ```winVS****``` folder.

3). Delete the ```include``` and ```lib``` folders.

4). Move the ```Debug``` folder into the ```winVS****``` folder (the one inside of the cloned TSGL workspace folder).

5). Move the ```tsgl``` solution and ```tsgl``` project files into the ```winVS****``` folder.

6). Move the ```testProperties``` sheet into the ```winVS****``` folder.

7). Move the ```readmetests.txt``` and ```runtests.bat``` files out of the ```windows``` folder and into the ```winVS****``` folder.

Now, let's do a dry-run install of TSGL BEFORE pushing your changes.

If you haven't already, please read [[Creating New Windows Installer]] *BEFORE* continuing.

------------
| Test run |
------------
   
1). Unzip a fresh ```TSGL-master``` folder (the one downloaded from GitHub).

2). Copy over the ```winVS****``` folder and installer from your cloned TSGL workspace folder over to the freshly unzipped ```TSGL-master``` folder.

3). As you would normally do, run the new installer from the freshly unzipped ```TSGL-master``` folder. Hopefully, the new ```VS****``` components will be in the correct places. 

Fire up the ```VS****``` solution, make sure that ```Debug``` is ```x86```, and ```Rebuild``` the solution.

Run a test, any test. They *ALL* should work.

If they do, then congrats! You have successfully created a new VS solution for TSGL.

Be sure to update [readMeWindows.txt](https://github.com/Calvin-CS/TSGL/blob/master/readMeWindows.txt) afterwards to include the new VS version!

--------
| MISC |
--------

***NOTE***: Always wait until VS says "Ready" at the bottom left corner when adding projects, building, etc...

***NOTE:*** Delete the ```x64``` folders from the ```Property Manager``` section for *ALL* projects. Delete ```.VC``` files (if you can).

***NOTE:*** Delete the ```Debug``` folder from the VS**** folder. 

***NOTE:*** If you ever want to make an edit to the solution:

1). Move the hidden ```.suo``` file into ```VS****``` file and call it ```tsgl.v**```, replacing ```**``` with whatever toolset version you are using. (It'll be in a hidden folder called ```vs```, inside of the cloned TSGL workspace folder.)

2). Take the ```include``` and ```lib``` folders from an unzipped ```TSGL-master``` folder (that's been installed), and place them inside of the cloned TSGL workspace folder. 

3). Then, move the ```VS****``` folder, the ```tsgl``` solution and project folders, ```tsgl.v**.suo``` file, and ```testProperties``` sheet out of the ```winVS****``` folder and into the cloned TSGL workspace.

4). Make as many edits to your heart's content.

***PLEASE*** update this document if you come across new links, or if there's missing/false information.