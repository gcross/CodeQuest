groff -mandoc codequest.1 > codequest.ps
gs -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=codequest.pdf codequest.ps
