#include "Menus/JournalMenuEx.h"

namespace SkyrimSoulsRE
{
	std::uint32_t GamepadMaskToKeycode(std::uint32_t a_keyMask)
	{
		using Key = RE::BSWin32GamepadDevice::Key;

		switch (a_keyMask)
		{
		case Key::kUp:
			return 266;
		case Key::kDown:
			return 267;
		case Key::kLeft:
			return 268;
		case Key::kRight:
			return 269;
		case Key::kStart:
			return 270;
		case Key::kBack:
			return 271;
		case Key::kLeftThumb:
			return 272;
		case Key::kRightThumb:
			return 273;
		case Key::kLeftShoulder:
			return 274;
		case Key::kRightShoulder:
			return 275;
		case Key::kA:
			return 276;
		case Key::kB:
			return 277;
		case Key::kX:
			return 278;
		case Key::kY:
			return 279;
		case Key::kLeftTrigger:
			return 280;
		case Key::kRightTrigger:
			return 281;
		default:
			return 282;  // Invalid
		}
	}

	void JournalMenuEx::AdvanceMovie_Hook(float a_interval, std::uint32_t a_currentTime)
	{
		HUDMenuEx* hudMenu = static_cast<HUDMenuEx*>(RE::UI::GetSingleton()->GetMenu(RE::HUDMenu::MENU_NAME).get());
		if (hudMenu)
		{
			hudMenu->UpdateHUD();
		}
		this->UpdateClock();
		return _AdvanceMovie(this, a_interval, a_currentTime);
	}

	RE::UI_MESSAGE_RESULTS JournalMenuEx::ProcessMessage_Hook(RE::UIMessage& a_message)
	{
		if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
		{
			RE::BSUIScaleformData* data = static_cast<RE::BSUIScaleformData*>(a_message.data);

			//Block all input when saving, so the menu can't get closed, but let the cursor move around so users don't freak out
			if (JournalMenuEx::isSaving && data->scaleformEvent->type != RE::GFxEvent::EventType::kMouseMove)
			{
				return RE::UI_MESSAGE_RESULTS::kIgnore;
			}
		}

		return _ProcessMessage(this, a_message);
	}

	void JournalMenuEx::UpdateClock()
	{
		char timeDateString[200];
		RE::Calendar::GetSingleton()->GetTimeDateString(timeDateString, 200, true);

		RE::GFxValue dateText;
		this->uiMovie->GetVariable(&dateText, "_root.QuestJournalFader.Menu_mc.BottomBar_mc.DateText");
		if (dateText.GetType() != RE::GFxValue::ValueType::kUndefined)
		{
			RE::GFxValue newDate(timeDateString);
			dateText.SetMember("htmlText", newDate);
		}
	}

