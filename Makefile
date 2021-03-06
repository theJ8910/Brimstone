# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug32
endif
export config

PROJECTS := Brimstone UnitTests

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

Brimstone: 
	@echo "==== Building Brimstone ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Brimstone.make

UnitTests: 
	@echo "==== Building UnitTests ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f UnitTests.make

clean:
	@${MAKE} --no-print-directory -C . -f Brimstone.make clean
	@${MAKE} --no-print-directory -C . -f UnitTests.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug32"
	@echo "   release32"
	@echo "   debug64"
	@echo "   release64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Brimstone"
	@echo "   UnitTests"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
