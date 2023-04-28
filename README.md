
<p align="center">
  <img src="logo.png" style="object-fit: cover; width: 100%; height: 300px;" />
  <h1 style="position: absolute; top: 150px; left: 50%; transform: translateX(-50%); color: white; text-shadow: 2px 2px 2px black;">General Electronic Module Tester (GEMT)</h1>
</p>

# Table of Contents

- [Project Overview](#Project-Overview)
    - [Description](##Description)
    - [Revision Changes](##Revision-Changes)
    - [Authors and Repository](##Authors-and-Repository)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Step 1: Download the Library](#step-1-download-the-library)
  - [Step 2: Import the Library](#step-2-import-the-library)
  - [Step 3: Verify the Installation](#step-3-verify-the-installation)
- [How to Create Your Own Testing Menu](#how-to-create-your-own-testing-menu)
  - [Step 1: Define Your Test Functions](#step-1-define-your-test-functions)
  - [Step 2: Define Your Menus](#step-2-define-your-menus)
  - [Step 3: Boot Up Your Menu](#step-3-boot-up-your-menu)
  - [Step 4: Start Your Menu](#step-4-start-your-menu)
- [Issues](#issues)
- [License](#license)
- [Acknowledgments](#acknowledgments)

# Project Overview

## Description

The GEMT library allows you to easily create your own testing menus and protocols for IoT and robotic electronic modules. With this library, you can design custom testing procedures, automate tests, and efficiently analyze the performance of your electronic components.

The repository currently contains the custom interfacing library [`gemt_interface.cpp`](./gemt_interface.cpp), [`gemt_interface.h`](gemt_interface.h) as well as example testing programs to demonstrate the project's intended use. 


## Revision Changes
V3 is an OOP focused adaptation of GEMT V2.

## Authors and Repository

This library was developed by Team GEMT for our Senior Design project at UTSA.

For the latest version, documentation, and issue tracking, please visit the library's GitHub repository:

[GEMT Repository](https://github.com/jc-cr/gemt_v3)

## Repository Contents and Future Plans

Our future plans include:

1. Separating the individual components for better modularity and maintainability.
2. Packaging the library for open-source distribution as an Arduino library, making it even more accessible and convenient for users.


# Installation

This section provides a step-by-step guide for installing the General Electronic Module Tester (GEMT) library on your Arduino Mega development environment.


## Prerequisites

Before you install the GEMT library, make sure you have the following:

- Arduino IDE or compatible development environment
- Arduino Mega board

## Step 1: Download the Library

Download the GEMT library from the repository or release page as a ZIP file.

## Step 2: Import the Library

Open your Arduino IDE or compatible development environment, and follow these steps:

1. Click on **Sketch** in the top menu.
2. Hover over **Include Library** and click on **Add .ZIP Library...**.
3. Locate the downloaded ZIP file and click **Open**.

The GEMT library is now imported into your Arduino development environment.

## Step 3: Verify the Installation

To verify the installation, follow these steps:

1. Click on **File** in the top menu.
2. Hover over **Examples** and navigate to **GEMT** in the list of libraries.
3. Select one of the example sketches provided with the library.

The example sketch should open in a new window. Upload the sketch to your Arduino Mega board and observe the results to ensure the library is installed correctly.


# How to Create Your Own Testing Menu

The GEMT library allows you to easily create your own testing menu for IOT and robotic electronics using the Arduino framework. This section provides a step-by-step guide for creating your own testing menu.

Your file structure should end up like this:
```
├── main.ino
├── test1.h
├── test2.h
└── ...
```

### Step 1: Define Your Test Functions

Create a new header file for your test function. Name the file something descriptive, like ```myTest.h```.

```
// myTest.h

#ifndef MY_TEST_H
#define MY_TEST_H

void myTest(void);

#endif
```

In your new header file, define your test function. Your function should follow this format:
```
void myTest(void)
{
    // Your test code goes here
}
```
Your test function should not take any arguments or return any values. It should be a simple function that performs the test and reports any results.


In your "MyTest.h" file, define a function called runMyTest(). This function should be used to call and execute your test function. The function should follow this format:

```
void runMyTest()
{
  // Set up any variables or objects needed for the test here

  // Call your test function here
  myTest();

  // Clean up any variables or objects here
}
```

### Step 2: Define Your Menus

Next, you will need to define your main menu and submenus using the GEMTmenu class. These will be defined in your main `.ino` file. This class takes an integer argument that specifies the number of items in your menu.

For example, here is a sample menu that includes the runMotorTest function from Step 1:
```
GEMTmenu MyMenu(1);

MyMenu.setFirstLine("My Testing Menu:");
MyMenu.addItem("Test Motor", runMotorTest);
```

In this example, the first line of the menu is set to "My Testing Menu", and the menu includes one item that runs the runMotorTest function when selected.

You can add as many items as you want to your menu by calling the addItem method and passing in the name of the item as a String and the test function as a function pointer.

#### Adding Submenus

First, you need to define your sub-menu using the GEMTmenu class. This class takes an integer argument that specifies the number of items in your sub-menu.
Add items as before.
Add your sub-menu to the main menu using the addItem function. The second argument of this function should be a pointer to the function that you want to execute when the user selects the menu item. You will need to update the current menu when this is clicked. To do so create a function such as,
```
void mySubMenuFunction()
{
  updateMenu(mySubMenu);
}
```
and add `mySubMenuFunction` to you `addItem` parameter.

### Step 3: Boot Up Your Menu

After defining your menu, you need to call the bootUp method of the GEMTmenu class to set up the menu. This method checks the hardware, sets up interrupt handlers, and displays the logo.

```
MyMenu.bootUp();
```
### Step 4: Start Your Menu

Finally, you need to call the startGEMT function in your main loop and pass in your menu to start the menu and begin testing.

```
void loop()
{
    startGEMT(MainMenu);
    delay(3);
}
```

# Issues
See issues section for current issues being resolved.

# License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

# Acknowledgments

This project uses several third-party libraries. The complete list of libraries, their authors, and respective licenses can be found in the [NOTICE](./NOTICE) file.
