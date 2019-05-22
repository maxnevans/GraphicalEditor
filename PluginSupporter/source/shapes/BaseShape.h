#pragma once
#include <windows.h>
#include <gdiplus.h>

#include "../DLLSupport.h"

#include "../interfaces/ISerializable.h"
#include "../interfaces/IDeserializable.h"
#include "../interfaces/ICursor.h"
#include "../interfaces/IDrawable.h"

class __dll BaseShape :
	public ISerializable,
	public IDeserializable,
	public ICursor,
	public IDrawable
{
public:
	BaseShape();
	virtual ~BaseShape() = default;
	inline virtual void SelectPoint(int x, int y) override
	{
	}
	inline virtual void SelectArea(int x1, int y1, int x2, int y2) override
	{
		this->SetPointsSafe(x1, y1, x2, y2);
	}
	inline virtual void SetPoints(int x1, int y1, int x2, int y2)
	{
		this->SetPointsSafe(x1, y1, x2, y2);
	}
	inline virtual int GetX() const
	{
		return this->x;
	}
	inline virtual int GetY() const
	{
		return this->y;
	}
	inline virtual void SetXAnchor(int x)
	{
		this->x = x;
		this->x1 = x;
		this->x2 = this->width + x;
	}
	inline virtual void SetYAnchor(int y)
	{
		this->y = y;
		this->y1 = y;
		this->y2 = y + this->height;
	}
	inline virtual int GetWidth() const
	{
		return this->width;
	}
	inline virtual int GetHeight() const 
	{
		return this->height;
	}
	virtual const wchar_t* GetName() const = 0;
	inline virtual Gdiplus::ARGB GetColor() const
	{
		return this->color;
	}
	inline virtual void SetColor(Gdiplus::ARGB color)
	{
		this->color = color;
	}
	virtual void SerializeText(std::wstring& text) override;
	virtual void DeserializeText(std::wstring text) override;

protected:
	void SetPointsSafe(int x1, int y1, int x2, int y2);

protected:
	int x, y, width, height;
	int x1, y1, x2, y2;
	Gdiplus::ARGB color;
};