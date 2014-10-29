#include "Color.hpp"

namespace hpl
{
Color Color::fromHSV(float h, float s, float v)
{
	int h = H * 6;
	float f = H * 6.0f - h;
	float p = V*(1.0f-S);
	float q = V*(1.0f-S*f);
	float t = V*(1.0f-S*(1.0f-f));
	
	switch (h) {
	case 0:
	case 6:
		*r = V; *g = t; *b = p;
		break;
	case 1:
		*r = q; *g = V; *b = p;
		break;
	case 2:
		*r = p; *g = V; *b = t;
		break;
	case 3:
		*r = p; *g = q; *b = V;
		break;
	case 4:
		*r = t; *g = p; *b = V;
		break;
	case 5:
		*r = V; *g = p; *b = q;
		break;
	default:
		break;
	}
}
}

#endif // COLOR_HPP
