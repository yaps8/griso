#subdirs := "src/"
#all_tex := $(wildcard $(addsuffix *.tex,$(subdirs)))
all_tex := $(wildcard src/*.tex)
pdf_from_tex := $(patsubst %.tex,%.pdf,$(all_tex))
TEXCOMP = latexmk -pdf


default: $(pdf_from_tex)

all: default
doc: default

%.pdf: %.tex
	$(TEXCOMP) $< 

clean:
	rm -f *.aux *.bbl *.blg *.log *.nav *.out *.snm *.toc *.idf *.maf *.mtc *.idx *.lof *.glo *.bcf *.mtc* *.flg *.xml *.fls *.dvi *.ilg *.ind *.old *latexmk

