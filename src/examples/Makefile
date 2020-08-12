# Master Makefile for Examples

# *****************************************************

# SUBDIRS_TO_BUILD := $(wildcard test*/.)							# Used to build the examples
SUBDIRS_TO_BUILD := ArrayBubbleSort/. \
			ArrayShakerSort/. \
			Ballroom/. \
			Clock/. \
 			Conway/. \
			CubeRun/. \
			DiningPhilosophers/. \
			DiningPhilosophers3D/. \
 			Fireworks/. \
 			ForestFire/. \
 			Langton/. \
			MergeSort/. \
			NewtonPendulum/. \
			Pong/. \
 			ProducerConsumer/. \
 			ReaderWriter/. \
			SeaUrchin/. \
			ShakerSort/. \
			SolarSystem/. \
			ThreadedArrayAddition/. \
			ThreadedArrayBubbleSort/. \
			ThreadedArrayOperations/. \
			ThreadedSolarSystem/. \
# 			Mandelbrot/. \
# 			ParallelPandemic/. \


SUBDIRS_TO_CLEAN := $(subst /.,..., $(SUBDIRS_TO_BUILD))	# Used to clean the examples


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
	cd $(subst ...,,$@) && $(MAKE) clean