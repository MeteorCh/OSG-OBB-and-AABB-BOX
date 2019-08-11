#include "stdafx.h"
#include "Trianngle.h"


Trianngle::Trianngle()
{
	init();
}


Trianngle::~Trianngle()
{
}

void Trianngle::init()
{
	this->vertexIndexs[0] = this->vertexIndexs[1] = this->vertexIndexs[2] = -1;
	this->normal.set(0.0f, 0.0f, 0.0f);
}

