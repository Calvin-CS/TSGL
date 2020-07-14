# Master Makefile for Examples

# *****************************************************

SUBDIRS_TO_BUILD := $(wildcard */.)							# Used to build the examples 
SUBDIRS_TO_CLEAN := $(subst /.,..., $(SUBDIRS_TO_BUILD))	# Used to clean the examples

all: $(SUBDIRS_TO_BUILD)

$(SUBDIRS_TO_BUILD):
	@echo ""
	@tput setaf 3;
	@echo "++++++++++++++++ Generating Binaries for$(subst /., , $@)++++++++++++++++"
	@tput sgr0;
	@echo ""
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS_TO_BUILD) clean $(SUBDIRS_TO_CLEAN)

clean: $(SUBDIRS_TO_CLEAN)

$(SUBDIRS_TO_CLEAN):
	cd $(subst ...,/.,$@) && $(MAKE) clean
