#ifndef GWM_ICCCM_H_
#define GWM_ICCCM_H_ 1

// The flags for WM_SIZE_HINTS as defined at:
// http://tronche.com/gui/x/icccm/sec-4.html
enum WM_SIZE_HINTS_FLAGS {
	USPosition = 1,  // User set position
	USSize = 2,      // User set siez
	PPosition = 4,   // Program set position
	PSize = 8,       // Program set size
	PMinSize = 16,   // Program minimum size
	PMaxSize = 32,   // Program maximum size
	PResizeInc = 64, // Program resize increasements
	PAspect = 128,   // Program aspect ratio
	PBaseSize = 256, // Program base size
	PWinGravity = 512// Program window gravity
};

#endif

