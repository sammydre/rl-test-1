.PHONY: all clean run

PYTHON="/c/Coding/Python27/python.exe"
SCONS="/c/Coding/Python27/Scripts/scons.py"

all:
	@$(PYTHON) $(SCONS)

clean:
	@$(PYTHON) $(SCONS) -q -c

run: all
	cd build; ./rl.exe
