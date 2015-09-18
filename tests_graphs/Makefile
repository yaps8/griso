all_dot := $(wildcard */pattern_*.dot */test.dot)
orig_dot := $(filter-out $(wildcard */*_an.dot), $(all_dot))
edg_from_dot := $(patsubst %.dot,%.edg,$(orig_dot))
annotateddot := $(patsubst %.dot,%_an.dot,$(orig_dot))
D2EDG = ../dot2edg
E2DOT = ../edg2dot

default: $(edg_from_dot) $(annotateddot)

all: default
edg: default

%.edg: %.dot $(D2EDG)
	$(D2EDG) $< $@

%_an.dot: %.edg $(EDG2DOT)
	$(E2DOT) $< $@

clean:
	rm -f */*.edg */*_an.dot

