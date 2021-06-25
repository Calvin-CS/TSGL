/*
 * tsgl.h
 */

#ifndef TSGL_H_
#define TSGL_H_

#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE //To avoid issues with fopen, http://stackoverflow.com/questions/14386/fopen-deprecated-warning
#endif

#include <TSGL/Canvas.h>
#include <TSGL/CartesianCanvas.h>
#include <TSGL/Color.h>
#include <TSGL/Error.h>
#include <TSGL/IntegralViewer.h>
#include <TSGL/Keynums.h>
#include <TSGL/Spectrogram.h>
#include <TSGL/Timer.h>
#include <TSGL/Util.h>
#include <TSGL/VisualTaskQueue.h>

#endif /* TSGL_H_ */
