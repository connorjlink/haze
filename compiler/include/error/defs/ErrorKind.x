#ifndef HAZE_ERRORKIND_H
#define HAZE_ERRORKIND_H

// Haze ErrorKind.x
// (c) Connor J. Link. All Rights Reserved.

#define ERROR_KINDS(X) \
	X(OUTPUT, output) \
	X(INFORMATION, information) \
	X(WARNING, warning) \
	X(ERROR, error) \
	X(UNCORRECTABLE, uncorrectable internal error)

#endif
