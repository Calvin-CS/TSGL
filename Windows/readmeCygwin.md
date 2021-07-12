# TSGL installation on Windows using Cygwin


### Installing Cygwin

1. Download [Cygwin](https://www.cygwin.com/)
2. Install the setup file; Continue until you get to the "Select Packages" window
   - **NOTE**: If the window crashes, open the setup file and follow the steps again
4. Click View > Not Installed
5. In the search box type `lynx` and click the dropdown next to "Skip" and select the last one
6. Do the same for `dos2unix` and `wget`
7. Click "Next" and finish the installation

![me](https://github.com/samuelth47/TSGL/blob/master/Windows/Cygwin.gif)

### Installing TSGL

1. Open a Cygwin terminal
2. Clone the [TSGL repository](https://github.com/Calvin-CS/TSGL.git) and cd into it
3. Run the install-cygwin.sh file ( `source install-cygwin.sh` ) 
   - If you want to change the installation directory from the default, you can append a path when running the installer(`source install-cygwin.sh /usr`)
   - `/usr` is the default installation directory
   - If running the install script doesn't work run `dos2unix install-cygwin.sh` and try it again
   - It may take a while so please be patient

      ##### Check
      - Open a new cygwin terminal and run `startxwin`
      - Run `xeyes`
      - You should see a new window with a pair of eyes
 
2. cd to src/examples and run `make`
3. cd to one of the examples and run the test files
