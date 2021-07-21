***WORK IN PROGRESS!!!***

------------------------------------------------------------------------------
| HOW TO CREATE NEW TSGL DEBIAN PACKAGES FOR UBUNTU (Last updated: 08/11/16) |
------------------------------------------------------------------------------

***NOTE:*** *Before* reading this page, please make sure that you have read the following pages:
    
* [[GPG Keys and You]]
* [[PPA and our TSGL team]]

(Example of making the same package for a different distro:  tsgl (1.1-1~<distro-name>) <distro-name>

If you are creating a new TSGL package for a new distro, see ***NEW PAGE COMING SOON***

***NOTE:*** This document attempts to guide you in the process of Debian package creation. Given that, package creation can be a long and complex process. If there's something that we missed, or perhaps have misleading information, please edit this page accordingly!

Hello!

So, you need to make a new Debian package for TSGL. 

Package creation isn't trivial, and has a steep learning curve.

What's more, we think that creating a Debian package is a ***LOT*** more complex than creating an RPM package. 

The reason? Instead of having to keep track of one file (SPEC file), you have to keep track of multiple. 

Luckily, this document will attempt to guide you through the process. 

Let's get started with a brief intro to Debian packages. 

-----------------------------------
| Creating Debian packages: intro |
-----------------------------------
Before we can even begin to talk about how to build a Debian package, we need to know what's inside one.

So, what exactly is inside of a Debian package?

Magic? Close.

There are three components to a Debian package:  

1). Upstream tarball - the original source code for the library/application. This comes from developers "upstream", which means that they are the original authors of the code.

2). Source package - has all of the necessary files needed to compile/build the software. Includes: the upstream tarball with ```.tar.gz``` ending, a description file (```.dsc```), and a tarball with any changes made to the upstream source code and all files created for the Debian package (usually has a ```.debian.tar.gz``` or ```.diff.gz``` ending). 

3). Binary packages - what actually get installed on a user's system. 

You usually have to get the upstream tarball from some developers who are working on a piece of software. 
(You type ```apt-get source <package-name>``` in order to fetch it).  

In this case, however, the upstream tarball will be created by you. 

Therefore, there is no need to get an upstream tarball. You'll create one yourself!

Follow these steps:

1). Create a new folder and call it ```tsgl-<version-number>```, replacing ```<version-number>``` with whatever version TSGL is currently. 

(If you've made minor changes, like new features or improvements, bump up the middle number by 1. 
 
 e.g. 1.1.1 -> 1.2.1.
 
 If you've added a bug fix, bump up the third number by one (or add 1 if there is no third number).
 
 e.g. 1.3 -> 1.3.1 OR 1.1.1 -> 1.1.2

 If you've made only improvements to the documentation, make no changes to the version number.)

2). Go to wherever you have the TSGL folder (which you can get using git if you don't have it; ```git clone https://www.github.com/Calvin-CS/TSGL.git```).

3). Take *ALL* of the files in the TSGL folder, and copy them over to the ```tsgl-<version-number>``` folder.

Okay, now we'll create the actual upstream tarball.

1). Open up a terminal, and go to the place where the ```tsgl-<version-number>``` folder is located.

2). Type:

```
	
	tar czvf tsgl-<version-number>.tar.gz tsgl-<version-number>

```

3). Type ```ls```, and you should see files that look like this: 

```
	
	tsgl-<version-number>.tar.gz tsgl-<version-number> 

```

Congrats! You created the upstream tarball, and have taken your first step in making a Debian package.  

Now, let's take a closer look at the intricacies of debian package creation. 
 
----------------------------------------------------------
| Creating Debian packages: files, files, and more files |
----------------------------------------------------------

Now that you have the upstream tarball created, it's time to get into the nitty gritty details of Debian packaging.

We'll go into preparing the library so that it can be packaged up.

You know how you have to prepare a turkey before actually cooking it?

The process is somewhat similar when creating a Debian package.  

