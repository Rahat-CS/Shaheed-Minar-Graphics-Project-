#pragma once

#include <GL/freeglut.h>

#include <iostream>


class BMPLoader
{
private:
	BITMAPFILEHEADER header{};
	BITMAPINFOHEADER info{};

public:
	unsigned char* m_Data;
	int m_Width;
	int m_Height;
	int m_NRChannels;
public:
	BMPLoader(const std::string& filename);
	~BMPLoader();
};