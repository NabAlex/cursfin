#include "camera.h"

Camera::~Camera()
{}

Camera::Camera(const Dot &eye, const Dot &center, const Dot &up, double XZangle, double YZangle, double far, double near, int height, int weight)
{
    Vertex = nullptr;
    Connections = nullptr;
    
    Eye = eye;
    Center = center;
    Up = up;
    
    Width = weight;
    Heigth = height;
    this->XZangle = XZangle;
    this->YZangle = YZangle;
    this->far = far;
    this->near = near;
    
    ReCalcMatrix();
}

void Camera::SetActive(Model* Model) {
    Vertex = Model->getVertex();
    Connections = Model->getConnections();
}


void Camera::Transform() {
    int g = 9;
    Dot *target;
    for (int i = 0, N = Vertex->size(); i < N; i++) {
        target = &(*Vertex)[i];
        CameraPort(*target);
        PProjection(*target);
        ToScreen(*target);
    }
}


void Camera::Reverse(double matr[4][4]) {
    double det = matr[0][0] * matr[1][1] * matr[2][2] - matr[0][0] * matr[1][2] * matr[2][1]
        - matr[0][1] * matr[1][0] * matr[2][2] + matr[0][1] * matr[1][2] * matr[2][0]
        + matr[0][2] * matr[1][0] * matr[2][1] - matr[0][2] * matr[1][1] * matr[2][0];
    
    
    double dopoln[3][3] = { { matr[1][1] * matr[2][2] - matr[1][2] * matr[2][1],-matr[1][0] * matr[2][2] + matr[2][0] * matr[1][2],matr[1][0] * matr[2][1] - matr[1][1] * matr[2][0] },
        { -matr[0][1] * matr[2][2] + matr[0][2] * matr[2][1],matr[0][0] * matr[2][2] - matr[2][0] * matr[0][2],-matr[0][0] * matr[2][1] + matr[0][1] * matr[2][0] },
        { matr[0][1] * matr[1][2] - matr[1][1] * matr[0][2],-matr[0][0] * matr[1][2] + matr[1][0] * matr[0][2],matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1] }
    };
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matr[j][i] = (dopoln[i][j] / det);
        }
    }
}


void Camera::MultiMatrix(double matrix1[4][4], double matrix2[4][4], double(res[4][4])) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int r = 0; r < 4; r++)
                res[i][j] += matrix1[i][r] * matrix2[r][j];
}


void Camera::MultiDot(Dot &V, double matrix[4][4]) {
    Dot new_dot = V;
    new_dot.x = matrix[0][0] * V.x + matrix[1][0] * V.y + matrix[2][0] * V.z + matrix[3][0] * V.w;
    new_dot.y = matrix[0][1] * V.x + matrix[1][1] * V.y + matrix[2][1] * V.z + matrix[3][1] * V.w;
    new_dot.z = matrix[0][2] * V.x + matrix[1][2] * V.y + matrix[2][2] * V.z + matrix[3][2] * V.w;
    new_dot.w = matrix[0][3] * V.x + matrix[1][3] * V.y + matrix[2][3] * V.z + matrix[3][3] * V.w;
    V = new_dot;
}


void Camera::SetPProjMatrix() {
    double hfou = XZangle / 180 * M_PI;
    double right = tan(hfou / 2);
    double left = -right;
    
    double aspect = double(Width) / Heigth;
    
    double ufou = YZangle / 180 * M_PI;
    double top = tan(ufou / 2);
    double bottom = -top;
    
    ViewPort[0][0] = 2. / (right - left);
    ViewPort[1][1] = 2. / (top - bottom);
    ViewPort[2][0] = (left + right) / (left - right);
    ViewPort[2][1] = (top + bottom) / (bottom - top);
    ViewPort[2][2] = (far + near) / (far - near);
    ViewPort[2][3] = 1;
    ViewPort[3][2] = -2.*near*far / (far - near);
}


