# Master Makefile for Tests

# *****************************************************

# SUBDIRS_TO_BUILD := $(wildcard test*/.)							# Used to build the tests
SUBDIRS_TO_BUILD := test2Dvs3D \
			test3DRotation \
			testAlphaRectangle \
			testArrows \
 			testAura \
			testBackground \
			testCircle \
 			testColorWheel \
			testConcavePolygon \
			testCone \
			testConvexPolygon \
			testCube \
			testCuboid \
			testCylinder \
			testDiorama \
			testEllipse \
			testEllipsoid \
 			testGetPixels \
 			testGradientWheel \
 			testGraydient \
 			testGreyscale \
 			testHighData \
			testImage \
 			testInverter \
 			testLineChain \
 			testLineFan \
			testLines \
 			testPixels \
			testPrism \
			testProcedural \
			testPyramid \
			testRectangle \
			testRegularPolygon \
			testSphere \
			testSquare \
			testStar \
			testText \
			testTransparency \
			testTriangle \
			testTriangleStrip \
#			test_specs \
# 			testBlurImage \
# 			testCalcPi \
# 			testCosineIntegral \
#			testDice \
# 			testFunction \
# 			testImageCart \
# 			testMouse \
# 			testProgressBar \
# 			testProjectiles \
# 			testScreenshot \
# 			testSpectrogram \
# 			testSpectrum \
# 			testTextCart \
# 			testTextTwo \
# 			testUnits \

SUBDIRS_TO_CLEAN := $(subst test,..., $(SUBDIRS_TO_BUILD))	# Used to clean the tests


all: $(SUBDIRS_TO_BUILD)

$(SUBDIRS_TO_BUILD):
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++ Generating Binaries for$(subst /., , $@) +++++++++++++++++"
	@tput sgr0;
	@echo ""
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS_TO_BUILD) clean $(SUBDIRS_TO_CLEAN)

clean: $(SUBDIRS_TO_CLEAN)

$(SUBDIRS_TO_CLEAN):
	cd $(subst ...,test,$@) && $(MAKE) clean