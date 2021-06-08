***WORK IN PROGRESS***

-------------------------------------------------------
| UPDATING MAC OS X PACKAGES (Last updated: 08/12/16) |
-------------------------------------------------------

***NOTE:*** This page does *NOT* teach you how to create a new Mac OS X package. It merely tells you how to update the one that we currently have. **If you would like to learn how to create new MacOS X packages, please see [[Creating New MacOS X Packages]]**.

***NOTE:*** You *MUST* be on a Mac (with OS X 10.5 or greater) to continue.

To update the Mac OS X package for TSGL without having to use [Packages](http://s.sudre.free.fr/Software/Packages/about.html), mount the ```.dmg``` file from our repo, open up a Terminal and follow these steps:

1). ```cd``` into ```/Volumes/TSGL```.
2). Do a quick ```ls```. You should see the ```InstallTSGL.pkg``` file.
3). Type:

```

    pkgutil --expand InstallTSGL.pkg workspace 

```

This command will open up the ```.pkg``` file and store the contents inside of ```workspace```.

Do a quick ```ls```. You should now see the ```workspace``` folder.

4). ```cd``` into ```workspace```. 