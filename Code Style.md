# Code Style

## General Guidelines

Global variables: G_Name, using global variables requires review

Class name: ClassName

Class members: class_member_

Function name: CamelCase: functionName(). Note: This rule does not apply to Qt's automatic slot functions

Enum name: KEnumName, note, enums should use enum class

All files should use scope

All namespaces should be lowercase, starting with rf

All library names should follow a similar naming convention to class names, starting with Rf, such as RfIO

In CMakeLists, when including files, write the full name instead of using *.h etc. (This way, when there are unnecessary files, they will not be compiled incorrectly, especially when rolling back local versions)

All header files should use ifndef

Header file inclusion order:

0. stdafx.h
1. Corresponding .h for .cpp
2. std library - Qt library - other third-party libraries, separated by blank lines, clang-format will automatically sort
3. Header files from other libraries within the project
4. Files within the same unit, use ""
5. UI files *_ui.h that will be automatically generated during compilation

## Recommended Requirements

- Signals: ClassName+Sig

- Slots: ClassName+Slot

- Parameter naming: _name

- Encapsulate members as much as possible, use set and const get

- Try to only declare in header files, put #include in .cpp files

- File naming: For interdependent or similar modules, use similar naming, it is recommended that the first few letters are the same, so they can be close in position when sorted

Example:

example.h:

```c++
#pragma once
#ifndef ARM_H
#define ARM_H

#include <string>

namespace NameSpaceName
{
class NameOfAnotherClass; //declaration
class ExampleClass
{
    enum class AppMode
    {
        Leisure,
        InCommand,
        InCalc,
        NoGui
    };

public:
    void thisIsFunction();

    void setName(const std::string& _name){name_=_name;}
    const std::string getName(){return name_;}

private:
    NameOfAnotherClass* ano_; // Pointer. If it's an instance, it can only be included in the header file
    std::string name_;        // Consider byte alignment without affecting logical classification
};
}
#endif
```

example.cpp

```c++
#include "example.h"
#include "nameofanotherclass.h"
namespace NameSpaceName
{
void ExampleClass::thisIsFunction()
{}
}
```