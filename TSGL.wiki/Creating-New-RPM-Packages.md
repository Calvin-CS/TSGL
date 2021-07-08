---------------------------------------------------------------------------
| CREATING NEW RPM PACKAGES FOR RED HAT (FEDORA) (Last updated: 08/05/16) |
---------------------------------------------------------------------------

***NOTE:*** This document guides you in the process of creating RPM packages by hand. The package creation process has already been automated, so you do not need to follow this document if the automation process is working fine. If, however, the automation process has failed and you need to make an RPM package by hand, please continue to read the document. ***DO NOT ATTEMPT TO CREATE AN RPM PACKAGE BY HAND IF THE AUTOMATION PROCESS IS STILL FUNCTIONAL!!!***

***NOTE:*** This document attempts to guide you in the process of RPM package creation. Given that, package creation can be a long and complex process. If there's something that we missed, or perhaps have misleading information, please edit this page accordingly!

Hey there!

So, you need to create a new RPM package for Red Hat (Fedora). 

Package creation is no simple task. 

It is a complex procedure, with a steep learning curve. 

Fortunately, this document will guide you in the creation of RPM packages.

Before we continue, make sure that you are on a machine that has Red Hat on it! (Preferably Fedora!)

Also, make sure that your graphics drivers are up-to-date. (TSGL needs at least OpenGL 3.0 in order to function correctly). There are many online tutorials that will help you do this. 

Got all that? Okay, we're ready to go!

--------------
| Setting up |
--------------

Okay, first we need to set you up so that you can build and test RPM packages.  

Open up a terminal, and type the following:

```

sudo dnf install fedora-packager rpmdevtools

```

(For Fedora 23 and lower, use ```yum``` instead of ```dnf```). 

This should set you up with the necessary tools. Once they are all installed, make sure that you are in your home directory. Then, type:

```
    rpmdev-setuptree

```
(If you get a permission denied error, use ```sudo```).

What this command does is setup all of the directories that will be used as we create RPM packages. If you do a quick ```ls```, you will see that a new directory named ```rpmbuild``` has been created. ```cd``` into it, and do a quick ```ls```. You should see the following directories:

```BUILD``` - where your application/library is unpacked and compiled. 

```RPMS``` - where the binary RPMS are located (what gets installed on a user's machine).

```SOURCES``` - where the source code for the application/library should be when you create an RPM (e.g. the upstream tarball, patch files, etc).

```SPECS``` - where SPEC files should go (more on those later).

```SRPMS``` - where the source RPMS are located (sRPMs are files that contain the upstream tarball and patch files for your application/library). 

Each one of these directories will be utilized in the creation of RPM packages. 

That concludes the "Setting up" section!

-----------------------------------
| SPEC file: the heart of an RPM |
-----------------------------------

Regardless of what package you are trying to make, there's always some sort of metadata involved.

For Debian, it's the debian files (control, changelog, rules....). 

For RPM, it's the SPEC file. 

We like to call the metadata the "heart" of the package.

The reason is because, without metadata, it would almost be impossible to create a package.

The metadata specifies dependencies, how to build the application/library, and more. 

In the case of an RPM package, the SPEC file does all of this. 

SPEC files are not hard to make, though they can get complicated. 

Let's start out with a simple one: 

### A simple SPEC file

```

Summary:

Name: 

Version:

Release:

Group: 

License: 

URL:

Source:

BuildRequires:

Requires:

%description

%prep

%build

%install

%files

%changelog

```

There are 9 different sections included within a SPEC file, 7 of which are shown. Let's go through each one:

1). The Preamble

```

Summary:

Name: 

Version:

Release:

Group: 

License: 

URL:

Source:

BuildRequires:

Requires:

```

This puppy is a human-readable form for the specification of the RPM package being created. Each tag has a function:

```Summary``` - a one sentence summary of the application/library being packaged (do NOT end it with a period). 

```Name``` - the name of the package.

```Version``` - the version of the software being packaged.

```Release``` - how many times the software has been packaged up.

```Group``` - how the packaged software should be grouped with other packages (e.g. Development/Library/Other) (type less /usr/share/doc/rpm/GROUPS in a Terminal for more names)

```License``` - the open source license protecting the software. 

```URL``` - a website that gives more information for the software being packaged.

```Source``` - the source code being packaged. (Usually a URL, but not for our purposes).

```BuildRequires``` - what libraries need to be present in order to build the application/library.

```Requires``` - what libraries need to be present in order for the application/library to function correctly. (The libraries may or may not be the same as those in ```BuildRequires```). 

There are many more tags, but these should suffice. 

2). ```%description``` - a longer explanation of the software being packaged. 

