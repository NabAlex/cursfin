#include "renderer.h"

void Renderer::ResetBuffer() {
    for (register int i = 0, N = Width*Height; i < N; i++)
        ZBuffer[i] = INT_MAX;
    /*for (int x = 0; x < Width; x++)
        for (int y = 0; y < Height; y++)
            ZBuffer[x + y*Width] = 10000000;*/
    
    
}

inline int max(double a, double b, double c) {
    if (a < b) a = b;
    if (a < c) a = c;
    return int(a + 0.5);
}

inline int min(double a, double b, double c) {
    if (a > b) a = b;
    if (a > c) a = c;
    return int(a + 0.5);
}

Renderer::Renderer(Model *model)
{
    Width = 1; // TODO Image->width();
    Height = 1; // TODO Image->height();
    
    this->model = model;
    ZBuffer = new double[Width*Height];
    ResetBuffer();
}


Renderer::~Renderer()
{
    delete[] ZBuffer;
}


Dot Renderer::MultiVect(const Dot &A, const Dot &B) {
    Dot Temp;
    Temp.x = A.y*B.z - A.z*B.y;
    Temp.y = A.z*B.x - A.x*B.z;
    Temp.z = A.x*B.y - A.y*B.x;
    return Temp;
}

Dot Renderer::barycentric(Dot &A, Dot &B, Dot &C, Dot &P) {
    
    double delta = (B.x - A.x)*(C.y - A.y) - (C.x - A.x)*(B.y - A.y);
    double delta1 = (B.x*C.y - C.x*B.y) + P.x*(B.y - C.y) + P.y*(C.x - B.x);
    double delta2 = (C.x*A.y - A.x*C.y) + P.x*(C.y - A.y) + P.y*(A.x - C.x);
    
    double a = delta1 / delta;
    double b = delta2 / delta;
    
    
    return Dot(a, b, 1. - a - b);
    
}

void Renderer::Triangle( Dot &v1,  Dot &v2,  Dot &v3, Dot &Tv1, Dot &Tv2, Dot &Tv3,int rgb) {
    int MaxX = max(v1.x, v2.x, v3.x); //WITH OKRUGL
    int MinX = min(v1.x, v2.x, v3.x);
    int MaxY = max(v1.y, v2.y, v3.y);
    int MinY = min(v1.y, v2.y, v3.y);
    int MaxZ = max(v1.z, v2.z, v3.z);
    int MinZ = min(v1.z, v2.z, v3.z);
    
    //Cutter
    {
        if (MaxZ < near || MinZ > far) return;
        if (MaxX < 0 || MaxY <0 || MinX>Width || MinY > Height) return;
        if (MaxX >= Width) MaxX = Width - 1;
        if (MinX < 0) MinX = 0;
        if (MaxY >= Height) MaxY = Height - 1;
        if (MinY < 0) MinY = 0;
    }
    //Cutter
    
    Dot P(MinX, MinY,1);
    
    double color = 0;
    double z = 0;
    Dot Bary;
    for (int x = MinX; x <= MaxX; x++, P.x++) {
        P.y = MinY;
        for (int y = MinY; y <= MaxY; y++, P.y++) {
            Bary = barycentric(v1, v2, v3, P);
            
            if (Bary.x < 0 || Bary.y < 0 || Bary.z < 0)
                continue;
            
            
            z = Bary.x*v1.z + Bary.y*v2.z + Bary.z*v3.z;
            //z = 1 / z;
            
            if (z >= near && z < ZBuffer[x + y*Width]) {
                ZBuffer[x + y*Width] = z;
                
                // TODO Image->setPixel(x, y, rgb);
            }
            
            
            
            
        }
    }
}

void  Renderer::DrawAll(double Far,double Near) {
//    far = Far;
//    near = Near;
//
//
//    int rgb = color.rgb();
//
//    Image->fill(QColor(196,246,255));
//    ResetBuffer();
//
//    for (int i = 0, N = Connections->size(); i < N; i++) {
//        rgb = cl[i];// QColor(rand() % 255, rand() % 255, rand() % 255).rgb();
//        //rgb = QColor(Qt::gray).rgb();
//        Triangle((*Vertex)[(*Connections)[i][0]], (*Vertex)[(*Connections)[i][1]], (*Vertex)[(*Connections)[i][2]],
//            (*TexVertex)[(*TexConnections)[i][0]], (*TexVertex)[(*TexConnections)[i][1]], (*TexVertex)[(*TexConnections)[i][2]]
//            ,rgb);
//    }
//
//    vector <Dot>* Vp = Grass->getVertex();
//    vector <vector<int>>* Cp = Grass->getConnections();
//    vector <Dot>* TVp = Grass->getTexVertex();
//    vector <vector<int>>* TCp = Grass->getTexConnections();
//
//    for (int i = 0, N = Cp->size(); i < N; i++) {
//        TriangleTex((*Vp)[(*Cp)[i][0]], (*Vp)[(*Cp)[i][1]], (*Vp)[(*Cp)[i][2]],
//            (*TVp)[(*TCp)[i][0]], (*TVp)[(*TCp)[i][1]], (*TVp)[(*TCp)[i][2]]
//            , rgb);
//    }
//
//
//    for (int i = 0, N = Pyramids->size(); i < N; i++) {
//        vector <Dot>* V = (*Pyramids)[i].getVertex();
//        vector <vector<int>>* C = (*Pyramids)[i].getConnections();
//        for (int j = 0, M = C->size(); j < M; j++) {
//            TriangleG((*V)[(*C)[j][0]], (*V)[(*C)[j][1]], (*V)[(*C)[j][2]], rgb);
//        }
//    }
}
