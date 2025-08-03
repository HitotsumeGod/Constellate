CC=gcc
SRC=src/main
DEPS=src/include
BUILD=build

$(BUILD)/pilot: $(SRC)/*.c $(DEPS)/*.h $(BUILD)
	$(CC) -o $@ $(SRC)/*.c -I $(DEPS)
$(BUILD):
	if ! [ -d $@ ]; then	\
		mkdir $@;	\
	fi
clean:
	rm -rf $(BUILD)
