#pragma once
#include <memory>
#include <SDL_render.h>
#include <vector>

#include "graphic/Subscribable.h"
#include "scene/SceneManager.h"


namespace gamelib
{
	class Event;
	class Window;
}

namespace libcausality
{
	class ToolWindow final : public gamelib::Subscribable<gamelib::Window>
	{
	public:
		explicit ToolWindow(const char* windowName, gamelib::uint width = 800, gamelib::uint height = 600, const char* windowTitle = nullptr)
			:  Subscribable(std::make_shared<gamelib::Window>(windowName, width, height, windowTitle),
			                        windowName)
		{
			SubscribeToEvent(gamelib::DrawCurrentSceneEventId);
			GetItem()->Initialize();
		}

		std::vector<std::shared_ptr<gamelib::Event>> HandleEvent(const std::shared_ptr<gamelib::Event>& evt, const unsigned long deltaMs) override
		{		
			if(evt->Id.PrimaryId == gamelib::DrawCurrentSceneEventId.PrimaryId) 
			{
				const auto window = GetItem();

				if(!IsDirty) 
				{
					window->PresentOnly();
				}
				else
				{
					window->ClearAndDraw([&](SDL_Renderer* renderer)
					{				
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Black
						SDL_RenderDrawLine(renderer, 0,0, static_cast<int>(window->Width()), static_cast<int>(window->Height()));
						IsDirty = false;
					});
				}
			}
			return {};
		}
		bool IsDirty = true;
	};
}