Go into the ```tsgl-<version-number>``` folder after creating the upstream tarball. Create a folder named ```debian```.

This one folder, ```debian```, will store all of the necessary files needed in order to create a Debian package. 

And quite a lot of files we need....

+ ```changelog```
+ ```control```
+ ```rules```
+ ```compat```
+ ```tsgl.install```
+ ```copyright```
+ ```source/format```
+ ```postinst```

There are more, but these will suffice. 

You're going to have to create each and every one of these files.

Like we said, a bit more complicated than RPM package creation. 

Let's get started with the ```changelog``` file.

------------------------
| ```changelog``` file |
------------------------

This file essentially explains what has been changed in the Debian package. Ranging from a minor change to the ```control``` file (explained shortly), or a full scale rework of the entire package, you have to explain what you changed in this file. 

Don't worry, you don't have to explain ***EVERY*** little detail. You only have to summarize your changes. 

This file is also ***EXTREMELY*** picky in regards to format. There is a specific format for this file that *MUST* be followed. 

Since you're creating a brand new package, we assume that the only thing that has been changed is the TSGL source code.   

To create the ```changelog``` file, be in the ```tsgl-<version-number>``` directory, and type this command:

```

    dch --create -v <version-number> --package tsgl

```
(If ```dch``` isn't installed, type ```sudo apt-get install devscripts``` and repeat this step).

You'll be asked to pick an editor. Pick whichever one is the easiest one to use. 

Now, you should see something like this: 

```

    tsgl (<version-number>-<debnumber>) UNRELEASED; urgency=medium

      * Initial release. (Closes: #XXXXXX)

     -- kodemonkey <kodemonkey@kodemonkey-desktop>  Mon, 06 Jun 2016 15:56:57 -0400

```

Let's break it down:

+ ```tsgl (<version-number>-<debnumber>)``` specifies the name of the package and the current version number. The ```<debnumber>``` tells us what version is the DEBIAN package. You can create multiple versions of the same package, without changing the source code of the package software. Usually this is when a new dependency needs to be added, or a new file needs to be added to the ```debian``` folder. In this case, since we are creating it, set this number to 1. 

If this were an already created package, and we didn't make any changes to the TSGL source code, but made changes to the ```debian``` files, we would have to add 1 to the ```<debnumber>``` 

E.g. tsgl (1.1.2-1) -> tsgl (1.1.2-***2***)

+ ```UNRELEASED``` specifies the Linux distro that we developed the library for. 

```trusty``` is for ```Ubuntu 14.04```, ```xenial``` is for ```Ubuntu 16.04```.

If you are creating a new TSGL package for either one of those distros, replace ```UNRELEASED``` with the distro name. 

+ ```urgency=medium``` specifies the priority level for this package. You can leave it as is. 

+ ```* Initial release.``` specifies what has been changed in the library. You can leave it as is. 

+ Remove the ```(Closes: #XXXXXX)``` as that is for bug fixes.

+ ```-- kodemonkey <kodemonkey@kodemonkey-desktop>  Mon, 06 Jun 2016 15:56:57 -0400``` specifies the person who made the changes (```kodemonkey```), an email that you can contact them by (```kodemonkey@kodemonkey-desktop```), and the time and date that the changes were made. 
  
You'll want to replace the name and email with the Real Name and Email associated with your GPG key (see [[GPG keys and You]]).

You should end up with a file like this after changes have been made:

```

    tsgl (1.0-1) trusty; urgency=medium

      * Initial release.

     -- Chris Dilley <codemonkey941@gmail.com>  Mon, 06 Jun 2016 15:56:57 -0400

```

Save the changes and close the file.

Next, let's take a look at the ```control``` file.

----------------------
| ```control``` file |
----------------------

This file tells ```apt-get``` how to manage the package. 

It's also a real workhorse of a file. 

Here's the general format:

```

    1 Source:
    2 Maintainer:
    3 Section:
    4 Priority:
    5 Standards-Version:
    6 Build-Depends: 
    7
    8 Package:
    9 Architecture:
    10 Depends:
    11 Description:

(Line numbers added)

```

As you can see, there are two sections. Lines 1-7 specify information for the source package, while lines 9-12 specify information for the binary package.

Let's break them down:

----SOURCE----

+ ```Source``` specifies the name of the source package. 

+ ```Maintainer``` specifies the person who is keeping track of the package in terms of updates, tweaks, etc.

+ ```Section``` species the section of the distribution that the source package goes into (libs, for example). 

+ ```Priority``` specifies the importance of this package. 

+ ```Standards-Version``` specifies the Debian standards for packaging that we are following. 

+ ```Build-Depends``` specifies what libraries need to be present in order to build our library.

----BINARY----

+ ```Package``` specifies the name of the package. Note that the Source and Package name fields do NOT have to be the same.

+ ```Architecture``` specifies the architectures that the binary package can be compiled for.

+ ```Depends``` specifies what libraries need to be present in order for our binary package to behave correctly.

+ ```Description``` specifies a short summary of what the package is. 

At the time of this writing, this was the ```control``` file for the ```trusty``` TSGL debian package:

```

    1 Source: tsgl
    2 Maintainer: Chris Dilley <codemonkey941@gmail.com>
    3 Section: libs
    4 Priority: optional
    5 Standards-Version: 3.9.5
    6 Build-Depends: debhelper (>= 9), glfw, doxygen, libglew-dev, libfreetype6-dev, libfreetype6, libxrandr-dev, libxi-dev, x11proto-xf86vidmode-dev, libglu1-mesa-dev
    7
    8  Package: tsgl
    9 Architecture: amd64
    10 Depends: ${shlibs:Depends}, ${misc:Depends}, glfw, libfreetype6, libfreetype6-dev, libglew-dev, libxrandr-dev, doxygen, devscripts, build-essential 
    11 Description: A Thread-Safe Graphics Library.

```

(Line numbers added)

The one for the ```xenial``` package is the same.

If nothing has changed (no new dependencies have been added...), simply copy and paste it into the new ```control``` file. 
(Be sure to remove the added line numbers!)

Change the ```Maintainer``` field to the Real Name and Email associated with your GPG key (see [[GPG keys and You]]).  

Save the changes and close the file.

---------------------
| ```compat``` file |
---------------------

This is the easiest file to make. Create the file, and type ```9```. Save the changes and don't touch the file again.

Essentially, all this file does is tell the packaging tools to use ```debhelper``` version 9. 

This means, "Use the tools which help build Debian packages from toolset version 9."

--------------------
| ```rules``` file |
--------------------

This file tells the build system how to make the binary package (in this case, the one that will be built in our PPA after we upload the source package). 

As mentioned above, there are three components to a Debian package: upstream tarball, source package, and binary package. 

We've already made the upstream tarball. 

Our source and binary packages, however, will be made for us. 

The source package will be made as a result of the work done in the next section. 

The PPA has a build system which takes our uploaded source package and creates a binary package out of that. 

It then publishes the upstream tarball, source package, and binary packages so that users can download and install tsgl.

You can upload binaries, though it is NOT recommended. 

At any rate, this is what the ```rules``` files looks like for both the ```trusty``` and ```xenial``` packages:

```

#!/usr/bin/make -f 

%:
	dh $@

override_dh_auto_install:
	#Do nothing.

```

The ```override_dh_auto_install:``` section tells the build system to not perform an automatic dry install of TSGL. 
Originally, we had the build system do the automatic install (to test whether it would install correctly), but we kept getting ```Permission denied``` errors whenever it tried to install the ```libtsgl.so``` file. 

This is why we had to override the auto install and create a ```tsgl.install``` file that would install the files manually (see next section). 

The ```%:``` tells the build system to use our ```Makefile``` in order to create TSGL.

If no changes need to be made, copy and paste the code section into the ```rules``` file.

Save the changes and close the file.

---------------------------
| ```tsgl.install``` file |
---------------------------

This file explains where files need to go when the package is being installed. 

Whenever something is installed on a user's system, certain files need to go to certain places.

An example would be an executable. Normally, you put executables inside of a user's ```/usr/bin``` or ```/usr/local/bin``` directory when it is being installed. 

This file specifies where certain files go on a user's system.

In our case, we need to put the ```libtsgl.so``` and ```libtsgl.a``` files into ```/usr/lib/```, the ```.h``` header files from ```src/TSGL``` into ```/usr/include/TSGL/```, and the generic ```Makefile``` into ```/usr/include/TSGL_GENERIC_MAKEFILE/``` (so users can create programs with TSGL with ease). 

Here is what the file looks like for both ```trusty``` and ```xenial```:

```

    lib/libtsgl.a usr/lib
    lib/libtsgl.so usr/lib
    src/TSGL/*.h usr/include/TSGL
    genericMakefile/Makefile usr/include/TSGL_GENERIC_MAKEFILE/

```

If no changes need to be made, simplu copy and paste it into ```tsgl.install```.

Save the changes and close the file.

----------------------------
| ```source/format``` file |
----------------------------

------------------------
| ```copyright``` file |
------------------------

This file is used to store licensing information and legal stuff associated with your software.

In much the same way as inventors patent their products in order to protect them, software developers use licenses in order to protect their software.

"Protection" means not having your ideas stolen, etc.

It's essentially a way of telling everyone, "This software is available for use, but with some restrictions."

In the case of TSGL, we use the GPL v3 license. 

This is located in the TSGL main folder, in a file called, "license".

Simply open that file up, copy the contents, and paste them into the ```copyright``` file. 

Save the changes and close the file.

-----------------------
| ```postinst``` file |
-----------------------

This file is usually a shell/bash script that is executed ***after*** the installation of the package.

You can do many things with this file.

We use it to change file permissions on the generic Makefile.

This is so that users can do whatever they wish to it.

Here is what the file looks like for the ```trusty``` and ```xenial``` packages:

```

    #!/bin/sh

    sudo chmod 777 /usr/include/TSGL_GENERIC_MAKEFILE/Makefile

```

If no changes need to be made, copy and paste it into the ```postinst``` file.

Save the changes and close the file.

Whew, that was a lot of files! 

Let's move onto the easiest part of Debian package creation: actually building one!

----------------------------------------------------------
| Creating Debian packages: ```debuild``` is your friend |
----------------------------------------------------------

So, you have created all of the files in the debian folder. 

Now all that's left to do is to create the files needed in order to create our full Debian package and upload said package. 

You'll also have to sign the package (so Debian knows that you made the package) with a GPG key, located in keyID.txt.

How do we do all that? 

With the ```debuild``` command.

The ```debuild``` command creates our Debian package and files needed in order to upload said package. 

Simply type this command either inside of the ```debian``` directory or ```tsgl-<version-number>``` directory:

```
	debuild -S -rfakeroot -k<key-id> 

```
(replacing <key-id> with the key ID located in key.txt).

and debuild takes care of the rest!

debuild -S -rfakeroot -k<key-id>
cd ..
ls
<package-name>.source_changes

--------
| MISC |
--------

Does something not make sense? Have we missed something? Take a look at these links for more information. 

https://wiki.debian.org/Packaging/Intro?action=show&redirect=IntroDebianPackaging 

https://en.wikipedia.org/wiki/Upstream_%28software_development%29

https://wiki.debian.org/Packaging/SourcePackage?action=show&redirect=SourcePackage

https://www.debian.org/doc/manuals/maint-guide/dreq.en.html

https://www.debian.org/doc/manuals/maint-guide/dreq.en.html#control

https://wiki.debian.org/Packaging/SourcePackage?action=show&redirect=SourcePackage#Working_with_a_source_package

***PLEASE*** update this document if you come across new links, or if there's missing/false information.
