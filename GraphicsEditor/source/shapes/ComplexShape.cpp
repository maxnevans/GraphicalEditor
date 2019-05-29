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
		int shapeX1, shapeY1, shapeX2, shapeY2;
		this->shapes[i]->GetPoints(shapeX1, shapeY1, shapeX2, shapeY2);


		shapeX1 = static_cast<int>(shapeX1 * distortionX + this->x);
		shapeY1 = static_cast<int>(shapeY1 * distortionY + this->y);
		shapeX2 = static_cast<int>(shapeX2 * distortionX + this->x);
		shapeY2 = static_cast<int>(shapeY2 * distortionY + this->y);

		this->gfxShapes[i]->SetPoints(shapeX1, shapeY1, shapeX2, shapeY2);
	}
}

ComplexShape::ComplexShapeFactoryFunctor::~ComplexShapeFactoryFunctor()
{
	for (const BaseShape* shape : this->shapes)
		delete shape;
}