void Camera::CameraPort(Dot &target) {
    
    target.x -= Eye.x;
    target.y -= Eye.y;
    target.z -= Eye.z;
    
    MultiDot(target, Minv);
    
}


Dot Camera::MultiVect(const Dot &A, const Dot &B) {
    Dot Temp;
    Temp.x = A.y*B.z - A.z*B.y;
    Temp.y = A.z*B.x - A.x*B.z;
    Temp.z = A.x*B.y - A.y*B.x;
    return Temp;
}


void Camera::SetViewMatrix() {
    Dot VectZ = (Center - Eye);
    VectZ.normalize();  // ���������� �������
    
    Dot VectX = MultiVect(Up, VectZ);
    VectX.normalize();
    
    Dot VectY = MultiVect(VectZ, VectX);
    VectY.normalize(); //�����
    
    Minv[0][0] = VectX.x;
    Minv[1][0] = VectX.y;
    Minv[2][0] = VectX.z;
    
    Minv[0][1] = VectY.x;
    Minv[1][1] = VectY.y;
    Minv[2][1] = VectY.z;
    
    Minv[0][2] = VectZ.x;
    Minv[1][2] = VectZ.y;
    Minv[2][2] = VectZ.z;
    
    
}


void Camera::ReCalcMatrix() {
    SetScreenMatrix();
    SetPProjMatrix();
    SetViewMatrix();
}


void Camera::PProjection(Dot &target) {
    MultiDot(target, ViewPort);
    
    target.w = abs(target.w);
    
    target.x /= target.w;
    target.y /= target.w;
    //target.z /= target.w;
    target.w /= target.w;
}


void  Camera::SetScreenMatrix() {
    Screen[0][0] = double(Width - 1) / 2;
    Screen[1][1] = double(Heigth - 1) / 2;
    Screen[2][2] = 1;
    Screen[3][0] = double(Width - 1) / 2;
    Screen[3][1] = double(Heigth - 1) / 2;
    Screen[3][3] = 1;
}


void  Camera::ToScreen(Dot &target) {
    MultiDot(target, Screen);
}

void Camera::MoveTo(double x, double y, double z) {
    Center = Dot(x + Center.x - Eye.x, y + Center.y - Eye.y, z + Center.z - Eye.z);
    Eye = Dot(x, y, z);
    SetViewMatrix();
    
    //Transform();
}

void Camera::MoveOn(double dx, double dy, double dz) {
    Dot VectZ = (Center - Eye);
    VectZ.normalize();
    
    Dot VectX = MultiVect(Up, VectZ);
    VectX.normalize();
    
    Dot VectY = MultiVect(VectZ, VectX);
    VectY.normalize();
    
    VectY = VectY * dy;
    VectX = VectX * dx;
    VectZ = VectZ * dz;
    
    Dot Vect = VectX + VectY + VectZ;
    
    Eye = Eye + Vect;
    Center = Center + Vect;
    
    //Transform();
}

void Camera::RotateX(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { 1,0,0,0 },
        { 0,cos(phi),sin(phi),0 },
        { 0,-sin(phi),cos(phi),0 },
        { 0,0,0,1 }
    };
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    
    SetViewMatrix();
    //Transform();
}

void Camera::RotateY(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { cos(phi),0,-sin(phi),0 },
        { 0,1,0,0 },
        { sin(phi),0,cos(phi),0 },
        { 0,0,0,1 }
    };
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    
    
    
    SetViewMatrix();
    //Transform();
}

void Camera::RotateZ(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { cos(phi),sin(phi),0,0 },
        { -sin(phi),cos(phi),0,0 },
        { 0,0,1,0 },
        { 0,0,0,1 }
        
    };
    
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    
    
    
    SetViewMatrix();
    //Transform();
}



double Camera::getFar() {
    return far;
}

double Camera::getNear() {
    return near;
}