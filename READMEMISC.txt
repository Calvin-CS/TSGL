Last updated: 07/06/2016.

1). Follow these tutorials for adding an SSH key:
	* Windows:
		+ https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/#platform-windows
		+ https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/#platform-windows
	* Mac:
		+ https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/#platform-mac 
		+ https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/#platform-mac
	* Linux:
		+ https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/#platform-linux
		+ https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/#platform-linux

2). Type this command: ssh-keyscan -t rsa github.com > ~/.ssh/known_hosts.
3). Go back to the TSGL folder.
4). Type git submodule init
5). cd into docs, type git branch, and make sure it is on the gh-pages branch. 
    If not, type git checkout gh-pages from within docs.
6). Navigate into the docs-wiki folder, type git branch, and make sure that you 
    are on the master branch. If not, type git checkout master from within
    docs-wiki.
7). Read our "Doxygen and Wiki" page in order to update the correct submodule. 

***NOTE*** It is recommended that you first update the docs-wiki folder
           (git pull origin master) before you update the docs folder
           (git pull origin gh-pages). (If you have want to update the Wiki pages). 

***NOTE*** You may see docs show up as having modified content even after commiting and uploading your changes to the master branch.
	cd into docs, and type 'git checkout . '. This should fix the issue.
