
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
