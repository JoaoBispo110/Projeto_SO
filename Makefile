Program_List:= Proc1 Execprocd Execproc Cancela_proc Termina_execprocd

bin: 
	mkdir bin

all: bin
	@$(foreach var,$(Program_List), $(MAKE) $@ -C $(var);)

clean:
	@$(foreach var,$(Program_List), $(MAKE) $@ -C $(var);)