#pragma once

#include <saltysd/SaltySD_core.h>
#include <saltysd/SaltySD_dynamic.h>
#include <saltysd/SaltySD_ipc.h>
#include <switch_min.h>

extern "C" {
extern u32 __start__;

static char g_heap[0x10000];

void __libnx_init (void* ctx, Handle main_thread, void* saved_lr);
void __attribute__ ((weak)) NORETURN __libnx_exit (int rc);
void __attribute__ ((weak)) __libnx_exception_handler (ThreadExceptionDump* ctx);
void __nx_exit (int, void*);
void __libc_fini_array (void);
void __libc_init_array (void);
// Put linkable SDK functions here
}

u32 __nx_applet_type = AppletType_None;
Handle orig_main_thread;
void* orig_ctx;
void* orig_saved_lr;

void __libnx_init (void* ctx, Handle main_thread, void* saved_lr) {
	extern char* fake_heap_start;
	extern char* fake_heap_end;

	fake_heap_start = &g_heap[0];
	fake_heap_end   = &g_heap[sizeof g_heap];

	orig_ctx         = ctx;
	orig_main_thread = main_thread;
	orig_saved_lr    = saved_lr;

	// Call constructors.
	// void __libc_init_array(void);
	__libc_init_array ();
}

void __attribute__ ((weak)) NORETURN __libnx_exit (int rc) {
	// Call destructors.
	// void __libc_fini_array(void);
	__libc_fini_array ();

	SaltySD_printf ("SaltySD Plugin: jumping to %p\n", orig_saved_lr);

	__nx_exit (0, orig_saved_lr);
	while (true)
		;
}

void __attribute__ ((weak)) __libnx_exception_handler (ThreadExceptionDump* ctx) {
	// I dunno
}