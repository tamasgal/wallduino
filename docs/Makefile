TYP_FILES := $(wildcard *.typ)
PDF_FILES := $(TYP_FILES:.typ=.pdf)

all: $(PDF_FILES)

%.pdf: %.typ
	typst compile $<

clean:
	rm -f $(PDF_FILES)

