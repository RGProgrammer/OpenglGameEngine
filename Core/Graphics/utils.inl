

_bool RGP_CORE::LoadImageFromFile4Channels(const _s8b* filename ,Image** Output)
{
	(*Output) = NULL;
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	FIBITMAP *dib32(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);

	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename, FIT_UINT32);
	//if the image failed to load, return failure
	if (!dib)
		return false;
	//retrieve the image data
	dib32 = FreeImage_ConvertTo32Bits(dib);

	bits = FreeImage_GetBits(dib32);
	//get the image width and height
	width = FreeImage_GetWidth(dib32);
	height = FreeImage_GetHeight(dib32);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0)) {
		FreeImage_Unload(dib);
		return false;
	}


	//Copy Data to our own data structure 

	(*Output) = (pImage)malloc(sizeof(Image));
	if (!Output) {
		FreeImage_Unload(dib);
		return false;
	}
	(*Output)->Height = height;
	(*Output)->Width = width;
	(*Output)->Pixels = (_u8b*)malloc(width*height * 4);
	for (_u32b i = 0; i < width*height; ++i) {
		(*Output)->Pixels[i * 4] = bits[i * 4];
		(*Output)->Pixels[i * 4 + 1] = bits[i * 4 + 1];
		(*Output)->Pixels[i * 4 + 2] = bits[i * 4 + 2];
		(*Output)->Pixels[i * 4 + 3] = bits[i * 4 + 3];
	}

	FreeImage_Unload(dib);
	return true;
};


_bool  RGP_CORE::LoadImageFromFile4Channels2(const _s8b* filename, _s32b* outWidth, _s32b* outHeight, _u8b** outPixels)
{
	if (!(outPixels  && outHeight && outWidth)) {
		return false;
	}
	
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	FIBITMAP *dib32(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);

	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename, FIT_UINT32);
	//if the image failed to load, return failure
	if (!dib)
		return false;
	//retrieve the image data
	dib32 = FreeImage_ConvertTo32Bits(dib);

	bits = FreeImage_GetBits(dib32);
	//get the image width and height
	width = FreeImage_GetWidth(dib32);
	height = FreeImage_GetHeight(dib32);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0)) {
		FreeImage_Unload(dib);
		return false;
	}


	//Copy Data to our own data structure 
	

	(*outPixels) = (_u8b*)malloc(width*height*4*sizeof(_u8b));
	if (!(*outPixels)) {
		FreeImage_Unload(dib);
		return false;
	}

	(*outHeight) = height;
	(*outWidth) = width;
	for (_u32b i = 0; i < width*height; ++i) {
		(*outPixels)[i * 4] = bits[i * 4];
		(*outPixels)[i * 4 + 1] = bits[i * 4 + 1];
		(*outPixels)[i * 4 + 2] = bits[i * 4 + 2];
		(*outPixels)[i * 4 + 3] = bits[i * 4 + 3];
	}

	FreeImage_Unload(dib);
	return true;
};