Changelog
=========

Version numbers comply with the [Sementic Versioning Specification (SemVer)].


## [v0.2.3] (2021-10-15)

### Added

* Macro `RX_DISABLE_TEST_DISCOVERY` to disable the automatic discovery of tests.
* Support for MinGW.


### Changed

* Disable address sanitizer for custom data sections.
* Disable test discovery for unsupported compilers.
* Bump Contributor Covenant's code of conduct to version 2.1.


### Fixed

* Compilation with MSVC in C++ mode failing.


## [v0.2.2] (2021-08-04)

### Changed

* Append a slash character as required by ‘vcpkg’.


## [v0.2.1] (2021-07-22)

### Added

* Checks for some printf-style functions.


## [v0.2.0] (2021-07-13)

### Added

* Support for automatic registration of tests.
* Test suites to group test cases.
* Configuration of test suites, test cases, and test fixtures.
* Designated initializer-like syntax.
* GitHub documentation and test workflows.
* Tests covering most code paths and the supported language standards.
* Version constant.
* Comprehensive documentation.
* Code of conduct file.
* Readme file.


### Changed

* Code style to snake case.
* Strip the intermediate ‘rexo’ include folder.
* Switch the license from MIT to Unlicense.


## v0.1.0 (2018-06-11)

* Initial release.


[Sementic Versioning Specification (SemVer)]: https://semver.org
[v0.2.3]: https://github.com/christophercrouzet/rexo/compare/v0.2.2...v0.2.3
[v0.2.2]: https://github.com/christophercrouzet/rexo/compare/v0.2.1...v0.2.2
[v0.2.1]: https://github.com/christophercrouzet/rexo/compare/v0.2.0...v0.2.1
[v0.2.0]: https://github.com/christophercrouzet/rexo/compare/v0.1.0...v0.2.0
