# StPos

[StPos](https://github.com/tmatijevich/StPos) is an Automation Studio library for the SuperTrak modular transport system.

The StPos library provides functions to translate between local section positions and global loop positions.  
The functions are valid for any network layout.  
The global reference is defined by two user inputs: origin section and direction.  
**Notice**: This is not an offical library. StPos is provided as is under the GNU GPL v3.0 license agreement.  
[Download the library here](https://github.com/tmatijevich/StPos/releases/latest/download/StPos.zip).

## Features

1. SuperTrak positions on a global reference
2. User-defined positive direction
3. Support for all SuperTrak layouts
    - Standard 180<span>&deg;</span> loop
    - Wide 180<span>&deg;</span> loop
    - 90<span>&deg;</span> loop
    - Linear
4. Hardware-specific calibrated section lengths
5. Efficient function calls for cyclic use
6. Export system layout for external processing

![StPos 6 section layout example900](https://user-images.githubusercontent.com/33841634/138628068-90dba192-4208-4b9a-aa2f-dcc6cd941c00.png)

<p align="center">
<img style="width:562px; height:auto;"  src="https://user-images.githubusercontent.com/33841634/138628120-03121570-b82e-4fb1-99d8-d8afd9ce0f57.png" alt="StPos 6 section sample plot" >
</p>

In this example, the origin section is 2 and the positive direction is left (clockwise). The section lengths are exactly 1000.0 and 1030.0, however, the user's system will likely vary in section length slightly accounting for the unique encoder calibration. 

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

## Calibration

Physical SuperTrak systems require encoder calibration when commissioned. This leads to calibrated section lengths - deviations in microns from the nominal lengths. StPos functions use the calibrated lengths when considering global positions. This ensures global position accuracy will reflect physical systems. 

**Note**: Simulated systems have calibrated lengths exactly equal to the nominal lengths.

Here is an example of global positions two sections on a system with unique calibrated lengths. The global direction is right (the section position direction is always right).

<img width="8504" alt="Calibration" src="https://user-images.githubusercontent.com/33841634/153919536-6ad95d8f-3a1a-4ebe-9b76-7ffe5dff8ff8.png">

Here is another example with the global direction going left (the section position direction is always right).

<img width="8504" alt="Calibration left" src="https://user-images.githubusercontent.com/33841634/153919578-6b5ebc50-a37c-4cd4-9273-f7cc29be621b.png">

## Functions

- [SuperTrakGlobalPosition()](#supertrakglobalposition)
- [SuperTrakSectionPosition()](#supertraksectionposition)
- [SuperTrakTotalLength()](#supertraktotallength)
- [SuperTrakSystemLayout()](#supertraksystemlayout)

The internal `SuperTrakReadLayout()` function is only called when there is a change in user origin section or direction. This keeps CPU usage to a minimum. If the user re-calibrates the section encoders, the user may warm-restart or call `SuperTrakRefreshLayout()` to force a new internal call to `SuperTrakReadLayout()`. This will update the calibrated section lengths.

---

#### [SuperTrakGlobalPosition](https://github.com/tmatijevich/StPos/blob/main/SuperTrakGlobalPosition.c?ts=4)

```C
/* Global position on SuperTrak loop from section position */
long SuperTrakGlobalPosition(unsigned char section, long sectionPosition, unsigned char originSection, long direction, long *globalPosition, struct SuperTrakPositionInfoType *info);
```

##### Parameters

Parameter           | In/Out    | C Type (IEC Type)             | Unit      | Description       
--------------------|-----------|-------------------------------|-----------|------------------
section             | In        | unsigned char (USINT)         |           | Input section 
sectionPosition     | In        | long (DINT)                   | um        | Input section position 
originSection       | In        | unsigned char (USINT)         |           | Define global reference section on system layout 
direction           | In        | long (DINT)                   |           | Define global direction 0: left (CW) 1: right (CCW) 
globalPosition      | Out       | long (DINT)                   | um        | Calculated global position 
info                | Out       | SuperTrakPositionInfoType     |           | Diagnostic information 
return              | Out       | long (DINT)                   |           | 0: Success, see SuperTrakPositionErrorEnum otherwise 

---

#### [SuperTrakSectionPosition](https://github.com/tmatijevich/StPos/blob/main/SuperTrakSectionPosition.c?ts=4)

```C
/* Section and section position on SuperTrak loop from global position */
long SuperTrakSectionPosition(long globalPosition, unsigned char originSection, long direction, unsigned char *section, long *sectionPosition, struct SuperTrakPositionInfoType *info);
```

##### Parameters

Parameter           | In/Out    | C Type (IEC Type)             | Unit      | Description       
--------------------|-----------|-------------------------------|-----------|------------------
globalPosition      | In        | long (DINT)                   | um        | Input global position 
originSection       | In        | unsigned char (USINT)         |           | Define global reference section on system layout 
direction           | In        | long (DINT)                   |           | Define global direction 0: left (CW) 1: right (CCW) 
section             | Out       | unsigned char (USINT)         |           | Calculated section 
sectionPosition     | Out       | long (DINT)                   | um        | Calculated section position 
info                | Out       | SuperTrakPositionInfoType     |           | Diagnostic information 
return              | Out       | long (DINT)                   |           | 0: Success, see SuperTrakPositionErrorEnum otherwise 

---

#### [SuperTrakTotalLength](https://github.com/tmatijevich/StPos/blob/main/SuperTrakTotalLength.c?ts=4)

```C
/* Total pallet travel length of the SuperTrak section network */
long SuperTrakTotalLength(long *length, struct SuperTrakPositionInfoType *info);
```

This function provides the total **calibrated** length of all sections on the system. If in simulation, not encoder calibration is required and the calibrated lengths are initialized to the nominal lengths.

##### Parameters

Parameter           | In/Out    | C Type (IEC Type)             | Unit      | Description       
--------------------|-----------|-------------------------------|-----------|------------------
length              | Out       | long (DINT)                   | um        | Calculated total length of SuperTrak layout
info                | Out       | SuperTrakPositionInfoType     |           | Diagnostic information 
return              | Out       | long (DINT)                   |           | 0: Success, see SuperTrakPositionErrorEnum otherwise 

---

#### [SuperTrakSystemLayout](https://github.com/tmatijevich/StPos/blob/main/SuperTrakSystemLayout.c?ts=4)

```C
/* Obtain the SuperTrak system layout information */
long SuperTrakSystemLayout(struct SuperTrakSystemLayoutType *layout, struct SuperTrakPositionInfoType *info)
```

##### Parameters

Parameter           | In/Out    | C Type (IEC Type)             | Unit      | Description       
--------------------|-----------|-------------------------------|-----------|------------------
layout              | Out       | SuperTrakSystemLayoutType     |           | System layout information 
info                | Out       | SuperTrakPositionInfoType     |           | Diagnostic information 
return              | Out       | long (DINT)                   |           | 0: Success, see SuperTrakPositionErrorEnum otherwise 

#### Example

```
MyLayout
  sectionCount          6
  networkOrder[0]       5  (Left network tail)
  networkOrder[1]       4
  networkOrder[2]       3  (Left network head)
  networkOrder[3]       2  (Right network head)
  networkOrder[4]       1
  networkOrder[5]       6  (Right network tail)
  ...
  networkMapping[1]     4  (Section #1 is in network order 4)
  networkMapping[2]     3
  networkMapping[3]     2
  networkMapping[4]     1
  networkMapping[5]     0  (Section #5 is at the left tail)
  networkMapping[6]     5 
  ...
  flowOrder[0]          1  (Section #1 is the logical head section)
  flowOrder[1]          2  (Next section in flow direction of head)
  flowOrder[2]          3 
  flowOrder[3]          4 
  flowOrder[4]          5 
  flowOrder[5]          6 
  ...
  sectionType[1]        0  (Standard straight)
  sectionType[2]        0
  sectionType[3]        1  (Standard curve)
  sectionType[4]        0
  sectionType[5]        0
  sectionType[6]        1  
  ...
  layoutLinear          FALSE
```

---

## Usage

#### Variable Declaration

```iecst
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

```iecst
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

```iecst
VAR
    PositionResult : SuperTrakPositionErrorEnum;
    PositionInfo : SuperTrakPositionInfoType;
    Format : STRING[125]; 
    Message : STRING[125]; 
    FormatArgs : FormatStringArgumentsType; 
END_VAR
```

Create and call action if an error is detected, `PositionResult <> stPOS_ERROR_NONE`.

```iecst
(* Log StPos error *)
ACTION LogStPosError:
    // This action assumes PositionResult and PositionInfo is declared and populated from a recent call to an StPos function
    CASE PositionResult OF
        stPOS_ERROR_SERVCHAN:
            Format          := 'StPos layout service channel read error %i parameter %i';
            FormatArgs.i[0] := PositionInfo.serviceChannelResult;
            FormatArgs.i[1] := PositionInfo.serviceChannelParameter;
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
        stPOS_ERROR_HEADSECTION:
            Format          := 'StPos layout head section %i exceeds section count %i';
            FormatArgs.i[0] := PositionInfo.headSection;
            FormatArgs.i[1] := PositionInfo.sectionCount;
        stPOS_ERROR_FLOWDIRECTION:
            Format          := 'StPos layout invalid flow direction %i (0 - left, 1 - right)';
            FormatArgs.i[0] := PositionInfo.flowDirection;
        stPOS_ERROR_NETWORKORDER:
            Format          := 'StPos layout unexpected network order';
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
