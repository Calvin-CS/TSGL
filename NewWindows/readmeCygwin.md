# TSGL installation on Windows using Cygwin


### Installing Cygwin

1. Download [Cygwin](https://www.cygwin.com/)
2. Install the setup file; Continue until you get to the "Select Packages" window
3. Click View > Not Installed
4. In the search box type `lynx` and click the dropdown next to "Skip" and select the last one
5. Do the same for `dos2unix`
6. Click "Next" and finish the installation

##### Check

- Open a cygwin terminal and run `startxwin`
- On another terminal run `export DISPLAY=:0.0`
- Run `xeyes` in the terminal
- You should see a new window with a pair of eyes

![me](https://github.com/samuelth47/TSGL/blob/fix-cygwin-ubuntu/NewWindows/Cygwin.gif)

### Installing TSGL

1. Clone the [TSGL repository](https://github.com/Calvin-CS/TSGL.git) inside a cygwin terminal
2. Run the install-cygwin.sh file ( `./install-cygwin.sh` )
   - If it doesn't work run `dos2unix install-cygwin.sh` and try it again
2. cd to src/examples and run `make`
3. cd to one of the examples and run the ./test... files
