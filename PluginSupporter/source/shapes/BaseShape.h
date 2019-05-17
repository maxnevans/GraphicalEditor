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
	virtual void SelectPoint(int x, int y) override;
	virtual void SelectArea(int x1, int y1, int x2, int y2) override;
	virtual void SetPoints(int x1, int y1, int x2, int y2);
	virtual int GetX();
	virtual int GetY();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual const wchar_t* GetName() = 0;
	virtual Gdiplus::ARGB GetColor();
	void SetColor(Gdiplus::ARGB color);
	virtual const wchar_t* SerializeText() override;
	virtual void DeserializeText(const wchar_t* text) override;

protected:
	void SetPointsSafe(int x1, int y1, int x2, int y2);

protected:
	int x, y, width, height;
	int x1, y1, x2, y2;
	Gdiplus::ARGB color;
};