3). ```%prep``` - prepares the sources and gets stuff ready in order to build the application/software. This is similar to the configure step in a standard configure-make-make install process. You can just use the %setup macro here, which unpacks the sources and cd's into the source directory. 

4). ```%build``` - the compilation and building of the application/library occurs here. Similar to the make part of the standard configure-make-make install. If you have a Makefile in your sources, you can simply put make in here. 

5). ```%install``` - installs your application/library on a user's machine. Similar to the make install part of the standard configure-make-make install. If your Makefile already has a make install section, place make install here. 

6). ```%files``` - lists the files that are included in the binary package. You *MUST* have this section in the SPEC file, and it must list *ALL* files that have been installed. 

7). ```%changelog``` - this lists the changes that have been made to the RPM package. Format for a new entry in the change log is: 

```
	* <date> <packager's Name> <packager's-email> <version-revision>

```
Then, under that line, you create a list of changes made:

```
	- Change #1.
	- Change #2.
	.
	.
	.

```

The other two sections are ```%clean```, which cleans up after the installation of an RPM, and scriptlet sections which peform various tasks before the installation of an RPM, after the installation, before the removal of an RPM, and after the removal of an RPM.

We will not worry about the ```%clean``` section for the TSGL SPEC file.

Unfortunately, the simplified SPEC file above will *NOT* suffice for our TSGL SPEC file. 

You can see why by looking at an old TSGL SPEC file below:

```

# specfile for package TSGL

Summary: A Thread-Safe Graphics Library
Name: tsgl
Version: 1.1
Release: 1%{?dist}
Group: Development/Libraries/Other
License: GPLv3
URL:https://github.com/Calvin-CS/TSGL
Source: tsgl-1.1.tar.gz
#This tag tells rpm which packages need to be on the system to build TSGL (like Build-Depends)
BuildRequires: glfwTSGL, libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel mesa-libGLU-devel libXmu-devel libXi-devel libGL-devel glew-devel freetype-devel doxygen
#This tag tells rpm which packages need to be on the system (like Depends)
Requires: glfwTSGL, freetype, glew

#https://fedoraproject.org/wiki/Packaging:Debuginfo#Useless_or_incomplete_debuginfo_packages_due_to_other_reasons
#It is not necessary to generate debug packages
%global debug_package %{nil}

%description
A Thread-Safe Graphics Library which draws 2D graphics in parallel.

#This is the preparation step.
#Simply use the setup macro. 
%prep
%setup

#This is the build step.
#Use the master Makefile to make the library.
%build
make 

#http://www.tldp.org/HOWTO/RPM-HOWTO/build.html
#This is the install step.
#Create any directories (lib64, TSGL, GENERIC_MAKEFILE).
#Install the files into those directories (install is like cp, but you can set file 
#permissions on the fly).
%install
mkdir -p $RPM_BUILD_ROOT/usr/lib64
mkdir -p $RPM_BUILD_ROOT/usr/include/TSGL
mkdir -p $RPM_BUILD_ROOT/usr/include/TSGL_GENERIC_MAKEFILE

install -m 0644 lib/libtsgl.a $RPM_BUILD_ROOT/usr/lib64/ 
install -m 0755 lib/libtsgl.so $RPM_BUILD_ROOT/usr/lib64/
install -m 0777 genericMakefile/Makefile $RPM_BUILD_ROOT/usr/include/TSGL_GENERIC_MAKEFILE/
install -t $RPM_BUILD_ROOT/usr/include/TSGL src/TSGL/*.h

#This is the postinstall step.
#Create a symlink to the OpenGL library.
%post
ln -sfn /usr/lib64/libGL.so.1 /usr/lib64/libGL.so
exit 0

#This is the postuninstall step. 
#Remove the symlink created in the postinstall step.
%postun
unlink /usr/lib64/libGL.so
exit 0

#These files belong to the tsgl package.
#The license file, libtsgl.so, libtsgl.a, and the Makefile.
%files
%license license
/usr/lib64/*
/usr/include/*

#https://docs.fedoraproject.org/en-US/Fedora_Draft_Documentation/0.1/html/Packagers_Guide/sect-Packagers_Guide-Creating_a_Basic_Spec_File.html
# * date Packager's Name <packager's-email> version-revision
# - Summary of changes
%changelog
* Wed Jul 6 2016 Chris Dilley <codemonkey941@gmail.com> 1.0-1
- Created the first TSGL rpm package.
- Uploaded to openSUSE build server, tested installation on Fedora 24. 
- Added post, postun, and pre sections to SPEC file.

```

As you can see, we have the 7 sections, but we also have a lot more than that. 

Here's the extra stuff that we have:

