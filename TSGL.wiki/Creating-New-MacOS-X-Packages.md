***WORK IN PROGRESS!!!"***

----------------------------------------------------------------------
| CREATING A NEW MAC OS X INSTALLER PACKAGE (Last updated: 08/11/16) |
----------------------------------------------------------------------

Creating a new ```.pkg``` Installer for Mac OS X isn't as difficult as creating an RPM or Debian package.

You do have to be on a Mac, however. 

If you cannot procure a Mac with OS X 10.5 or greater (or a VM with OS X 10.5 or greater on it, *though this has not been tested*), ***please stop reading and go get a Mac***.

Otherwise, please continue.

------------
| Packages |
------------

When we first started to create the Mac OS X Installer package, we used a tool called ```Packages```.

This made the process of creating a Mac OS X binary installer much easier and less tedious. 

You have to download it from this [link](http://s.sudre.free.fr/Software/Packages/about.html).

The download will be a ```.dmg``` file. Move it to your ```Desktop```. 

Now, mount it. (Double-click on it). You should see a Volume appear on your ```Desktop``` with the name, ```Packages #.#.#```. Double-click on it. 

Now, double-click on the ```Install Packages.pkg``` file. 

Follow the installation process, and you should have the ```Packages``` tool installed.

-------------------------------
| Creating the TSGL Installer |
-------------------------------

After installing ```Packages```, open up the application. 

A screen should pop-up with the title, ```New Project```. 

This is where you choose what project you'd like to make. 

We used the ```Distribution``` template, so go ahead and click on that option.

Click ```Next```. 

The project name will be ```InstallTSGL```, and the project directory will be ```~/Desktop```. 

A screen should appear showing you the project details. 

In here, you can edit the settings for the project, how the Installer looks, and more.  

On the left, you'll see a section named ```PACKAGES```. 

At the moment, there's only one package in it: ```InstallTSGL```. 

We're going to rename it to just ```TSGL```. 

Double-click on it, and when the text appears highlighted, type ```TSGL``` and hit ```ENTER```.

Now, take a look at the ```Settings``` page. 

This contains the settings that you can edit for this package. 

If you take a look at the text box next to the ```Identifier: ``` label, you can see that the Identifier for the package still has ```InstallTSGL``` at the end. 

We're going to change the ```InstallTSGL``` part to say ```TSGL```. 

Click on the text box next to the ```Identifier: ``` label, and delete the ```InstallTSGL``` part. Replace it with ```TSGL``` and hit ```ENTER```.

We can now leave the ```Settings``` page as it is. 

On the left, click on ```Project```. 

You'll be taken back to the ```Settings``` page for the Installer.

Leave the ```Settings``` page as is, and click on the ```Presentation``` tab. 

In here, you'll be able to customize the text for the ```Introduction```, ```Read Me```, and ```License``` screens. 

To do so, we must first create the text files that will hold the customized screen text.

Create three new text files: ```intro.txt```, ```README.txt```, and ```license.txt```.

We'll edit ```intro.txt``` first.

-------------------
| ```intro.txt``` |
-------------------
  
This is the first thing that a user will see when they double-click on the Installer package. 

Here's the introduction that we used:

```
    Hello! Thank you for choosing to install TSGL on your Mac!
	
    To start, please click "Continue".

```

Copy and paste this into your ```intro.txt``` file and make any changes. 

Save and close it when you are satisfied.

--------------------
| ```README.txt``` |
--------------------

This is the next thing that a user will see when they continue with the installation process.

Essentially, we specified what dependencies would be installed and with which package manager. 

```

    The following dependencies will be installed on your Mac:
	
        - X11 libs
        - Homebrew
        - Freetype (through Homebrew)
        - glfw3 (through Homebrew)
        - glew (through Homebrew)
        - doxygen (through Homebrew)
        - gcc5 (through Homebrew)

    By clicking "Continue", you agree to having these dependencies installed. 


```

Copy the above text and paste it into your ```README.txt``` file.

Make any changes as you see fit, then save and close the file.

---------------------
| ```license.txt``` |
---------------------

This is the last thing a user will see before selecting a destination, choosing installation type, and installing TSGL on their Mac.

We took the opportunity to use the license associated with TSGL (GPLv3).

This file is actually inside of the TSGL source code folder. 

Simply open up the TSGL source code folder (the clone or downloaded zip file), open up ```license```, and copy the text inside of it. Paste that text inside of ```license.txt```.

Save and close the file. 

----------------
| Almost there |
----------------

Now that we have those three text files made, it's time to integrate them into the Installer.

Back in ```Packages```, make sure that you are in the ```Project``` details (click on ```Project``` on the left side of the window), and on the ```Presentation``` page (click on the ```Presentation``` tab). 

In the drop down box on the right, make sure that it says ```Introduction```.

Then, click the big plus (```+```) button underneath the ```Custom Introduction``` table. 

One of the table slots inside of the ```Custom Introduction``` table should be filled in and highlighted. 

Click on the section that has the ```-``` in it, and click ```Choose``` from the drop down menu. 

Double-click on the ```intro.txt``` file that we made earlier. 

Now the text inside of the ```Introduction``` Installer page should change to the text inside of ```intro.txt```.

Do the same thing for the ```README.txt``` and ```license.txt``` files. 

Once you have done that, you are almost ready to create the ```.pkg``` Installer!

---------------------
| Installer plugins |
---------------------

Our Installer can have two shell scripts which are executed before and after TSGL is installed.

The ```preinstall``` and ```postinstall``` scripts, respectively.

We will go into more detail later on these scripts, but for now just know that they are needed for our Installer.

However, they are not the *ONLY* things that are needed.

As we developed the ```.pkg``` Installer, we focused primarily on making it self-contained. 

We wanted a user to simply mount our ```.dmg``` file, double-click on the ```.pkg``` file, and have TSGL installed on their Mac. 

We didn't want them to execute any programs beforehand; we wanted everything to be done by the ```.pkg``` Installer. 

This proved to be most difficult with just the ```preinstall``` and ```postinstall``` scripts.

Which is why, another resource was needed: an Installer plugin.

An Installer plugin is essentially another pane that you can add to the installation process.

What's important about this resource?

You can execute commands based off of user input.

Even shell scripts.

This was *VERY* useful for us when we had to install ```Homebrew``` as a dependency, as well as a solution to many problems, for the reasons outlined in the next section.

-----------------------------------------------------------
| The trouble with ```preinstall``` and ```postinstall``` |
-----------------------------------------------------------

As mentioned above, the ```preinstall``` and ```postinstall``` scripts are shell scripts which can be executed before and after TSGL is installed, respectively.

They are perfect for small tasks, like making symlinks or installing some dependencies. 

However, they are run as ```root```. 

This doesn't seem like such a big deal, but consider this scenario:

1). The ```preinstall``` script is executed when the user clicks ```Install``` during the TSGL installation process.

