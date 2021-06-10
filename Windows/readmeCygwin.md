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

1. Clone the [TSGL repository](https://github.com/Calvin-CS/TSGL.git) inside a cygwin terminal
2. Run the install-cygwin.sh file ( `./install-cygwin.sh` )
   - If it doesn't work run `dos2unix install-cygwin.sh` and try it again
   - It may take a while so please be patient

      ##### Check
      - Open a new cygwin terminal and run `startxwin`
      - On another terminal run `export DISPLAY=:0.0`
      - Run `xeyes`
      - You should see a new window with a pair of eyes
 
2. cd to src/examples and run `make`
3. cd to one of the examples and run the test files
