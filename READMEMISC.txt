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
4). git submodule init && git submodule update --remote.
5). cd into docs, type git branch, and make sure it is on the gh-pages branch. 
    If not, type git checkout gh-pages from within docs.
6). Navigate into the docs-wiki folder, type git branch, and make sure that you 
    are on the master branch. If not, type git checkout master from within
    docs-wiki.
7). To update the wiki pages, first cd into the docs-wiki folder, and type
    git pull origin master. Then, navigate to the main TSGL root folder, add
    docs-wiki, commit and push it to master.
8). To update the Doxygen, first cd into docs, type git pull origin gh-pages.
    Then, follow the steps from the main TSGL root directory in the "FAQS"
    on how to update Doxygen.

***NOTE*** It is recommended that you first update the docs-wiki folder
           (git pull origin master) before you update the docs folder
           (git pull origin gh-pages). 

 
