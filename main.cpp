#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <vector>
#include <tchar.h>
#include <cmath>
#include <cwchar>
#include <stack>
#include <cstdlib> // for exit function

using namespace std;

#define MAXINT 10e9
#define MAXENTRIES 600

#include<list>


#define Mouse_ARROW 0
#define Mouse_CROSS 1
#define Mouse_HAND 2
#define Mouse_HELP 3


#define DDA 4
#define midpoint 5
#define Parametric 6


#define Circle_Direct 7
#define Circle_Polar 8
#define Circle_IterativePolar 9
#define Circle_Midpoint 10
#define Circle_ModifiedMidpoint 11


#define Filling_Circle_withlines 12
#define Filling_Circle_withothercircles 13
#define Filling_Square 14
#define Filling_Rectangle 15
#define Filling_Convex 16
#define Filling_Non_Convex 17

#define FloodFill_Recursive 18
#define FloodFill_NonRecursive 19

#define Ellipse_Direct 20
#define Ellipse_Polar 21
#define Ellipse_Midpoint 22

#define Clipping_Rectangle_Point 23
#define Clipping_Rectangle_Line 24
#define Clipping_Rectangle_Polygon 25


#define Clipping_Square_Point 26
#define Clipping_Square_Line 27


#define Clipping_circle_Point 28
#define Clipping_circle_Line 29

#define Curve_CardinalSpline 30

#define RED 31
#define BLUE 32
#define WHITE 33
#define GREEN 34
#define BLACK 35
#define Yellow 36
#define Cyan  37
#define Magenta  38
#define Teal 39
#define Purple 40

#define Store 41
#define Load 42
#define Clear 43


int NumberOfPoints = 6;

COLORREF c = RGB(0, 0, 0);
int xc1, xc2, yc1, yc2;
int type = 100, num_of_the_quarter;
double radius;

RECT rect;


string alldata = "";


int size = 0;
LPCSTR mouse = IDC_ARROW;

void AddMenus(HWND hwnd);

void Save() {
    ofstream myfile;
    myfile.open("database.txt");
    myfile << alldata;
    myfile.close();
}

void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

int Round(double x) {
    return (int) (x + 0.5);
}

double distanceBetween2Points(int x1, int x2, int y1, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

}

void DrawLine1(HDC hdc, int x1, int y1, int x2, int y2, COLORREF Color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, Color);

        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            SetPixel(hdc, x, Round(y), Color);
        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, Color);
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            SetPixel(hdc, Round(x), y, Color);
        }
    }

}

void draw8Points(HDC hdc, int x, int y, int xc, int yc) {
    SetPixel(hdc, xc + x, yc + y, RGB(0, 0, 0));
    SetPixel(hdc, xc - x, yc + y, RGB(0, 0, 0));
    SetPixel(hdc, xc + x, yc - y, RGB(0, 0, 0));
    SetPixel(hdc, xc - x, yc - y, RGB(0, 0, 0));
    SetPixel(hdc, xc - y, yc + x, RGB(0, 0, 0));
    SetPixel(hdc, xc + y, yc - x, RGB(0, 0, 0));
    SetPixel(hdc, xc + y, yc + x, RGB(0, 0, 0));
    SetPixel(hdc, xc - y, yc - x, RGB(0, 0, 0));
}


void Draw4Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
}

//Direct-cartisian
void DrawEllipse_Direct(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
    int x = 0;
    double y = b;
    Draw4Points(hdc, xc, yc, 0, b, c);
    while (b * b * x < a * a * y) { //slope<1
        x++;
        y = b * sqrt(1.0 - (double) x * x / (a * a));
        Draw4Points(hdc, xc, yc, x, Round(y), c);

    }
    double x1 = a;
    int y1 = 0;
    Draw4Points(hdc, xc, yc, a, 0, c);
    while (b * b * x1 > a * a * y1) { //slope>1
        y1++;
        x1 = a * sqrt(1.0 - (double) y1 * y1 / (b * b));
        Draw4Points(hdc, xc, yc, round(x1), y1, c);
    }
}

