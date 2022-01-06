#ifndef __UTILS_H__
#define __UTILS_H__
#include<algorithm>
#include<iostream>
class Color
{
private:
	int r, g, b;
	unsigned int color;
public:
	Color();
	Color(int _r, int _g, int _b);
	~Color();
	int color_255(int _c);
	unsigned int get_color_uint32();
	friend std::ostream& operator<<(std::ostream& out, const Color& c);
};

#endif //__UTILS_H__