#ifndef IMAGE_H_
#define IMAGE_H_

#include "Auto.h"



class ImageReference
{
protected:
	UInt32	m_Format;
	Int32	m_Width;
	Int32	m_Height;
	Int32	m_RowBytes;
	UInt8*	m_Image;

public:

	enum ClearMode
	{
		CLEAR_COLOR = 1,
		CLEAR_ALPHA = 2,
		CLEAR_COLOR_ALPHA = CLEAR_COLOR | CLEAR_ALPHA
	};

	enum BlitMode
	{
		BLIT_COPY,
		BLIT_SCALE,
		BLIT_BILINEAR_SCALE,
	};

	ImageReference() { m_Image = NULL; m_Width = 0; m_Height = 0; m_Format = 0; }
	//ImageReference(int width, int height, int rowbytes, TextureFormat format, void* image);
	//ImageReference(int width, int height, TextureFormat format);

	// Returns true if the image is exactly the same by camping width, height, and image data
	friend bool operator == (const ImageReference& lhs, const ImageReference& rhs);

	// Returns a subpart of the image
	ImageReference ClipImage(int x, int y, int width, int height) const;

	UInt8* GetImageData() const { return m_Image; }
	int GetRowBytes() const { return m_RowBytes; }
	//UInt8* GetRowPtr(int y) const { DebugAssertIf(y >= m_Height || y < 0); return m_Image + m_RowBytes * y; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	//TextureFormat GetFormat() const { return (TextureFormat)m_Format; }

	void BlitImage(const ImageReference& source, BlitMode mode = BLIT_COPY);
	void BlitImage(int x, int y, const ImageReference& source);

	//void ClearImage(const ColorRGBA32& color, ClearMode mode = CLEAR_COLOR_ALPHA);
	void FlipImageY();

	bool IsValidImage() const;

	//bool NeedsReformat(int width, int height, TextureFormat format) const;
};


class Image
{
public:
	Image();
	~Image();
};

#endif //!IMAGE_H_