/**
 * @file
 * @brief Save Menu
 * @author project.kuto
 */

#include <kuto/kuto_render_manager.h>
#include <kuto/kuto_file.h>
#include "game_save_menu.h"
#include "game_system.h"
#include "game_field.h"


GameSaveMenu::GameSaveMenu(GameField* gameField)
: GameSystemMenuBase(gameField)
{
	menu_ = addChild(GameSaveLoadMenu::createTask(gameField->getGameSystem(), true));
}

bool GameSaveMenu::initialize()
{
	if (menu_->initialize()) {
		start();
		return true;
	}
	return false;
}

void GameSaveMenu::update()
{
	switch (state_) {
	case kStateTop:
		if (menu_->selected()) {
			gameField_->getGameSystem().saveLSD(menu_->selectIndex());
			/*
			GameSaveData saveData;
			saveData.save(gameField_);

			char dirName[256];
			sprintf(dirName, "%s/Documents/%s", kuto::Directory::getHomeDirectory().c_str(),
				kuto::File::getFileName(gameField_->getGameSystem().getLDB().directory()).c_str());
			if (!kuto::Directory::exists(dirName)) {
				kuto::Directory::create(dirName);
			}
			char saveName[256];
			sprintf(saveName, "%s/Save%02d.lsdi", dirName, menu_->selectIndex() + 1);
			FILE* fp = fopen(saveName, "w");
			if (fp) {
				fwrite(&saveData, sizeof(saveData), 1, fp);
				fclose(fp);
			}
			state_ = kStateNone;
			 */
		} else if (menu_->canceled()) {
			state_ = kStateNone;
		}
		break;
	}
}

void GameSaveMenu::start()
{
	freeze(false);
	state_ = kStateTop;
	menu_->start();
}

void GameSaveMenu::render(kuto::Graphics2D* g) const
{
}
