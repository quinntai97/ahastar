aticle.ps: *.tex ref.bib
	latex article
#	bibtex article
	latex article
	latex  article
	dvips -t letter -Ppdf -G0 -o article.ps article.dvi
	ps2pdf14 -dPDFSETTINGS=/prepress article.ps
clean:
	rm -f *.aux *.bbl *.blg *.log article.dvi article.ps article.pdf
