(******************************************************************************
 * File: StPos.typ
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************)

TYPE
	SuperTrakPositionErrorEnum : 
		( (*StPos results*)
		stPOS_ERROR_NONE := 0, (*Function call successful*)
		stPOS_ERROR_SERVCHAN := 10, (*The service channel read request returned an error. See SuperTrak library for error code descriptions.*)
		stPOS_ERROR_SECTIONCOUNT, (*SuperTrak section count exceeds the maximum*)
		stPOS_ERROR_SECTIONADDRESS, (*SuperTrak section address exceeds the layout section count*)
		stPOS_ERROR_SECTIONTYPE, (*Invalid SuperTrak section type*)
		stPOS_ERROR_HEADSECTION, (*Invalid head section 1..64*)
		stPOS_ERROR_FLOWDIRECTION, (*Invalid flow direction 0..1*)
		stPOS_ERROR_NETWORKORDER := 100, (*Unexpected SuperTrak section network order*)
		stPOS_ERROR_INPUTORIGIN, (*Invalid input origin section for the given layout section count*)
		stPOS_ERROR_INPUTSECTION, (*Invalid input section for the given layout section count*)
		stPOS_ERROR_INPUTSECTIONPOS, (*Input position exceeds the allowed section position limit*)
		stPOS_ERROR_INPUTGLOBALPOS, (*Input global position exceeds the layout total length limit*)
		stPOS_ERROR_NETWORKGLOBAL (*The global position was not found in the SuperTrak section network*)
		);
	SuperTrakPositionInfoType : 	STRUCT  (*StPos diagnostic information*)
		serviceChannelResult : UINT; (*Service channel read result*)
		serviceChannelParameter : UINT; (*Service channel read parameter associated with serviceChannelResult*)
		section : UINT; (*User address of the section in context of an error*)
		sectionCount : UINT; (*Section count of the system layout*)
		sectionCountMax : UINT; (*Maximum sections allowed in a SuperTrak layout*)
		sectionType : UINT; (*Section type result*)
		sectionTypeMax : UINT; (*Maximum allowed section type*)
		networkIndex : UINT; (*Index in the network layout array of section user addresses*)
		headSection : UINT; (*(Par 1104) Active logical head section *)
		flowDirection : UINT; (*(Par 1105) Active pallet flow direction 0..1*)
		originSection : USINT; (*Input origin section when in context of an error*)
		sectionPosition : DINT; (*Input section position when in context of an error*)
		sectionPositionMax : DINT; (*Maximum allowed input position depending on the section type*)
		globalPosition : DINT; (*Input global position when in context of an error*)
		totalLength : DINT; (*Total length of the valid SuperTrak layout*)
	END_STRUCT;
	SuperTrakSystemLayoutType : 	STRUCT  (*StPos system layout information*)
		sectionCount : USINT; (*Number of sections in the layout*)
		sectionType : ARRAY[0..stPOS_SECTION_MAX]OF USINT; (*Section type, see SuperTrakReadyLayout comments*)
		networkOrder : ARRAY[0..stPOS_SECTION_INDEX]OF USINT; (*Section numbers from left network tail to right network tail*)
		networkMapping : ARRAY[0..stPOS_SECTION_MAX]OF USINT; (*Map from section number 1..64 to network index 0..63*)
		flowOrder : ARRAY[0..stPOS_SECTION_INDEX]OF USINT; (*Section numbers starting at head and continuing in flow direction*)
		layoutLinear : BOOL; (*True - linear, False - loop*)
	END_STRUCT;
END_TYPE
