
FUNCTION SuperTrakGlobalPosition : DINT (*Return the global position on the SuperTrak loop*)
	VAR_INPUT
		section : USINT; (*Section number*)
		sectionPosition : DINT; (*[um] Position referenced to section's left edge*)
		originSection : USINT; (*First section in layout for global reference*)
		direction : DINT; (*Global direction (Left - 0, Right - 1)*)
		globalPosition : REFERENCE TO DINT; (*[um] Global position result*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakSectionPosition : DINT (*Return the section position on the SuperTrak loop*)
	VAR_INPUT
		globalPosition : DINT; (*[um] Global position referenced to origin section*)
		originSection : USINT; (*First section in layout for global reference*)
		direction : DINT; (*Global direction (Left - 0, Right - 1)*)
		section : REFERENCE TO USINT; (*Section number result*)
		sectionPosition : REFERENCE TO DINT; (*[um] Section position result referenced to section's left edge*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakTotalLength : DINT (*Return the total pallet travel length of the SuperTrak section network*)
	VAR_INPUT
		length : REFERENCE TO DINT; (*Pointer to DINT to store the SuperTrak total length*)
		info : SuperTrakPositionInfoType; (*Diagnostic information*)
	END_VAR
END_FUNCTION