//polar
void DrawEllipse_polar(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {

    double x = 0;
    double y = b;
    double dtheta = 1.0 / max(a, b);
    double cosdtheta = cos(dtheta);
    double sindtheta = sin(dtheta);
    Draw4Points(hdc, xc, yc, 0, b, c);
    while (x < y) { //slope<1
        double x1 = x * cosdtheta - ((double) a / (double) b) * y * sindtheta;
        y = ((double) b / (double) a) * x * sindtheta + y * cosdtheta;
        x = x1;
        Draw4Points(hdc, xc, yc, Round(x), Round(y), c);

    }
    double x1 = a;
    double y1 = 0;
    Draw4Points(hdc, xc, yc, a, 0, c);
    while (x1 > y1) { //slope>1
        double x2 = x * cosdtheta - ((double) a / (double) b) * y * sindtheta;
        y = ((double) b / (double) a) * x * sindtheta + y * cosdtheta;
        x1 = x2;
        Draw4Points(hdc, xc, yc, Round(x1), Round(y1), c);

    }
}

///midpoint ellipse drawing
void DrawEllipse_midpoint(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {

    float dx, dy, d1, d2, x = 0, y = B;

    d1 = (B * B)
         - (A * A * B)
         + (0.25 * A * A);
    dx = 2 * B * B * x;
    dy = 2 * A * A * y;

    while (dx < dy) {

        Draw4Points(hdc, xc, yc, Round(x), Round(y), c);
        if (d1 < 0) {
            x++;
            dx = dx + (2 * B * B);
            d1 = d1 + dx + (B * B);
        } else {
            x++;
            y--;
            dx = dx + (2 * B * B);
            dy = dy - (2 * A * A);
            d1 = d1 + dx - dy + (B * B);
        }
    }

    d2 = ((B * B) * ((x + 0.5) * (x + 0.5)))
         + ((A * A) * ((y - 1) * (y - 1)))
         - (A * A * B * B);

    while (y >= 0) {
        Draw4Points(hdc, xc, yc, Round(x), Round(y), c);
        if (d2 > 0) {
            y--;
            dy = dy - (2 * A * A);
            d2 = d2 + (A * A) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * B * B);
            dy = dy - (2 * A * A);
            d2 = d2 + dx - dy + (A * A);
        }
    }

}

//midPoint circle second diffrence
void DrawCircleMidPoint(HDC hdc, int xc, int yc, int R) {
    int x = 0;
    int y = R;
    draw8Points(hdc, x, y, xc, yc);
    int d = 1 - R;
    int d1 = 3;
    int d2 = 5 - 2 * R;

    while (x < y) {
        if (d < 0) {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        } else {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        draw8Points(hdc, x, y, xc, yc);
    }
}

void filling1stQuarter(HDC hdc, int xc, int yc, int R, bool is_Circle) {
    if (is_Circle) {
        for (int i = 0; i < R; i++) {
            int x = 0;
            int y = i;
            SetPixel(hdc, R, 0, RGB(255, 0, 0));
            int d = 1 - R;
            int d1 = 3;
            int d2 = 5 - 2 * R;

            while (x < y) {
                if (d < 0) {
                    d += d1;
                    d1 += 2;
                    d2 += 2;
                    x++;
                } else {
                    d += d2;
                    d1 += 2;
                    d2 += 4;
                    x++;
                    y--;
                }
                SetPixel(hdc, xc + x, yc - y, RGB(100, 0, 0));
                SetPixel(hdc, xc + y, yc - x, RGB(100, 0, 0));
                if (is_Circle) {

                } else {
                    DrawLine1(hdc, xc, yc, xc + x, yc - y, RGB(255, 0, 0));// first
                    DrawLine1(hdc, xc, yc, xc + y, yc - x, RGB(255, 0, 0));//first
                }

            }
        }
    } else {
        int x = 0;
        int y = R;
        SetPixel(hdc, R, 0, RGB(255, 0, 0));
        int d = 1 - R;
        int d1 = 3;
        int d2 = 5 - 2 * R;

        while (x < y) {
            if (d < 0) {
                d += d1;
                d1 += 2;
                d2 += 2;
                x++;
            } else {
                d += d2;
                d1 += 2;
                d2 += 4;
                x++;
                y--;
            }
            DrawLine1(hdc, xc, yc, xc + x, yc - y, RGB(255, 0, 0));// first
            DrawLine1(hdc, xc, yc, xc + y, yc - x, RGB(255, 0, 0));//first
        }
    }
}

void filling2ndQuarter(HDC hdc, int xc, int yc, int R, bool is_Circle) {
    if (is_Circle) {
        for (int i = 0; i < R; i++) {
            int x = 0;
            int y = i;
            SetPixel(hdc, R, 0, RGB(255, 0, 0));
            int d = 1 - R;
            int d1 = 3;
            int d2 = 5 - 2 * R;

            while (x < y) {
                if (d < 0) {
                    d += d1;
                    d1 += 2;
                    d2 += 2;
                    x++;
                } else {
                    d += d2;
                    d1 += 2;
                    d2 += 4;
                    x++;
                    y--;
                }
                SetPixel(hdc, xc + x, yc + y, RGB(255, 0, 0));//second
                SetPixel(hdc, xc + y, yc + x, RGB(255, 0, 0));//second

            }
        }
    } else {
        int x = 0;
        int y = R;
        SetPixel(hdc, R, 0, RGB(255, 0, 0));
        int d = 1 - R;
        int d1 = 3;
        int d2 = 5 - 2 * R;

        while (x < y) {
            if (d < 0) {
                d += d1;
                d1 += 2;
                d2 += 2;
                x++;
            } else {
                d += d2;
                d1 += 2;
                d2 += 4;
                x++;
                y--;
            }
            DrawLine1(hdc, xc, yc, xc + x, yc + y, RGB(255, 0, 0));//second
            DrawLine1(hdc, xc, yc, xc + y, yc + x, RGB(255, 0, 0));//second
        }
    }

}

void filling3rdQuarter(HDC hdc, int xc, int yc, int R, bool is_Circle) {
    if (is_Circle) {
        for (int i = 0; i < R; i++) {
            int x = 0;
            int y = i;
            SetPixel(hdc, R, 0, RGB(255, 0, 0));
            int d = 1 - R;
            int d1 = 3;
            int d2 = 5 - 2 * R;

            while (x < y) {
                if (d < 0) {
                    d += d1;
                    d1 += 2;
                    d2 += 2;
                    x++;
                } else {
                    d += d2;
                    d1 += 2;
                    d2 += 4;
                    x++;
                    y--;
                }
                SetPixel(hdc, xc - x, yc + y, RGB(255, 0, 0));//third
                SetPixel(hdc, xc - y, yc + x, RGB(255, 0, 0));// third
            }
        }
    } else {
        int x = 0;
        int y = R;
        SetPixel(hdc, R, 0, RGB(255, 0, 0));
        int d = 1 - R;
        int d1 = 3;
        int d2 = 5 - 2 * R;
        while (x < y) {
            if (d < 0) {
                d += d1;
                d1 += 2;
                d2 += 2;
                x++;
            } else {
                d += d2;
                d1 += 2;
                d2 += 4;
                x++;
                y--;
            }
            DrawLine1(hdc, xc, yc, xc - x, yc + y, RGB(255, 0, 0));//third
            DrawLine1(hdc, xc, yc, xc - y, yc + x, RGB(255, 0, 0));// third
        }
    }

}

void filling4thQuarter(HDC hdc, int xc, int yc, int R, bool is_Circle) {
    if (is_Circle) {
        for (int i = 0; i < R; i++) {
            int x = 0;
            int y = i;
            SetPixel(hdc, R, 0, RGB(255, 0, 0));
            int d = 1 - R;
            int d1 = 3;
            int d2 = 5 - 2 * R;

            while (x < y) {
                if (d < 0) {
                    d += d1;
                    d1 += 2;
                    d2 += 2;
                    x++;
                } else {
                    d += d2;
                    d1 += 2;
                    d2 += 4;
                    x++;
                    y--;
                }
                SetPixel(hdc, xc - x, yc - y, RGB(255, 0, 0));
                SetPixel(hdc, xc - y, yc - x, RGB(255, 0, 0));
            }
        }
    } else {
        int x = 0;
        int y = R;
        SetPixel(hdc, R, 0, RGB(255, 0, 0));
        int d = 1 - R;
        int d1 = 3;
        int d2 = 5 - 2 * R;

        while (x < y) {
            if (d < 0) {
                d += d1;
                d1 += 2;
                d2 += 2;
                x++;
            } else {
                d += d2;
                d1 += 2;
                d2 += 4;
                x++;
                y--;
            }
            DrawLine1(hdc, xc, yc, xc - x, yc - y, RGB(255, 0, 0));
            DrawLine1(hdc, xc, yc, xc - y, yc - x, RGB(255, 0, 0));
        }
    }

}


struct Vector {
    double v[2];

    Vector(double x = 0, double y = 0) {
        v[0] = x;
        v[1] = y;
    }

    double &operator[](int i) {
        return v[i];
    }
};

void DrawHermiteCurve(HDC hdc, Vector &p1, Vector &T1, Vector &p2, Vector &T2, COLORREF c) {
    double a0 = p1[0], a1 = T1[0],
            a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
            a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
    double b0 = p1[1], b1 = T1[1],
            b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
            b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
    for (double t = 0; t <= 1; t += 0.001) {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), c);
        SetPixel(hdc, Round(x), Round(y), c);
    }
}

void MyFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
    COLORREF C = GetPixel(hdc, x, y);
    if (C == Cb || C == Cf)return;
    SetPixel(hdc, x, y, Cf);
    MyFloodFill(hdc, x + 1, y, Cb, Cf);
    MyFloodFill(hdc, x - 1, y, Cb, Cf);
    MyFloodFill(hdc, x, y + 1, Cb, Cf);
    MyFloodFill(hdc, x, y - 1, Cb, Cf);
}

void NRMyFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
    stack<Vector> S;
    S.push(Vector(x, y));
    while (!S.empty()) {
        Vector v = S.top();
        S.pop();
        COLORREF c = GetPixel(hdc, v[0], v[1]);
        if (c == Cb || c == Cf)continue;
        SetPixel(hdc, v[0], v[1], Cf);
        S.emplace(v[0] + 1, v[1]);
        S.emplace(v[0] - 1, v[1]);
        S.emplace(v[0], v[1] + 1);
        S.emplace(v[0], v[1] - 1);
    }
}

//draw square
void drawSquare(HDC hdc, Vector &p1, double sideLen, COLORREF c) {
    double left = p1[0];
    double right = p1[0] + sideLen;
    double top = p1[1];
    double bottom = p1[1] + sideLen;
    DrawLine1(hdc, left, top, right, top, RGB(0, 0, 0));
    DrawLine1(hdc, left, top, left, bottom, RGB(0, 0, 0));
    DrawLine1(hdc, right, top, right, bottom, RGB(0, 0, 0));
    DrawLine1(hdc, left, bottom, right, bottom, RGB(0, 0, 0));

    while (left < right) {
        double nwLeft = left - sideLen / 16;
        Vector T1((left - nwLeft), (top - (top + sideLen / 16)));
        Vector T2((left - nwLeft), (bottom - (bottom - sideLen / 16)));
        Vector P1(left, top);
        Vector P2(left, bottom);
        DrawHermiteCurve(hdc, P1, T1, P2, T2, c);
        left += sideLen / 128;
    }

    /*DrawLine1(hdc,50,50,50+sideLen,50,c);
    DrawLine1(hdc,50+sideLen,50,50+sideLen,50+sideLen,c);
    DrawLine1(hdc,50+sideLen,50+sideLen,50,50+sideLen,c);
    DrawLine1(hdc,50,50+sideLen,50,50,c);*/

}

void drawSquare2(HDC hdc, Vector &p1, double sideLen, COLORREF c) {

    double left = p1[0];
    double right = p1[0] + sideLen;
    double top = p1[1];
    double bottom = p1[1] + sideLen;
    DrawLine1(hdc, left, top, right, top, c);
    DrawLine1(hdc, left, top, left, bottom, c);
    DrawLine1(hdc, right, top, right, bottom, c);
    DrawLine1(hdc, left, bottom, right, bottom, c);
}

//code For Rectangle  **Done**
void drawRectangle_clipping(HDC hdc, Vector &p1, double sideLen, double sidewidth, COLORREF c) {
    double left = p1[0];
    double right = p1[0] + sideLen;
    double top = p1[1];
    double bottom = p1[1] + sidewidth;

    DrawLine1(hdc, left, top, right, top, c);
    DrawLine1(hdc, left, top, left, bottom, c);
    DrawLine1(hdc, right, top, right, bottom, c);
    DrawLine1(hdc, left, bottom, right, bottom, c);
}

void drawBezier(HDC hdc, Vector &p1, Vector &p2, Vector &p3, Vector &p4, COLORREF c) {
    for (double t = 0.0; t <= 1.0; t += 0.001) {
        double drawX =
                pow(1 - t, 3) * p1[0] + 3 * t * pow(1 - t, 2) * p2[0] + 3 * t * t * (1 - t) * p3[0] + t * t * t * p4[0];
        double drawY =
                pow(1 - t, 3) * p1[1] + 3 * t * pow(1 - t, 2) * p2[1] + 3 * t * t * (1 - t) * p3[1] + t * t * t * p4[1];
        SetPixel(hdc, Round(drawX), Round(drawY), c);
    }
}

