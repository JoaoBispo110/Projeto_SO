Program_List:= Proc1 Execprocd Execproc

all:
	@$(foreach var,$(Program_List), $(MAKE) $@ -C $(var);)
	
clean:
	@$(foreach var,$(Program_List), $(MAKE) $@ -C $(var);)