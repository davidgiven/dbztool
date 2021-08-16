dbztool
=======


What?
-----

dbztool is a command-line tool for manipulating the bootstrap interface of a
Dragonball VZ processor. It will allow you to upload and download data, run
programs, examine and modify the system registers, etc. Currently it's only
been tested on the Alphasmart Dana but it should work on other machines too
(although a number of places in the code assume you're using UART1, which need
to be fixed).



Where?
------

- [Check out the GitHub repository](http://github.com/davidgiven/dbztool) and
  build from source.

- [Ask a question by creating a GitHub
  issue](https://github.com/davidgiven/dbztool/issues/new), or just email me
  directly at [dg@cowlark.com](mailto:dg@cowlark.com). (But I'd prefer you
  opened an issue, so other people can see them.)



How?
----

Dependencies are minimal, but you'll need `xxd` and the gcc M68K toolchain. You
should just be able to edit the Makefile and then run `make` and get a binary
(in `./dbztool`).

The online help should be _reasonably_ up to date, but as an example:

```
dbztool -p /dev/ttyUSB0 dump 0xfffffe00 512
```

However, everything's in flux, so that's basically all there is...



Who?
----

dbztool was written mostly by me, David Given. Feel free to contact me by email
at [dg@cowlark.com](mailto:dg@cowlark.com). You may also [like to visit my
website](http://cowlark.com); there may or may not be something interesting
there.



License?
--------

dbztool is open source software available [under the 2-clause BSD
license](https://github.com/davidgiven/dbztool/blob/master/LICENSE).  Simplified
summary: do what you like with it, just don't claim you wrote it.

