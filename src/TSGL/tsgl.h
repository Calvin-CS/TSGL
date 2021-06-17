/*
 * tsgl.h
 */

#ifndef TSGL_H_
#define TSGL_H_

#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE //To avoid issues with fopen, http://stackoverflow.com/questions/14386/fopen-deprecated-warning
#endif

#include "Canvas.h"
#include "CartesianCanvas.h"
#include "Color.h"
#include "Error.h"
#include "IntegralViewer.h"
#include "Keynums.h"
#include "Spectrogram.h"
#include "Timer.h"
#include "Util.h"
#include "VisualTaskQueue.h"

#endif /* TSGL_H_ */
