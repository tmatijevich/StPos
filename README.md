# StPos

[StPos](https://github.com/tmatijevich/StPos) is a library for Automation Studio projects to use with the SuperTrak modular transport system.

This library provides functions to translate between local section positions and global loop positions.  
The functions are valid for any network layout.  
The global reference is defined by two user inputs: origin section and direction.  
**Notice**: This is not an offical B&R library. StPos is provided as is under the GNU GPL v3.0 license agreement.  
[Download the library here](https://github.com/tmatijevich/StPos/releases/latest/download/StPos.zip).

## Features

1. SuperTrak positions on a global reference
2. User-defined positive direction
3. Support for all SuperTrak layouts
	- Standard 180<span>&deg;</span> loop
	- Wide 180<span>&deg;</span> loop
	- 90<span>&deg;</span> loop
	- Linear
4. Efficient function calls for cyclic use
5. Export system layout for external processing

<p align="center">
<img src="https://user-images.githubusercontent.com/33841634/124838969-831b1c80-df4d-11eb-886e-6ab1ff9d2343.png" alt="Global position sample 2021-07-07_18 01 23" >
</p>

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

Then add the library in Automation Studio logical view with toolbox object "Existing Library".

## Functions

- [SuperTrakGlobalPosition()](#supertrakglobalposition)
- [SuperTrakSectionPosition()](#supertraksectionposition)
- [SuperTrakTotalLength()](#supertraktotallength)
- [SuperTrakSystemLayout()](#supertraksystemlayout)

**Output** parameters in bold.

---

#### [SuperTrakGlobalPosition](https://github.com/tmatijevich/StPos/blob/main/SuperTrakGlobalPosition.c?ts=4)

```C
/* Global position on SuperTrak loop from section position */
signed long SuperTrakGlobalPosition(unsigned char section, signed long sectionPosition, unsigned char originSection, signed long direction, signed long *globalPosition, struct SuperTrakPositionInfoType *info);
```

##### Parameters

Type 						| Parameter 			| Unit 	| Description
----------------------------|-----------------------|-------|------------
signed long (DINT) 			| **return** 			| 		| 0 - success, otherwise see SuperTrakPositionErrorEnum
unsigned char (USINT) 		| section 				| 		| Input section
signed long (DINT) 			| sectionPosition 		| um 	| Input section position 
unsigned char (USINT) 		| originSection 		| 		| Global reference section on system layout 
signed long (DINT) 			| direction 			| 		| System left/clockwise (0) or right/counter-clockwise (1) defined as position direction 
signed long (DINT) 			| **globalPosition** 	| um 	| Output global position 
SuperTrakPositionInfoType 	| **info** 				| 		| Diagnostic information

---

#### [SuperTrakSectionPosition](https://github.com/tmatijevich/StPos/blob/main/SuperTrakSectionPosition.c?ts=4)

```C
/* Section and section position on SuperTrak loop from global position */
signed long SuperTrakSectionPosition(signed long globalPosition, unsigned char originSection, signed long direction, unsigned char *section, signed long *sectionPosition, struct SuperTrakPositionInfoType *info);
```

##### Parameters

Type 						| Parameter 			| Unit 	| Description
----------------------------|-----------------------|-------|------------
signed long (DINT) 			| **return** 			| 		| 0 - success, otherwise see SuperTrakPositionErrorEnum
unsigned char (USINT) 		| globalPosition 		| um 	| Input global position 
unsigned char (USINT) 		| originSection 		| 		| Global reference section on system layout 
signed long (DINT) 			| direction 			| 		| System left/clockwise (0) or right/counter-clockwise (1) defined as position direction 
unsigned char (USINT) 		| **section** 			| 		| Output section
signed long (DINT) 			| **sectionPosition** 	| um 	| Output section position 
SuperTrakPositionInfoType 	| **info** 				| 		| Diagnostic information

---

#### [SuperTrakTotalLength](https://github.com/tmatijevich/StPos/blob/main/SuperTrakTotalLength.c?ts=4)

```C
/* Total length of SuperTrak section network */
signed long SuperTrakTotalLength(signed long *length, struct SuperTrakPositionInfoType *info);
```

##### Parameters

Type 						| Parameter 			| Unit 	| Description
----------------------------|-----------------------|-------|------------
signed long (DINT) 			| **return** 			| 		| 0 - success, otherwise see SuperTrakPositionErrorEnum
signed long (DINT) 			| **length** 			| um 	| Total length of SuperTrak layout
SuperTrakPositionInfoType 	| **info** 				| 		| Diagnostic information

---

#### [SuperTrakSystemLayout](https://github.com/tmatijevich/StPos/blob/main/SuperTrakSystemLayout.c?ts=4)

```C
/* Obtain the SuperTrak system layout information */
signed long SuperTrakSystemLayout(struct SuperTrakSystemLayoutType *layout, struct SuperTrakPositionInfoType *info)
```

##### Parameters

Type 						| Parameter 			| Unit 	| Description
----------------------------|-----------------------|-------|------------
signed long (DINT) 			| **return** 			| 		| 0 - success, otherwise see SuperTrakPositionErrorEnum
SuperTrakSystemLayoutType 	| **layout** 			| 		| System layout information
SuperTrakPositionInfoType 	| **info** 				| 		| Diagnostic information

#### Example

```
MyLayout
  sectionCount          6
  sectionAddress[0]     5  (Left network tail)
  sectionAddress[1]     4
  sectionAddress[2]     3  (Left network head)
  sectionAddress[3]     2  (Right network head)
  sectionAddress[4]     1
  sectionAddress[5]     6  (Right network tail)
  ...
  sectionType[0]        0
  sectionType[1]        0
  sectionType[2]        1
  sectionType[3]        0
  sectionType[4]        0
  sectionType[5]        1
  ...
  layoutLinear          FALSE
```

---

## Usage

#### Variable Declaration

```
VAR
    MyOriginSection : USINT;
    MyDirection : DINT;
    MyInputSection : USINT;
    MyInputSectionPosition : DINT; (*[um]*)
    PositionResult : SuperTrakPositionErrorEnum;
    PositionInfo : SuperTrakPositionInfoType;
    ReturnGlobalPosition : DINT; (*[um]*)
END_VAR
```

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
PositionResult = SuperTrakGlobalPosition(
    MyInputSection,
    MyInputSectionPosition,
    MyOriginSection,
    MyDirection,
    &ReturnGlobalPosition,
    &PositionInfo
);
```

## Error Handling

With the help of libraries [IecString](https://github.com/tmatijevich/IecString#readme) and [UserLog](https://github.com/tmatijevich/UserLog#readme), any errors from StPos functions can be logged with diagnostic information.

Declare logging variables.

```
VAR
    PositionResult : SuperTrakPositionErrorEnum;
    PositionInfo : SuperTrakPositionInfoType;
    Format : STRING[125]; 
    Message : STRING[125]; 
    FormatArgs : FormatStringArgumentsType; 
END_VAR
```

Create and call action if an error is detected, `PositionResult <> stPOS_ERROR_NONE`.

```
(* Log StPos error *)
ACTION LogStPosError:
    // This action assumes PositionResult and PositionInfo is declared and populated from a recent call to an StPos function
    CASE PositionResult OF
        stPOS_ERROR_SERVCHAN:
            IF PositionInfo.sectionCountResult <> scERR_SUCCESS THEN
                Format          := 'StPos layout service channel read error %i for section count';
                FormatArgs.i[0] := PositionInfo.sectionCountResult;
            ELSIF PositionInfo.sectionUserAddressResult <> scERR_SUCCESS THEN
                Format          := 'StPos layout service channel read error %i for section user address';
                FormatArgs.i[0] := PositionInfo.sectionUserAddressResult;
            ELSE // Section i type
                Format          := 'StPos layout service channel read error %i for section %i type';
                FormatArgs.i[0] := PositionInfo.sectionTypeResult;
                FormatArgs.i[1] := PositionInfo.section;
            END_IF
        stPOS_ERROR_SECTIONCOUNT:
            Format          := 'StPos layout section count %i exceeds maximum %i';
            FormatArgs.i[0] := PositionInfo.sectionCount;
            FormatArgs.i[1] := PositionInfo.sectionCountMax;
        stPOS_ERROR_SECTIONADDRESS:
            Format          := 'StPos layout section user address %i, network index %i, exceeds section count %i';
            FormatArgs.i[0] := PositionInfo.section;
            FormatArgs.i[1] := PositionInfo.networkIndex;
            FormatArgs.i[2] := PositionInfo.sectionCount;
        stPOS_ERROR_SECTIONTYPE:
            Format          := 'StPos layout invalid type %i for section %i';
            FormatArgs.i[0] := PositionInfo.sectionType;
            FormatArgs.i[1] := PositionInfo.section;
        stPOS_ERROR_NETWORKORDER:
            Format := 'StPos layout unexpected network order';
        stPOS_ERROR_INPUTORIGIN:
            Format          := 'StPos invalid input origin section %i for layout section count %i';
            FormatArgs.i[0] := PositionInfo.originSection;
            FormatArgs.i[1] := PositionInfo.sectionCount;
        stPOS_ERROR_INPUTSECTION:
            Format          := 'StPos invalid input section %i for layout section count %i';
            FormatArgs.i[0] := PositionInfo.section;
            FormatArgs.i[1] := PositionInfo.sectionCount;
        stPOS_ERROR_INPUTSECTIONPOS:
            Format          := 'StPos input section %i position %i um exceeds limits [0, %i] um';
            FormatArgs.i[0] := PositionInfo.section;
            FormatArgs.i[1] := PositionInfo.sectionPosition;
            FormatArgs.i[2] := PositionInfo.sectionPositionMax;
        stPOS_ERROR_INPUTGLOBALPOS:
            Format          := 'StPos input global position %i um exceeds limits [0, %i] um';
            FormatArgs.i[0] := PositionInfo.globalPosition;
            FormatArgs.i[1] := PositionInfo.totalLength;
        stPOS_ERROR_NETWORKGLOBAL:
            Format := 'StPos unexpected global position';
        ELSE
            Format := 'StPos unknown error';
    END_CASE
    
    IecFormatString(Message, SIZEOF(Message), Format, FormatArgs);
    LogMessage(USERLOG_SEVERITY_ERROR, 1234, Message);
END_ACTION
```

## Build

You may notice build warnings such as "Additional directory/file found ..." from the StPos submodule.

![Build warnings](https://user-images.githubusercontent.com/33841634/132114205-df6cd296-ad92-4381-a31f-fe0b8be230eb.png)

Add `-W 9232 9233` to your CPU's build properties window under *Additional build options* to suppress.

## Dependencies

- `SuperTrak` >= 0.6.0

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
