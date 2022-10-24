//
// Created by palom on 23/10/2022.
//

#ifndef UNTITLED_IMAGEAOS_HPP
#define UNTITLED_IMAGEAOS_HPP


#include <vector>
struct Color {
    float r, g, b;

    Color();
    Color(float r, float g, float b);
    ~Color();
};


class Image {
public:
    Image(int width, int height);
    ~Image();

    Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);
    void Export(const char* path) const;
    void Read(const char* path);
    void Copy(const char* path);
    void Gauss(const char* path);
    void Mono(const char* path);
    void Histo(const char* path);

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};


#endif //UNTITLED_IMAGEAOS_HPP
