# StPos

StPos is a library for Automation Studio projects to use with the SuperTrak modular transport system.

This library provides functions to translate between local section positions and global loop positions.  The functions are valid for any network layout.  The global reference is defined by user inputs: origin section and direction.  [Download the library here](https://github.com/tmatijevich/StPos/releases/latest/download/StPos.zip).

<img src="https://user-images.githubusercontent.com/33841634/124838969-831b1c80-df4d-11eb-886e-6ab1ff9d2343.png" alt="Global position sample 2021-07-07_18 01 23" style="display:block; margin-left:auto; margin-right:auto;">

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

SuperTrakGlobalPosition

```C
/* Global position on SuperTrak loop from section position */
signed long SuperTrakGlobalPosition(unsigned char section,
 signed long sectionPosition, /* [um] */
 unsigned char originSection, 
 signed long direction,
 signed long *globalPosition, /* [um] */
 struct SuperTrakPositionInfoType *info);
```

SuperTrakSectionPosition

```C
/* Section and section position on SuperTrak loop from global position */
signed long SuperTrakSectionPosition(signed long globalPosition, /* [um] */
 unsigned char originSection,
 signed long direction,
 unsigned char *section,
 signed long *sectionPosition, /* [um] */
 struct SuperTrakPositionInfoType *info);
```

SuperTrakTotalLength

```C
/* Total length of SuperTrak section network */
signed long SuperTrakTotalLength(signed long *length,
 struct SuperTrakPositionInfoType *info);
```

### Usage

#### IEC Structured Text

```
PositionResult := SuperTrakGlobalPosition(
    section         := MyInputSection,
    sectionPosition := MyInputSectionPosition,
    originSection   := MyOriginSection,
    direction       := MyDirection,
    globalPosition  := ADR(ReturnGlobalPosition),
    info            := PositionInfo
);
```

#### C

```C
positionResult = SuperTrakGlobalPosition(
    myInputSection,
    myInputSectionPosition,
    myOriginSection,
    myDirection,
    &returnGlobalPosition,
    &positionInfo
);
```

## Automation Studio

Version 4.9.3.144 SP

### Build

You may notice build warnings such as "Additional directory/file found ..." from the StPos submodule.

![File folder build warnings 2021-07-12_13 51 29](https://user-images.githubusercontent.com/33841634/125345690-77996e00-e31e-11eb-9f58-4bf563ac07a4.png)

Add `-W 9232 9233` to your CPU's build properties window under *Additional build options* to suppress.

## Dependencies

- `SuperTrak` >= 0.6.0

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
