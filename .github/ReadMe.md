# StPos

StPos is a library for Automation Studio projects to use with the SuperTrak modular transport system.
This library provides functions to translate between local section positions and global loop positions.
The functions are valid for any network layout. The global reference is defined by user defined settings: origin section and direction.

## Clone

Clone to a reference location

```
git clone https://github.com/tmatijevich/StPos.git
```

Or, add as submodule to your project's libraries

```
cd <project_root>
git submodule add https://github.com/tmatijevich/StPos.git <apj_root>/Logical/Libraries
```

You will then have to add the existing library in Automation Studio.

## Functions

- `SuperTrakGlobalPosition(section, sectionPosition, originSection, direction)`
	- Returns `globalPosition` given the position referenced to a section on the network
- `SuperTrakSectionPosition(globalPosition, originSection, direction)`
	- Returns `section` and `sectionPosition` given the global reference position on the loop

## Build

You may notice build warnings such as "Additional directory/file found ..." from the StPos submodule.
Add `-W 9233` in your configuration property "Additional build options:" to suppress.

## Dependencies

- `SuperTrak` >= 0.6.0

## Authors

- [tmatijevich](https://github.com/tmatijevich)
