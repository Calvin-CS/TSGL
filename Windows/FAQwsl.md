# FAQs for WSL

**Q**: I get this error when I try to run the examples
```
glfwGetVideoMode: Assertion 'Monitor != NULL' failed
``` 
**A**: Try running `export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0` <br>
       If that did not work, restart XLaunch and **DO NOT FORGET** to **Disable** `Native opengl` and **Enable** `Disable access control`

