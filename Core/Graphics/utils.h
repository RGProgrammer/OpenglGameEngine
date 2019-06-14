#ifndef RGP_GRAPHICS_UTILS_H_
#define RGP_GRAPHICS_UTILS_H_

#include "..//..//tools//freeimage//include//FreeImage.h"

namespace RGP_CORE {

	typedef struct {
		_s32b		Width;
		_s32b		Height;
		_u8b*		Pixels;
	} Image , *pImage;/// we only accepts RGBA format

	typedef struct {
		_s8b*			Name;
		Image*			DiffuseMap;
		Image*			SpecularMap;
		Image*			NormalsMap;
		Image*          EmissiveMap;
		_float			IOR;
		_float			Opacity;
	} Material , *pMaterial;

	inline _bool LoadImageFromFile4Channels(const _s8b* filename ,pImage* Output);

	inline _bool LoadImageFromFile4Channels2(const _s8b* filename, _s32b* outWidth, _s32b* outHeight, _u8b** outPixels);


}
#include "./utils.inl"
#endif // !RGP_GRAPHICS_UTILS_H_
