#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "ISerializable.h"
#include "IDeserializable.h"

namespace Custom {
	class BaseShape :
		public ISerializable,
		public IDeserializable
	{
	public:
		BaseShape(const wchar_t* name);
		virtual void Redraw(Gdiplus::Graphics* const graphics) = 0;
		virtual void SetPoints(int x1, int y1, int x2, int y2);
		void SetColor(Gdiplus::Color color);
		virtual std::wstring SerializeText() override;
		virtual void DeserializeText(std::wstring text) override;

	protected:
		int x, y, width, height;
		int x1, y1, x2, y2;
		Gdiplus::Color color;
		const wchar_t* name;
	};
}

