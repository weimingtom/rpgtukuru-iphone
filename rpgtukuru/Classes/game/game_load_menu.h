/**
 * @file
 * @brief Load Menu
 * @author project.kuto
 */
#pragma once

#include <kuto/kuto_task.h>
#include "game_saveload_menu.h"

namespace rpg2k { namespace model { class Project; } }


class GameLoadMenu : public GameSaveLoadMenu, public kuto::TaskCreatorParam1<GameLoadMenu, rpg2k::model::Project&>
{
	friend class kuto::TaskCreatorParam1<GameLoadMenu, rpg2k::model::Project&>;
private:
	GameLoadMenu(rpg2k::model::Project& gameSystem);

	virtual bool initialize();

public:
	void start();
	int selectSaveId() const { return GameSaveLoadMenu::selectIndex() + 1; }
};
