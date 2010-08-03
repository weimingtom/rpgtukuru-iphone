/**
 * @file
 * @brief main loop
 * @author project.kuto
 */

#include <cstdlib>
#include <kuto/kuto_graphics_device.h>
#include "AppMain.h"

#if RPG2K_IS_PSP

#include <pspkernel.h>

// PSP_MODULE_INFO("RPG_RT_EMU_2000", 0, 0, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

PSP_HEAP_SIZE_KB(5000);

#endif


namespace
{
	AppMain appMain;

	void update(float dt)
	{
		appMain.update();
	}
}; // namespace

#if RPG2K_IS_PSP
extern "C" int SDL_main(int argc, char* argv[])
#else
extern "C" int main(int argc, char* argv[])
#endif
{
	kuto::GraphicsDevice::createInstance();
	kuto::GraphicsDevice::instance()->initialize(argc, argv, 320, 480, "RPG Tukuru", update);
	appMain.initialize();

	glutMainLoop();

	return EXIT_SUCCESS;
}
