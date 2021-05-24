
TYPE
	SuperTrakPositionErrorEnum : 
		(
		ST_POS_ERROR_NONE,
		ST_POS_ERROR_RTC,
		ST_POS_ERROR_SERV_CHAN,
		ST_POS_ERROR_LAYOUT, (*Unexpected system layout. Section count or section addresses out of range*)
		ST_POS_ERROR_SECTION, (*Input section out of range. 1..sectionCount.*)
		ST_POS_ERROR_POSITION, (*Input position out of range. 0..1000000 or 0..1030000 for curves.*)
		ST_POS_ERROR_ORIGIN (*Input origin section out of range. 1..sectionCount.*)
		);
	SuperTrakPositionDiagType : 	STRUCT  (*SuperTrak position transformation diagnostic information*)
		Read : BOOL; (*A new scan was detected and the SuperTrak parameters were read on the service channel*)
		ServiceChannel1080Result : UINT; (*Service channel read result for parameter 1080 - Section Count [UInt16]*)
		ServiceChannel1081Result : UINT; (*Service channel read result for parameter 1081 - Section User Address [UInt16]*)
		ServiceChannel1082Result : UINT; (*Service channel read result for parameter 1082 - Section Type [UInt16]*)
		CurrentTimeResult : UINT;
		PreviousTimeResult : UINT;
		Index : USINT;
		Address : UINT;
		SectionCount : UINT;
	END_STRUCT;
END_TYPE
