#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_1_Core>

class Texture : protected QOpenGLFunctions_4_1_Core
{
public:
    Texture();
    Texture(const std::string &filename);
    GLuint id() const;
    std::string textureFilename;

private:
    GLubyte pixels[16];
    GLuint mId{0};
    unsigned char *mBitmap{nullptr};
    int mColumns;
    int mRows;
    int mBytesPrPixel;
    bool mAlphaUsed{false};
    bool readBitmap(const std::string& filename);
    void setTexture();

    class Logger *mLogger{nullptr};

private:
    typedef unsigned short int OWORD;
    typedef unsigned int ODWORD;
    typedef int OLONG;

    struct OBITMAPFILEHEADER {
        OWORD bfType;
        ODWORD bfSize;
        OWORD bfReserved1;
        OWORD bfReserved2;
        ODWORD bfOffBits;
    };

    struct OBITMAPINFOHEADER {
            ODWORD biSize;
            OLONG  biWidth;
            OLONG  biHeight;
            OWORD  biPlanes;
            OWORD  biBitCount;
            ODWORD biCompression;
            ODWORD biSizeImage;
            OLONG  biXPelsPerMeter;
            OLONG  biYPelsPerMeter;
            ODWORD biClrUsed;
            ODWORD biClrImportant;
        };
        void makeDummyTexture();

};

#endif // TEXTURE_H
