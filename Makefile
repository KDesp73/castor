.DEFAULT_GOAL := help

include common.mk

SUBDIRS_ENGINE := engine demo
SUBDIRS_TOOLS  := builder launcher
SUBDIRS_ALL    := $(SUBDIRS_ENGINE) $(SUBDIRS_TOOLS)

ifeq ($(BUILD_TOOLS),0)
  SUBDIRS := $(SUBDIRS_ENGINE)
else
  SUBDIRS := $(SUBDIRS_ALL)
endif

.PHONY: deps
deps: ## Build Raylib into deps/lib (required for builder and launcher)
	@chmod +x scripts/build_raylib.sh
	@./scripts/build_raylib.sh

.PHONY: all
all: deps ## Build every project
	@$(MAKE) -C engine static
	@for dir in $(SUBDIRS); do \
		echo "Building $$dir..."; \
		$(MAKE) -C $$dir all || exit 1; \
	done

.PHONY: test
test: ## Run engine unit tests
	@$(MAKE) -C engine test

.PHONY: clean
clean: ## Clean every project
	@for dir in $(SUBDIRS_ALL); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean || exit 1; \
	done

.PHONY: compile_commands.json
compile_commands.json: ## Generate compile_commands.json for every project
	@for dir in $(SUBDIRS_ALL); do \
		echo "Generating compile_commands.json for $$dir..."; \
		rm -f "$$dir/compile_commands.json"; \
		$(MAKE) -C "$$dir" compile_commands.json || exit 1; \
	done

.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
