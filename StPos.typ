
TYPE
	SuperTrakPositionErrorEnum : 
		( (*StPos function call results*)
		stPOS_ERROR_NONE, (*Function call successful*)
		stPOS_ERROR_SERV_CHAN, (*The service channel read request returned an error. See SuperTrak library for error code descriptions.*)
		stPOS_ERROR_LAYOUT, (*Unexpected system layout. Section count or section number out of range.*)
		stPOS_ERROR_SECTION, (*Input section out of range. 1..sectionCount.*)
		stPOS_ERROR_POSITION, (*Input position out of range or global position not found. 0..1000000 or 0..1030000 for curves.*)
		stPOS_ERROR_ORIGIN (*Input origin section out of range. 1..sectionCount.*)
		);
	SuperTrakPositionDiagType : 	STRUCT  (*StPos position transformation diagnostic information*)
		ServiceChannelResult_1080 : UINT; (*Service channel read result for parameter 1080 - Section Count [UInt16]*)
		ServiceChannelResult_1081 : UINT; (*Service channel read result for parameter 1081 - Section User Address [UInt16]*)
		ServiceChannelResult_1082 : UINT; (*Service channel read result for parameter 1082 - Section Type [UInt16]*)
		SectionCount : UINT; (*Section count of the system layout. 0 if no error.*)
		SectionAddress : UINT; (*Section address index on the system network. 0 if no error.*)
		SectionNumber : UINT; (*Section number at the network address index. 0 if no error.*)
	END_STRUCT;
END_TYPE
