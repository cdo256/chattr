# TCP Chat

This project is a very simple client-server TCP chat program. It results from a personal effort to teach myself networking, so it's a toy/learning project rather than anything serious.

It can be run on Both Linux and Windows, although the Windows compatibility layer is less well-tested.

## Dependencies

It has been designed to depend on as few other packages as possible. It depends on:

 - `make`
 - a C compiler toolchain such as Clang, GCC, or MinGW
 - A few common utilities like `rm`

This can be set up using Guix as:

    guix shell gcc-toolchain make

## Building

To build, you can run:

    make

This gives you the binary `chattr`.

## Running

After building, you can simply type `./chattr` to run. It will then ask whether you want to run as a client or a server.

    $ ./chattr
    Client or server? [cs]: s
    Enter a port number: 2000
    Making socket on port 2000
    Setting socket 3 as non-blocking
    Socket bound and listening...

Then you can get a client to connect from another computer after first building on that machine.

    $ ./chattr
    Client or server? [cs]: c
    Enter an IP address: 192.168.1.43
    Enter a port: 2000
    Connecting to 127.0.0.1 on port 2000...
    Setting socket 3 as non-blocking
    Successfully connected!
    Enter a username: Abigail
    Welcome!
    /identify Abigail
    > hi
    Sending 'hi'
    *hi
    >
    
You can connect with as many clients as you like:

    $ ./chattr
    Client or server? [cs]: c
    Enter an IP address: 192.168.1.43
    Enter a port: 2000
    Connecting to 127.0.0.1 on port 2000...
    Setting socket 3 as non-blocking
    Successfully connected!
    Enter a username: Babylas
    Welcome!
    /identify Babylas
    > hello Abigail
    Sending 'hello Abigail'
    *hello Abigail
    >

Then Abigail will see:

    /identify Babylas
    *hello Abigail
    >

And they can carry out a conversation.

## Conclusion

To reiterate, this isn't a big, polished project, but it is just a way to exercise my skills and teach myself how to network computers.
