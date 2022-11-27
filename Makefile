PROJECT_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# ------------------------------------------------------------------------------

ifndef outdir
OUT_DIR := build
else
OUT_DIR := $(outdir)
endif

ifndef config
CONFIG := debug
else ifneq "$(filter-out debug release all,$(config))" ""
$(error the 'config' option is not valid)
else ifneq "$(filter all,$(config))" ""
CONFIG := debug release
else
CONFIG := $(config)
endif

# ------------------------------------------------------------------------------

FILES :=

# ------------------------------------------------------------------------------

# $(1): build directory.
# $(2): rule.
define rx_forward_rule_impl =
$(MAKE) -C $(1) -s $(2)
endef

# Forward a rule to the generated Makefiles.
# $(1): rule.
define rx_forward_rule =
$(foreach _x,$(BUILD_DIRS), $(call \
	rx_forward_rule_impl,$(_x),$(1)))
endef

# ------------------------------------------------------------------------------

# Create a Makefile rule.
# $(1): configuration.
define rx_create_makefile =
$(OUT_DIR)/$(1)/Makefile:
	@ mkdir -p $(OUT_DIR)/$(1)
	@ cd $(OUT_DIR)/$(1) && cmake \
		-DCMAKE_BUILD_TYPE=$(1) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		$(PROJECT_DIR)

BUILD_DIRS += $(OUT_DIR)/$(1)
MAKE_FILES += $(OUT_DIR)/$(1)/Makefile
endef

$(foreach _config,$(CONFIG),$(eval $(call \
	rx_create_makefile,$(_config))))

# ------------------------------------------------------------------------------

FILES += $(wildcard include/*.h)

# ------------------------------------------------------------------------------

# Create the rule to build a test.
# $(1): path.
define rx_create_test_rules =
_rule := test-$(subst /,-,$(patsubst \
	tests/%.c,%,$(patsubst tests/%.cpp,%,$(1))))


$(_rule): $(MAKE_FILES)
	$(call rx_forward_rule,$(_rule))

FILES += $(1)

.PHONY: $(_rule)
endef

# $(1): parent directory.
define rx_find_tests_impl =
$(foreach _x,$(wildcard $(1:=/*)),$(call \
	rx_find_tests_impl,$(_x)) $(filter %.c %.cpp,$(_x)))
endef

# Recursively find all the tests.
define rx_find_tests =
$(call rx_find_tests_impl,tests)
endef

# Create the rules for each test.
$(foreach _x,$(rx_find_tests),$(eval $(call \
    rx_create_test_rules,$(_x))))

# ------------------------------------------------------------------------------

tests: $(MAKE_FILES)
	@ $(call rx_forward_rule,tests)

test: tests
	@ $(call rx_forward_rule,test)

.PHONY: tests test

# ------------------------------------------------------------------------------

CLANG_VERSION := $(shell \
	clang --version \
	| grep version \
	| sed 's/^.*version \([0-9]*\.[0-9]*\.[0-9]*\).*$$/\1/')
CLANG_DIR := $(shell dirname $(shell which clang))
CLANG_INCLUDE_DIR := $(CLANG_DIR)/../lib/clang/$(CLANG_VERSION)/include

# Run the formatter on a file.
# $(1): file.
define rx_format =
clang-format -style=file $(1) | diff --color -u $(1) -;
endef

format:
	@ $(foreach _file,$(FILES),$(call \
		rx_format,$(_file)))

tidy: $(MAKE_FILES)
	@ clang-tidy $(FILES) \
		-p $(firstword $(BUILD_DIRS))/compile_commands.json \
		-- -I$(CLANG_INCLUDE_DIR) -Iinclude

.PHONY: format tidy

# ------------------------------------------------------------------------------

coverity: clean
	@ cov-build --dir cov-int make tests -j 4
	@ tar czvf rexo.tgz cov-int

.PHONY: coverity

# ------------------------------------------------------------------------------

install: $(MAKE_FILES)
	@ $(call rx_forward_rule,install)

.PHONY: install

# ------------------------------------------------------------------------------

clean:
	@ rm -rf $(OUT_DIR) cov-int rexo.tgz

.PHONY: clean

# ------------------------------------------------------------------------------

all:

.PHONY: all

.DEFAULT_GOAL := all
