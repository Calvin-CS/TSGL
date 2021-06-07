# TSGL Installation on windows using WSL

**For this to work you need to install a WSL and an Xserver**

### Installing WSL and Xserver
1. [Install WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) (_Ubuntu recommended_)
2. Download and install [VcXsrv](https://sourceforge.net/projects/vcxsrv/)
   - It is downloaded as `XLaunch`
   - When starting XLaunch **disable** the `Native opengl` and **enable** `Disable access control`

<img src="https://github.com/samuelth47/TSGL/blob/fix-cygwin-ubuntu/xlaunch.gif" width="500" height="600"/>

##### Check
* Start XLaunch
* On your terminal run `xeyes`
  - If it doesn't work, run `export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0` then try again
* You should see a pair of eyes on the VcXsrc server

### Installing TSGL
1. clone the [TSGL repository](https://github.com/samuelth47/TSGL/blob/fix-cygwin-ubuntu/NewWindows/xlaunch.gif)
2. cd to TSGL
3. run the install-ubuntu.sh (`./install-ubuntu.sh`)

##### Check
* cd to src/examples
* run `make`
* cd to one of the examples and run the /test... file (`./test...`)
