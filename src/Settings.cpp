#include "Settings.h"

namespace SkyrimSoulsRE
{
	Settings* Settings::GetSingleton()
	{
		static Settings singleton;
		return &singleton;
	}

	void LoadSettings()
	{
		Settings* settings = Settings::GetSingleton();

		//Unpaused
		settings->unpausedMenus[RE::BarterMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::BookMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::Console::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::ContainerMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::FavoritesMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::GiftMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::InventoryMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::JournalMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::LevelUpMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::LockpickingMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::MagicMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::MapMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::MessageBoxMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::ModManagerMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::SleepWaitMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::StatsMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::TrainingMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::TutorialMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus[RE::TweenMenu::MENU_NAME.data()] = true;
		settings->unpausedMenus["CustomMenu"] = true;

		//Auto-close
		settings->autoCloseMenus = true;        // Auto close menus if the distance to the object is larger than the specified value.
		settings->autoCloseDistance = 400.0f;   // The maximum distance (in in-game units) before the menus will automatically close. Too small values can cause the menus to close unexpectedly.
		settings->autoCloseTolerance = 100.0f;  // The maximum distance (in in-game units) where the menus NEVER auto-close (in relation to the players initial position when the menu was opened). This is used as a failsafe when the initial distance is larger than the maximum allowed to prevent the menu from closing immediately (e.g. a container opened by a script)

		//Controls
		settings->enableMovementInMenus = true;       // If enabled, you will be able to move when a menu is open. Use the mouse (or the D-pad on controllers) to navigate the menus.\n#  (For controllers users) To change tabs in SkyUI favorites menu, use LB and RB buttons
		settings->enableToggleRun = false;            // Allows \"Toggle walk/run\" control to be available when in menus (Caps Lock by default)
		settings->enableGamepadCameraMove = true;     // If enabled, you will be able to move the camera when using controllers. To rotate items in the inventory, maximize the preview first by pressing on the thumb stick
		settings->enableCursorCameraMove = true;      // If enabled, you will be able to move the camera with the mouse by moving it to the edge of the screen (similar to how it works in the dialogue menu)
		settings->cursorCameraVerticalSpeed = 0.15f;  // The vertical and horizontal speed the camera moves when bEnableCursorCameraMove is enabled
		settings->cursorCameraHorizontalSpeed = 0.25f;

		//Slowmotion
		settings->slowMotionMultiplier = 1.0f;  // This is the multiplier that will affect the game speed when a menu is open.\n# 1.0 is no slowdown, 0.5 is half the speed, etc.\n# Too low values can result in dropped items floating in space

		settings->slowMotionMenus[RE::BarterMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::BookMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::Console::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::ContainerMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::FavoritesMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::GiftMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::InventoryMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::JournalMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::LevelUpMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::LockpickingMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::MagicMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::MapMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::MessageBoxMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::ModManagerMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::SleepWaitMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::StatsMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::TrainingMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::TutorialMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus[RE::TweenMenu::MENU_NAME.data()] = false;
		settings->slowMotionMenus["CustomMenu"] = false;

		//CombatAlertOverlay
		// Shows a blinking red overlay when your character is in combat. Especially useful in full screen menus. You can enable or disable it individually for each menu.
		settings->enableCombatAlertOverlayMenu = false;
		settings->overlayMenus[RE::BarterMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::BookMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::ContainerMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::DialogueMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::FavoritesMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::GiftMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::InventoryMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::JournalMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::LevelUpMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::LockpickingMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::MagicMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::MapMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::MessageBoxMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::ModManagerMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::SleepWaitMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::StatsMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::TrainingMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::TutorialMenu::MENU_NAME.data()] = false;
		settings->overlayMenus[RE::TweenMenu::MENU_NAME.data()] = false;
		settings->overlayMenus["CustomMenu"] = false;

		//HUD
		settings->disableHUDModifications = false;  // If set to true, SkyrimSouls will not attempt to modify the location of the sneak meter. Use it if you have a compatibility issue with something
		settings->sneakMeterPosX = 24.0f;           // The position where the sneak meter will appear on the screen when a menu is open. It's necessary as some menu elements would hide it otherwise (eg. lockpicking).\n# The values are a bit arbitrary so just try different values until you find what suits you
		settings->sneakMeterPosY = 120.0f;

		//Blur
		settings->disableBlur = false;  // If this is enabled, background blur will be disabled in menus

		//Saving
		settings->saveDelayMS = 1000;  // Additional delay (in milliseconds) when saving from the Journal Menu, to make sure that saving happens form a paused state.\n# Larger values will increase the time saving takes

		//Messages
		settings->hideEngineFixesWarning = true;  // Disables the warning message on startup if Engine Fixes is not detected
	}
}
