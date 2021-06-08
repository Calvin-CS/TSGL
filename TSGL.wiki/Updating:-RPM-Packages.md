-----------------------------------------------------
| UPDATING TSGL RPM PACKAGE (Last update: 08/12/16) |
-----------------------------------------------------

***NOTE:*** This page does *NOT* teach you how to create a new RPM package. It merely tells you how to update the RPM package that we currently have. **If you would like to know how to create a new RPM package, please see [[Creating New RPM Packages]]**.

Take a look at the [Installers](https://github.com/Calvin-CS/TSGL/tree/master/Installers) folder located inside of our GitHub repo. 

You'll see that there are two items:

* a ```debian``` folder, 
* and the SPEC file for the TSGL RPM package.

The SPEC file is there because of how we automated the build of TSGL RPM packages using OBS.

It gets extracted from the downloaded GitHub zip file on OBS, and then used to make the RPM package.

If you would like to update that file (perhaps a new dependency has been added, documentation, etc.), simply edit that file and make a Pull Request in order to merge the new changes.

(Or do a git add-commit-push, if you are allowed to do so). 

The SPEC file changes and any changes to the source code will be integrated into the new RPM package. 