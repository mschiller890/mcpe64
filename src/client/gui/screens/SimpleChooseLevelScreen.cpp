#include "SimpleChooseLevelScreen.h"
#include "ProgressScreen.h"
#include "ScreenChooser.h"
#include "../components/Button.h"
#include "../components/ImageButton.h"
#include "../../Minecraft.h"
#include "../../../world/level/LevelSettings.h"
#include "../../../platform/time.h"
#include "../../../platform/input/Keyboard.h"

SimpleChooseLevelScreen::SimpleChooseLevelScreen(const std::string& levelName)
:	bCreate(0),
	bBack(0),
	levelName(levelName),
	hasChosen(false),
	_isCreative(true),
	_nameInput(""),
	_cursorBlink(0)
{
}

SimpleChooseLevelScreen::~SimpleChooseLevelScreen()
{
	delete bCreate;
	delete bBack;
}

void SimpleChooseLevelScreen::init()
{
	if (minecraft->useTouchscreen()) {
		bCreate = new Touch::TButton(1, "Create World");
	} else {
		bCreate = new Button(1, "Create World");
	}
	bBack = new ImageButton(3, "");
	ImageDef def;
	def.name = "gui/touchgui.png";
	def.width = 34;
	def.height = 26;
	def.setSrc(IntRectangle(150, 0, (int)def.width, (int)def.height));
	bBack->setImageDef(def, true);

	buttons.push_back(bCreate);
	buttons.push_back(bBack);

	tabButtons.push_back(bCreate);
	tabButtons.push_back(bBack);
}

void SimpleChooseLevelScreen::setupPositions()
{
	bCreate->width = 120;
	bCreate->x = (width - bCreate->width) / 2;
	bCreate->y = height / 2 + 36;
	bBack->x = width - bBack->width;
	bBack->y = 0;
}

void SimpleChooseLevelScreen::tick()
{
	_cursorBlink++;
}

void SimpleChooseLevelScreen::render( int xm, int ym, float a )
{
	renderDirtBackground(0);
	glEnable2(GL_BLEND);

	const int cx = width / 2;
	const int cy = height / 2;

	// World name label + input box
	drawCenteredString(minecraft->font, "World Name:", cx, cy - 46, 0xffffffff);
	const int boxW = 160;
	const int boxH = 16;
	const int boxX = cx - boxW / 2;
	const int boxY = cy - 36;
	fill(boxX - 1, boxY - 1, boxX + boxW + 1, boxY + boxH + 1, 0xff000000);
	fill(boxX,     boxY,     boxX + boxW,     boxY + boxH,     0xff202020);
	std::string display = _nameInput;
	if ((_cursorBlink / 10) % 2 == 0)
		display += '|';
	minecraft->font->draw(display, (float)(boxX + 3), (float)(boxY + (boxH - 8) / 2 + 1), 0xffffffff);

	// Game mode label
	drawCenteredString(minecraft->font, "Game Mode:", cx, cy - 10, 0xffffffff);

	// Toggle switch
	const int toggleW = 160;
	const int toggleH = 18;
	const int toggleX = cx - toggleW / 2;
	const int toggleY = cy;
	const int halfW   = toggleW / 2;

	fill(toggleX - 1, toggleY - 1, toggleX + toggleW + 1, toggleY + toggleH + 1, 0xff000000);
	fill(toggleX,          toggleY, toggleX + halfW,          toggleY + toggleH, _isCreative  ? 0xff2d5a2d : 0xff1a1a1a);
	fill(toggleX + halfW,  toggleY, toggleX + toggleW,        toggleY + toggleH, !_isCreative ? 0xff5a2d2d : 0xff1a1a1a);
	fill(toggleX + halfW,  toggleY, toggleX + halfW + 1,      toggleY + toggleH, 0xff000000);

	drawCenteredString(minecraft->font, "Creative",
		toggleX + halfW / 2,          toggleY + (toggleH - 8) / 2 + 1,
		_isCreative  ? 0xffffffff : 0xff666666);
	drawCenteredString(minecraft->font, "Survival",
		toggleX + halfW + halfW / 2,  toggleY + (toggleH - 8) / 2 + 1,
		!_isCreative ? 0xffffffff : 0xff666666);

	Screen::render(xm, ym, a);
	glDisable2(GL_BLEND);
}

void SimpleChooseLevelScreen::mouseClicked(int x, int y, int buttonNum)
{
	const int cx      = width / 2;
	const int cy      = height / 2;
	const int toggleW = 160;
	const int toggleH = 18;
	const int toggleX = cx - toggleW / 2;
	const int toggleY = cy;
	const int halfW   = toggleW / 2;

	if (y >= toggleY && y <= toggleY + toggleH &&
	    x >= toggleX && x <= toggleX + toggleW) {
		_isCreative = (x < toggleX + halfW);
		return;
	}
	Screen::mouseClicked(x, y, buttonNum);
}

void SimpleChooseLevelScreen::buttonClicked( Button* button )
{
	if (button == bBack) {
		minecraft->screenChooser.setScreen(SCREEN_STARTMENU);
		return;
	}
	if (button == bCreate && !hasChosen) {
		std::string chosenName = _nameInput.empty() ? levelName : _nameInput;
		std::string levelId = getUniqueLevelName(chosenName);
		int gameType = _isCreative ? GameType::Creative : GameType::Survival;
		LevelSettings settings(getEpochTimeS(), gameType);
		minecraft->selectLevel(levelId, chosenName, settings);
		minecraft->hostMultiplayer();
		minecraft->setScreen(new ProgressScreen());
		hasChosen = true;
	}
}

void SimpleChooseLevelScreen::keyPressed(int eventKey)
{
	if (eventKey == Keyboard::KEY_BACKSPACE) {
		if (!_nameInput.empty())
			_nameInput.erase(_nameInput.size() - 1, 1);
	} else if (eventKey == Keyboard::KEY_ESCAPE) {
		minecraft->screenChooser.setScreen(SCREEN_STARTMENU);
	}
}

void SimpleChooseLevelScreen::keyboardNewChar(char inputChar)
{
	if (_nameInput.size() < 24 && inputChar >= 32 && inputChar < 127)
		_nameInput += inputChar;
}

bool SimpleChooseLevelScreen::handleBackEvent(bool isDown) {
	if (!isDown)
		minecraft->screenChooser.setScreen(SCREEN_STARTMENU);
	return true;
}
