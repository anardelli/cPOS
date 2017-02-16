# cPOS

### What is this?

This is an implementation of ISO 8583, in regular ANSI C89.

This library provides an [ISO 8583](https://en.wikipedia.org/wiki/ISO_8583)
message creation system, compatible with C/C++ and guarantees the following:

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
messaging scheme the wish. That being said, **messages** need to be created
with headers, to be sent over the wire. These are all implemented within
jPOS's channel system.


### What Isn't Handled By This Library?
**Security** is not handled by this library. It is expected that a user will
utilize the [`openssl.h`](https://www.openssl.org/) library. cPOS exists to
assist c programmers in message creation. cPOS also does not contain any
GUI components, so nothing from, `org.jpos.iso.gui` exists (at this time), in
this library.


### What Still Has To Be Implemented?
1. [space](http://jpos.org/doc/javadoc/org/jpos/space/package-summary.html)
2. [channels](http://jpos.org/doc/javadoc/org/jpos/iso/channel/package-summary.html)
3. [headers](http://jpos.org/doc/javadoc/org/jpos/iso/header/package-summary.html)
4. [validator](http://jpos.org/doc/javadoc/org/jpos/iso/validator/package-summary.html)

Basically all of the functionality of
[`org.jpos.iso`](http://jpos.org/doc/javadoc/org/jpos/iso/package-summary.html)
needs to be put into this library.


### What Should Go In This Library?
1. Translate Messages to Web/Human-Friendly Formats
	1. JSON
	2. xml
	3. csv
2. Functions to handle a [transaction](http://jpos.org/doc/javadoc/org/jpos/transaction/package-summary.html).
