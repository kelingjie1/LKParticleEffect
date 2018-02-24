//
//  LKParticleEffectTexture.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectTexture.h"

#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>


using namespace LKKit;
using namespace std;
struct LKData
{
    unsigned char *data;
    unsigned long length;
};

typedef struct __attribute__((packed))
{
    uint8_t identifier[12];
    uint32_t endianness;
    uint32_t glType;
    uint32_t glTypeSize;
    uint32_t glFormat;
    uint32_t glInternalFormat;
    uint32_t glBaseInternalFormat;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t arrayElementCount;
    uint32_t faceCount;
    uint32_t mipmapCount;
    uint32_t keyValueDataLength;
} KTXHeader;

uint32_t LKSwapInt32(uint32_t arg) {
#if CF_USE_OSBYTEORDER_H
    return OSSwapInt32(arg);
#else
    uint32_t result;
    result = ((arg & 0xFF) << 24) | ((arg & 0xFF00) << 8) | ((arg >> 8) & 0xFF00) | ((arg >> 24) & 0xFF);
    return result;
#endif
}

unsigned char buffer[1024*1024*5];
void LKParticleEffectTexture::loadKTXFile(string path)
{
    fstream f(path,ios::in|ios::binary);
    f.read((char*)buffer, 1024*1024*5);
    int length = (int)f.gcount();
    loadKTXData(buffer, length);
}

void LKParticleEffectTexture::loadKTXData(uint8_t *data,int length)
{
    KTXHeader *header = (KTXHeader *)data;
    
    bool endianSwap = (header->endianness == 0x01020304);
    
    width = endianSwap ? LKSwapInt32(header->width) : header->width;
    height = endianSwap ? LKSwapInt32(header->height) : header->height;
    internalFormat = endianSwap ? LKSwapInt32(header->glInternalFormat) : header->glInternalFormat;
    
    if (internalFormat == GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG||
        internalFormat == GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG||
        internalFormat == GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG||
        internalFormat == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG||
        internalFormat == GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT||
        internalFormat == GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT||
        internalFormat == GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT||
        internalFormat == GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT)
    {
        premultalpha = true;
    }
    else
    {
        premultalpha = false;
    }
    
    uint32_t mipCount = endianSwap ? LKSwapInt32(header->mipmapCount) : header->mipmapCount;
    uint32_t keyValueDataLength = endianSwap ? LKSwapInt32(header->keyValueDataLength) : header->keyValueDataLength;
    
    const uint8_t *bytes = data + sizeof(KTXHeader) + keyValueDataLength;
    const size_t dataLength = length - (sizeof(KTXHeader) + keyValueDataLength);

    vector<LKData> datas;
    
    const uint32_t blockSize = 16;
    uint32_t dataOffset = 0;
    uint32_t levelWidth = width, levelHeight = height;
    while (dataOffset < dataLength)
    {
        uint32_t levelSize = *(uint32_t *)(bytes + dataOffset);
        dataOffset += sizeof(uint32_t);
        
        LKData mipData;
        mipData.data = (uint8_t*)bytes + dataOffset;
        mipData.length = levelSize;
        datas.push_back(mipData);
        
        dataOffset += levelSize;
        levelWidth = fmax(levelWidth / 2, 1);
        levelHeight = fmax(levelHeight / 2, 1);
    }
    
    LKData imageData = datas[0];
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei)width, (GLsizei)height, 0, (GLsizei)imageData.length, imageData.data);
    GLenum glerror = glGetError();
    glBindTexture(GL_TEXTURE_2D, 0);
    
}