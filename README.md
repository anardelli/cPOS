# cPOS

### What is this?

This is an implementation of ISO 8583, in regular ANSI C89.

This library provides an (insert link here) API, and guarantees that:

* all functions expressed in jPOS that are implemented in this library have the same function declaration as jPOS does (String -> char *)
* all output messages adhere to the ISO 8583 standard
* all functions have an '_r' equivalent that will allow the function to be reentrant and thread-safe


### Okay, Tell Me More

Another added benefit of this library is that all of the functions have
equivalent names to their jPOS counterpart. This means that all functions
that exist in jPOS, and follow camelCase, will be expressed in camelCase in the
cPOS.h library.


### Message Sending
Another thing that jPOS does is create messages in an XML format. This
library will not do that, to allow the user the freedom to use whatever
messaging scheme the wish.


### What Isn't Handled By This Library
**Security** is not handled by this library. It is expected that a user will
utilize the (openssl.h)[https://www.openssl.org/]
