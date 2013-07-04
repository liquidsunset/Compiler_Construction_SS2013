default: phoenix.dvi

%.dvi %.auc %.idx: %.tex
	latex $<
	while grep 'Rerun to get' $*.log; do latex $< ; done
	-killall -USR1 -r xdvi || true

clean:
	rm phoenix.aux
	rm phoenix.dvi
	rm phoenix.log
