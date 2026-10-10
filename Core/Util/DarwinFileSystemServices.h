//
//  DarwinFileSystemServices.h
//  SuikaEmulator
//
//  Created by Serena on 20/01/2023.
//

#pragma once

#include "suikaemulator_config.h"
#include "Common/File/Path.h"
#include "Common/System/Request.h"

#if SuikaEmulator_PLATFORM(IOS) || SuikaEmulator_PLATFORM(MAC)

#define PreferredMemoryStickUserDefaultsKey "UserPreferredMemoryStickDirectoryPath"

typedef std::function<void (bool, Path)> DarwinDirectoryPanelCallback;

/// A Class providing help functions to work with the FileSystem
/// on Darwin platforms.
class DarwinFileSystemServices {
public:
	/// Present a panel to choose a file or directory.
	static void presentDirectoryPanel(
		DarwinDirectoryPanelCallback panelCallback,
		bool allowFiles = false,
		bool allowDirectories = true,
		BrowseFileType fileType = BrowseFileType::ANY);

	static Path appropriateMemoryStickDirectoryToUse();
	static void setUserPreferredMemoryStickDirectory(Path);
	static Path defaultMemoryStickPath();
	static void ClearDelegate();

	static Path reauthorizeBookmarkByPath(const Path &pathStr);
	static void stopAccessingPath(const Path &pathStr);
	static void terminate();
private:
#if SuikaEmulator_PLATFORM(IOS)
	// iOS only, needed for UIDocumentPickerViewController
	static void *__pickerDelegate;
#endif // SuikaEmulator_PLATFORM(IOS)
};

void RestartMacApp();

#endif
