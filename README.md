# Toriel-IDE

An IDE for GPC Scripting, written in C++ using the Qt Framework. Toriel aims to enhance the scripting experience for developers with a clean and intuitive interface, focusing on key features like syntax highlighting, project structure management, and more.

Currently, Toriel is in its Alpha Stages. Features are limited, and documentation is focused on the current release version.

## Key Features

* Syntax Highlighting
    * Toriel provides Syntax Highlighting for GPC Scripts, making it easier to read and write your code.
* True Project Structuring
    * Allows you to manage your scripts in a structured way by organizing them into a project format.
    * File inclusions
* Tools
    * Image array generator
    * Zen Studio slot sync fix
* IntelliSense
    * Code Completion

## Planned Features
* IntelliSense
    * Suggestions
    * Error Diagnostics
* Marketplace Uploading

## Syntax Highlighting
Toriel supports JSON-based themes, allowing you to customize the appearance of the IDE, including the color scheme of the interface and the text editor. Themes are stored in the `bin/data/theme` subdirectory of the project’s installation directory.


JSON Structure:
```json
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

### Key notes
* Color: Specify color values in HEX format (e.g., "#FFFFFF").
* Font-family: Provide a valid font family name (e.g., "JetBrains Mono").
* Font-style: Specify a single font style, such as "default", "italic", or "bold". No combinations are allowed.


## True Project Structuring
Toriel introduces a True Project Structure system that enables better organization of your GPC scripts. This system allows you to include external source files into the main source file declared in a project.json file.

### Project.json Structure

The project.json file is essential for defining the main source file of your project. As of version `a0.0.1`, the `name` and `version` keys are placeholders and do not affect the build process, but they will be utilized in future updates.
```json
{
    "project": {
        "name": "YourProject",
        "version": "1.0.0",
        "source": "main.gpc"
    }
}
```
* **name**: Name of the Project (Future Use)
* **version**: Version of the project (Future Use)
* **source**: The main source file that contains the primary logic of your script.

### Including Additional Files
To include other source files, use the following syntax:
```js
@include filename.gpc

you can also use

@include SubDir/filename.gpc
```
This approach emulates how C++ compilers compile. By replacing the `@include` directive with the content from the referenced file before sending the combined data to Zen Studio.

### Example
**Included File** (`example.gpc`)
```js
function add(a, b) {
    return a + b;
}
```
**Main Source File** (`main.gpc`)
```js
@include example.gpc
main {
    set_val(TRACE_1, add(10, 10));
}
```
After parsing by Toriel, the code sent to Zen Studio would look something like this:
```js
function add(a, b) {
    return a + b;
}

main {
    set_val(TRACE_1, add(10, 10));
}
```
This system simplifies managing larger scripts by splitting them into smaller, more maintainable files.

## Tools
Toriel offers some tools to help aid the development process.
### Image Array Generator
This tool will create an Image array usable in your script that will then be copy and paste able so you can include it anywhere in your code.
### Zen Studio Slot Sync Fix
This tool will delete the ZenStudio directory in `appdata/Roaming`, which will fix any extremely long Slot Syncing in Zen Studio if swapping tabs dont work.

# Credits

### Discord Members
* DLCENERGY
    * for the Image Array Generator
