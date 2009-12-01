@FORFILES -m *.txt -c "CMD /C @python docutils\tools\rst2html.py @FILE > website\@FNAME.html"
@echo WebSite generated Successfully
@pause