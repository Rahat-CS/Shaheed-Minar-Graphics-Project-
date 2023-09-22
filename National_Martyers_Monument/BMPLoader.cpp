#include "BMPLoader.h"

BMPLoader::BMPLoader(const std::string& filename)
{
	FILE* file;
	errno_t error = fopen_s(&file, filename.c_str(), "rb");

	if (error)
	{
		std::cerr << "Could not be opened: " << filename << std::endl;
		return;
	}

	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	if (header.bfType != 0x4D42)
	{
		std::cerr << filename << " is not a valid Bitmap Image" << std::endl;
		fclose(file);
		return;
	}

	fread(&info, sizeof(BITMAPINFOHEADER), 1, file);
	if (info.biBitCount != 24 && info.biBitCount != 32)
	{
		std::cerr << filename << " is not a valid 24 or 32 bit Bitmap Image" << std::endl;
		fclose(file);
		return;
	}

	m_NRChannels = info.biBitCount / 8;
	//std::cout << "Channels: " << m_NRChannels << std::endl;
	//std::cout << "Size: " << info.biSizeImage << std::endl;

	if (info.biSizeImage == 0) info.biSizeImage = info.biWidth * info.biHeight * m_NRChannels;
	//std::cout << "Size: " << info.biSizeImage << std::endl;

	m_Data = new unsigned char[info.biSizeImage];

	fseek(file, header.bfOffBits, SEEK_SET);
	fread(m_Data, info.biSizeImage, 1, file);

	m_Width = info.biWidth;
	m_Height = info.biHeight;

	fclose(file);
}

BMPLoader::~BMPLoader()
{
	delete[] m_Data;
}