* ```%post```, ```%postun```.

	+ These are scriplets which are run before and after installing the TSGL package, as well as after removing the TSGL package. The ```%post``` scriplet makes a symlink to the correct OpenGL library (if one doesn't already exist). The ```%postun``` scriplet removes the symlink and the generic Makefile directory. Each scriplet *MUST* exit with 0 (to show that it executed successfully).

* ```%install``` section.

	+ We have a make install section in our master Makefile for the TSGL library, but we ran into some problems when attempting to use it for the %install section. (In particular, "permission denied" problems). So, we decided to install each file manually. 

	+ We had to create ```/usr/lib64```, ```/usr/include```, and ```/usr/include/TSGL_GENERIC_MAKEFILE``` directories whenever we simulated a build of the package. This is because they are not there by default whenever you simulate a build. You must include ```$RPM_BUILD_ROOT``` in the directory path. See http://stackoverflow.com/questions/8084796/what-actually-is-rpm-build-root for reasons why.

* ```%global debug_package %{nil}```

	+ Whenever an RPM package is created, debuginfo packages are also created (see https://fedoraproject.org/wiki/Packaging:Debuginfo#Useless_or_incomplete_debuginfo_packages_due_to_other_reasons). We do NOT need those for TSGL, and so as a result, they are not built. This causes errors in the build process, and so we needed to define a global macro which told our build repos to not make debuginfo packages.  

* ```%files``` section

	+ The ```%license``` macro specifies the license for the software and separates it from documentation files. See https://fedoraproject.org/wiki/Changes/Use_license_macro_in_RPMs_for_packages_in_Cloud_Image

	+ The ```*``` after the directories says that whatever the binary package put into those directories belongs to it. This is an easy way to specify what files belong to the binary package without having to know what files are installed by it.

This makes the TSGL SPEC file a bit more complicated than your run of the mill SPEC file. 

If anything needs changing, please make your changes to this version of the SPEC file. 

That includes new library dependencies that have not been added, new version number, etc.

Save your changes in a file named ```tsgl.spec```, and place that file in the ```SPECS directory``` inside of the ```rpmbuild``` directory. 

That concludes this section!

--------------------
| Building the RPM |
--------------------

The heart of the RPM package isn't the only thing you need.

You've got most of what you need to create an RPM, but you're missing a key ingredient.

Can you think of what it is?

Hint: there's code involved. 

The upstream tarball.

You need that in order to build an RPM. 

Why? Think about it.

The upstream tarball contains the source code needed in order to build your application/library... 
You need to build your application/library before installing it onto a user's machine...
Because you can't create the TSGL .so and .a files out of thin air....
You can only create them through compilation...
Which involves the TSGL source code...

See where we're going with this?

To create the upstream tarball, first make sure that you have the most recent version of the TSGL source code. 

You can get this either by downloading the zip file from our GitHub repo, or by cloning the repo with this command: 

```

	git clone https://www.github.com/Calvin-CS/TSGL.git

```

If you've already done this, and have an edited version of the code, use that. 

Make sure that you know the version number associated with it. This is usually found in the SPEC file, so whatever version you put in the SPEC file, make sure that that's the version you use!

Now, create a directory named ```tsgl-<version-number>```, replacing ```<version-number>``` with the version number that TSGL is currently. 

Copy all files from your TSGL source code directory over to th. 

After that, type:

```

	tar xczf tsgl-<version-number>.tar.gz tsgl-<version-number>

```
This will create your upstream tarball. 

Usually, upstream tarballs go into the ```SOURCE``` directory that's in your ```rpmbuild``` directory. Place the newly created upstream tarball inside of that directory. 

Now, the moment of truth.

Go into the ```SPECS``` folder, and type this command:

```
	
	rpmbuild -ba tsgl.spec

```

You should see a bunch of output text. If you see ```exit 0``` at the end of all of that, then the build was successful!

The RPM package should be located in ```RPMS``` and the source RPM should be located in ```SRPMS```.

That concludes the building of RPM packages!

--------
| MISC |
--------

If you need anymore help, or something is unclear, or you have run across an error, please see these links: 

https://fedoraproject.org/wiki/How_to_create_an_RPM_package#Getting_ready_to_package_a_particular_program

http://www.rpm.org/max-rpm/s1-rpm-build-creating-spec-file.html

http://www.rpm.org/max-rpm/ch-rpm-build.html

https://fedoraproject.org/wiki/Packaging:Scriptlets

https://docs.fedoraproject.org/en-US/Fedora_Draft_Documentation/0.1/html/Packagers_Guide/sect-Packagers_Guide-Creating_a_Basic_Spec_File.html

http://www.tldp.org/HOWTO/RPM-HOWTO/build.html

If you find anymore links, or want to update this document, please change the "Last updated:" date above and make your changes. (This will help future students down the road!)
