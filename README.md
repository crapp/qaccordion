#qAccordion

qAccordion provides an accordion widget for Qt 5. The widget is heavily inspired by the [jQuery UI accordion control](https://jqueryui.com/accordion/).

## Setting up qAccordion

qAccordion is based on Qt5 and c++11. Make sure your development environment meets this requirements. 

To make use of this Qt Widget in your Project you have two choices.

1. Use the included CMake project files to compile a shared library which you link to your application.
2. Add the source code of the library to your project and compile it. Make sure to include the icons qrc file

## Usage

Using this widget is quite easy. You can add this to your application either programmatically or by using Qt Designer. The widget does __not__ support configuration via the Designer. 

A minimum code example

```c++
QAccordion *myAccordion = new QAccordion();
// add the Accordion to your layout
this->layout()->addWidget(myAccordion);
// add a content pane with a header
int firstPaneIndex = myAccordion->addContentPane("My first content pane");
// make sure our content pane was added
QFrame *contentFrame = nullptr;
if (firstPaneIndex != -1) {
	// get a QFrame* from the Content Pane. This is where we place our content
	contentFrame = myAccordion->getContentPane(firstPaneIndex)->getContentFrame();
}
```
	
qAccordion makes use of Qt memory management. This means all objects will be freed by Qt when the program is finished. Be aware when you use QAccordion::removeContentPane() you can tell qAccordion only to remove the Pane from the layout (and the internal vector). In this case it is up to the user to delete the object. 