void drawRectangle(HDC hdc, Vector &p1, double width, double height, COLORREF c) {
    double left = p1[0];
    double right = p1[0] + width;
    double top = p1[1];
    double bottom = p1[1] + height;
    DrawLine1(hdc, left, top, right, top, RGB(0, 0, 0));
    DrawLine1(hdc, left, top, left, bottom, RGB(0, 0, 0));
    DrawLine1(hdc, right, top, right, bottom, RGB(0, 0, 0));
    DrawLine1(hdc, left, bottom, right, bottom, RGB(0, 0, 0));
    while (top < bottom) {
        Vector T2((left + width / 32), (top + height / 32));
        Vector T1((right - width / 32), (top + height / 32));
        Vector P1(left, top);
        Vector P2(right, top);
        drawBezier(hdc, P1, T1, T2, P2, c);
        top += width / 128;
    }
}

//My code For hermiteCurve Ysing Functions    **Done**

void hermitMatrixCurve(HDC hdc, Vector &p1, Vector &T1, Vector &p2, Vector &T2) {
    double a0 = p1[0], a1 = T1[0],
            a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
            a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];

    double b0 = p1[1], b1 = T1[1],
            b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
            b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];

    for (double t = 0; t <= 1; t += 0.001) {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), c);
    }
}


void DrawCardinalSpline(HDC hdc, Vector P[], double d) {
    Vector T[NumberOfPoints];
    T[0][0] = ((d / 2) * (P[1][0] - P[0][0]));
    T[0][1] = ((d / 2) * (P[1][1] - P[0][1]));

    for (int i = 1; i < NumberOfPoints - 1; i++) {
        T[i][0] = ((d / 2) * (P[i + 1][0] - P[i - 1][0]));
        T[i][1] = ((d / 2) * (P[i + 1][1] - P[i - 1][1]));
    }
    T[NumberOfPoints - 1][0] = ((d / 2) * (P[NumberOfPoints - 1][0] - P[NumberOfPoints - 2][0]));
    T[NumberOfPoints - 1][1] = ((d / 2) * (P[NumberOfPoints - 1][1] - P[NumberOfPoints - 2][1]));

    for (int i = 0; i < NumberOfPoints - 1; i++) {
        hermitMatrixCurve(hdc, P[i], T[i], P[i + 1], T[i + 1]);
    }
}


//My Code in Line
void DrawLine_DDA(HDC hdc, int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    SetPixel(hdc, x1, y1, c);

    if (abs(dx) >= abs(dy)) {
        int x = x1;
        int xinc;
        if (dx > 0) {
            xinc = 1;
        } else xinc = -1;
        cout << "xinc= " << xinc << "\n";

        double y = y1, yinc = (double) dy / dx * xinc;
        while (x != x2) {
            x += xinc;
            y += yinc;
            SetPixel(hdc, x, Round(y), c);
        }
    } else {
        int y = y1;
        int yinc;
        if (dy > 0) {
            yinc = 1;
        } else yinc = -1;
        cout << "yinc= " << yinc << "\n";
        double x = x1, xinc = (double) dx / dy * yinc;
        while (y != y2) {
            x += xinc;
            y += yinc;
            SetPixel(hdc, Round(x), y, c);
        }
    }

}

void DrawLine_MidPoint(HDC hdc, int x1, int y1, int x2, int y2) {
    int x, y, dx, dy, d, d1, d2;
    dx = x2 - x1;
    dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        d = 2 * abs(dy) - abs(dx);
        d1 = 2 * (abs(dy) - abs(dx));
        d2 = 2 * abs(dy);
        if (dx >= 0) // 0 to 45  => /  ;  x1=50 , x2=100
        {
            x = x1;
            y = y1;
        } else // 135 to 180  => \  ;   x1=0 , x2 =-50 we should swap the 2 points of x
        {
            x = x2;
            y = y2;
            swap(x1, x2);
        }
        SetPixel(hdc, x, y, c);
        while (x < x2) {
            if (d < 0) {
                d += d2;
            } else {
                d += d1;
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y++;
                else y--;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    } else {
        d = 2 * abs(dx) - abs(dy);
        d1 = 2 * (abs(dx) - abs(dy));
        d2 = 2 * abs(dx);
        if (dy >= 0) {
            x = x1;
            y = y1;
        } else {
            x = x2;
            y = y2;
            swap(y1, y2);
        }
        SetPixel(hdc, x, y, c);
        while (y < y2) {
            if (d <= 0) {
                d += d2;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x++;
                else x--;
                d += d1;
            }
            y++;
            SetPixel(hdc, x, y, c);
        }
    }
}

void DrawLine_Parametric(HDC hdc, int x1, int y1, int x2, int y2) {
    for (double t = 0; t <= 1; t += 0.001) {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        SetPixel(hdc, Round(x), Round(y), c);
    }
}


//My Code in Circle
void DrawCircle_Direct(HDC hdc, int xc, int yc, int R) //x power 2 + y power 2 = R power 2
{
    int x = 0;
    double y = R;
    draw8Points(hdc, x, Round(y), xc, yc);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        draw8Points(hdc, x, Round(y), xc, yc);
    }
}

void DrawCircle_Polar(HDC hdc, int xc, int yc, int R) {
    double x = R, y = 0;
    double theta = 0;
    draw8Points(hdc, x, Round(y), xc, yc);
    while (x > y) {
        theta += double(1.0 / R);
        x = R * cos(theta);
        y = R * sin(theta);
        draw8Points(hdc, x, Round(y), xc, yc);
    }
}

void DrawCircle_Iterative(HDC hdc, int xc, int yc, int R) {
    double x = R, y = 0, dtheta = 1.0 / R, ct = cos(dtheta), st = sin(dtheta);
    draw8Points(hdc, x, Round(y), xc, yc);
    while (x > y) {
        double x1 = x * ct - y * st,
                y1 = y * ct + x * st;
        x = x1, y = y1;
        draw8Points(hdc, x, Round(y), xc, yc);
    }
}

void DrawCircle_MidPoint(HDC hdc, int xc, int yc, int R) {
    int x = 0, y = R, d = 1 - R;
    draw8Points(hdc, x, Round(y), xc, yc);
    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
            x++;
        } else {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        draw8Points(hdc, x, Round(y), xc, yc);
    }

}

void DrawCircle_ModifiedMidPoint(HDC hdc, int xc, int yc, int R) {
    int x = 0, y = R, d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    draw8Points(hdc, x, Round(y), xc, yc);
    while (x < y) {
        if (d < 0) {
            d += d1;
            d2 += 2;
            d1 += 2;
            x++;
        } else {
            d += d2;
            d2 += 4;
            d1 += 2;
            x++;
            y--;
        }
        draw8Points(hdc, x, Round(y), xc, yc);
    }

}


//cliping Line   **Done**
union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1;
    else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1;
    else if (y > ybottom)out.bottom = 1;
    return out;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom) {
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;//line
    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi, yi;
        if (out1.All) {
            if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        } else {
            if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        }
    }
    if (!out1.All && !out2.All) {
        DrawLine_MidPoint(hdc, x1, y1, x2, y2);
    }
}


//////////////// convex functions///////////////////////////
typedef struct edgetable {
    int xmin, xmax;

} table;

struct point {
    int x, y;
};

void InitEdgeTable(table edgetable[]) {
    for (int i = 0; i < MAXENTRIES; i++) {
        edgetable[i].xmin = MAXINT;
        edgetable[i].xmax = -MAXINT;
    }
}

void edge2edgetable(point v1, point v2, table edgetable[]) {
    //horizontal line
    if (v1.y == v2.y)return;
    if (v1.y > v2.y)swap(v1.x, v1.y, v2.x, v2.y);
    double minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    while (y < v2.y) {
        if (x < edgetable[y].xmin)edgetable[y].xmin = (int) ceil(x); //check left -->
        if (x > edgetable[y].xmax)edgetable[y].xmax = (int) floor(x); //check right <--
        y++;
        x += minv;
    }
}

void edgetable2screen(HDC hdc, table edgetable[], COLORREF color) {
    for (int y = 0; y < MAXENTRIES; y++)
        if (edgetable[y].xmin < edgetable[y].xmax)
            //draw line from xmin to xmax
            for (int x = edgetable[y].xmin; x <= edgetable[y].xmax; x++)
                SetPixel(hdc, x, y, color);
}

