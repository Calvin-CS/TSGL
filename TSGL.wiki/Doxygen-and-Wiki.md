***NOTE:*** This process is for people who have cloned the repository on Linux/Mac. Windows users may use [Git Bash](https://git-scm.com/download/win), though that has not been tested.

***NOTE:*** Before updating the Doxygen, please see the [READMEMISC.txt](https://github.com/Calvin-CS/TSGL/blob/master/READMEMISC.txt) file. **You will not be able to update the Doxygen correctly if you fail to read [READMEMISC.txt](https://github.com/Calvin-CS/TSGL/blob/master/READMEMISC.txt) first!**

***BIG NOTE:*** You ***NEED*** doxygen 1.8.6 in order to update the documentation correctly. Previous versions will cause a ***LOT*** of problems.

In order to provide an API and extensive documentation for the TSGL library, we rely on a tool known as [Doxygen](http://www.stack.nl/~dimitri/doxygen/).

We also use the [Wiki](https://github.com/Calvin-CS/TSGL/wiki) pages to provide important information about the TSGL library (Debian and RPM packages, FAQs, etc...).

If you take a look at our [Doxygen](http://calvin-cs.github.io/TSGL/html/pages.html) pages, you'll notice that our Wiki pages also show up as well as the documentation for the TSGL code.

Which means, in order to keep these two up-to-date, whenever you make a change to the Wiki pages, you'll have to also update the Doxygen pages.

If you only added new documentation to the TSGL source code, then you do ***NOT*** have to update the Wiki pages. You simply have to update the Doxygen pages.

We provide two different processes for each situation. Follow the one that matches closely to your situation.

## Updating the Doxygen pages only ##

1). Navigate into the TSGL root directory (where the Makefile is).

2). Type "make cleandocs".

3). cd into the docs/ directory.

4). Type "rm -r html".

5). cd back out of the docs/ directory (cd ../).

6). Type "make docs".

7). cd into the docs/ directory again.

8). Type "git add --all" .

9). Do a "git commit -m "" " inside of the docs/ directory.

10). Type "git push origin gh-pages" . This should push the new Doxygen pages to our gh-pages branch (where our documentation ***SHOULD*** reside).

11). cd back out of the docs/ directory (cd ../).

12). Type "git add ." .

13). Do a "git commit -m "" ".

14). Type "git push origin master". This should update the docs/ submodule inside of our main GitHub branch.

## Updating the Wiki and Doxygen pages ##

1). Make any changes to the Wiki pages and save them. 

2). Navigate into the TSGL root directory (where the Makefile is).

3). Type "git submodule update --remote". This should cause the docs-wiki/ submodule to update (and docs/ if it has been updated recently).

4). Type "git add ." . This should add the docs-wiki/ submodule (and possibly the docs/ submodule if it has been updated recently).

5). Do a "git commit -m "" ".

6). Type "git push origin master" . This should push the changes to the main GitHub branch.

7). Type "make cleandocs".

8). cd into the docs/ directory.

9). Type "rm -r html".

10). cd back out of the docs/ directory (cd ../).

11). Type "make docs".

12). cd into the docs/ directory again.

13). Type "git add --all" .

14). Do a "git commit -m "" " inside of the docs/ directory.

15). Type "git push origin gh-pages" . This should push the new Doxygen pages to our gh-pages branch (where our documentation ***SHOULD*** reside).

16). cd back out of the docs/ directory (cd ../).

17). Type "git add ." .

18). Do a "git commit -m "" ".

19). Type "git push origin master". This should update the docs/ submodule inside of our main GitHub branch.
