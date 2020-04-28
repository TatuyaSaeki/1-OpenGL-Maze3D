
struct Color
{
	static const Color BLACK;

	//Color() {}

	Color(u8 r = 255, u8 g = 255, u8 b = 255, u8 a=255)
	: _r(r), _g(g), _b(b), _a(a)
	{}

	u8	_r;
	u8	_g;
	u8	_b;
	u8	_a;
};
