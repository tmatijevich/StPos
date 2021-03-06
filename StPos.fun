(******************************************************************************
 * File: StPos.fun
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************)

FUNCTION SuperTrakGlobalPosition : DINT (*Global position on SuperTrak*)
	VAR_INPUT
		section : USINT; (*Section number*)
		sectionPosition : DINT; (*[um] Position referenced to section's left edge*)
		originSection : USINT; (*First section in layout for global reference*)
		direction : DINT; (*Global direction (Left - 0, Right - 1)*)
		globalPosition : REFERENCE TO DINT; (*[um] Global position result*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakSectionPosition : DINT (*Section position on SuperTrak*)
	VAR_INPUT
		globalPosition : DINT; (*[um] Global position referenced to origin section*)
		originSection : USINT; (*First section in layout for global reference*)
		direction : DINT; (*Global direction (Left - 0, Right - 1)*)
		section : REFERENCE TO USINT; (*Section number result*)
		sectionPosition : REFERENCE TO DINT; (*[um] Section position result referenced to section's left edge*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakTotalLength : DINT (*Total pallet travel length of the SuperTrak section network*)
	VAR_INPUT
		length : REFERENCE TO DINT; (*Pointer to DINT to store the SuperTrak total length*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakSystemLayout : DINT (*SuperTrak system layout*)
	VAR_INPUT
		layout : SuperTrakSystemLayoutType; (*Reference to the system layout structure*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakRefreshLayout : DINT (*Force re-read of SuperTrak system layout after encoder calibration*)
END_FUNCTION
