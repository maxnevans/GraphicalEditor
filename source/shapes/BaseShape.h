#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "../interfaces/ISerializable.h"
#include "../interfaces/IDeserializable.h"
#include "../interfaces/ICursor.h"
#include "../interfaces/IDrawable.h"

namespace Custom {
	class BaseShape :
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
		virtual std::wstring GetName() = 0;
		virtual Gdiplus::Color GetColor();
		void SetColor(Gdiplus::Color color);
		virtual std::wstring SerializeText() override;
		virtual void DeserializeText(std::wstring text) override;

	protected:
		void SetPointsSafe(int x1, int y1, int x2, int y2);

	protected:
		int x, y, width, height;
		int x1, y1, x2, y2;
		Gdiplus::Color color;
	};
}