void ConvexFill(HDC hdc, point p[], int n, COLORREF color) {
    auto *edgetable = new table[MAXENTRIES];
    InitEdgeTable(edgetable);
    point v1 = p[n - 1];
    //polygon to edgetable
    for (int i = 0; i < n; i++) {
        point v2 = p[i];
        edge2edgetable(v1, v2, edgetable);
        v1 = p[i];
    }
    edgetable2screen(hdc, edgetable, color);
    delete[] edgetable;
}

//////////////non convex functions///////////////////////////
struct EdgeRec {
    double x, minv;
    int ymax;

    //sorting function to sort edge records in ascending order with x
    bool operator<(EdgeRec r) const {
        return x < r.x;
    }
};

typedef list<EdgeRec> EdgeList;

EdgeRec InitEdgeRec(point &v1, point &v2) {
    if (v1.y > v2.y)swap(v1, v2);
    EdgeRec rec{};
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}

void InitEdgeTable2(point *polygon, int n, EdgeList table[]) {
    point v1 = polygon[n - 1];
    for (int i = 0; i < n; i++) {
        point v2 = polygon[i];
        if (v1.y == v2.y) //horizontal egge
        {
            v1 = v2;
            continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1 = polygon[i];
    }
}

void GeneralPolygonFill(HDC hdc, point *polygon, int n, COLORREF c) {
    auto *table = new EdgeList[MAXENTRIES];
    InitEdgeTable2(polygon, n, table);
    int y = 0;
    while (y < MAXENTRIES && table[y].empty())y++; //get the first not null index
    if (y == MAXENTRIES)return;
    EdgeList ActiveList = table[y];
    while (!ActiveList.empty()) {
        ActiveList.sort();
        //1.sort according to x
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++) {
            //2.for each non overlapping two points draw a line
            int x1 = (int) ceil(it->x); //first point
            it++;
            int x2 = (int) floor(it->x); //second point
            //draw line
            for (int x = x1; x <= x2; x++)SetPixel(hdc, x, y, c);
        }
        //3. increase y
        y++;
        //4.for each node N in active remove the node if N.ymax = y
        auto it = ActiveList.begin();
        while (it != ActiveList.end())
            if (y == it->ymax) it = ActiveList.erase(it);
            else it++;
        //5.for each node N in active update N.xmin
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
            it->x += it->minv;
        //6. append table[y] to active
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    delete[] table;
}


static point convex[10];

static point *nonConvex = new point[10];
static int counter = 0;


void write_tofile_1(string t, int x_1, int y_1, int x_2, int y_2) {
    alldata += t + "," + to_string(x_1) + "," + to_string(y_1) + "," + to_string(x_2) + "," + to_string(y_2) + "," +
               to_string(c) + "\n";
}

void write_tofile_2(string t, int x_1, int y_1, int x_2, int y_2, int x_3, int y_3) {
    alldata += t + "," + to_string(x_1) + "," + to_string(y_1) + "," + to_string(x_2) + "," + to_string(y_2) + "," +
               to_string(x_3) + "," + to_string(y_3) + "," + to_string(c) + "\n";
}

void write_tofile_3(string t, int x_1, int y_1) {
    alldata += t + "," + to_string(x_1) + "," + to_string(y_1) + "," + to_string(c) + "\n";
}

write_tofile_4(string
type,
int num_quarter,
int width,
int height
)
{
alldata += type + "," +
to_string(num_quarter)
+ "," +
to_string(width)
+ "," +
to_string(height)
+","+
to_string(c)
+ "\n";
}


void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom) {
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom)
        SetPixel(hdc, x, y, c);
}


//cliping Polygon   **Done**
struct Vertex {
    double x, y;

    Vertex(int x1 = 0, int y1 = 0) {
        x = x1;
        y = y1;
    }
};

typedef vector<Vertex> VertexList;

typedef bool (*IsInFunc)(Vertex &v, int edge);

typedef Vertex(*IntersectFunc)(Vertex &v1, Vertex &v2, int edge);

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect) {
    VertexList OutList;
    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < (int) p.size(); i++) {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in) {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        } else if (v1_in && v2_in) OutList.push_back(v2);
        else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }
    return OutList;
}

bool InLeft(Vertex &v, int edge) {
    return v.x >= edge;
}

bool InRight(Vertex &v, int edge) {
    return v.x <= edge;
}

bool InTop(Vertex &v, int edge) {
    return v.y >= edge;
}

bool InBottom(Vertex &v, int edge) {
    return v.y <= edge;
}

Vertex VIntersect(Vertex &v1, Vertex &v2, int xedge) {
    Vertex res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}

Vertex HIntersect(Vertex &v1, Vertex &v2, int yedge) {
    Vertex res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}

void PolygonClip(HDC hdc, Vector p[], int n, int xleft, int ytop, int xright, int ybottom) {
    VertexList vlist;
    for (int i = 0; i < n; i++)vlist.emplace_back(p[i][0], p[i][1]);
    vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
    vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
    vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
    vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
    Vertex v1 = vlist[vlist.size() - 1];
    for (auto v2: vlist) {
        DrawLine_DDA(hdc, v1.x, v1.y, v2.x, v2.y);
        v1 = v2;
    }
}

//cliping circle point   **Done**
void PointClippingCircle(HDC hdc, int x, int y, int R, int xc, int yc, COLORREF c) {
    int distance = Round(sqrt(pow(Round(x) - xc, 2) + pow(Round(y) - yc, 2)));
    if (distance <= R)SetPixel(hdc, Round(x), Round(y), c);
}

//cliping circle line **Done**
void DrawLineByDDACircle(HDC hdc, int x1, int x2, int y1, int y2, int xc, int yc, int R, COLORREF c) {
    int dx = x2 - x1, distance;
    int dy = y2 - y1;
    distance = Round(sqrt(pow(x1 - xc, 2) + pow(y1 - yc, 2)));
    if (distance <= R) {
        SetPixel(hdc, x1, y1, c);
    }
    if (abs(dx) >= abs(dy)) {
        int x = x1;
        int xinc;
        if (dx > 0) {
            xinc = 1;
        } else xinc = -1;
        cout << "xinc= " << xinc << "\n";

        double y = y1, yinc = (double) dy / dx * xinc;
        while (x != x2) {
            x += xinc;
            y += yinc;
            distance = Round(sqrt(pow(x - xc, 2) + pow(Round(y) - yc, 2)));
            if (distance <= R) {
                SetPixel(hdc, x, Round(y), c);
            }
        }
    } else {
        int y = y1;
        int yinc;
        if (dy > 0) {
            yinc = 1;
        } else yinc = -1;
        cout << "yinc= " << yinc << "\n";
        double x = x1, xinc = (double) dx / dy * yinc;
        while (y != y2) {
            x += xinc;
            y += yinc;
            distance = Round(sqrt(pow(Round(x) - xc, 2) + pow(y - yc, 2)));
            if (distance <= R) {
                SetPixel(hdc, Round(x), y, c);
            }
        }
    }
}


