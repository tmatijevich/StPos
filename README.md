# StPos

StPos is a library for Automation Studio projects to use with the SuperTrak modular transport system.  
This library provides functions to translate between local section positions and global loop positions.  
The functions are valid for any network layout.  
The global reference is defined by user inputs: origin section and direction.  
[Download the library here](https://github.com/tmatijevich/StPos/releases/latest/download/StPos.zip)

![Global position sample 2021-07-07_18 01 23](https://user-images.githubusercontent.com/33841634/124838969-831b1c80-df4d-11eb-886e-6ab1ff9d2343.png)

## Clone

Clone to a reference location

```
git clone https://github.com/tmatijevich/StPos.git
```

Or, add as submodule to your project's libraries

```
cd <project_root>
git submodule add https://github.com/tmatijevich/StPos.git <apj_root>/Logical/Libraries/StPos
```

Then add the library through the existing library toolbox object in Automation Studio.

## Functions

- `SuperTrakGlobalPosition(section, sectionPosition, originSection, direction)`
	- Returns `globalPosition` given the position referenced to a section on the network
- `SuperTrakSectionPosition(globalPosition, originSection, direction)`
	- Returns `section` and `sectionPosition` given the global reference position on the loop

### Usage

Example for declared variables

```
VAR CONSTANT
    ORIGIN_SECTION  : USINT := 2; (*Defined by the application*)
    DIRECTION       : DINT  := stDIRECTION_LEFT; (*Or `stDIRECTION_RIGHT` from SuperTrak, See also `stCOM_DIR_LEFT` and `stCOM_DIR_RIGHT` from StCom*)
END_VAR
VAR
    Section         : USINT;
    SectionPosition : DINT; (*[um]*)
    PositionResult  : SuperTrakPositionErrorEnum;
    PositionInfo    : SuperTrakPositionDiagType;
    GlobalPosition  : DINT; (*[um]*)
END_VAR
```

Example function call
```
PositionResult := SuperTrakGlobalPosition(Section, SectionPosition, ORIGIN_SECTION, DIRECTION, // Inputs
                                          GlobalPosition, PositionInfo); // Output references
```

## Build

You may notice build warnings such as "Additional directory/file found ..." from the StPos submodule.

![File folder build warnings 2021-07-12_13 51 29](https://user-images.githubusercontent.com/33841634/125345690-77996e00-e31e-11eb-9f58-4bf563ac07a4.png)

Add `-W 9232 9233` in your configuration property "Additional build options:" to suppress.

## Dependencies

- `SuperTrak` >= 0.6.0

## Authors

- [tmatijevich](https://github.com/tmatijevich)
