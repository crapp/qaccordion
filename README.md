#qAccordion

qAccordion provides an accordion widget for Qt 5. The widget is heavily inspired by the [jQuery UI accordion control](https://jqueryui.com/accordion/).

Watch a screencast made with the [Demo Application](#demo-application) to see what qAccordion is.

[![qAccordion Demo Screencast](http://i.imgur.com/vLz2dYk.png)](https://www.youtube.com/watch?v=czhRcNdSHw4 "qAccordion Demo Screencast")

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Contents** *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Setting up qAccordion](#setting-up-qaccordion)
- [Installation](#installation)
  - [Linux](#linux)
  - [Windows](#windows)
- [Usage](#usage)
  - [A minimum code example](#a-minimum-code-example)
  - [Memory management](#memory-management)
  - [API Documentation](#api-documentation)
  - [Demo Application](#demo-application)
- [Bugs and Feature requests](#bugs-and-feature-requests)
- [ToDo](#todo)
- [Development](#development)
  - [Repositories](#repositories)
  - [Coding standards](#coding-standards)
  - [Versioning](#versioning)
- [FAQ](#faq)
- [License](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Setting up qAccordion

qAccordion is based on Qt5 and c++11. Make sure your development environment meets these requirements:

* cmake >= 2.8.12
* gcc >= 4.7
* clang >= 3.4
* MSVC >= 12 (Visual Studio 2013)
* Qt 5.2 (Qt5Widgets, Qt5Gui, Qt5Core [Qt5Network])

To make use of this Qt Widget in your Project you have two choices.

1. Use the included CMake project files to compile a shared library which you link to your application.
2. Add the source code of the library to your project and compile it. Make sure to include the icons qrc file

## Installation 

Obtain the source code from our [github repository](https://github.com/crapp/qaccordion). Either download the latest release or use git to check out the master branch.

### Linux 
Run the following commands to compile a shared library on Linux after you obtained the source code.

```shell
# create a build directory
mkdir build
cd build
# run cmake to create make files. use -DQACCORDION_EXTERNAL=ON if you make use of
# git submodules
cmake -DCMAKE_BUILD_TYPE=RELEASE ../
# now compile the source code and create the shared library. you can speed up 
# compilation with the j option.
make 
# install the shared library
sudo make install
```
There are ready to use packages for the following Linux distributions:

* [Archlinux (AUR)](https://aur.archlinux.org/packages/qaccordion/)

### Windows

Compiling the library on Windows is possible with the Microsoft Visual C++ compiler (MinGW is currently __not__ supported).
Generate a Visual Studio Solution with the appropriate cmake generator. After that you can compile the library with the Microsoft IDE.

```shell
#create build directory
mkdir build
cd build
# assuming you are using Visual Studio 2013 on a 64bit windows installation and Qt installed to C:\\Qt
# Please change these options so they suit your build evironment.
cmake -G"Visual Studio 12 2013 Win64" -DCMAKE_PREFIX_PATH="C:\\Qt\\5.5\\msvc2013_64" ../
```
Open the solution file and compile the library. 

Another possibility is to use the cmake NMake generator. 

```shell
#create build directory
mkdir build
cd build
# check the prefix path so it matches your Qt installation
cmake -G"NMake Makefiles" -DCMAKE_PREFIX_PATH="C:\\Qt\\5.5\\msvc2013_64" ../
# build the library
nmake
```
## Usage

Using this widget is quite easy. You can add this to your application either programmatically or by using Qt Designer. The widget does __not__ support configuration via the Designer. 

### A minimum code example

```c++
QAccordion *myAccordion = new QAccordion();
// add the Accordion to your layout
this->layout()->addWidget(myAccordion);
// add a content pane with a header
int firstPaneIndex = myAccordion->addContentPane("My first content pane");
// make sure our content pane was added
if (firstPaneIndex != -1) {
	// get a QFrame* from the Content Pane. This is where we place our content
	QFrame *contentFrame = myAccordion->getContentPane(firstPaneIndex)->getContentFrame();
	// give the QFrame a QLayout
	contentFrame->setLayout(new QVBoxLayout());
	// add a simpel QLabel to the frame
	contentFrame->layout()->addWidget(new QLabel("Hello Cute World!!!"));
}
```
This would display a Content Pane with the Header _My first content pane_. When you click it, it will expand and you can see the message _Hello Cute World!!!_

### Memory management

qAccordion makes use of Qt memory management. This means all objects will be freed by Qt when the program is finished. Be aware when you use QAccordion::removeContentPane() you can tell qAccordion to remove the ContentPane object from the layout (and the internal vector) but not to free the allocated memory. This might be useful if you want to add the same ContentPane again at a later time or to another QAccordion widget. If you don't do so, it is up to you to delete the object.

### API Documentation

The library is documented using doxygen style comments. A static HTML version of the API documentation can be found on [github](https://crapp.github.io/qaccordion/). This is the documentation of the latest release. 

The doxygen project file to generate the HTML documents is located in the `doc` folder. Run the following command in a shell to generate your own local HTML documentation so you may read it offline or have an updated documentation for features that are still under development (needs doxygen >= 1.8.6).

```shell
doxygen qAccordionDoxyfile
```

### Demo Application

In the `test` folder you can find a demo application that you can build with the library.
You must use the CMake option `BUILD_TESTER` so it gets compiled.

Linux example

```shell
cd build
cmake -DQACCORDION_BUILD_TESTER=ON ../
make
```
Have a look at the source code of the demo application if you are searching for a more advanced example on how to use this Qt Widget. I have recorded a short video that shows how qAccordion works.

## Bugs and Feature requests

If you find a Bug or have a feature request head over to github and open a new [issue](https://github.com/crapp/qaccordion/issues). 

## ToDo ##
* Drag and Drop support. The API already supports moving Content Panes but only programmatically. 
* User defined Icons and Icon position.
* Definable animation type.
* Trigger open / close not only on single mouse click (e.g. double click, mouse over).

## Development

Brief overview over the development process of qAccordion.

### Repositories 
The github repository of qAccordion has several different branches.

* master: Main development branch. Everything in here is guaranteed to compile and is tested (at least a little bit :)). This is the place for new features and bugfixes. Pull requests welcome. 
* develop: Test branch. Code is guaranteed to compile on the developers build environment. Nothing more nothing less.
* release-x.x: Branch for a release. Only bugfixes are allowed here. Pull requests welcome. 
* gh-pages: Special branch for the static HTML documentation that will be hosted by github.io. Content is generated by doxygen.

### Coding standards

The source code is formatted with clang-format using the following configuration

```
Language:        Cpp,
BasedOnStyle:  LLVM,
AccessModifierOffset: -4,
AllowShortIfStatementsOnASingleLine: false,
AlwaysBreakTemplateDeclarations: true,
ColumnLimit:     81,
IndentCaseLabels: false,
Standard:        Cpp11,
IndentWidth:     4,
TabWidth:        4,
BreakBeforeBraces: Linux,
CommentPragmas:  '(^ IWYU pragma:)|(^.*\[.*\]\(.*\).*$)|(^.*@brief|@param|@return|@throw.*$)|(/\*\*<.*\*/)'
```

### Versioning

Somehow every developer faces the problem of version numbers. I decided to use [semantic versioning](http://semver.org/) and stick to their rules.

> Given a version number MAJOR.MINOR.PATCH, increment the:
>
> 1. MAJOR version when you make incompatible API changes,
>
> 2. MINOR version when you add functionality in a backwards-compatible manner, and
>
> 3. PATCH version when you make backwards-compatible bug fixes.

We are currently at this stage

> Major version zero (0.y.z) is for initial development. Anything may change at any time. The public API should not be considered stable.

## FAQ ##

**Why does it not have feature X?**

Feel free to fork the project and do a pull request!

## License
```
Copyright (C) 2015 Christian Rapp

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```