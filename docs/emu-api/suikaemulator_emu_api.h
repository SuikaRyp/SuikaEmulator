// SuikaEmulator emulator API - header-only C wrapper.
//
// SuikaEmulator exposes a handful of emulator-only tricks to homebrew through the
// "emulator:"/"kemulator:" pseudo-devices of sceIoDevctl. None of this exists
// on real hardware - always check suikaemulator_is_emulator() before depending on
// anything else here, and keep a working fallback path for real PSPs.
//
// C API docs: https://www.suikaemulator.org/docs/development/suikaemulator-internals/emu-api/
//
// Raw sceIoDevctl protocol this wraps (cmd numbers, layouts, quirks): see protocol.md
// in this same directory, or Core/HLE/sceIo.cpp (search for "emulator:") in the main
// SuikaEmulator source tree, which is the source of truth.
//
// Usage: just #include this file in a PSPSDK-based homebrew project. It's
// entirely static inline functions over sceIoDevctl, so there's nothing to
// link.

#pragma once

#include <pspiofilemgr.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

enum SuikaEmulatorEmulatorDevctlCmd {
	SuikaEmulator_DEVCTL__GET_HAS_DISPLAY     = 1,
	SuikaEmulator_DEVCTL__SEND_OUTPUT         = 2,
	SuikaEmulator_DEVCTL__IS_EMULATOR         = 3,
	SuikaEmulator_DEVCTL__VERIFY_STATE        = 4,

	SuikaEmulator_DEVCTL__EMIT_SCREENSHOT     = 0x20,

	SuikaEmulator_DEVCTL__TOGGLE_FASTFORWARD  = 0x30,
	SuikaEmulator_DEVCTL__GET_ASPECT_RATIO    = 0x31,
	SuikaEmulator_DEVCTL__GET_SCALE           = 0x32,
	SuikaEmulator_DEVCTL__GET_AXIS            = 0x33,
	SuikaEmulator_DEVCTL__GET_VKEY            = 0x34,
};

// Either name works - SuikaEmulator currently treats them identically.
#define SuikaEmulator_EMULATOR_DEVICE "emulator:"

// Returns non-zero if running under SuikaEmulator. Call this before trusting any
// of the other functions below - on real hardware, or on an emulator that
// doesn't implement this API, sceIoDevctl will simply fail here.
static inline int suikaemulator_is_emulator(void) {
	return sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__IS_EMULATOR, NULL, 0, NULL, 0) == 0;
}

// Returns non-zero if there's an actual display to render to (i.e. not
// running under SuikaEmulatorHeadless).
static inline int suikaemulator_has_display(void) {
	unsigned int hasDisplay = 0;
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__GET_HAS_DISPLAY, NULL, 0, &hasDisplay, sizeof(hasDisplay));
	return (int)hasDisplay;
}

// Sends a block of text straight to SuikaEmulator's debug/log output.
static inline void suikaemulator_send_output(const char *data, int len) {
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__SEND_OUTPUT, (void *)data, len, NULL, 0);
}

// Convenience wrapper for suikaemulator_send_output() over a NUL-terminated string.
static inline void suikaemulator_send_output_str(const char *str) {
	suikaemulator_send_output(str, (int)strlen(str));
}

// Asks SuikaEmulator to do an internal savestate round-trip as a consistency check.
// Asynchronous - the pass/fail result is only logged on the SuikaEmulator side, not
// reported back here. Mainly useful when testing the emulator itself.
static inline void suikaemulator_verify_state(void) {
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__VERIFY_STATE, NULL, 0, NULL, 0);
}

// Delivers the current framebuffer through SuikaEmulator's internal debug-screenshot
// hook (used by the pspautotests/frametest infrastructure). This does not
// write a file to the memory stick - it's not a general screenshot feature.
static inline void suikaemulator_emit_screenshot(void) {
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__EMIT_SCREENSHOT, NULL, 0, NULL, 0);
}

// Turns SuikaEmulator's fast-forward mode on or off.
static inline void suikaemulator_toggle_fastforward(int enable) {
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__TOGGLE_FASTFORWARD, enable ? (void *)1 : NULL, 0, NULL, 0);
}

// Current display aspect ratio. Only correct in landscape orientation.
static inline float suikaemulator_get_aspect_ratio(void) {
	float ar = 0.0f;
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__GET_ASPECT_RATIO, NULL, 0, &ar, sizeof(ar));
	return ar;
}

// Current display scale factor. Only correct in landscape orientation.
static inline float suikaemulator_get_scale(void) {
	float scale = 0.0f;
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__GET_SCALE, NULL, 0, &scale, sizeof(scale));
	return scale;
}

// Reads an analog axis value injected by a SuikaEmulator-side input plugin, by
// JOYSTICK_AXIS_* index (see SuikaEmulator's Common/Input/KeyCodes.h). This is NOT
// normal controller input - use sceCtrl* for that. Returns 0 if no plugin
// has set this axis.
static inline float suikaemulator_get_axis(int axisIndex) {
	float value = 0.0f;
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__GET_AXIS, (void *)(long)axisIndex, 0, &value, sizeof(value));
	return value;
}

// Reads whether a virtual key injected by a SuikaEmulator-side input plugin is
// currently pressed, by NKCODE_* value (see SuikaEmulator's Common/Input/KeyCodes.h,
// codes mostly mirror Android's). This is NOT normal controller input - use
// sceCtrl* for that. Returns 0 if no plugin has set this key.
static inline unsigned char suikaemulator_get_vkey(int keyCode) {
	unsigned char value = 0;
	sceIoDevctl(SuikaEmulator_EMULATOR_DEVICE, SuikaEmulator_DEVCTL__GET_VKEY, (void *)(long)keyCode, 0, &value, sizeof(value));
	return value;
}

#ifdef __cplusplus
}  // extern "C"
#endif
