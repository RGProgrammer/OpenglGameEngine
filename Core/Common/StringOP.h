#ifndef TTB_UTILITIES_H_
#define TTB_UTILITIES_H_
#include ".//BasePrimitiveTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


namespace RGP_CORE{

	_s16b contains(const _s8b* str1,const _s8b* str2);//the resturn the index of the occurence starting from 1
	_s16b CatStrings(const _s8b* str1,const _s8b* str2,_s8b** Dest);//concat 2 string
	_s8b* ExtractString(_s8b* source,_s8b* borderstr1,_s8b* borderstr2);//extract a string form the source
	_s8b* ExtractDirectory  (_s8b* filename);
}
#endif
