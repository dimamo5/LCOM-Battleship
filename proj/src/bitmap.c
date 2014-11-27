#include "bitmap.h"
#include "graphics.h"
#include "stdio.h"

Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *fileptr;
	fileptr = fopen(filename, "rb");
	if (fileptr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader.type, 2, 1, fileptr);

	// verify that this is a bmp is a real Bitmap
	if (bitmapFileHeader.type != BITMAP_TYPE) {
		fclose(fileptr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, fileptr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, fileptr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, fileptr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfo bitmapInfo;
	fread(&bitmapInfo, sizeof(BitmapInfo), 1, fileptr);

	// move file pointer to the begining of bitmap data
	fseek(fileptr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(bitmapInfo.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(fileptr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfo.imageSize, 1, fileptr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(fileptr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(fileptr);

	bmp->Data = bitmapImage;
	bmp->bitmapInfo = bitmapInfo;

	return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfo.width;

	int height = bmp->bitmapInfo.height;

	if (x + width < 0 || x > getHRes() || y + height < 0 || y > getVRes())
		return;
	aloca_pixmap(x,y,(unsigned short*)bmp->Data,width,height);


}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->Data);
	free(bmp);
}
