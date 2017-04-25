BIN=myls mycp mycat mysh

.PHONY: all
all: $(BIN)
		
$(BIN): 
	@$(MAKE) -C $@

.PHONY: clean
clean:
	$(foreach DIR, $(BIN), $(MAKE) -C $(DIR) $@;)







