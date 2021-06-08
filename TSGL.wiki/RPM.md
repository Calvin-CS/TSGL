***NOTE:*** This page is for Fedora/RedHat users. If you are an Ubuntu/Debian user, please see the [[Debian (Aptitude)]] section!

***NOTE:*** These steps are for Fedora users only. CentOS users can use the ```install-centos``` script. 

As of Summer 2016, an RPM package has been made for easy installation of TSGL.

This means that you can use yum/dnf to install TSGL on your machine.

Follow these steps in order to do so:

1). Add the OBS repo.

* ```cd``` into the ```/etc/yum.repos.d/``` directory.

* Type ```sudo su```.

* Create this file: ```home_TSGL_Maker.repo```.

* Using your favorite text editor, add these changes to the ```.repo``` file:

```
        [home_TSGL_Maker]
        name=home:TSGL_Maker (Fedora_24)
        type=rpm-md
        baseurl=http://download.opensuse.org/repositories/home:/TSGL_Maker/Fedora_24/
        gpgcheck=1
        gpgkey=http://download.opensuse.org/repositories/home:/TSGL_Maker/Fedora_24//repodata/repomd.xml.key	
        enabled=1

```

2). Clean yum/dnf. 

* Type ```sudo dnf clean all``` (or ```sudo yum clean all``` for Fedora versions earlier than 24).

* Type ```sudo dnf update``` (or ```sudo yum update``` for Fedora versions earlier than 24).

3). Install tsgl.

* Type ```sudo dnf install tsgl``` (or ```sudo yum install tsgl``` for Fedora versions earlier than 24). 

The TSGL package will be installed on your machine along with a packaged version of GLFW. 

As a result of installing the TSGL package, the generic Makefile for building programs will be located in ```/usr/include/TSGL_GENERIC_MAKEFILE/```. Copy the Makefile over to your project directory in order to create applications that utilize the TSGL library.

***NOTE:*** In order to receive updated TSGL packages from the OBS repo, you will have to repeat step 2. This ensures that the OBS repo appears as updated on your machine. 