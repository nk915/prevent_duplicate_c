PROGRAM_NAME = prevent_duplicate
EXEC = $(PROGRAM_NAME)-$(VERSION)
VERSION = 1.0
CC = g++

all: $(EXEC)

$(EXEC):
	$(CC) -o $(EXEC) $(PROGRAM_NAME).cpp -DPROGRAM_NAME=\"${PROGRAM_NAME}\"

clean:
	rm -f $(PROGRAM_NAME)-*
