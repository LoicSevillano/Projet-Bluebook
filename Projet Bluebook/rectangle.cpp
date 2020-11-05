#include "rectangle.h"
#include <math.h>

Crectangle::Crectangle()
{
	m_left = 0;
	m_right	= 0;
	m_top = 0;
	m_bottom = 0;
}

int Crectangle::Longueur()
{
	return abs (m_right - m_left);
}

int Crectangle::Largeur()
{
	return abs(m_bottom - m_top);
}

int Crectangle::Perimetre()
{
	return 2 * (Longueur() + Largeur());
}
Crectangle::Crectangle(int lLeft, int rRight, int tTop, int bBottom)
{
	m_left = lLeft;
	m_right = rRight;
	m_top = tTop;
	m_bottom = bBottom;
}

