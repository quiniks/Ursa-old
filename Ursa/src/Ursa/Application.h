#pragma once

#include "Core.h"

namespace Ursa {

	class URSA_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	//Defined in client
	Application* CreateApplication();
}

