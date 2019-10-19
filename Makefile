PROJECT_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# ------------------------------------------------------------------------------

ifndef outdir
OUT_DIR := build
else
OUT_DIR := $(outdir)
endif

ifndef config
CONFIG := release
else ifneq "$(filter-out debug release all,$(config))" ""
$(error the 'config' option is not valid)
else ifneq "$(filter all,$(config))" ""
CONFIG := debug release
else
CONFIG := $(config)
endif

# ------------------------------------------------------------------------------

PROJECT := rexo

# ------------------------------------------------------------------------------

FORMAT_FILES :=
TIDY_FILES :=

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
$$(OUT_DIR)/$(1)/Makefile:
	@ mkdir -p $$(OUT_DIR)/$(1)
	@ cd $$(OUT_DIR)/$(1) && cmake \
		-DCMAKE_BUILD_TYPE=$(1) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		$(PROJECT_DIR)

BUILD_DIRS += $$(OUT_DIR)/$(1)
MAKE_FILES += $$(OUT_DIR)/$(1)/Makefile
endef

$(foreach _config,$(CONFIG),$(eval $(call \
	rx_create_makefile,$(_config))))

# ------------------------------------------------------------------------------

FORMAT_FILES += $(wildcard include/*.h)
TIDY_FILES += $(wildcard include/*.h)

# ------------------------------------------------------------------------------

tests: $(MAKE_FILES)
	@ $(call rx_forward_rule,tests)

.PHONY: tests

FORMAT_FILES += $(wildcard tests/*.[ch])
TIDY_FILES += $(wildcard tests/*.[ch])

run-tests: tests
	@ $(call rx_forward_rule,test)

.PHONY: tests

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
	@ $(foreach _file,$(FORMAT_FILES),$(call \
		rx_format,$(_file)))

tidy: $(MAKE_FILES)
	@ clang-tidy $(TIDY_FILES) \
		-p $(firstword $(BUILD_DIRS))/compile_commands.json \
		-- -I$(CLANG_INCLUDE_DIR) -Iinclude

.PHONY: format tidy

# ------------------------------------------------------------------------------

install: $(MAKE_FILES)
	@ $(call rx_forward_rule,install)

.PHONY: install

# ------------------------------------------------------------------------------

clean:
	@ rm -rf $(OUT_DIR)

.PHONY: clean

# ------------------------------------------------------------------------------

all:

.PHONY: all

.DEFAULT_GOAL := all
