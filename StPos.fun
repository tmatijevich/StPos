
FUNCTION SuperTrakGlobalPosition : DINT (*Return the global position on the SuperTrak loop*)
	VAR_INPUT
		section : USINT; (*Section number*)
		sectionPosition : DINT; (*[um] Position with reference to left side of section*)
		originSection : USINT; (*First section number on loop of global position reference*)
		direction : USINT; (*Global direction*)
	END_VAR
	VAR_IN_OUT
		globalPosition : DINT; (*[um] Position computed on the global reference*)
		diag : SuperTrakPositionDiagType; (*Share diagnostic information*)
	END_VAR
END_FUNCTION

FUNCTION SuperTrakSectionPosition : DINT (*Return the section position on the SuperTrak loop*)
	VAR_INPUT
		globalPosition : DINT; (*[um] Global reference position*)
		originSection : USINT; (*First section number on loop of global position reference*)
		direction : USINT; (*Global direction*)
	END_VAR
	VAR_IN_OUT
		section : USINT; (*Computed section number*)
		sectionPosition : DINT; (*[um] Computed section position*)
		diag : SuperTrakPositionDiagType; (*Share diagnostic information*)
	END_VAR
END_FUNCTION