2). The script finds that a user doesn't have ```Homebrew``` installed.

3). It attempts to install ```Homebrew```. 

4). Because the script was run as ```root```, ```Homebrew``` refuses to install. 

5). No dependencies are installed, and the installation process shows that TSGL was installed correctly (even though it wasn't).

See [the Homebrew FAQ page](https://github.com/Homebrew/brew/blob/master/share/doc/homebrew/FAQ.md) for more information on why using ```sudo``` and ```brew``` is a bad thing.

This was a recurring problem, with many workarounds attempted. 

One of them involved creating a separate executable that the user could run *before* the ```.pkg``` Installer, but that would mean that the Installer would no longer be self-contained. Strike 1.

You can execute the scripts as normal user by changing a setting inside of the ```.pkg``` file *after* it's been created.
That would be a workaround. 

However, that brings another problem to the table.

When we have to install ```X11``` libs, we use the ```curl``` command in order to download the ```xQuartz``` ```.dmg``` file which contains a ```.pkg``` file so that they can be installed. 

We then open up that file and tell the user to follow the installation process. 

But...two Installers *cannot* be running at the same time. 

If they are, one will wait for the other to finish before beginning. 

This *always* happened with the installation of the ```X11``` libs. 

The installation would wait until *after* the installation of TSGL was completed, then move onto installing ```X11``` libs. TSGL needs those libs in order to compile, and because they were not installed *before* TSGL, this would mean that TSGL would fail to compile and would *NOT* be installed correctly. 

A workaround for this would be to use the ```installer``` command and use it to install the ```.pkg``` file via a Terminal. 

But...the execution of the ```installer``` command could cause the same issue (two Installer running at the same time).

Even if it were to work, this workaround brings with it a new problem.

For some odd reason, the commands executed by the ```preinstall``` script are asynchronous.

What this means is that when one command is executed, the next command doesn't wait until the first has terminated. It executes, and the successive commands do the same.

This is really bad in terms of installing dependencies.

To give an example, consider ```Homebrew``` again. 

```Homebrew``` needs to be installed before ```glew```, ```glfw3```, ```doxygen```, ```freetype```, and ```gcc5```. 

If a user doesn't have ```Homebrew``` installed, the script needs to install ```Homebrew```.

As a consequence of the commands being executed asynchronously, the other dependencies will also be installed *regardless of the ```Homebrew``` install finishing or not*.

This would lead to another failed install. Strike 2.

There are other problems, but we won't go into them in detail. 

We attempted numerous workarounds, ranging from having the ```preinstall``` script execute other install scripts, to using ```sleep``` and ```wait``` commands. 

None of them provided a solid, foolproof binary Installer. Strike 3.

Given all of these reasons, we ultimately decided to go with Installer plugins.

They provided a solid, self-contained, foolproof Installer. 

We used one to install ```Homebrew```, and ```X11``` libs. 

We also utilized the ```sleep``` command in order to avoid having multiple dependencies installed at once (e.g. ```XCode Command Line Tools``` and dependencies installed by ```Homebrew```).

-----------------------------
| Back to Installer plugins |
-----------------------------

--------------------------
| Building the Installer |
--------------------------



---------------------------------
| Wrapping it up in a .dmg file |
---------------------------------

Now that you have the ```.pkg``` file created, it's time to wrap it up in a ```.dmg``` file.

Open up the ```Disk Utility``` application (which should be located in a folder called ```Utilities```).

This is where we'll be creating the .dmg file.





That concludes this page!





--------
| MISC |
--------

For more help with the Packages tool, please see this website: http://s.sudre.free.fr/Software/documentation/Packages/en/index.html


