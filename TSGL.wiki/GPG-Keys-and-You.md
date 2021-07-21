-----------------------------------------------------------------
| GPG KEYS FOR SIGNING DEBIAN PACKAGES (Last updated: 08/09/16) |
-----------------------------------------------------------------

***NOTE:*** This document attempts to guide you in the process of creating a GPG key. If there's something that we have missed, or perhaps have misleading information, please edit this page accordingly!

Before you can begin the process of creating a Debian package, you *NEED* to have a GPG key. 
This is so that you can sign Debian packages and upload them to our PPA (see [[PPA and our TSGL team]]).

This is a security measure; Launchpad has you sign Debian packages with a GPG key. 

Think of this as a stamp of approval that says, "this package does not contain malware". 

Also, if the package is tampered with while you are uploading it, the GPG key will reflect this. 
This will result in the package being rejected, since it was tampered with. 

A GPG key has two parts: a public key that you can distribute freely to others, and a private key that you should NEVER distribute. 

In order to generate those parts, you need to use the ```gpg``` program via a terminal. (There's a GUI version that you can use called ```seahorse```, but that process has never been tested).

Open up a terminal, and let's get started. 

1). Type:

```

    sudo apt-get install haveged

```

This daemon will help generate random bytes, used in the process of making a GPG key. 

2). Type:

```

   gpg --gen-key
  
```
  
You should now have the ```gpg``` program up and running. Something like this will show up:
    
```

    Please select what kind of key you want:
    (1) DSA and Elgamal (default)
    (2) DSA (sign only)
    (5) RSA (sign only)
    
    Your selection? 

```
       
Type ```1```, then hit ```ENTER```.

3). Next prompt:
    
```

    RSA keys may be between 1024 and 4096 bits long.
    What keysize do you want? (2048) 

```    

Type ```2048```, and hit ```ENTER```.
    
4). Next prompt:

```

    Please specify how long the key should be valid.
             0 = key does not expire
          <n>  = key expires in n days
          <n>w = key expires in n weeks
          <n>m = key expires in n months
          <n>y = key expires in n years
    Key is valid for? (0) 

```

Type```0```, and hit ```ENTER```.
Type ```y``` afterwards and hit ```ENTER```.

5). Next prompt:

```
    You need a user ID to identify your key; the software constructs the user ID
    from the Real Name, Comment and Email Address in this form:
        "Heinrich Heine (Der Dichter) <heinrichh@duesseldorf.de>"
    
    Real name: 

```

Use your name and email for the ```Real name:``` and ```Email address:``` portion.
For the ```Comment:```` part, you can just press ```ENTER```.

Once you have done that, you should be shown this:

```

You selected this USER-ID:
    "Real name (Comment) <Email address>"

Change (N)ame, (C)omment, (E)mail or (O)kay/(Q)uit? 

```

Make any changes, then type ```O``` to move on.


6). You will now be asked for a passphrase. Make this something easy for you to remember, but hard for others to guess. Keep this passphrase in mind, because you'll need it whenever you sign Debian packages with this new key.

7). Your key will now be generated. 

Now that your key is generated, it's time to send it to the Ubuntu keyserver.  

First, copy the ```<key-id>``` part of the text that was generated after your key was generated:

```

    gpg: key <key-id> marked as ultimately trusted.
    public and secret key created and signed. 

    (more stuff here...)

```

Once you have the ```<key-id>``` copied, type:

```

    gpg --send-keys --keyserver keyserver.ubuntu.com <key-id>

```

(Replacing ```<key-id>``` with the key id that you copied).

Now, type:

```

    gpg --list-keys

```

You should see something like this:

```

    pub   #####/******** 2016-06-20
    uid                  Real Name <Email@Email.com>
    sub   #####/******** 2016-06-20

```

The ```*********``` in the ```sub``` line is what you will use as your ```<key-id>``` when signing packages.
You can also use the ```*********``` in the ```pub``` line, but *PLEASE* be consistent!

To get the GPG key fingerprint (which is what you'll need if you haven't seen our [[PPA and our TSGL team]] page!), type:

```

    gpg --fingerprint

```

You should see something like this:

```

    pub   #####/******** 2016-06-02
          Key fingerprint = **** **** **** **** ****  **** **** **** **** ****
    uid                  Real Name <Email@Email.com>
    sub   #####/******** 2016-06-02

```
The ```Key fingerprint = **** **** **** **** ****  **** **** **** **** **** ``` is your fingerprint. 

You should now be ready to sign Debian packages!

If you haven't already, please see the [[PPA and our TSGL team]] page.

You may also want to take a look at the [[Saving GPG keys]] page, if you haven't already.  

Otherwise, feel free to continue with the [[Creating New Deb Packages]] page.

--------
| MISC |
--------

For more information, please see this [website](http://ubuntuforums.org/showthread.php?t=680292).

***PLEASE*** update this document if you come across new links, or if there's missing/false information.