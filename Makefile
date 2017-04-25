BIN=myls mycp mycat mysh
CFLAGS=

.PHONY: all $(BIN) clean
all: $(BIN)
		
$(BIN): 
	@$(MAKE) -C $@

clean:
	$(foreach DIR, $(BIN), $(MAKE) -C $(DIR) $@;)







