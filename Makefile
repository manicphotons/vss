#──────────────────────────────────────────────────────────────────────────────
#	MAKE			I want to be warned if I reference an undefined
#				variable in a recipe.
#					
#				I want to run the 'all' recipe by default.
#
#				I change the recipe prefix from tab to '> '.
#				This helps avoid errors due to incorrect use
#				of whitespace.
#
#				If a recipe fails, I also delete its target.
#				This forces me to fix the underlying issue.
#
#				Finally, I disable make's archaic suffix rules.

MAKEFLAGS += --warn-undefined-variables
.DEFAULT_GOAL := all
.RECIPEPREFIX = >
.DELETE_ON_ERROR:
.SUFFIXES:


#──────────────────────────────────────────────────────────────────────────────
#	SHELL			I want to use bash by default. Some of my
#				systems have multiple shells, but all of them
#				have bash.
#
#				I use a strict mode for bash that causes
#				failures to propagate through piped commands
#				(-euo).
#
#				Make relies on bash executing commands using
#				the -c option.

SHELL := bash
.SHELLFLAGS := -euo pipefail -c


#──────────────────────────────────────────────────────────────────────────────
#	COMMANDS		These are the targets that should be requested
#				by the user when they run make. They should not
#				have any dependancies (such as other targets).
#				Instead, I would prefer that each command runs
#				regardless of the ctime of any files in the
#				project.

all:
> @echo -e "\nBuilding…"
> gcc -std=c99 -lX11 -L/usr/X11/lib -o vss main.c
> @echo -e "\nRunning…"
> ./vss
> @echo ""
.PHONY: all

clean:
> @echo -e "\nCleaning…"
> rm -f vss
> @echo ""
.PHONY: clean