void Load_function(HDC hdc) {
    ifstream file("database.txt");
    string one_line;

    if (file.is_open()) {
        while (getline(file, one_line)) {
            string line_content = "";
            vector<string> vec;
            for (int i = 0; i < one_line.length(); i++) {
                if (one_line[i] != ',') {
                    line_content += one_line[i];
                } else {
                    vec.push_back(line_content);
                    line_content = "";
                }
            }
            vec.push_back(line_content);
            line_content = "";
            c = stoi(vec[vec.size() - 1]);
            if ("DDA" == vec[0]) {
                DrawLine_DDA(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]));
            } else if ("line_midpoint" == vec[0]) {
                DrawLine_MidPoint(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]));
            } else if ("line_Parametric" == vec[0]) {
                DrawLine_Parametric(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]));
            } else if ("Circle_Direct" == vec[0]) {
                DrawCircle_Direct(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]));
            } else if ("Circle_Polar" == vec[0]) {
                DrawCircle_Polar(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]));
            } else if ("Circle_IterativePolar" == vec[0]) {
                DrawCircle_Iterative(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]));
            } else if ("Circle_Midpoint" == vec[0]) {
                DrawCircle_MidPoint(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]));
            } else if ("Circle_ModifiedMidpoint" == vec[0]) {
                DrawCircle_ModifiedMidPoint(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]));
            } else if ("Filling_Circle_withlines" == vec[0] || "Filling_Circle_withothercircles" == vec[0]) {
                bool is_Circle = true;
                string filling_type = vec[0];
                if (filling_type == "Filling_Circle_withlines") {
                    is_Circle = false;
                }


                DrawCircleMidPoint(hdc, stoi(vec[2]), stoi(vec[3]), stoi(vec[4]));///draw cilcle.
                if (stoi(vec[1]) == '1')           ///choose the quarter to fill
                {
                    filling1stQuarter(hdc, stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), is_Circle);
                } else if (stoi(vec[1]) == '2') {
                    filling2ndQuarter(hdc, stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), is_Circle);
                } else if (stoi(vec[1]) == '3') {
                    filling3rdQuarter(hdc, stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), is_Circle);
                } else {
                    filling4thQuarter(hdc, stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), is_Circle);
                }
            } else if ("Filling_Square" == vec[0]) {
                Vector p[2];
                p[0] = Vector(stoi(vec[1]), stoi(vec[2]));
                drawSquare(hdc, p[0], stoi(vec[3]), c);
            } else if ("Filling_Rectangle" == vec[0]) {
                Vector p[2];
                p[0] = Vector(stoi(vec[1]), stoi(vec[2]));
                drawRectangle(hdc, p[0], stoi(vec[3]), stoi(vec[4]), c);
            } else if ("Filling_Convex" == vec[0]) {
                int size_point = stoi(vec[vec.size() - 2]);
                int temp = 1;
                point conv[10];
                for (int i = 0; i < size_point + 1; i++) {
                    conv[i].x = stoi(vec[temp++]);
                    conv[i].y = stoi(vec[temp++]);
                }
                ConvexFill(hdc, conv, size_point + 1, c);
            } else if ("Filling_Non_Convex" == vec[0]) {
                int size_point = stoi(vec[vec.size() - 2]);
                int temp = 1;
                point conv[10];
                for (int i = 0; i < size_point + 1; i++) {
                    conv[i].x = stoi(vec[temp++]);
                    conv[i].y = stoi(vec[temp++]);
                }
                GeneralPolygonFill(hdc, conv, size_point + 1, c);
            } else if ("FloodFill_Recursive" == vec[0]) {
                MyFloodFill(hdc, stoi(vec[1]), stoi(vec[2]), c, RGB(0, 255, 0));
            } else if ("FloodFill_NonRecursive" == vec[0]) {
                NRMyFloodFill(hdc, stoi(vec[1]), stoi(vec[2]), c, RGB(0, 255, 0));
            } else if ("Ellipse_Direct" == vec[0]) {
                DrawEllipse_Direct(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), c);
            } else if ("Ellipse_Polar" == vec[0]) {
                DrawEllipse_polar(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), c);
            } else if ("Ellipse_Midpoint" == vec[0]) {
                DrawEllipse_midpoint(hdc, stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), stoi(vec[4]), c);
            } else if ("Clipping_Rectangle_Point" == vec[0])//6
            {
                int tem = 1;
                int l1 = stoi(vec[tem++]), l2 = stoi(vec[tem++]), num1 = stoi(vec[tem++]), num2 = stoi(vec[tem++]);
                int num3 = stoi(vec[tem++]), num4 = stoi(vec[tem]);
                Vector s;
                s[0] = num1;
                s[1] = num2;
                drawRectangle_clipping(hdc, s, num3, num4, c);
                PointClipping(hdc, l1, l2, s[0], s[1], s[0] + num3, s[1] + num4);
            } else if ("Clipping_Rectangle_Line" == vec[0]) {
                int tem = 1;
                int l1 = stoi(vec[tem++]), l2 = stoi(vec[tem++]), l3 = stoi(vec[tem++]), l4 = stoi(
                        vec[tem++]), num1 = stoi(vec[tem++]), num2 = stoi(vec[tem++]);
                int num3 = stoi(vec[tem++]), num4 = stoi(vec[tem]);
                Vector s;
                s[0] = num1;
                s[1] = num2;
                drawRectangle_clipping(hdc, s, num3, num4, c);
                CohenSuth(hdc, l1, l2, l3, l4, s[0], s[1], s[0] + num3, s[1] + num4);
            } else if ("Clipping_Rectangle_Polygon" == vec[0]) {
                int Pointsnum = stoi(vec[1]);
                Vector p1[Pointsnum];  //ahmed

                int tem = 2;
                for (int i = 0; i < Pointsnum; i++) {
                    p1[i] = Vector(stoi(vec[tem++]), stoi(vec[tem++]));
                    swap(p1[i][0], p1[i][1]);
                }
                int num1 = stoi(vec[tem++]), num2 = stoi(vec[tem++]), num3 = stoi(vec[tem++]), num4 = stoi(vec[tem]);

                Vector s;
                s[0] = num1;
                s[1] = num2;
                drawRectangle_clipping(hdc, s, num3, num4, c);
                PolygonClip(hdc, p1, Pointsnum, num1, num2, num1 + num3, num2 + num4);
            } else if ("Clipping_Square_Point" == vec[0]) {
                int tem = 1;
                int l1 = stoi(vec[tem++]), l2 = stoi(vec[tem++]);
                Vector s;
                s[0] = stoi(vec[tem++]);
                s[1] = stoi(vec[tem++]);
                int len = stoi(vec[tem]);
                drawSquare2(hdc, s, len, c);
                PointClipping(hdc, l1, l2, s[0], s[1], s[0] + len, s[1] + len);
            } else if ("Clipping_Square_Line" == vec[0]) {
                int tem = 1;
                int l1 = stoi(vec[tem++]), l2 = stoi(vec[tem++]), l3 = stoi(vec[tem++]), l4 = stoi(vec[tem++]);
                Vector s;
                s[0] = stoi(vec[tem++]);
                s[1] = stoi(vec[tem++]);
                int len = stoi(vec[tem]);

                drawSquare2(hdc, s, len, c);
                CohenSuth(hdc, l1, l2, l3, l4, s[0], s[1], s[0] + len, s[1] + len);
            } else if ("Clipping_circle_Point" == vec[0]) {
                int tem = 1;
                int l3 = stoi(vec[tem++]), l4 = stoi(vec[tem++]), distance = stoi(vec[tem++]), xi = stoi(
                        vec[tem++]), yi = stoi(vec[tem]);
                DrawCircleMidPoint(hdc, xi, yi, distance);
                PointClippingCircle(hdc, l3, l4, distance, xi, yi, c);
            } else if ("Clipping_circle_Line" == vec[0]) {
                int tem = 1;
                int l1 = stoi(vec[tem++]), l3 = stoi(vec[tem++]);
                int l2 = stoi(vec[tem++]), l4 = stoi(vec[tem++]), xi = stoi(vec[tem++]), yi = stoi(
                        vec[tem++]), distance = stoi(vec[tem]);
                DrawCircleMidPoint(hdc, xi, yi, distance);
                DrawLineByDDACircle(hdc, l1, l3, l2, l4, xi, yi, distance, c);
            } else if ("DrawCardinalSpline" == vec[0]) {
                int Pointsnum = stoi(vec[1]);
                Vector p1[Pointsnum];  //ahmed
                int tem = 2;
                for (int i = 0; i < Pointsnum; i++) {
                    p1[i] = Vector(stoi(vec[tem++]), stoi(vec[tem++]));
                    swap(p1[i][0], p1[i][1]);
                }
                DrawCardinalSpline(hdc, p1, 1);
            }
        }
        file.close();
    }
}

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lParam) {
    HDC hdc = GetDC(hwnd);
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR) brush);
    static Vector p[2]; //samaa
    static int index = 0; //samaa
    static bool enter = false;
    static int startx, starty, dx, dy, distance = 0, px, py, l1, l2, l3, l4, xc, yc; //ahmed
    static Vector p1[100];  //ahmed
    static Vector poly[100];  //ahmed
    static Vector s; //ahmed
    static int count2 = 0;
    int static index1 = 0; //ahmed
    static double len, width; //ahmed

    switch (msg) {
        case WM_CHAR:
            num_of_the_quarter = (TCHAR) wp;  //take the num of the quarter
            break;
        case WM_COMMAND:
            switch (LOWORD(wp)) {
                case Mouse_ARROW:
                    mouse = IDC_ARROW;
                    break;
                case Mouse_CROSS:
                    mouse = IDC_CROSS;
                    break;
                case Mouse_HAND:
                    mouse = IDC_HAND;
                    break;
                case Mouse_HELP:
                    mouse = IDC_HELP;
                    break;


                case DDA:
                    type = 0;
                    break;
                case midpoint :
                    type = 1;
                    break;
                case Parametric :
                    type = 2;
                    break;


                case Circle_Direct  :
                    type = 3;
                    break;
                case Circle_Polar :
                    type = 4;
                    break;
                case Circle_IterativePolar  :
                    type = 5;
                    break;
                case Circle_Midpoint  :
                    type = 6;
                    break;
                case Circle_ModifiedMidpoint  :
                    type = 7;
                    break;


                case Filling_Circle_withlines   :
                    type = 8;
                    break;
                case Filling_Circle_withothercircles   :
                    type = 9;
                    break;
                case Filling_Square    :
                    type = 10;
                    break;
                case Filling_Rectangle  :
                    type = 11;
                    break;
                case Filling_Convex  :
                    cout << "Enter Number of Points -->";
                    cin >> NumberOfPoints;
                    type = 12;
                    break;
                case Filling_Non_Convex  :
                    cout << "Enter Number of Points -->";
                    cin >> NumberOfPoints;
                    type = 13;
                    break;
                case FloodFill_Recursive  :
                    type = 14;
                    break;
                case FloodFill_NonRecursive  :
                    type = 15;
                    break;


                case Ellipse_Direct  :
                    type = 16;
                    break;
                case Ellipse_Polar   :
                    type = 17;
                    break;
                case Ellipse_Midpoint  :
                    type = 18;
                    break;
                case Clipping_Rectangle_Point  :
                    type = 19;
                    count2 = 0;
                    break;
                case Clipping_Rectangle_Line   :
                    count2 = 0;
                    type = 20;
                    break;
                case Clipping_Rectangle_Polygon :
                    count2 = 0;
                    type = 21;
                    break;
                case Clipping_Square_Point   :
                    type = 22;
                    count2 = 0;
                    break;
                case Clipping_Square_Line  :
                    type = 23;
                    count2 = 0;
                    break;
                case Clipping_circle_Point  :
                    type = 24;
                    count2 = 0;
                    break;
                case Clipping_circle_Line   :
                    type = 25;
                    count2 = 0;
                    break;
                case Curve_CardinalSpline :
                    cout << "Enter NumberOfPoints --> ";
                    cin >> NumberOfPoints;
                    type = 26;
                    count2 = 0;
                    break;
                case GREEN:
                    c = RGB(0, 255, 0);
                    break;
                case BLACK:
                    c = RGB(0, 0, 0);
                    break;
                case WHITE:
                    c = RGB(255, 255, 255);
                    break;
                case RED:
                    c = RGB(255, 0, 0);
                    break;
                case BLUE:
                    c = RGB(0, 0, 255);
                    break;
                case Yellow:
                    c = RGB(255, 215, 0);
                    break;
                case Cyan:
                    c = RGB(0, 255, 255);
                    break;
                case Magenta:
                    c = RGB(0, 255, 255);
                    break;
                case Teal:
                    c = RGB(0, 128, 128);
                    break;
                case Purple:
                    c = RGB(128, 0, 128);
                    break;
                case Store:
                    Save();
                    break;
                case Load:
                    Load_function(hdc);
                    break;
                case Clear:
                    InvalidateRect(hwnd, 0, TRUE);
                    alldata = "";
                    break;
            }
            break;
        case WM_LBUTTONDOWN:
            xc1 = LOWORD(lParam);
            yc1 = HIWORD(lParam);
            break;
        case WM_RBUTTONDOWN:
            xc2 = LOWORD(lParam);
            yc2 = HIWORD(lParam);
            radius = distanceBetween2Points(xc1, xc2, yc1, yc2);
            ////////////////////////////////line//////////////////////////////////////
            if (type == 0)//call line_DDA  fundtion
            {
                DrawLine_DDA(hdc, xc1, yc1, xc2, yc2);
                write_tofile_1("DDA", xc1, yc1, xc2, yc2);
            } else if (type == 1) //call line_midpoint fundtion
            {
                DrawLine_MidPoint(hdc, xc1, yc1, xc2, yc2);
                write_tofile_1("line_midpoint", xc1, yc1, xc2, yc2);
            } else if (type == 2) //call line_Parametric  fundtion
            {
                DrawLine_Parametric(hdc, xc1, yc1, xc2, yc2);
                write_tofile_1("line_Parametric", xc1, yc1, xc2, yc2);
            }

                ////////////////////////////////Circle///////////////////////////////////////
            else if (type == 3)//call Circle_Direct  fundtion
            {
                DrawCircle_Direct(hdc, xc1, yc1, radius);
                alldata += "Circle_Direct," + to_string(xc1) + "," + to_string(yc1) + "," + to_string(radius) + "," +
                           to_string(c) + "\n";
            } else if (type == 4)  //call Circle_Polar  fundtion
            {
                DrawCircle_Polar(hdc, xc1, yc1, radius);
                alldata += "Circle_Polar," + to_string(xc1) + "," + to_string(yc1) + "," + to_string(radius) + "," +
                           to_string(c) + "\n";
            } else if (type == 5) //call Circle_IterativePolar  fundtion
            {
                DrawCircle_Iterative(hdc, xc1, yc1, radius);
                alldata += "Circle_IterativePolar," + to_string(xc1) + "," + to_string(yc1) + "," + to_string(radius) +
                           "," + to_string(c) + "\n";
            } else if (type == 6) //call Circle_Midpoint fundtion
            {
                DrawCircle_MidPoint(hdc, xc1, yc1, radius);
                alldata += "Circle_Midpoint," + to_string(xc1) + "," + to_string(yc1) + "," + to_string(radius) + "," +
                           to_string(c) + "\n";
            } else if (type == 7) //call Circle_ModifiedMidpoint  fundtion
            {
                DrawCircle_ModifiedMidPoint(hdc, xc1, yc1, radius);
                alldata +=
                        "Circle_ModifiedMidpoint," + to_string(xc1) + "," + to_string(yc1) + "," + to_string(radius) +
                        "," + to_string(c) + "\n";
            }

                ////////////////////////////////Filling///////////////////////////////////////
            else if (type == 8 || type == 9) {
                bool is_Circle = true;
                string filling_type = "Filling_Circle_withothercircles";
                if (type == 8) {
                    is_Circle = false;
                    filling_type = "Filling_Circle_withlines";
                }
                DrawCircleMidPoint(hdc, xc1, yc1, radius);  ///draw cilcle.
                if (num_of_the_quarter == '1')           ///choose the quarter to fill
                {
                    filling1stQuarter(hdc, xc1, yc1, radius, is_Circle);
                } else if (num_of_the_quarter == '2') {
                    filling2ndQuarter(hdc, xc1, yc1, radius, is_Circle);
                } else if (num_of_the_quarter == '3') {
                    filling3rdQuarter(hdc, xc1, yc1, radius, is_Circle);
                } else {
                    filling4thQuarter(hdc, xc1, yc1, radius, is_Circle);
                }
                write_tofile_1(filling_type, num_of_the_quarter, xc1, yc1, radius);
            } else if (type == 10)  //call Filling_Square    fundtion
            {
                p[index] = Vector(LOWORD(lParam), HIWORD(lParam));
                //DrawHermiteCurve(hdc, p[0], T1, p[3], T2, RGB(255, 0, 0));
                double sideLen = distanceBetween2Points(p[0][0], p[0][1], p[1][0], p[1][1]);
                drawSquare(hdc, p[0], sideLen, c);
                alldata +=
                        "Filling_Square," + to_string(p[0][0]) + "," + to_string(p[0][1]) + "," + to_string(sideLen) +
                        "," + to_string(c) + "\n";
            } else if (type == 11)  //call Filling_Rectangle    fundtion
            {
                p[index] = Vector(LOWORD(lParam), HIWORD(lParam));
                double width = distanceBetween2Points(p[0][0], p[0][1], p[1][0], p[1][1]);
                double height = distanceBetween2Points(p[0][0], p[0][1], p[2][0], p[2][1]);
                drawRectangle(hdc, p[0], width, height, c);
                alldata +=
                        "Filling_Rectangle," + to_string(p[0][0]) + "," + to_string(p[0][1]) + "," + to_string(width) +
                        "," + to_string(height) + "," + to_string(c) + "\n";
            } else if (type == 12)  //call Filling_Convex    fundtion
            {
                if (!enter) {
                    // cout<<"Enter Number of Points -->"; cin>>NumberOfPoints;
                    alldata += "Filling_Convex,";
                    enter = true;
                }
                convex[counter].x = LOWORD(lParam);
                convex[counter].y = HIWORD(lParam);
                alldata += to_string(convex[counter].x) + "," + to_string(convex[counter].y) + ",";
                if (counter == NumberOfPoints) {
                    int polygonSz = counter + 1;
                    ConvexFill(hdc, convex, polygonSz, c);
                    alldata += to_string(counter) + "," + to_string(c) + "\n";
                    counter = 0;
                    enter = false;
                } else counter++;

            } else if (type == 13) //call Filling_Non_Convex    fundtion
            {
                if (!enter) {
                    alldata += "Filling_Non_Convex,";
                    enter = true;
                }
                nonConvex[counter].x = LOWORD(lParam);
                nonConvex[counter].y = HIWORD(lParam);
                alldata += to_string(nonConvex[counter].x) + "," + to_string(nonConvex[counter].y) + ",";
                if (counter == NumberOfPoints - 1) {
                    int polygonSz = counter + 1;
                    GeneralPolygonFill(hdc, nonConvex, polygonSz, c);
                    alldata += to_string(counter) + "," + to_string(c) + "\n";
                    counter = 0;
                    enter = false;
                } else counter++;
            }
                ////////////////////////////////Floodfill///////////////////////////////////////
            else if (type == 14)   //call FloodFill_Recursive   fundtion
            {
                MyFloodFill(hdc, xc2, yc2, c, RGB(0, 255, 0));
                write_tofile_3("FloodFill_Recursive", xc2, yc2);
            } else if (type == 15)    //call FloodFill_NonRecursive   fundtion
            {
                NRMyFloodFill(hdc, xc2, yc2, c, RGB(0, 255, 0));
                write_tofile_3("FloodFill_Recursive", xc2, yc2);
            }
                ////////////////////////////////Ellipse///////////////////////////////////////
            else if (type == 16) //call Ellipse_Direct  fundtion
            {
                if (count2 == 0) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    xc = LOWORD(lParam);
                    yc = HIWORD(lParam);

                    len = distanceBetween2Points(xc, l1, yc, l2);
                    width = distanceBetween2Points(xc, l3, yc, l4);

                    DrawEllipse_Direct(hdc, xc, yc, len, width, c);
                    write_tofile_1("Ellipse_Direct", xc, yc, len, width);
                    count2 = 0;
                    break;
                }
            } else if (type == 17) //call Ellipse_Polar  fundtion
            {
                if (count2 == 0) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    xc = LOWORD(lParam);
                    yc = HIWORD(lParam);

                    len = distanceBetween2Points(xc, l1, yc, l2);
                    width = distanceBetween2Points(xc, l3, yc, l4);

                    DrawEllipse_polar(hdc, xc, yc, len, width, c);
                    write_tofile_1("Ellipse_Polar", xc, yc, len, width);
                    count2 = 0;
                    break;
                }
            } else if (type == 18) //call Ellipse_Midpoint fundtion
            {
                if (count2 == 0) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    xc = LOWORD(lParam);
                    yc = HIWORD(lParam);

                    len = distanceBetween2Points(xc, l1, yc, l2);
                    width = distanceBetween2Points(xc, l3, yc, l4);

                    DrawEllipse_midpoint(hdc, xc, yc, len, width, c);
                    write_tofile_1("Ellipse_Midpoint", xc, yc, len, width);
                    count2 = 0;
                    break;
                }
            }
                ////////////////////////////////Clipping///////////////////////////////////////

            else if (type == 19) //call Clipping_Rectangle_Point fundtion
            {
                if (!enter) {
                    count2 = 0;
                    enter = true;
                }
                if (count2 == 0) {
                    s[0] = LOWORD(lParam);
                    s[1] = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);

                    len = distanceBetween2Points(s[0], l1, s[1], l2);
                    width = distanceBetween2Points(s[0], l3, s[1], l4);

                    drawRectangle_clipping(hdc, s, len, width, c);
                    count2++;
                    break;
                } else if (count2 == 3) {
                    cout << "count2: " << count2 << endl;
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);

                    PointClipping(hdc, l1, l2, s[0], s[1], s[0] + len, s[1] + width);
                    //CohenSuth(hdc,l1,l2,l3,l4,s[0],s[1],s[0]+len,s[1]+width);

                    alldata +=
                            "Clipping_Rectangle_Point," + to_string(l1) + "," + to_string(l2) + "," + to_string(s[0]) +
                            "," + to_string(s[1]) + "," + to_string(len) + "," + to_string(width) + "," + to_string(c) +
                            "\n";
                    break;
                }

            } else if (type == 20) //call Clipping_Rectangle_Line  fundtion
            {

                if (count2 == 0) {
                    s[0] = LOWORD(lParam);
                    s[1] = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    count2++;

                    len = distanceBetween2Points(s[0], l1, s[1], l2);
                    width = distanceBetween2Points(s[0], l3, s[1], l4);
                    drawRectangle_clipping(hdc, s, len, width, c);
                    count2++;
                    break;
                } else if (count2 == 3) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 4) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);

                    CohenSuth(hdc, l1, l2, l3, l4, s[0], s[1], s[0] + len, s[1] + width);

                    alldata += "Clipping_Rectangle_Line," + to_string(l1) + "," + to_string(l2) + "," + to_string(l3) +
                               "," + to_string(l4) + "," + to_string(s[0]) + "," + to_string(s[1]) + "," +
                               to_string(len) + "," + to_string(width) + "," + to_string(c) + "\n";
                    count2 = 3;
                    break;
                }

            } else if (type == 21) //call Clipping_Rectangle_Polygon  fundtion
            {
                if (!enter) {
                    alldata += "Clipping_Rectangle_Polygon," + to_string(NumberOfPoints) + ",";
                    enter = true;
                }

                if (count2 == 0) {
                    s[0] = LOWORD(lParam);
                    s[1] = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);

                    len = distanceBetween2Points(s[0], l1, s[1], l2);
                    width = distanceBetween2Points(s[0], l3, s[1], l4);


                    drawRectangle_clipping(hdc, s, len, width, c);
                    count2++;
                    break;
                } else if (count2 == 3) {
                    poly[index1] = Vector(LOWORD(lParam), HIWORD(lParam));
                    alldata += to_string(poly[index1][0]) + "," + to_string(poly[index1][1]) + ",";
                    if (index1 == NumberOfPoints - 1) {
                        PolygonClip(hdc, poly, NumberOfPoints, s[0], s[1], s[0] + len, s[1] + width);
                        alldata += to_string(s[0]) + "," + to_string(s[1]) + "," + to_string(len) + "," +
                                   to_string(width) + "," + to_string(c) + "\n";
                        index1 = 0;
                        enter = false;
                    } else index1++;
                    break;
                }
                break;
            } else if (type == 22) //call Clipping_Square_Point   fundtion
            {
                if (count2 == 0) {
                    s[0] = LOWORD(lParam);
                    s[1] = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    len = distanceBetween2Points(s[0], l1, s[1], l2);
                    drawSquare2(hdc, s, len, c);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    PointClipping(hdc, l1, l2, s[0], s[1], s[0] + len, s[1] + len);
                    alldata += "Clipping_Square_Point," + to_string(l1) + "," + to_string(l2) + "," + to_string(s[0]) +
                               "," + to_string(s[1]) + "," + to_string(len) + "," + to_string(c) + "\n";
                    break;
                }
            } else if (type == 23) //call Clipping_Square_Line   fundtion
            {
                if (count2 == 0) {
                    s[0] = LOWORD(lParam);
                    s[1] = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    len = distanceBetween2Points(s[0], l1, s[1], l2);
                    drawSquare2(hdc, s, len, c);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    CohenSuth(hdc, l3, l4, xc1, yc1, s[0], s[1], s[0] + len, s[1] + len);
                    alldata +=
                            "Clipping_Square_Line," + to_string(l3) + "," + to_string(l4) + "," + to_string(xc1) + "," +
                            to_string(yc1) + "," + to_string(s[0]) + "," + to_string(s[1]) + "," + to_string(len) +
                            "," + to_string(c) + "\n";
                    //count2=2;
                    break;
                }
            } else if (type == 24) //call Clipping_circle_Point
            {
                if (count2 == 0) {
                    xc = LOWORD(lParam);
                    yc = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    dx = LOWORD(lParam);
                    dy = HIWORD(lParam);
                    distance = distanceBetween2Points(xc, dx, yc, dy);
                    DrawCircleMidPoint(hdc, xc, yc, distance);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    PointClippingCircle(hdc, l3, l4, distance, xc, yc, c);
                    alldata +=
                            "Clipping_circle_Point," + to_string(l3) + "," + to_string(l4) + "," + to_string(distance) +
                            "," + to_string(xc) + "," + to_string(yc) + "," + to_string(c) + "\n";
                    break;
                }
            } else if (type == 25) //call Clipping_circle_Line
            {
                if (count2 == 0) {
                    xc = LOWORD(lParam);
                    yc = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 1) {
                    dx = LOWORD(lParam);
                    dy = HIWORD(lParam);
                    distance = distanceBetween2Points(xc, dx, yc, dy);
                    DrawCircleMidPoint(hdc, xc, yc, distance);
                    count2++;
                    break;
                } else if (count2 == 2) {
                    l1 = LOWORD(lParam);
                    l2 = HIWORD(lParam);
                    count2++;
                    break;
                } else if (count2 == 3) {
                    l3 = LOWORD(lParam);
                    l4 = HIWORD(lParam);
                    DrawLineByDDACircle(hdc, l1, l3, l2, l4, xc, yc, distance, c);
                    alldata +=
                            "Clipping_circle_Line," + to_string(l1) + "," + to_string(l3) + "," + to_string(l2) + "," +
                            to_string(l4) + "," + to_string(xc) + "," + to_string(yc) + "," + to_string(distance) +
                            "," + to_string(c) + "\n";
                    count2 = 2;
                    break;
                }

            } else if (type == 26)//call DrawCardinalSpline
            {
                if (!enter) {

                    alldata += "DrawCardinalSpline," + to_string(NumberOfPoints) + ",";
                    enter = true;
                }

                p1[index1] = Vector(LOWORD(lParam), HIWORD(lParam));
                alldata += to_string(p1[index1][0]) + "," + to_string(p1[index1][1]) + ",";
                //cout<<p1[index1][0]<<"            "<<p1[index1][1]<<endl;
                if (index1 == NumberOfPoints - 1) {
                    DrawCardinalSpline(hdc, p1, 1);
                    alldata += to_string(c) + "\n";
                    index1 = 0;
                    enter = false;
                } else {
                    index1++;
                }
            }
            break;
        case WM_SETCURSOR:
            SetCursor(LoadCursor(nullptr, mouse));
            break;

        case WM_CREATE:
            AddMenus(hwnd);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lParam);
    }
    return 0;
}

