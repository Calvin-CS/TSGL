1). Follow up to step 5: https://help.github.com/articles/generating-ssh-keys/
2). Type this command: ssh-keyscan -t rsa github.com > ~/.ssh/known_hosts
3). Go back to the TSGL folder.
4). Type: git submodule init
5). Type: git submodule update.
6). cd into docs, type git branch, and make sure it is on the gh-pages branch. 
    If not, type git checkout gh-pages from within docs.
7). Navigate into the docs-wiki folder, type git branch, and make sure that you 
    are on the master branch. If not, type git checkout master from within
    docs-wiki.
8). To update the wiki pages, first cd into the docs-wiki folder, and type
    git pull origin master. Then, navigate to the main TSGL root folder, add
    docs-wiki, commit and push it to master.
9). To update the Doxygen, first cd into docs, type git pull origin gh-pages.
    Then, follow the steps from the main TSGL root directory in the "FAQS"
    on how to update Doxygen.

 