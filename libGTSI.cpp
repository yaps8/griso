#ifndef LIBGTSI_CPP
#define LIBGTSI_CPP

#include "libGTSI.h"
// #include <Traversal.hpp>

char* labCharToString(vsize_t label){
	switch (label){
	case 0:
		return (char*) "INIT";
		break;
	case 1:
		return (char*) "RET";
		break;
	case 2:
		return (char*) "CALL";
		break;
	case 3:
		return (char*) "JMP";
		break;
	case 4:
		return (char*) "END";
		break;
	case 5:
		return (char*) "SCALL";
		break;
	case 6:
		return (char*) "UREACH";
		break;
	case 7:
		return (char*) "UNDEF";
		break;
	case 8:
		return (char*) "JCC";
		break;
	case 9:
		//return "SEQ";
		return (char*) "INST";
		break;
	case 10:
		return (char*) "PATH";
		break;
	case 11:
		return (char*) "END";
		break;
	default:
		return (char*) "(UNKNOWN)";
	}
}

#endif
