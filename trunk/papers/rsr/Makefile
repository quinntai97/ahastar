all: article short
article: *.tex references.bib
	pdflatex article
	bibtex article
	pdflatex article
	pdflatex  article
	#dvips -t letter -Ppdf -G0 -o article.ps article.dvi
	#ps2pdf14 -dPDFSETTINGS=/prepress article.ps

short:
	pdflatex short
	bibtex short
	pdflatex short
	pdflatex  short
clean:
	rm -f *.aux *.bbl *.blg *.log *.dvi *.ps *.pdf 
