#pragma once

namespace Engine
{
	class GameProcessor;

	struct GameDesc
	{
		shared_ptr<GameProcessor> app = nullptr;
		wstring appName = L"SampleGameName";
		HINSTANCE hInstance = nullptr;
		HWND hWnd = 0;
		float width = 800.f;
		float height = 600.f;
		bool vsync = false;
		bool windowed = true;
		Color clearColor = Color(0.5f, 0.5f, 0.5f);
	};

	class Game
	{
		DECLARE_SINGLE(Game);
	public:
		WPARAM Run(GameDesc& desc);

		GameDesc& GetGameDesc() { return m_desc; }

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance(int cmdShow);

		void Update();
		void ShowFPS();

		static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		GameDesc m_desc;
	};
}

