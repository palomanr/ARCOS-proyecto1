//
// Created by palom on 23/10/2022.
//

#include "Image_aos.hpp"
#include <math.h>
#include <iostream>
#include <fstream>

Color::Color()
        : r(0), g(0), b(0)
{

}

Color::Color(float r, float g, float b)
        : r(r), g(g), b(b)
{

}

Color::~Color()
{

}

Image::Image(int width, int height)
        : m_width(width), m_height(height), m_colors(std::vector<Color>(width * height))
{

}
Image::~Image()
{

}

Color Image::GetColor(int x, int y) const
{
    return m_colors[y * m_width + x];
}

void Image::SetColor(const Color &color, int x, int y)
{
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;

}

void Image::Read(const char *path)
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open())
    {
        std::cout <<"File could not be opened" << std:: endl;
        return;
    }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout << "The specified path is not a bitmap image"<< std::endl;
        f.close();
        return;
    }

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    //int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] <<24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] <<24);

    m_colors.resize(m_width * m_height);

    const int paddingAmount = ((4- (m_width * 3) % 4) % 4);

    for (int y = 0; y < m_height; y++)
    {
        for (int x=0; x < m_width; x++)
        {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);

            m_colors[y *m_width + x].r = static_cast<float>(color[2] /255.0f);
            m_colors[y *m_width + x].g = static_cast<float>(color[1] /255.0f);
            m_colors[y *m_width + x].b = static_cast<float>(color[0] /255.0f);

        }
        f.ignore(paddingAmount);
    }
    f.close();

    std::cout << "File read" <<std::endl;
}



void Image::Export(const char *path) const
{
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open())
    {
        std::cout <<"File could not be opended\n";
        return;
    }

    unsigned char bmpPad[3] = {0, 0, 0};
    //3 * width es cuantos bytes ocupa un color
    //%4 es how many bytes over we are and 4 - that is how many bytes we have left
    //%4 y coger el resto
    const int paddingAmount = ((4 - (m_width * 3)%4)%4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_width;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //Reserved 2 (Not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    //Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //image width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    //image Height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per pixel (RGB)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    //Compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //Image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X pixeld per meter (no specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y pixeld per meter (no specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //total colors (color palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //importan colors (generally ignored)
    //X pixeld per meter (no specified)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();

    std::cout << "File created \n";
}

void Image::Copy(const char *path)
{
    Read(path);
    Export("copia.bmp");
    std::cout << "File copied" <<std::endl;
}

void Image::Histo(const char *path)
{
    std::fstream file;
    file.open("pruebahisto.hst",std::ios::out);

    Image::Read(path);
    Image image(m_width, m_height);

    std::vector<int> countColors(768, 0);

    for (int yy = 0; yy < m_height; yy++) {
        for (int xx = 0; xx < m_width; xx++) {
            double n[3];
            n[0] = m_colors[yy *m_width + xx].r*255.0f;
            n[1] = m_colors[yy *m_width + xx].g*255.0f;
            n[2] = m_colors[yy *m_width + xx].b*255.0f;
            countColors[n[0]] += 1;
            countColors[n[1] + 256] += 1;
            countColors[n[2] + 512] += 1;
        }
    }

    int count = 0;

    for (auto current: countColors) {
        file << current;
        file << " ";
        if (count == 255 || count == 511) {
            file << "\n";
        }
        count += 1;
    }

    file.close();
}

void Image::Gauss(const char *path)
{
    Image::Read(path);
    Image image(m_width, m_height);
    int m[5][5] = {{1, 4, 7, 4, 1},
                         {4, 16, 26, 16, 4},
                         {7, 26, 41, 26, 7},
                         {4, 16, 26, 16, 4},
                         {1, 4, 7, 4, 1}};
    double blur = 0.003663;
    double avgR = 0, avgG = 0, avgB = 0;
    for (int yy = 0; yy < m_height; yy++)
    {
        for (int xx = 0; xx < m_width; xx++)
        {
            for (int s = -2; s <=2; s++)
            {
                for (int t = -2; t<=2; t++)
                {
                        if (yy+s>m_height or xx+t > m_width or yy+s<0 or xx+t <0){
                            avgR += m[s + 2][t + 2] * (m_colors[(0) * m_width + (0)].r);
                            avgG += m[s+2][t+2] * (m_colors[(0) * m_width + (0)].g);
                            avgB += m[s+2][t+2] * (m_colors[(0) * m_width + (0)].b);
                            std::cout<<"nada";
                        }else {
                            avgR += m[s + 2][t + 2] * (m_colors[(yy+s) * m_width + (xx+t)].r);
                            avgG += m[s+2][t+2] * (m_colors[(yy+s) * m_width + (xx+t)].g);
                            avgB += m[s+2][t+2] * (m_colors[(yy+s) * m_width + (xx+t)].b);
                            std::cout<<"algo";
                        }
                }
            }
            double rojo = blur*avgR;
            double verde = blur*avgG;
            double azul = blur*avgB;
            image.SetColor(Color(rojo, verde, azul), xx, yy);
        }
    }
    image.Export("gaussprueba.bmp");
}

void Image::Mono(const char* path)
{
    Image::Read(path);
    Image image(m_width, m_height);
    double gamma;
    for (int yy = 0; yy < m_height; yy++)
    {
        for (int xx = 0; xx < m_width; xx++)
        {
            double n[3];
            n[0] = m_colors[yy *m_width + xx].r/255.0f;
            n[1] = m_colors[yy *m_width + xx].g/255.0f;
            n[2] = m_colors[yy *m_width + xx].b/255.0f;
            //Trasformacion a intensidad lineal
            for(int j=0; j<=2; j++){
                if (n[j]<=0.04045) {
                    n[j] = n[j]/12.92;
                }
                else{
                    n[j]= pow( (n[j] + 0.055)/1.0555 , 2.4);
                }
            }
            //Trasformacion lineal
            double cl = 0.2126*n[0] + 0.7152*n[1] + 0.0722*n[2];
            double cgamma;
            //Correccion gamma
            if (cl <= 0.0031308){
                cgamma = 12.92*cl;
            }
            else {
                cgamma = 1.055 * pow(cl, (1/2.4));
            }
            //Desnormalizacion
            gamma = cgamma*255;
            image.SetColor(Color(gamma, gamma, gamma), xx, yy);
        }
    }
    image.Export("monoprueba.bmp");
}





















