***WORK IN PROGRESS!!!***

--------------------------------------------------------------------
| CREATING NEW WINDOWS INSTALLER FOR TSGL (Last updated: 08/11/16) |
--------------------------------------------------------------------

So, you need to create a new Windows installer for TSGL.

This probably means that you also created a new Visual Studio solution for TSGL.

If not, please see [[New Visual Studio Versions]] *before* continuing. 

Creating a new Windows installer isn't particularly difficult. 

But it does require a tool called "Inno Setup".

You can download the installer for it [here](http://www.jrsoftware.org/isdl.php). 

Be sure to download the ```innosetup-#.#.#.exe``` file from that website!

After downloading it, open it up. 


Rename the installer to "install-windowsVS****" and move it to the cloned TSGL workspace folder.
Delete all files EXCEPT for the freetype6.dll and zlib.dll files.
Rebuild Solution.
