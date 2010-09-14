/**
 * @file
 * @brief Select Chara Window
 * @author project.kuto
 */

#include <kuto/kuto_render_manager.h>
#include <kuto/kuto_graphics2d.h>
#include <kuto/kuto_virtual_pad.h>
#include <kuto/kuto_utility.h>
#include "game_chara_select_menu.h"
#include "game_field.h"
#include "game_player.h"


GameCharaSelectMenu::GameCharaSelectMenu(GameField* gameField)
: GameSelectWindow(gameField->getGameSystem())
, gameField_(gameField), useFullSelectKey_(false)
{
	faceEnable_ = true;
	rowHeight_ = 48.f;
	lineSpace_ = 10.f;
}

void GameCharaSelectMenu::update()
{
	clearMessages();
	for (uint i = 0; i < gameField_->getPlayers().size(); i++) {
		addLine("");
	}
	switch (state_) {
	case kStateLoop:
		if (!pauseUpdateCursor_) {
			if (useFullSelectKey_) {
				kuto::VirtualPad* virtualPad = kuto::VirtualPad::instance();
				if (virtualPad->repeat(kuto::VirtualPad::KEY_LEFT)) {
					fullSelect_ = false;
				}
				if (virtualPad->repeat(kuto::VirtualPad::KEY_RIGHT)) {
					fullSelect_ = true;
				}
			}
		}
		break;
	default: break;
	}
	GameSelectWindow::update();
}

void GameCharaSelectMenu::render(kuto::Graphics2D* g) const
{
	renderFrame(g);

	if (showCursor_) {
		renderSelectCursor(g);
	}

	for (uint i = 0; i < gameField_->getPlayers().size(); i++) {
		renderPlayerInfo(i);
	}

	if (showCursor_) {
		int rowSize = (int)(size_.y / rowHeight_);
		if (rowSize * columnSize_ + scrollPosition_ * columnSize_ < (int)messages_.size()) {
			renderDownCursor(g);
		}
		if (scrollPosition_ > 0) {
			renderUpCursor(g);
		}
	}
}

void GameCharaSelectMenu::renderPlayerInfo(int index) const
{
	GamePlayer* gamePlayer = gameField_->getPlayers()[index];
	const rpg2k::model::DataBase& ldb = gameField_->getGameSystem().getLDB();
	// const GamePlayerInfo& player = gameField_->getGameSystem().getPlayerInfo(gamePlayer->getPlayerId());
	uint playerID = gamePlayer->getPlayerId();
	char temp[256];
	// face
	kuto::Vector2 facePos(8.f + position_.x, index * 58.f + 8.f + position_.y);
	gameField_->getPlayers()[index]->renderFace(facePos);
	// status
	kuto::Graphics2D* g = kuto::RenderManager::instance()->getGraphics2D();
	const kuto::Color color(1.f, 1.f, 1.f, 1.f);
	kuto::Vector2 windowPosition(position_.x + 64.f, position_.y);
	const GameCharaStatus::BadConditionList& badConditions = gamePlayer->getStatus().getBadConditions();
	const char* conditionStr = NULL;
	if (badConditions.empty()) {
		conditionStr = ldb.vocabulary(126).toSystem().c_str();
	} else {
		GameCharaStatus::BadCondition cond = badConditions[0];
		for (uint i = 1; i < badConditions.size(); i++) {
			if (ldb.condition()[badConditions[i].id][4].get<int>() > ldb.condition()[cond.id][4].get<int>()) {
				cond = badConditions[i];
			}
		}
		conditionStr = ldb.condition()[cond.id][1].get_string().toSystem().c_str();
	}
	if (size_.x < 200.f) {	// short version
		kuto::Vector2 pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 8.f + 2.f;
		g->drawText(gameField_->getGameSystem().name(playerID).c_str(), pos, color, fontSize_, kuto::Font::NORMAL);

		pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 16.f + 8.f;
		g->drawText(ldb.vocabulary(128).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 12.f;
		sprintf(temp, "%2d", gamePlayer->getStatus().getLevel());
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 48.f;
		g->drawText(ldb.vocabulary(129).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 12.f;
		sprintf(temp, "%3d/%3d", gamePlayer->getStatus().getHp(), gamePlayer->getStatus().getBaseStatus()[rpg2k::Param::HP]);
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);

		pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 16.f * 2.f + 8.f;
		g->drawText(conditionStr, pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 60.f;
		g->drawText(ldb.vocabulary(130).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 12.f;
		sprintf(temp, "%3d/%3d", gamePlayer->getStatus().getMp(), gamePlayer->getStatus().getBaseStatus()[rpg2k::Param::MP]);
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);
	} else {
		kuto::Vector2 pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 8.f + 2.f;
		g->drawText(gameField_->getGameSystem().name(playerID).c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 86.f;
		g->drawText(gameField_->getGameSystem().title(playerID).c_str(), pos, color, fontSize_, kuto::Font::NORMAL);

		pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 16.f + 8.f;
		g->drawText(ldb.vocabulary(128).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 12.f;
		sprintf(temp, "%2d", gamePlayer->getStatus().getLevel());
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 30.f;
		g->drawText(conditionStr, pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 50.f;
		g->drawText(ldb.vocabulary(129).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 22.f;
		sprintf(temp, "%3d/%3d", gamePlayer->getStatus().getHp(), gamePlayer->getStatus().getBaseStatus()[rpg2k::Param::HP]);
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);

		pos = windowPosition;
		pos.y += (rowHeight_ + lineSpace_) * index + 16.f * 2.f + 8.f;
		g->drawText(ldb.vocabulary(127).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 12.f;
		sprintf(temp, "%6d/%6d", gamePlayer->getStatus().getExp(), gamePlayer->getStatus().getNextLevelExp());
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 80.f;
		g->drawText(ldb.vocabulary(130).toSystem().c_str(), pos, color, fontSize_, kuto::Font::NORMAL);
		pos.x += 22.f;
		sprintf(temp, "%3d/%3d", gamePlayer->getStatus().getMp(), gamePlayer->getStatus().getBaseStatus()[rpg2k::Param::MP]);
		g->drawText(temp, pos, color, fontSize_, kuto::Font::NORMAL);
	}
}
