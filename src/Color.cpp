#include "Color.hpp"

namespace hpl
{
Color Color::fromHSV(float H, float S, float V)
{
	Color c(0.0f, 0.0f, 0.0f);
	
	int h = H * 6;
	float f = H * 6.0f - h;
	float p = V*(1.0f-S);
	float q = V*(1.0f-S*f);
	float t = V*(1.0f-S*(1.0f-f));
	
	switch (h) {
	case 0:
	case 6:
		c.r = V; c.g = t; c.b = p;
		break;
	case 1:
		c.r = q; c.g = V; c.b = p;
		break;
	case 2:
		c.r = p; c.g = V; c.b = t;
		break;
	case 3:
		c.r = p; c.g = q; c.b = V;
		break;
	case 4:
		c.r = t; c.g = p; c.b = V;
		break;
	case 5:
		c.r = V; c.g = p; c.b = q;
		break;
	default:
		break;
	}
	
	return c;
}
}
