# gemt_v3
OOP focused adapation of GEMT V2

## To Do:
  - [X] Test a Main menu
  - [X] Test selection actions
  - [X] Setup a submenu
  - [X] Setup nRF test example
  - [X] Port over all tests programs



## How to Create Your Own Testing Menu

The GEMT library allows you to easily create your own testing menu for IOT and robotic electronics using the Arduino framework. This section provides a step-by-step guide for creating your own testing menu.

Your file structure should end up like this:
```
├── gemt_interface.cpp
├── gemt_interface.h
├── main.ino
├── test1.h
└── test2.h
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

## Issues
See issues section for current issues being resolved

## Dev Notes
  - Don't use "display" outside of scope. Will crash
