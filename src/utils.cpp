#include "utils.h"

Color::Color() :r(0), g(0), b(0),color(0){}

Color::Color(int _r, int _g, int _b)
{
	r = clamp(_r);
	g = clamp(_g);
	b = clamp(_b);
	color = (r << 16) | (g << 8) | (b);
}

Color::~Color(){}

int Color::clamp(int _c)
{
	int c = std::max(0, std::min(_c, 255));
	return c;
}

unsigned int Color::get_color_uint32()
{
	return color;
}
std::ostream& operator<<(std::ostream& out, const Color& c)
{
	out << "r: " << c.r << ",g: " << c.g << ",b: " << c.b;
	return out;
}