#pragma once

// NOTE: This is only used on iOS, to present the availablility of getting SuikaEmulator Gold through IAP.

#include "suikaemulator_config.h"

#include "UI/BaseScreens.h"
#include "UI/SimpleDialogScreen.h"

class IAPScreen : public UITwoPaneBaseDialogScreen {
public:
	IAPScreen(bool useIAP) : UITwoPaneBaseDialogScreen(Path(), TwoPaneFlags::SettingsToTheRight | TwoPaneFlags::ContentsCanScroll), useIAP_(useIAP) {}
	void CreateSettingsViews(UI::ViewGroup *parent) override;
	void CreateContentViews(UI::ViewGroup *parent) override;
	std::string_view GetTitle() const override;

	const char *tag() const override { return "IAP"; }
private:
	// This screen can also be used to direct to Play Store purchases, for example.
	bool useIAP_ = false;
};

void LaunchPlayStoreOrWebsiteGold();
