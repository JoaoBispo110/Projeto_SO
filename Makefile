Program_List:= Proc1 Execprocd Execproc

all:
	$(foreach var,$(Program_List), $(MAKE) all -C $(var);)
	