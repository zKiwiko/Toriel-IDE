
![Toriel-IDE](repo/banner.png)

An IDE for GPC Scripting, written in C++ using the Qt Framework. Toriel aims to enhance the scripting experience for developers with a clean and intuitive interface, focusing on key features like syntax highlighting, project structure management, and more.

Currently, Toriel is in its Alpha Stages. Features are limited, and documentation is focused on the current release version.

## Key Features

* Syntax Highlighting
    * Toriel provides Syntax Highlighting for GPC Scripts, making it easier to read and write your code.
* Project Structuring
    * Allows you to manage your scripts in a structured way by organizing them into a project format.
    * File inclusions
    * Pre-Processors
* Tools
    * Image array generator
* IntelliSense
    * Code Completion
    * User-Defined Functions, Combos, Arrays, and Variables

## Planned Features
* IntelliSense
    * Error Diagnostics
* Marketplace Uploading
    * For select Developers

## Syntax Highlighting & Theme
Toriel has support for JSON based themes, Allowing users to customize the IDE's appearence how ever they please.
With the Required JSON structure below, you can change the color of specific GPC Keywords in the Editing field,
or the IDE itself.


### Value Info
* Colors
    * Colors in JSON themes must be Hex color codes, for example: `#FF0000` is a bright Red.
* Font Family
    * The font-family key defines the font the editor will use. You can choose any font you have installed onto your computer. The default is `Monospace`.
* Font Style
    * The font style will depict how the Keyword would look, either being default, **bold**, or *italic*. But not a combination of.

When creating a new theme, its recommended you copy and paste the format below, to avoid any typos. If a font style, family or color is not valid, they will default to `default`, `Monospace` and white (`#FFFFFF`) respectively.

### JSON Structure
```JSON
{
    "toriel": {
        "background-color": "",
        "menu-bar-color": "",
        "status-bar-color": "",
        "terminal-color": ""
    },
    "editor": {
        "font-family": "",
        "text-color": "",
        "background-color": "",
        "line-highlight-color": ""
    },
    "explorer": {
        "background-color": "",
        "font-family": ""
    },
    "numbers": {
        "color": "",
        "font-style": ""
    },
    "keywords": {
        "color": "",
        "font-style": ""
    },
    "strings": {
        "color": "",
        "font-style": ""
    },
    "functions": {
        "built-in-color": "",
        "built-in-font-style": "",
        "user-defined-color": "",
        "user-defined-font-style": ""
    },
    "boolean": {
        "color": "",
        "font-style": ""
    },
    "data-types": {
        "color": "",
        "font-style": ""
    },
    "constants": {
        "color": "",
        "font-style": ""
    },
    "comments": {
        "color": "",
        "font-style": ""
    }
}
```

## Project Structuring

"True Project Structuring" refers to the ability of organizing your code into multiple different files.
Toriel supports this idea in a robust way since Zen Studio lacks that ability.

**NOTE**: These features cannot be used with Standalone files, they can only be used inside project directories.

### Setting up a project
To set up a project in this manor; Open a pre-created directory with `Ctrl + Shift + O`.
Toriel by default will treat any opened directory as a project.

After opening your directory, create a new file titled `project.json`. This will hold the metadata needed for Toriel to be able to set up your
project to send to Zen Studio. The metadata file (`project.json`) should contain the following structure below.

* name
    * This should hold the main name of your project.
    it will be used for a backup in the `bin/data/backups` directory of Toriels installation path.
    It will also be used for the built file, and will goto the `build` directory of your projects directory.
* version
    * This should hold the current version of your project.
    It will be used for the backup system once more, but will place the backup in a folder with the version number. (e.g: `bin/data/backups/{project_name}/{project_version}`).
    It will also be used for the built file, and will goto the `build` directory of your projects directory.
* source
    * This should hold the main source file of your project. It will be the entry point for Toriel to start its preprocessing before sending the processed code to Zen Studio.
* Headers
    * This should contain all the header files you will use, this will allow intelliSense to see any defined functions or variables in those files.
* std
    * This should contain any STD Library files you will use, this will allow intelliSense to see any defined functions or variables in those files.


### project.json structure
```json
{
    "project": {
        "name": "",
        "version": "",
        "source": "",
    },
   "headers": [],
   "std": []
}
```

An example of a fully set up `project.json` file could look like this:
```json
{
   "project" : {
      "name": "quaoar",
      "version": "1.0.0",
      "source": "main.gpc",
      },
   "headers": [
      "include/variables.gpc"
      "include/functions.gpc"
      "helpers.gpc"
   ],
   "std": [
      "std/math/trig"
   ]
}
```

## Pre-Processors & Macros
These pre-processors are Toriel exlusive. Since theyre not including in GPC they are added to give developers a more native comfortable feeling.

### Const 
The const macro will go through the code and process the const identifier with the value that is assigned to it, allowing the GPC compiler to avoid needing to
add variables to the stack, and decrease the change of using the max variable limit.
The GPC compiler already does this when using the `define` keyword but can result in slower compile times if abused. The const macro helps avoid this.

You can use the const macro like:
```python
@const Variable = 10;

main {
   if(get_val(XB1_A)) set_val(TRACE_1, Variable);
}
```
The processed code would turn it into this:
```js
main {
   if(get_val(XB1_A)) set_val(TRACE_1, 10);
}
```

### File Inclusions
Toriel offers the ability to include other files into your main source file.
This can help with project organizing, aswell as readability within your code. 
It works by adding the contents of the included file where the inclusion is stated. This emulates how C++ compilers handle this type of preprocessor.

You can include another file thats within your projects directory with:
```python
@include "file.gpc"
```
This will copy the content of the file to where the inclusion takes place.

You can include files from the [Standard Library](#std) with the example below,
This will also copy the content of said file to where the inclusion is before Toriel sends it 
to ZenStudio.
When using <> to included a STD lib, it will look in this path: `"APPPATH/bin/data/gpc/lib"` then include the file matching the file name, as long as it ends with .gpc.
```python
@include <std/(dir|file)>
```

## IntelliSense
Toriel has IntelliSense features that aid in developer experience. As of now; Toriel's intelliSense only contains the following features.

### Auto-Complete
Auto-Complete is a mundane term, it doesnt necassarily complete your code for you, but instead gives you the option to complete specific words while youre typing.

This can aid developing by reducing the amount of typos one can make, and also showing how many options following the according text typed.

For example, if you start typing "`XB1`" the dropdown menu will show all Constants in GPC that have that following text.

To select an Auto-Complete option, you can use the Arrow Keys on your keyboard or your mouse to scroll through each option.
You can then press `Ctrl + Enter` or `Ctrl + Tab` to select the option, it will then complete the text with the option you choose.

### Icons
* ![func](bin/assets/intelliSense/functions.svg) | Used for Functions and Combos
* ![vars](bin/assets/intelliSense/variables.svg) | Used for Variables and Arrays
* ![keyw](bin/assets/intelliSense/keywords.svg)  | Used for keywords
* ![const](bin/assets/intelliSense/constants.svg) | Used for gpc constants

### Explorer
The Explorer in Toriel is a side panel that shows declared functions, variables, and arrays in the order they are declared in the current file. It would allow you to see the Data Type of a variable and the line its declared on. Clicking on a item will have you jump to the line the declaration is at.

# Credits

### Discord Members
* DLCENERGY
    * for the Image Array Generator
* YuFow
    * Bug testing
    * std library contributions