	RE::BSEventNotifyControl JournalMenuEx::RemapHandlerEx::ProcessEvent_Hook(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent**>* a_eventSource)
	{
		if (!a_event || !(*a_event) || (*a_event)->eventType != RE::INPUT_EVENT_TYPE::kButton)
		{
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::ButtonEvent* evn = (*a_event)->AsButtonEvent();

		if (evn->value == 0 || evn->heldDownSecs != 0.0)
		{
			return RE::BSEventNotifyControl::kContinue;
		}

		// Remove event sink so we don't receive more inputs
		RE::BSInputDeviceManager* idm = RE::BSInputDeviceManager::GetSingleton();
		idm->RemoveEventSink(this);

		// Create manual copy of buttonEvent as it is unconstructible
		// This is necessary since the original will be deleted by the time the lambda is executed
		const RE::ButtonEvent* buttonEvent = (*a_event)->AsButtonEvent();

		std::allocator<RE::ButtonEvent> alloc;
		RE::ButtonEvent* buttonEventCopy = alloc.allocate(1);

		buttonEventCopy->device = buttonEvent->device.get();
		buttonEventCopy->eventType = buttonEvent->eventType.get();
		buttonEventCopy->next = nullptr;
		//buttonEventCopy->userEvent = buttonEvent->userEvent; // - not really needed and causes problems when used
		buttonEventCopy->idCode = buttonEvent->idCode;
		buttonEventCopy->value = buttonEvent->value;
		buttonEventCopy->heldDownSecs = buttonEvent->heldDownSecs;

		auto task = [this, buttonEventCopy, alloc]() mutable {
			RE::InputEvent* inputEvent = static_cast<RE::InputEvent*>(buttonEventCopy);
			_ProcessEvent(this, &inputEvent, nullptr);
			alloc.deallocate(buttonEventCopy, 1);
		};

		UnpausedTaskQueue* queue = UnpausedTaskQueue::GetSingleton();
		queue->AddTask(task);

		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl JournalMenuEx::MCMRemapHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
	{
		if (!a_event || !(*a_event) || (*a_event)->eventType != RE::INPUT_EVENT_TYPE::kButton)
		{
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::ButtonEvent* evn = (*a_event)->AsButtonEvent();

		if (!evn->IsDown())
		{
			return RE::BSEventNotifyControl::kContinue;
		}

		a_eventSource->RemoveEventSink(this);

		RE::INPUT_DEVICE deviceType = evn->device.get();
		std::uint32_t keyMask = evn->idCode;
		std::int32_t keyCode;

		switch (deviceType)
		{
		case RE::INPUT_DEVICE::kMouse:
			keyCode = keyMask + 256;
			break;
		case RE::INPUT_DEVICE::kGamepad:
			keyCode = GamepadMaskToKeycode(keyMask);
			break;
		default:
			keyCode = keyMask;
		}

		// Valid scan code?
		if (keyCode >= 282)
		{
			keyCode = -1;
		}

		RE::GFxValue scope = this->scope;

		auto task = [keyCode, scope]() mutable {
			RE::GFxValue arg;
			arg.SetNumber(keyCode);
			scope.Invoke("EndRemapMode", nullptr, &arg, 1);
		};

		UnpausedTaskQueue* queue = UnpausedTaskQueue::GetSingleton();
		queue->AddTask(task);

		RE::MenuControls::GetSingleton()->remapMode = false;
		RE::PlayerControls::GetSingleton()->data.remapMode = false;

		return RE::BSEventNotifyControl::kContinue;
	}

	void JournalMenuEx::MCMRemapHandler::Call(RE::GFxFunctionHandler::Params& a_args)
	{
		scope = a_args.args[0];

		RE::BSInputDeviceManager::GetSingleton()->AddEventSink(this);
		RE::MenuControls::GetSingleton()->remapMode = true;
		RE::PlayerControls::GetSingleton()->data.remapMode = true;
	}

	void JournalMenuEx::SaveGameHandler::Call(Params& params)
	{
		RE::UI* ui = RE::UI::GetSingleton();

		RE::JournalMenu* menu = static_cast<RE::JournalMenu*>(ui->GetMenu(RE::JournalMenu::MENU_NAME).get());
		if (!menu)
		{
			SKSE::log::error("Journal Menu instance not found! Aborting save.");
			RE::DebugNotification("SAVE FAILED - Journal Menu instance not found!");
			return;
		}

		RE::GFxValue iSaveDelayTimerID;
		menu->uiMovie->GetVariable(&iSaveDelayTimerID, "_root.QuestJournalFader.Menu_mc.SystemFader.Page_mc.iSaveDelayTimerID");

		if (iSaveDelayTimerID.GetType() == RE::GFxValue::ValueType::kUndefined)
		{
			SKSE::log::error("Unable to get save delay timer ID. Attempting to ignore it.");
		}
		else
		{
			RE::GFxValue result;
			bool success = menu->uiMovie->Invoke("clearInterval", &result, &iSaveDelayTimerID, 1);  // Not sure if this actually does something
			assert(success);
		}

		// This function is normally supposed to close the menu, and it can get called multiple times. Make sure we only save once.
		if (!isSaving)
		{
			isSaving = true;
			RE::GFxValue selectedIndex;
			menu->uiMovie->GetVariable(&selectedIndex, "_root.QuestJournalFader.Menu_mc.SystemFader.Page_mc.SaveLoadListHolder.selectedIndex");

			if (selectedIndex.GetType() == RE::GFxValue::ValueType::kUndefined)
			{
				SKSE::log::critical("Unable to get selected index of selected save game. Aborting save and forcing Journal Menu to close.");
				RE::DebugNotification("SAVE FAILED - report issue to Skyrim Souls RE author!");
				RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
				uiMessageQueue->AddMessage(RE::JournalMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kForceHide, nullptr);
				isSaving = false;
				RE::PlaySound("UIMenuCancel");
				return;
			}

			if (!menu->PausesGame())
			{
				menu->menuFlags.set(RE::IMenu::Flag::kPausesGame);
				ui->numPausesGame++;
			}

			//Create save screenshot
			reinterpret_cast<void (*)()>(Offsets::Misc::CreateSaveScreenshot.address())();

			double selectedIndexValue = selectedIndex.GetNumber();

			auto task = [selectedIndexValue]() {
				RE::UI* ui = RE::UI::GetSingleton();
				RE::BSSpinLockGuard lk(ui->processMessagesLock);

				if (ui->IsMenuOpen(RE::JournalMenu::MENU_NAME))
				{
					RE::JournalMenu* menu = static_cast<RE::JournalMenu*>(ui->GetMenu(RE::JournalMenu::MENU_NAME).get());
					assert(menu);

					RE::GFxValue selectedIndex(selectedIndexValue);

					RE::FxDelegateArgs args(0, menu, menu->uiMovie.get(), &selectedIndex, 1);
					menu->fxDelegate->callbacks.GetAlt("SaveGame")->callback(args);

					RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();
					uiMessageQueue->AddMessage(RE::JournalMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kForceHide, nullptr);
					isSaving = false;
					RE::PlaySound("UIMenuCancel");
				}
			};

			UnpausedTaskQueue* queue = UnpausedTaskQueue::GetSingleton();
			queue->AddDelayedTask(task, std::chrono::milliseconds(Settings::GetSingleton()->saveDelayMS));
		}
	}

	RE::IMenu* JournalMenuEx::Creator()
	{
		RE::JournalMenu* menu = static_cast<RE::JournalMenu*>(CreateMenu(RE::JournalMenu::MENU_NAME.data()));
		JournalMenuEx::mcmRemapHandler = MCMRemapHandler{};
		JournalMenuEx::saveGameHandler = SaveGameHandler{};

		//fix for remapping from MCM menu
		RE::GFxValue globals, skse;

		bool result = menu->uiMovie->GetVariable(&globals, "_global");
		if (result)
		{
			result = globals.GetMember("skse", &skse);
			if (result)
			{
				RE::GFxFunctionHandler* fn = &mcmRemapHandler;
				RE::GFxValue fnValue;
				menu->uiMovie.get()->CreateFunction(&fnValue, fn);
				skse.SetMember("StartRemapMode", fnValue);
			}
		}

		// Fix for game saves
		RE::GFxValue obj;
		bool success = menu->uiMovie->GetVariable(&obj, "_root.QuestJournalFader.Menu_mc.SystemFader.Page_mc");
		assert(success);

		RE::GFxValue func;
		menu->uiMovie->CreateFunction(&func, &saveGameHandler);
		obj.SetMember("DoSaveGame", func);

		return menu;
	}

	void JournalMenuEx::InstallHook()
	{
		//Hook ProcessMessage
		REL::Relocation<std::uintptr_t> vTable(RE::VTABLE_JournalMenu[0]);
		_ProcessMessage = vTable.write_vfunc(0x4, &JournalMenuEx::ProcessMessage_Hook);

		//Hook AdvanceMovie
		_AdvanceMovie = vTable.write_vfunc(0x5, &JournalMenuEx::AdvanceMovie_Hook);

		REL::Relocation<std::uintptr_t> vTable_remapHandler(RE::VTABLE_JournalInputMapping__RemapHandler[0]);
		RemapHandlerEx::_ProcessEvent = vTable_remapHandler.write_vfunc(0x1, &JournalMenuEx::RemapHandlerEx::ProcessEvent_Hook);
	}
};
