#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "ISerializable.h"
#include "IDeserializable.h"
#include "ICursor.h"
#include "IDrawable.h"

namespace Custom {
	class BaseShape :
		public ISerializable,
		public IDeserializable,
		public ICursor,
		public IDrawable
	{
	public:
		BaseShape(const wchar_t* name);
		virtual void SelectPoint(int x, int y) override;
		virtual void SelectArea(int x1, int y1, int x2, int y2) override;
		virtual void SetPoints(int x1, int y1, int x2, int y2);
		void SetColor(Gdiplus::Color color);
		virtual std::wstring SerializeText() override;
		virtual void DeserializeText(std::wstring text) override;

	protected:
		void SetPointsSafe(int x1, int y1, int x2, int y2);

	protected:
		int x, y, width, height;
		int x1, y1, x2, y2;
		Gdiplus::Color color;
		const wchar_t* name;
	};
}

