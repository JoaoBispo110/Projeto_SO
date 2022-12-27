Program_List:= Proc1.bin Execprocd.bin Execproc.bin

all: $(Program_List)

$(Program_List): %.bin : %
	@cd $^ && $(MAKE) 