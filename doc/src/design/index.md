Design Rationale
================

With dozens of well-established unit testing frameworks out there, the question
_why yet another one?_ is unavoidable.

The answer is fairly simple—the goal has been to try to further **polish**
the user experience from existing frameworks while maintaining **compatibility**
with both C89 and C++.

Also, it sounded like a great exercise! 😊

It is important to note that nothing is truly novel in Rexo—the main concepts
are inspired by existing frameworks like [Criterion][criterion],
[novaprova][novaprova], and others, although their form and implementation
have been entirely revised.


## Automatic Registration

These two frameworks in particular proved that it was possible to make
automatic registration of tests also possible in C, where most C frameworks
require a _lot_ of boilerplate to register these.

The advantages that Rexo has in this regard are that:

* it only relies on a **widespread** compiler-specific feature to get
  the automatic registration going.
* it works on both **Unix** and **Windows** platforms.
* it is implemented in a fairly **simple** fashion.


Automatic features are always nice to have _unless_ they cannot be overridden.
Rexo is designed in such a way that it is possible for the users to skip either
parts or the whole automatic registration framework by allowing [explicit
registration of tests][explicit-registration-guide], like more conventional C
frameworks offer.

See the [automatic registration][auto-registration] design page for
more details.


## Options Definition

[Criterion][criterion] also cleverly demonstrated that it was possible to abuse
the languages to make a **named argument syntax** available for configuring
objects.

Rexo took this a step further by allowing test cases not only to **inherit**
options from their parent test suites, but also to **tweak only specific
fields** for each test case.

Since **consistency** is king in design, the approach was also extended to work
with fixtures, thus providing a **unified** interface to configure _all
the things_.

See the [options definition][options] design page for more details.


[criterion]: https://github.com/Snaipe/Criterion
[novaprova]: https://github.com/novaprova/novaprova

[auto-registration]: ./auto-registration.md
[explicit-registration-guide]: ../guides.md#explicit-registration
[options]: ./options.md
