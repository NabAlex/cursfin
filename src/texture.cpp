#include "texture.h"

Texture::Texture(size_t generate_size)
{
    init(generate_size, generate_size);
}

void Texture::init(size_t w, size_t h)
{
    heigth = w;
    width = h;
    
    texture = new color_t*[heigth];
    for (size_t i = 0; i < heigth; ++i)
        texture[i] = new color_t[width];
}

void Texture::fill(int32_t startx, int32_t starty, int32_t w, int32_t h, color_t color)
{
    for (int32_t y = starty; y < starty + h; ++y)
        for (int32_t x = startx; x < startx + w; ++x)
            texture[y][x] = color;
}

color_t Texture::get_safe(double u, double v)
{
    u *= width - 1;
    v *= heigth - 1;
    
    int32_t px = (int) fabs(u), py = (int) fabs(v);
    
    return texture[px % width][py % heigth];
}

StoneTexture::StoneTexture(size_t generate_size, size_t fill_w) : Texture(generate_size)
{
    fill_count_width = fill_w;
    fill_real_size = generate_size / fill_count_width;
    
    color_t colors[] = {
        COLOR_PACK(0, 0, 0),
        COLOR_PACK(97, 97, 97),
        COLOR_PACK(33, 33, 33)
    };
    
    size_t colors_size = SIZE_OF_ARRAY(colors);
    size_t color_index = 0;
    
    const PerlinNoise perlin(time(0));
    const double fx = width / 20;
    const double fy = heigth / 20;
    
    int octaves = 10;
    
    fill_map = new color_t*[fill_count_width];
    for (int32_t i = 0; i < fill_count_width; ++i)
    {
        fill_map[i] = new color_t[fill_count_width];
        
        for (int32_t j = 0; j < fill_count_width; ++j)
        {
            double t = perlin.octaveNoise0_1(i / fx, j / fy, octaves);
            color_t now_color = COLOR_PACK(t * 255, t * 255, t * 255);
            
            // color_t now_color = colors[rand() % colors_size];
            // color_t now_color = colors[color_index++];
            
            color_index %= colors_size;
            
            fill(i * fill_real_size, j * fill_real_size,
                fill_real_size,
                fill_real_size,
                now_color
            );
            fill_map[i][j] = now_color;
        }
    }
}

color_t StoneTexture::get_safe(double u, double v)
{
    const static int32_t w2 = fill_real_size / 2, h2 = fill_real_size / 2;
    const static int32_t tmp = fill_real_size * fill_real_size;
    
    u *= width - 1;
    v *= heigth - 1;
    
    double x = w2 + fabs(u);
    double y = h2 + fabs(v);
    
    int32_t ix = ((int32_t) u / fill_real_size) % fill_count_width,
        iy = ((int32_t) v / fill_real_size) % fill_count_width;

//        0--1
//        |**|
//        3**2
    
    
    int32_t px = w2 + (ix * fill_real_size), px2 = px + fill_real_size;
    int32_t py = h2 + (iy * fill_real_size), py2 = py + fill_real_size;
    
    int32_t ix2 = ix + 1, iy2 = iy + 1;
    ix2 = (ix2 != fill_count_width) ? ix2 : ix;
    iy2 = (iy2 != fill_count_width) ? iy2 : iy;
    
    color_t q  = fill_map[ix][iy],
        q1 = fill_map[ix2][iy],
        q2 = fill_map[ix2][iy2],
        q3 = fill_map[ix][iy2];

#define COLOR_MIX_UNPACK(unpack, x1, x2, c1, c2, v) (((x2) - (v)) * (unpack(c1)) + ((v) - (x1)) * (unpack(c2)))
#define COLOR_MIX(x1, x2, c1, c2, v) (((x2) - (v)) * (c1) + ((v) - (x1)) * (c2))
    
    int32_t rr1= COLOR_MIX_UNPACK(COLOR_UNPACK_R, px, px2, q3, q2, x),
        gr1 = COLOR_MIX_UNPACK(COLOR_UNPACK_G, px, px2, q3, q2, x),
        br1 = COLOR_MIX_UNPACK(COLOR_UNPACK_B, px, px2, q3, q2, x);
    
    int32_t rr2 = COLOR_MIX_UNPACK(COLOR_UNPACK_R, px, px2, q, q1, x),
        gr2 = COLOR_MIX_UNPACK(COLOR_UNPACK_G, px, px2, q, q1, x),
        br2 = COLOR_MIX_UNPACK(COLOR_UNPACK_B, px, px2, q, q1, x);
    
    color_t f = color_pack(
        COLOR_MIX(py, py2, rr1, rr2, y) / tmp,
        COLOR_MIX(py, py2, gr1, gr2, y) / tmp,
        COLOR_MIX(py, py2, br1, br2, y) / tmp
    );

#undef COLOR_MIX
#undef COLOR_MIX_UNPACK
    return f;
}
