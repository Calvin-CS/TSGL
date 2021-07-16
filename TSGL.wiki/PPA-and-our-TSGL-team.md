--------------------------------------------------------------------------------------------------
| INFORMATION ABOUT OUR PPA AND CONTRIBUTING TO THE TSGL DEBIAN PACKAGE (Last updated: 08/10/16) |
--------------------------------------------------------------------------------------------------

As of summer 2016, we currently have a PPA (Personal Package Archive) on [Launchpad](https://launchpad.net/) for our TSGL Debian packages.

You can visit our PPA by clicking [here](https://launchpad.net/~tsgl-test/+archive/ubuntu/tsgl-dev).

We also have a moderated team which allows us to add users, like you, so that the Debian package can be updated by others.

You will, however, need a Launchpad account in order to be added to our team.

---------------------
| Launchpad account |
---------------------

***NOTE:*** If you have already created a Launchpad account, feel free to skip this section and move onto the next.

Follow the process as described [here](https://login.launchpad.net/+new_account) in order to create an account.

Once you create your account (and have validated your email), log into [Launchpad](https://launchpad.net/). 

--------------------------------------
| Signing the Ubuntu Code of Conduct |
--------------------------------------

***NOTE:*** If you have already signed the Ubuntu Code of Conduct, feel free to skip this section.

Now, go to your profile (click on your username; it should be next to the ```Log Out``` button on the right corner of the webpage). 

On your profile page, you should see ```Signed Ubuntu Code of Conduct: ``` somewhere. If you have created a new account, it should say ```No```. 

In order to sign packages, upload them to PPAs, create your own PPAs, etcetera you'll have to sign that Code of Conduct. 

[Follow this process in order to sign the Ubuntu Code of Conduct](https://launchpad.net/codeofconduct).

***NOTE:*** See [[GPG keys and You]] on how to create a GPG key and get its fingerprint for step 1. 
***NOTE:*** If you see something like this when you try to import your GPG key using its fingerprint:

```

 Launchpad could not import your OpenPGP key
 Did you enter your complete fingerprint correctly? (Help with fingerprints)
 Is your key in the Ubuntu keyserver yet? You may have to wait between ten minutes (if you pushed directly to the Ubuntu key server) and one hour (if you pushed your key to another server). (Help with publishing keys)

```

And you've followed the process as outlined in [[GPG keys and You]], wait 10 minutes for your key to get into the Ubuntu keyserver. 

***NOTE:*** After you import your GPG key, you will be sent an email in order to validate the key. 

The contents will look something like:

```

    -----BEGIN PGP MESSAGE-----
    Version: GnuPG v1.4.11 (GNU/Linux)
    
    <Encrypted message here>
    
    -----END PGP MESSAGE-----
    
```

You will have to decode that message in order to validate your key.

To do so easily, copy the encrypted message (from ```-----BEGIN PGP MESSAGE-----``` to ```-----END PGP MESSAGE-----``` (***including those two lines***) ), open up a text editor, and paste the message into a text file. Save the file as ```encKey.txt```, and open up a terminal.

Go to the spot where you have ```encKey.txt```, and type the following command:

```

    gpg -d encKey.txt

```

You will be prompted for the passphrase associated with your GPG key. 

After you type your password in, you should see the decrypted message in your terminal. In particular, something like this at the end:

```

    Please go here to finish adding the key to your Launchpad account:

    <URL-link>

```

Copy the <URL-link> and paste it into a web browser. You should be taken to a page which says that you'd like to confirm your GPG key. Click "Continue", and you should be taken back to your profile page with a notification that says:

```

    The key #####/******** was successfully validated.

```

Congrats! You validated your GPG key and completed step 1. 

***NOTE:*** If you happen to have multiple GPG keys, use the following command in order to sign the Ubuntu Code of Conduct with *YOUR* key:

```

    gpg --clearsign -u0x<your-key-id> UbuntuCodeofConduct-2.0.txt 

```

Replacing ```<your-key-id>``` with the key id associated with your name and email. 

-------------------------------------------
| Contributing to the TSGL Debian package |
-------------------------------------------
Now that you've created a Launchpad account and have signed the Ubuntu Code of Conduct, it's time to be added to our team. 

Our team is known as ```tsgl-team``` on Launchpad, which you can view [here](https://launchpad.net/~tsgl-test).

To join our team, you can click on the ```+ Join the team``` link, which is found on our profile page.

Once you do this, you'll be given this message:

```

    Are you sure you want to join this team?

    Since this is a moderated team, one of its administrators will have to approve your membership before you actually become a member. 

```

Click "Join", and you'll be taken back to our profile page. 

We will be notified of your request to join, and will accept it when we can!

Once you have been accepted, you will be able to contribute to our Debian package!

To get started, you can take a look at our [[Creating new Deb Packages]] page.

--------
| MISC |
--------

Found something misleading? Perhaps we made an error? Check out the links below in order to receive more help!

http://lists.gnupg.org/pipermail/gnupg-users/2004-May/022471.html

***PLEASE*** update this document if you come across new links, or if there's missing/false information.