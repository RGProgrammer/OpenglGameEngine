#ifndef TTB_UTILITIES_H_
#define TTB_UTILITIES_H_

#include ".//BasePrimitiveTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace RGP_CORE {
	inline _s16b contains(const _s8b* str1, const _s8b* str2) {
		_s16b result;
		if (str1 == NULL || str2 == NULL)
			return 0;
		_s16b len1 = strlen(str1), len2 = strlen(str2);
		if (len1 < len2)
			return 0;
		result = 0;
		for (_s16b i = 0; i < len1 - len2 + 1 && !result; i++) {
			result = i + 1;
			for (_s16b j = 0; str2[j] && result; j++)
				if (str1[i + j] != str2[j])
					result = 0;
		}
		return result;
	};
	inline _s16b CatStrings(const _s8b* str1, const _s8b* str2, _s8b** Dest) {
		if (!str1 || !str2 || !Dest)
			return 0;
		*Dest = (_s8b*)malloc((strlen(str1) + strlen(str2) + 1) * sizeof(_s8b));
		if (!*Dest)
			return 0;
		_s16b i, len = strlen(str1);
		for (i = 0; str1[i]; i++)
			(*Dest)[i] = str1[i];
		for (; str2[i - len]; i++)
			(*Dest)[i] = str2[i - len];
		(*Dest)[i] = '\0';
		return 1;
	};
	inline _s8b* ExtractString(_s8b* source, _s8b* borderstr1, _s8b* borderstr2) {
		_s16b border1, border2;
		_s8b* rslt = NULL;
		_s16b length, str1len;
		if ((border1 = contains(source, borderstr1))) {
			border1--;
			str1len = strlen(borderstr1);
			if ((border2 = contains(source + border1 + str1len, borderstr2))) {
				length = border2 - 1;
				rslt = (_s8b*)malloc((length + 1) * sizeof(_s8b));
				if (!rslt) {
					return rslt;
				}
				for (_s16b i = 0; i < length; i++) {
					rslt[i] = source[i + border1 + str1len];
				}
				rslt[length] = '\0';
			}
		}
		return rslt;
	};

	inline _s8b* ExtractDirectory(_s8b* filename) {
		if (!filename || filename[0] == '\0')
			return NULL;
		_s32b divider = 0, length = strlen(filename);
		for (_s32b i = length - 1; i >= 0; i--) {
			if (filename[i] == 47 || filename[i] == 92) {
				divider = i;
				break;
			}
		}
		_s8b *Directory = (_s8b*)malloc((divider + 1));
		for (_s32b i = 0; i <= divider; i++) {
			Directory[i] = filename[i];
		}
		Directory[divider] = '\0';
		return Directory;
	};
	inline void* CopyBuffer(const void* source, _u32b sizeinbytes) {
		if (!source)
			return NULL;
		_s8b* ret = (_s8b*)malloc(sizeinbytes);
		if (!ret)
			return NULL;
		memcpy(ret, source, sizeinbytes);
		return ret;
	}
	inline _s8b* CreateStringCopy(const _s8b* s)
	{
		return (_s8b*)CopyBuffer(s,strlen(s)+1);
	};
	
}
#endif
