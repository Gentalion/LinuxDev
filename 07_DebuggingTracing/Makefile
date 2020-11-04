GENERATE = rm_wrapper.so
TEST_DIR = test
CURR_DIR = $(shell pwd)
MAKEFLAGS += --silent

wrapper: rm_wrapper.c
	cc -Wall -shared -fPIC rm_wrapper.c -ldl -o rm_wrapper.so

test_without_wrapper:
	mkdir $(TEST_DIR)

	touch $(TEST_DIR)/testFilename
	touch $(TEST_DIR)/testFileFIXname

	rm $(TEST_DIR)/testFilename
	rm $(TEST_DIR)/testFileFIXname

	@if [ ! -f $(TEST_DIR)/testFilename ]; then echo "OK"; else echo "FAIL"; fi;
	@if [ ! -f $(TEST_DIR)/testFileFIXname ]; then echo "OK"; else echo "FAIL"; fi;

	rm -rf $(TEST_DIR)

test_with_wrapper:
test_with_wrapper:
	mkdir $(TEST_DIR)
	
	touch $(TEST_DIR)/testFile
	touch $(TEST_DIR)/testFileFIXname

	LD_PRELOAD=$(CURR_DIR)/rm_wrapper.so rm $(TEST_DIR)/testFile
	LD_PRELOAD=$(CURR_DIR)/rm_wrapper.so rm $(TEST_DIR)/testFileFIXname

	@if [ ! -f $(TEST_DIR)/testFilename ]; then echo "OK"; else echo "FAIL"; fi;
	@if [ -f $(TEST_DIR)/testFileFIXname ]; then echo "OK"; else echo "FAIL"; fi;

	rm -rf $(TEST_DIR)

test: test_without_wrapper wrapper test_with_wrapper clean_generate
	echo "Test passed"


clean_generate:
	rm -rf $(GENERATE)

clean:
	rm -rf $(GENERATE) $(TEST_DIR)
