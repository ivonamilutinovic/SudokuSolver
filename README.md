# RS021-sudoku-solver
Program rešava sudoku koji korisnik zadaje slikom u .bmp formatu.

Sastoji se od sledećih delova:

    • GUI-a koji prihvata sliku sudokua i koji vraća rešenje sudokua ili ispisuje da rešenje ne postoji
    • Dela koji sliku pretvara u crno-belu kako bi se jasno uočili brojevi (funkcija readImage)
    • Dela koji prepoznaje tabelu sudokua i brojeve sa slike i koji upisuje prepoznate brojeve u odgovarajuća polja
    • Dela koji rešava sudoku i na izlaz šalje rešenje ili poruku da rešenje ne postoji (sudoku.cpp)
    
Završile smo deo koji rešava sudoku i deo koji sliku pretvara u crno-belu.

Algoritam za Ojlerov broj je preuzet sa sledećeg linka: https://pdfs.semanticscholar.org/64d9/63ee2e3b47b135cd39cf576069f034b0d4d5.pdf .
