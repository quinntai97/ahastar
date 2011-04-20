all: article short
article: *.tex references.bib
	pdflatex article
	bibtex article
	pdflatex article
	pdflatex  article
short: references.bib
	pdflatex article_short.tex
	bibtex article_short.tex
	pdflatex article_short.tex
	pdflatex  article_short.tex
clean:
	rm -f *.aux *.bbl *.blg *.log *.dvi *.ps *.pdf 