HMENU hmenu;

void AddMenus(HWND hwnd) {
    hmenu = CreateMenu();


    //////////////////////////Mouse_cursor//////////////////////////////////
    HMENU Mouse_cursor = CreateMenu();
    size = 4;


    char *Mouse_cursor_arr[size] = {"ARROW", "CROSS", "HAND", "HELP"};
    int Mouse_cursor2[size] = {Mouse_ARROW, Mouse_CROSS, Mouse_HAND, Mouse_HELP};

    for (int i = 0; i < size; i++) {
        AppendMenu(Mouse_cursor, MF_STRING, Mouse_cursor2[i], Mouse_cursor_arr[i]);
        if (i != size - 1) AppendMenu(Mouse_cursor, MF_SEPARATOR, NULL, nullptr);
    }
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Mouse_cursor, "Mouse_cursor");



    ////////////////////////// line //////////////////////////////////
    HMENU Line = CreateMenu();
    size = 3;
    char *line_types[size] = {"DDA", "midpoint", "Parametric"};
    int line1[size] = {DDA, midpoint, Parametric};

    for (int i = 0; i < size; i++) {
        AppendMenu(Line, MF_STRING, line1[i], line_types[i]);
        if (i != size - 1) AppendMenu(Line, MF_SEPARATOR, NULL, nullptr);
    }
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Line, "Line");


    ////////////////////////// Circle  //////////////////////////////////
    HMENU Circle = CreateMenu();


    size = 5;
    char *Circle_types[size] = {"Direct", "Polar", "Iterative Polar", "Midpoint", "Modified Midpoint"};
    int Circle1[size] = {Circle_Direct, Circle_Polar, Circle_IterativePolar, Circle_Midpoint, Circle_ModifiedMidpoint};

    for (int i = 0; i < size; i++) {
        AppendMenu(Circle, MF_STRING, Circle1[i], Circle_types[i]);
        if (i != size - 1) AppendMenu(Circle, MF_SEPARATOR, NULL, nullptr);
    }

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Circle, "Circle");

    //////////////////////////Filling//////////////////////////////////
    HMENU Filling = CreateMenu();
    HMENU FillingCircle = CreateMenu();
    AppendMenu(FillingCircle, MF_STRING, Filling_Circle_withlines, "with lines");
    AppendMenu(FillingCircle, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(FillingCircle, MF_STRING, Filling_Circle_withothercircles, "with other circles");

    AppendMenu(Filling, MF_POPUP, (UINT_PTR) FillingCircle, "Circle");
    AppendMenu(Filling, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Filling, MF_STRING, Filling_Square, "Square");
    AppendMenu(Filling, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Filling, MF_STRING, Filling_Rectangle, "Rectangle");
    AppendMenu(Filling, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Filling, MF_STRING, Filling_Convex, "Convex");
    AppendMenu(Filling, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Filling, MF_STRING, Filling_Non_Convex, "Non Convex");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Filling, "Filling");
    //////////////////////////FloodFill//////////////////////////////////
    HMENU FloodFill = CreateMenu();

    size = 2;
    char *FloodFill_types[size] = {"Recursive", "Non Recursive"};
    int FloodFill1[size] = {FloodFill_Recursive, FloodFill_NonRecursive};

    for (int i = 0; i < size; i++) {
        AppendMenu(FloodFill, MF_STRING, FloodFill1[i], FloodFill_types[i]);
        if (i != size - 1) AppendMenu(FloodFill, MF_SEPARATOR, NULL, nullptr);
    }

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) FloodFill, "FloodFill");

    //////////////////////////Ellipse//////////////////////////////////
    HMENU Ellipse = CreateMenu();


    size = 3;
    char *Ellipse_types[size] = {"Direct", "Polar", "Midpoint"};
    int Ellipse1[size] = {Ellipse_Direct, Ellipse_Polar, Ellipse_Midpoint};

    for (int i = 0; i < size; i++) {
        AppendMenu(Ellipse, MF_STRING, Ellipse1[i], Ellipse_types[i]);
        if (i != size - 1) AppendMenu(Ellipse, MF_SEPARATOR, NULL, nullptr);
    }
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Ellipse, "Ellipse");


    //////////////////////////Clipping algorithms//////////////////////////////////

    HMENU Clipping_algorithms = CreateMenu();

    HMENU Clipping_algorithms2 = CreateMenu();

    AppendMenu(Clipping_algorithms2, MF_STRING, Clipping_Rectangle_Point, "Point");
    AppendMenu(Clipping_algorithms2, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms2, MF_STRING, Clipping_Rectangle_Line, "Line");
    AppendMenu(Clipping_algorithms2, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms2, MF_STRING, Clipping_Rectangle_Polygon, "Polygon");

    HMENU Clipping_algorithms3 = CreateMenu();
    AppendMenu(Clipping_algorithms3, MF_STRING, Clipping_Square_Point, "Point");
    AppendMenu(Clipping_algorithms3, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms3, MF_STRING, Clipping_Square_Line, "Line");

    HMENU Clipping_algorithms4 = CreateMenu();
    AppendMenu(Clipping_algorithms4, MF_STRING, Clipping_circle_Point, "Point");
    AppendMenu(Clipping_algorithms4, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms4, MF_STRING, Clipping_circle_Line, "Line");


    AppendMenu(Clipping_algorithms, MF_POPUP, (UINT_PTR) Clipping_algorithms2, "Rectangle");
    AppendMenu(Clipping_algorithms, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms, MF_POPUP, (UINT_PTR) Clipping_algorithms3, "Square");
    AppendMenu(Clipping_algorithms, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(Clipping_algorithms, MF_POPUP, (UINT_PTR) Clipping_algorithms4, "Circle");


    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Clipping_algorithms, "Clipping algorithms");


    //////////////////////////Curve//////////////////////////////////

    HMENU Curve = CreateMenu();

    AppendMenu(Curve, MF_STRING, Curve_CardinalSpline, "Cardinal Spline");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Curve, "Curve");

    ////////////////////////color//////////////////////////////////


    HMENU color = CreateMenu();
    size = 10;
    char *colour[size] = {"Red", "Blue", "White", "Green", "Black", "Yellow", "Cyan", "Magenta", "Teal", "Purple"};
    int Colors[size] = {RED, BLUE, WHITE, GREEN, BLACK, Yellow, Cyan, Magenta, Teal, Purple};

    for (int i = 0; i < size; i++) {
        AppendMenu(color, MF_STRING, Colors[i], colour[i]);
        if (i != size - 1)AppendMenu(color, MF_SEPARATOR, NULL, nullptr);
    }
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) color, "Colors");


    //////////////////////////Load Store Clear//////////////////////////////////
    HMENU Load_Store_Clear = CreateMenu();
    size = 3;
    char *load_arr[size] = {"Store", "Load", "Clear"};
    int load1[size] = {Store, Load, Clear};

    for (int i = 0; i < size; i++) {
        AppendMenu(Load_Store_Clear, MF_STRING, load1[i], load_arr[i]);
        if (i != size - 1)AppendMenu(Load_Store_Clear, MF_SEPARATOR, NULL, nullptr);
    }
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) Load_Store_Clear, "Settings");

    SetMenu(hwnd, hmenu);
}

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh) {
    WNDCLASS wc;

    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Graphic Project", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, hinst,
                             nullptr);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
