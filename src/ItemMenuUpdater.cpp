#include "ItemMenuUpdater.h"

namespace SkyrimSoulsRE::ItemMenuUpdater
{
	// These hooks are used to send update events to the item menus when something occurs.
	// By default the game sends no updates for these events, causing the lists to retain old data (and possibly crash because of unloaded 3D models).
	// This is especially problematic when unpaused as external events can add or remove objects at any time.
	// Note: this does not necessarily cover all cases.

	RE::TESObjectREFR* GetTargetReference()
	{
		RE::TESObjectREFR* targetRef = nullptr;

		RE::UI* ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
		{
			RE::ContainerMenu* menu = static_cast<RE::ContainerMenu*>(ui->GetMenu(RE::ContainerMenu::MENU_NAME).get());

			RE::RefHandle handle = menu->GetTargetRefHandle();
			RE::TESObjectREFRPtr refptr = nullptr;
			if (RE::TESObjectREFR::LookupByHandle(handle, refptr))
			{
				targetRef = refptr.get();
			}
		}
		else if (ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
		{
			RE::BarterMenu* menu = static_cast<RE::BarterMenu*>(ui->GetMenu(RE::BarterMenu::MENU_NAME).get());

			RE::RefHandle handle = menu->GetTargetRefHandle();
			RE::TESObjectREFRPtr refptr = nullptr;
			if (RE::TESObjectREFR::LookupByHandle(handle, refptr))
			{
				targetRef = refptr.get();
			}
		}
		else if (ui->IsMenuOpen(RE::GiftMenu::MENU_NAME))
		{
			RE::GiftMenu* menu = static_cast<RE::GiftMenu*>(ui->GetMenu(RE::GiftMenu::MENU_NAME).get());

			RE::RefHandle handle = menu->GetTargetRefHandle();
			RE::TESObjectREFRPtr refptr = nullptr;
			if (RE::TESObjectREFR::LookupByHandle(handle, refptr))
			{
				targetRef = refptr.get();
			}
		}

		return targetRef;
	}

	void RequestItemListUpdate(RE::TESObjectREFR* a_ref, RE::TESForm* a_unk)
	{
		using func_t = decltype(&RequestItemListUpdate);
		REL::Relocation<func_t> func(Offsets::ItemMenuUpdater::RequestItemListUpdate);
		return func(a_ref, a_unk);
	}

	// RemoveItem for TESObjectREFRs
	RE::ObjectRefHandle& RemoveItem_TESObjectREFR(RE::TESObjectREFR* a_this, RE::ObjectRefHandle& a_handle, RE::TESBoundObject* a_item, std::int32_t a_count, RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList, RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc = 0, const RE::NiPoint3* a_rotate = 0)
	{
		using func_t = decltype(&RemoveItem_TESObjectREFR);
		REL::Relocation<func_t> func(Offsets::ItemMenuUpdater::RemoveItem_TESObjectREFR);
		return func(a_this, a_handle, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);
	}

	// RemoveItem for Actors
	RE::ObjectRefHandle& RemoveItem_Actor(RE::Actor* a_this, RE::ObjectRefHandle& a_handle, RE::TESBoundObject* a_item, std::int32_t a_count, RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList, RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc = 0, const RE::NiPoint3* a_rotate = 0)
	{
		using func_t = decltype(&RemoveItem_Actor);
		REL::Relocation<func_t> func(Offsets::ItemMenuUpdater::RemoveItem_Actor);
		return func(a_this, a_handle, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);
	}

	// Update after RemoveItem
	RE::ObjectRefHandle& RemoveItem_Hook(RE::TESObjectREFR* a_this, RE::ObjectRefHandle& a_handle, RE::TESBoundObject* a_item, std::int32_t a_count, RE::ITEM_REMOVE_REASON a_reason, RE::ExtraDataList* a_extraList, RE::TESObjectREFR* a_moveToRef, const RE::NiPoint3* a_dropLoc = 0, const RE::NiPoint3* a_rotate = 0)
	{
		if (a_this->formType == RE::FormType::ActorCharacter)
		{
			RemoveItem_Actor(static_cast<RE::Actor*>(a_this), a_handle, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);
		}
		else
		{
			RemoveItem_TESObjectREFR(a_this, a_handle, a_item, a_count, a_reason, a_extraList, a_moveToRef, a_dropLoc, a_rotate);
		}

		RE::TESObjectREFR* targetRef = GetTargetReference();

		if (a_this == RE::PlayerCharacter::GetSingleton() || a_this == targetRef)
		{
			RequestItemListUpdate(a_this, nullptr);
		}

		return a_handle;
	}

	// Update after RemoveAllItems
	void RemoveAllItems_Hook_1130(RE::BSExtraData* a_unk1, std::uint32_t a_unk2, void* a_unk3, RE::TESObjectREFR* a_containerRef, std::uint64_t a_unk5, std::uint32_t a_unk6, void* a_unk7, void* a_unk8)
	{
		using func_t = decltype(&RemoveAllItems_Hook_1130);
		REL::Relocation<func_t> func(Offsets::ItemMenuUpdater::RemoveAllItems_1130);
		func(a_unk1, a_unk2, a_unk3, a_containerRef, a_unk5, a_unk6, a_unk7, a_unk8);

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::TESObjectREFR* targetRef = GetTargetReference();

		if (a_containerRef == player || a_containerRef == targetRef)
		{
			// Some items might still remain in the list. Updating twice in a row seems to fix it for some reason.
			RequestItemListUpdate(a_containerRef, nullptr);
			RequestItemListUpdate(a_containerRef, nullptr);
		}
	}

	void RemoveAllItems_Hook(RE::BSExtraData* a_unk1, RE::TESObjectREFR* a_containerRef, void* a_unk3, std::uint64_t a_unk4, std::uint32_t a_unk5, void* a_unk6, void* a_unk7)
	{
		using func_t = decltype(&RemoveAllItems_Hook);
		REL::Relocation<func_t> func(Offsets::ItemMenuUpdater::RemoveAllItems);
		func(a_unk1, a_containerRef, a_unk3, a_unk4, a_unk5, a_unk6, a_unk7);

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::TESObjectREFR* targetRef = GetTargetReference();

		if (a_containerRef == player || a_containerRef == targetRef)
		{
			// Some items might still remain in the list. Updating twice in a row seems to fix it for some reason.
			RequestItemListUpdate(a_containerRef, nullptr);
			RequestItemListUpdate(a_containerRef, nullptr);
		}
	}

	void InstallHook()
	{
		if (REL::Module::get().version() >= SKSE::RUNTIME_SSE_1_6_1130)
		{
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook1_1130.address() + 0x3A, (std::uintptr_t)RemoveAllItems_Hook_1130);
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook2_1130.address() + 0x55, (std::uintptr_t)RemoveAllItems_Hook_1130);
		}
		else
		{
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook1.address() + 0x16, (std::uintptr_t)RemoveAllItems_Hook);
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook2.address() + 0x36, (std::uintptr_t)RemoveAllItems_Hook);
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook3.address() + 0xBA, (std::uintptr_t)RemoveAllItems_Hook);
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook4.address() + 0x255, (std::uintptr_t)RemoveAllItems_Hook);
			SKSE::GetTrampoline().write_call<5>(Offsets::ItemMenuUpdater::RemoveAllItems_Hook5.address() + 0x46, (std::uintptr_t)RemoveAllItems_Hook);
		}

		SKSE::GetTrampoline().write_call<6>(Offsets::ItemMenuUpdater::RemoveItem_Hook1.address() + 0x9A, (std::uintptr_t)RemoveItem_Hook);
		SKSE::GetTrampoline().write_call<6>(Offsets::ItemMenuUpdater::RemoveItem_Hook2.address() + 0xDB, (std::uintptr_t)RemoveItem_Hook);
		SKSE::GetTrampoline().write_call<6>(Offsets::ItemMenuUpdater::RemoveItem_Hook3.address() + 0x472, (std::uintptr_t)RemoveItem_Hook);
		SKSE::GetTrampoline().write_call<6>(Offsets::ItemMenuUpdater::RemoveItem_Hook4.address() + 0x26A, (std::uintptr_t)RemoveItem_Hook);
	}
}
