
TYPE
	SuperTrakPositionErrorEnum : 
		( (*StPos function call results*)
		stPOS_ERROR_NONE := 0, (*Function call successful*)
		stPOS_ERROR_SERVCHAN := 10, (*The service channel read request returned an error. See SuperTrak library for error code descriptions.*)
		stPOS_ERROR_SECTIONCOUNT, (*SuperTrak section count exceeds the maximum*)
		stPOS_ERROR_SECTIONADDRESS, (*SuperTrak section address exceeds the layout section count*)
		stPOS_ERROR_SECTIONTYPE, (*Invalid SuperTrak section type*)
		stPOS_ERROR_NETWORKORDER, (*Unexpected SuperTrak section network order*)
		stPOS_ERROR_INPUTORIGIN, (*Invalid input origin section for the given layout section count*)
		stPOS_ERROR_INPUTSECTION, (*Invalid input section for the given layout section count*)
		stPOS_ERROR_INPUTSECTIONPOS, (*Input position exceeds the allowed section position limit*)
		stPOS_ERROR_INPUTGLOBALPOS, (*Input global position exceeds the layout total length limit*)
		stPOS_ERROR_NETWORKGLOBAL (*The global position was not found in the SuperTrak section network*)
		);
	SuperTrakPositionInfoType : 	STRUCT  (*StPos position transformation diagnostic information*)
		sectionCountResult : UINT; (*Service channel read result for parameter 1080 - Section Count [UInt16]*)
		sectionUserAddressResult : UINT; (*Service channel read result for parameter 1081 - Section User Address [UInt16]*)
		sectionTypeResult : UINT; (*Service channel read result for parameter 1082 - Section Type [UInt16]*)
		section : UINT; (*User address of the section in context of an error*)
		sectionCount : UINT; (*Section count of the system layout*)
		sectionCountMax : UINT; (*Maximum sections allowed in a SuperTrak layout*)
		sectionType : UINT; (*Section type result*)
		sectionTypeMax : UINT; (*Maximum allowed section type*)
		networkIndex : UINT; (*Index in the network layout array of section user addresses*)
		originSection : USINT; (*Input origin section when in context of an error*)
		sectionPosition : DINT; (*Input section position when in context of an error*)
		sectionPositionMax : DINT; (*Maximum allowed input position depending on the section type*)
		globalPosition : DINT; (*Input global position when in context of an error*)
		totalLength : DINT; (*Total length of the valid SuperTrak layout*)
	END_STRUCT;
END_TYPE
