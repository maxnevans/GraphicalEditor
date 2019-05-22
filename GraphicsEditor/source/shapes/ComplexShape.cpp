#include "ComplexShape.h"
#include "../../../Exceptions/source/Exceptions.h"
#include <assert.h>
#include <string>

ComplexShape::ComplexShape(const std::wstring& name, const std::vector<const BaseShape*>& shapes)
	:
	name(name),
	shapes(shapes)
{
	assert(shapes.size() > 0);

	this->refWidth = shapes.front()->GetX() + shapes.front()->GetWidth();
	this->refHeight = shapes.front()->GetY() + shapes.front()->GetHeight();

	for (const BaseShape* shape : this->shapes)
	{
		this->gfxShapes.push_back(shape->Clone());
		int w = shape->GetX() + shape->GetWidth();
		int h = shape->GetY() + shape->GetHeight();
		this->refWidth = this->refWidth < w ? w : this->refWidth;
		this->refHeight = this->refHeight < h ? h : this->refHeight;
	}
		
}

ComplexShape::~ComplexShape()
{
	for (BaseShape* shape : this->gfxShapes)
		delete shape;
}

void ComplexShape::Redraw(Gdiplus::Graphics* const graphics)
{
	for (BaseShape* shape : this->gfxShapes)
		shape->Redraw(graphics);
}

void ComplexShape::SetPoints(int x1, int y1, int x2, int y2)
{
	BaseShape::SetPointsSafe(x1, y1, x2, y2);

	float distortionX = static_cast<float>(this->width) / this->refWidth;
	float distortionY = static_cast<float>(this->height) / this->refHeight;

	for (size_t i = 0; i < this->shapes.size(); i++)
	{
		
		float shapeX = this->shapes[i]->GetX();
		float shapeY = this->shapes[i]->GetY();
		float shapeWidth = this->shapes[i]->GetWidth();
		float shapeHeight = this->shapes[i]->GetHeight();

		shapeX = shapeX * distortionX + this->x;
		shapeY = shapeY * distortionY + this->y;
		shapeWidth *= distortionX;
		shapeHeight *= distortionY;

		this->gfxShapes[i]->SetPoints(shapeX, shapeY, shapeX + shapeWidth, shapeY + shapeHeight);
	}
}

ComplexShape::ComplexShapeFactoryFunctor::~ComplexShapeFactoryFunctor()
{
	for (const BaseShape* shape : this->shapes)
		delete shape;
}
