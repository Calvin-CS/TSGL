-------------------------------------------------------------
| IMPORTING AND EXPORTING GPG KEYS (Last updated: 08/09/16) |
-------------------------------------------------------------

Imagine yourself chugging along, building Debian packages, when all of a sudden, your computer crashes.

All of your data, lost! 

Especially...your GPG keys!

Okay, your GPG keys may be the last thing on your mind when your computer crashes. But, without them, you can't sign Debian packages.

"Ha, Right!" You say. "I remember my key ID! I can just use that when I use ```debuild```!" 
(See [[Creating New Deb Packages]] for more info on ```debuild```).

No. It doesn't work that way. You need both the public and private GPG key on the system in order to have the luxury of using just the key ID. 

"Well...I can get my key back from the keyserver on Ubuntu!"

Sorry, we tried to do that. No luck.

"Are you saying that I have to create a new GPG key?"

That's only the tip of it.

Not only do you have to create a new GPG key, you'll also have to:

* revoke your old key
* send the new key to the Ubuntu keyserver
* wait 10 minutes
* and memorize a brand new key ID. 

Here's [why](http://askubuntu.com/questions/191767/is-it-possible-to-recover-a-secret-key-from-a-gpg-public-key). 

Unless you exported your key beforehand, you are out of luck. 

This guide will help you make backups of your keys so that you can avoid a scenario like this.
(And will also remind you to make backups of your data! Especially your Debian package data!!)

------------------
| Exporting keys |
------------------

Type:

```
	
    gpg --export <key-id> > public.key
    gpg --export-secret-key <key-id> > private.key

```

Replacing ```<key-id>``` with the one associated with your GPG key (see [[GPG keys and You]]).

Move the ```.key``` files to a secure place (e.g. Google Drive, USB drive...)

That should keep your GPG keys safe.

------------------
| Importing keys |
------------------

In the event of a computer crash, hard drive malfunction, or some other unfortunate occurrence, follow this process to get your GPG keys back (assuming you exported them beforehand!!!).

1). Find the ```.key``` files that you exported. 

2). Put them on the system you wish to import the keys on.

3). Type:

```

    gpg --import public.key
    gpg --allow-secret-key-import --import private.key

```

4). Type:

```

    sudo add-apt-repository <ppa-link> 
    sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys <key-id> 
    sudo apt-get update 

```

Replacing ```<ppa-link>``` with ```ppa:tsgl-test/tsgl-dev```, and  replacing ```<key-id>``` with the one associated with your GPG key (see [[GPG keys and You]]).

This is so that you can send and receive packages to the PPA repo (assuming that you are allowed to; see [[PPA and our TSGL team]]).