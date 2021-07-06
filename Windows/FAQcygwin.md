# FAQs for Cygwin

### Installation

**Q**: What to do when Cygwin setup window crashes? <br />
**A**: Sometimes Cygwin installation window crashes unexpectedly. If that happens open the setup file and follow the [steps](https://github.com/samuelth47/TSGL/blob/master/NewWindows/readmeCygwin.md) again
<br />
<br />
**Q**: `source install-cygwin` is not working <br />
**A**: Try running `dos2unix install-cygwin` and run `source install-cygwin` again
<br />
<br />
**Q**: I am getting `wget is not installed, using lynx as fallback` error during installation <br />
**A**: You MUST install `wget, lynx, and dos2unix` using the setup file of Cygwin before running the install-cygwin.sh.
       Launch the setup file and follow these [steps](https://github.com/samuelth47/TSGL/blob/master/NewWindows/readmeCygwin.md), but this time only install `wget`


**Q**: It is taking too long to install <br />
**A**: For TSGL to work on windows it needs more packages to be installed. That is why it is taking long, but it's worth the wait :)

<br>

### Using TSGL

**Q**: I get this error when I try to run the examples
```
assertion "monitor != NULL" failed: file ".../glfw/src/monitor.c", line 445, function: glfwGetVideoMode
``` 
**A**: Did you open a new terminal and run `startxwin`? If not, do that, and don't stop the process nor close the terminal. <br>
       <t> If you did that and it still doesn't work, try running ```export DISPLAY:0.0``` on the terminal
