#include "sudoku.h"

using namespace std;

Sudoku::Sudoku(){

    for(unsigned i = 0; i < NUMBERS; i++)
        for(unsigned j = 0; j < NUMBERS; j++){
            _elements[i][j] = 0;

            _inRow[i][j]   = false;
            _inColl[i][j]  = false;
            _inBlock[i][j] = false;

            _newNumber[i][j] = false;


        }
    _solutionId = true;
    _validId    = true;
}

Sudoku::Sudoku(const Sudoku& other){

    for(int i = 0; i<NUMBERS;i++)
        for(int j = 0; j<NUMBERS;j++){
            _elements[i][j] = other._elements[i][j];
            _inRow[i][j] = other._inRow[i][j];
            _inColl[i][j] = other._inColl[i][j];
            _inBlock[i][j] = other._inBlock[i][j];
            _newNumber[i][j] = other._newNumber[i][j];

        }

    _solutionId = other._solutionId;
    _validId    = other._validId;
}

Sudoku& Sudoku::operator= (const Sudoku& other){

    for(int i = 0; i<NUMBERS;i++)
        for(int j = 0; j<NUMBERS;j++){
            _elements[i][j] = other._elements[i][j];
            _inRow[i][j] = other._inRow[i][j];
            _inColl[i][j] = other._inColl[i][j];
            _inBlock[i][j] = other._inBlock[i][j];
            _newNumber[i][j] = other._newNumber[i][j];

        }

    _solutionId = other._solutionId;
    _validId    = other._validId;
    return *this;
}

void Sudoku::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *){


    int x=0, y=0; // pocetna pozicija sudokua
    int width = 40, height=40; // sirina i visina kvadratica

    QFont font = painter->font(); // font slova

    if(_solutionId){
        font.setPixelSize(width/2); // stavljamo da bude malo veci
        painter->setFont(font);

        int widthBigLine = 3; //neke linije da budu deblje
        int widthThinLine = 1;

        QPen widthPen = QPen();
        QPen thinPen = QPen();
        widthPen.setWidth(widthBigLine);
        thinPen.setWidth(widthThinLine);

        // crtamo debele linije, po jednu uspravnu i jednu vodoravnu
        painter->setPen(widthPen); // povecavamo debljinu hemijske za crtanje
        x=0;
        y=0;
        for(int i=0;i<=SQR;i++){
            painter->drawLine(x, y, x+width*NUMBERS+SQR*widthBigLine, y);

            painter->drawLine(y, x, y, x+height*NUMBERS+SQR*widthBigLine);

            y+=SQR*width+widthBigLine;
        }


        // crtamo brojeve i kvadratice
        x=widthBigLine-widthThinLine;
        y=widthBigLine-widthThinLine;
        painter->setPen(thinPen);
        for(int i=0;i<NUMBERS;i++){

            for(int j=0; j<NUMBERS;j++){

                if(_newNumber[i][j])
                    painter->setBrush(QColor(179,119,51));
                //  painter->setBrush(QPixmap(":/images/background3.jpg"));
                else
                    painter->setBrush(QColor(255,253,175));
                QRect rectangle = QRect(x, y, width, height);
                painter->drawRect(rectangle);

                painter->setBrush(Qt::black);
                if(_elements[i][j])
                    painter->drawText(rectangle,  Qt::AlignCenter, QString::number(_elements[i][j]));
                x+=width;

                if((j+1)%SQR==0)
                    x+=widthBigLine;

            }
            x=widthBigLine-widthThinLine;
            y+=height;
            if((i+1)%3==0)
                y+=widthBigLine;
        }
    }
    else{
        font.setPixelSize(30); // stavljamo da bude malo veci
        painter->setFont(font);
        painter->setBrush(Qt::black);
        QRect rectangle = QRect(0,0,400,400);
        painter->drawText(rectangle,  Qt::AlignCenter, QString("Sudoku nema resenja!\n"));
    }
    
}


void Sudoku::write_in(unsigned value, int i, int j){
    //nemam deo za proveru da li je value dobra vrednost jer pretpostavaljam da cemo to raditi u delu
    //za prepoznavanje brojeva, tj. necemo prihvatati nista sem 1,..,9


    //upisujemo prepoznatu vrednost
    _elements[i][j] = value;
    if(value==0){
        _newNumber[i][j] = true; // u kvadraticu nije bilo broja, taj broj cemo mi da pogodimo
        return;
    }
    //dodajemo informaciju u info tablice da se ucitani broj nalazi u odredjenom redu, koloni i bloku

    //smanjujemo vrijednost za 1 jer su indeksi od 0 do 8, a brojevi od 1 do 9
    value--;

    if((_inRow[i][value]) || (_inColl[j][value]) || (_inBlock[(i/SQR)*SQR+j/SQR][value])){

        _validId=false;
    }
    else{
        _inRow[i][value]  = true;                   // red i sadrzi broj value
        _inColl[j][value] = true;                   // kolona j sadrzi broj value
        _inBlock[(i/SQR)*SQR+j/SQR][value] = true;  // blok (i/SQR)*SQR+j/SQR sadrzi broj value
    }
}

void Sudoku::solve(){

    _solutionId=_validId;
    if(_solutionId){

        // mozemo rjesavati pomocu backtrackinga, ali tu se javlja problem sto se za neke vrijednosti moze desiti da backtracking traje jako dugo, pa bi bilo dobro da nekako drugacije pojednostavimo problem

        // prvo cemo pokusati da popunimo neka polja onako kako bismo u realnosti razmisljali, a onda cemo staviti kandidate za svako polje i samo za te kandidate raditi backtracking.

        bool filledSomeNumber;  // indikator da li je u trenutnoj iteraciji upisan neki broj
        // ako se na kraju petlje desi da nije upisano nista onda treba da prekinemo petlju i idemo na backtracking
        while(true){

            // pocinje nova iteracija. Nista nije upisano za sad.
            filledSomeNumber = false;

            // provera da li postoje vrednosti u nekom redu, koloni ili bloku koje se mogu nalaziti samo na jednoj poziciji
            if(writeInRow())
                filledSomeNumber = true;
            if(writeInColl())
                filledSomeNumber = true;
            if(writeInBlock())
                filledSomeNumber = true;

            // provera da li na nekoj poziciji moze biti jedinstvena vrednost
            if(writeOnPos())
                filledSomeNumber = true;

            // ako nemamo novih podataka...
            if(filledSomeNumber == false)
                break; // ...izlazimo iz beskonacne petlje
        }

        if(backtracking(0,0) == false)
            _solutionId = false;
    }
}

bool Sudoku::isCandidate(int i, int j, int value){

    // smanjujemo vrijednost za 1 jer su indeksi od 0 do 8, a brojevi od 1 do 9
    value--;

    if(_inRow[i][value] == true)
        return false;    // red i sadrzi broj value
    if(_inColl[j][value] == true)
        return false;    // kolona j sadrzi broj value
    if(_inBlock[(i/SQR)*SQR+j/SQR][value] == true)
        return false;    // blok ... sadrzi broj value

    return true;
}

bool Sudoku::writeInRow(){

    int count;        // broji moguce pozicije za cifru n
    bool ind = false; // indikator da li smo pronasli novi podatak
    int row, column;

    // Za svaki red ...
    for(int i = 0; i < NUMBERS; i++){

        // ... proveravamo da li postoji neki broj da moze biti na samo jednom mjestu u tom redu
        for(int n = 1; n <= NUMBERS; n++){

            count = 0;

            // prolazimo kroz sva polja u tom redu
            for(int j = 0; j < NUMBERS; j++){

                // provera da li je pozicija vec popunjena
                if(_elements[i][j] > 0)
                    continue;

                // provra da li je n kandidat za poziciju [i][j]
                if(isCandidate(i, j, n)){

                    // ako jeste povecavamo brojac i pamtimo poziciju.
                    count++;
                    row = i;
                    column = j;
                }

                // ako count > 1 onda odustajemo od broja n i prelazimo na sledeci...
                if(count > 1)
                    break;
            }

            // ako je n ima tacno jednu mogucu poziciju, upisujemo ga
            if(count == 1){
                write_in(n, row, column);
                ind = true;
            }
        }
    }

    return ind;

}

bool Sudoku::writeInColl(){

    int  count;        // broji moguce pozicije za cifru n
    bool ind = false;  // indikator da li smo pronasli novi podatak
    int  row, column;

    // za svaku kolonu ...
    for(int j = 0; j < NUMBERS; j++){

        // ...proveravamo da li postoji neki broj da moze biti na samo jednom mjestu u toj koloni
        for(int n = 1; n <= NUMBERS; n++){

            count = 0;

            // prolazimo kroz sva polja u toj koloni
            for(int i = 0; i < NUMBERS; i++){

                // provra da li je pozicija popunjena
                if(_elements[i][j] > 0)
                    continue;

                // provera da li je n kandidat za poziciju [i][j]
                if(isCandidate(i, j, n)){

                    // ako jeste povecavamo brojac i pamtimo poziciju
                    count++;
                    row = i;
                    column = j;
                }

                // ako count > 1 onda odustajemo od broja n i prelazimo na sledeci
                if(count > 1)
                    break;

            }

            // ako n imalo tacno jednu mogucu poziciju, upisujemo ga
            if(count == 1){

                write_in(n, row, column);
                ind = true;
            }
        }
    }

    return ind;
}

bool Sudoku::writeInBlock(){

    int count;        // broji moguce pozicije za cifru n
    bool ind = false; // indikator da li smo pronasli novi podatak
    int row, column;

    // Za svaki blok ...
    for(int i = 0; i < NUMBERS; i++){

        // ...proveravamo da li postoji neki broj da moze biti na samo jednom mjestu u tom bloku
        for(int n = 1; n <= NUMBERS; n++){

            count = 0;

            //prolazimo kroz sva polja u tom bloku

            //i. blok:
            //  [(i/SQR)*SQR][(i%SQR)*SQR]   [(i/SQR)*SQR][(i%SQR)*SQR+1]   [(i/SQR)*SQR][(i%SQR)*SQR+2]
            //  [(i/SQR)*SQR+1][(i%SQR)*SQR] [(i/SQR)*SQR+1][(i%SQR)*SQR+1] [(i/SQR)*SQR+1][(i%SQR)*SQR+2]
            //  [(i/SQR)*SQR+2][(i%SQR)*SQR] [(i/SQR)*SQR+2][(i%SQR)*SQR+1] [(i/SQR)*SQR+2][(i%SQR)*SQR+2]

            int ir = (i / SQR) * SQR;
            int jr = (i % SQR) * SQR;

            for(int j = 0; j < SQR ; j++){
                for(int k = 0; k < SQR; k++){

                    //provera da li je pozicija vec popunjena
                    if(_elements[ir+j][jr+k]>0)
                        continue;

                    // provera da li je n kandidat da bude na poziciji [ir+j][jr+k]
                    if(isCandidate(ir+j, jr+k, n)){

                        // ako jeste povecavamo brojac i pamtimo poziciju
                        count++;
                        row = ir + j;
                        column = jr + k;
                    }

                    // ako count > 1 onda odustajemo od broja n i izlazimo iz unutrasnje petlje
                    if(count > 1)
                        break;
                }

                // ako count > 1 izlazimo iz spoljasnje petlje
                if(count > 1)
                    break;

            }

            // ako je n imalo tacno jednu mogucu poziciju upisujemo ga
            if(count == 1){

                write_in(n, row, column);
                ind = true;
            }
        }
    }

    return ind;
}

bool Sudoku::writeOnPos(){

    int count;
    int candidate;
    bool ind = false;

    // za svaku poziciju...
    for(int i = 0; i < NUMBERS; i++){
        for(int j = 0; j < NUMBERS; j++){

            // proveravamo da li je pozicija popunjena
            if(_elements[i][j] > 0)
                continue;

            // proveravamo da li postoji samo jedan kandidat za tu poziciju
            count = 0;
            for(int n = 1; n <= NUMBERS; n++){

                // provera da li je n kandidat
                if(isCandidate(i, j, n)){

                    count++;
                    if(count > 1)
                        break;
                    candidate = n;
                }
            }

            // ako smo imali samo jednog kandidata...
            if(count == 1){
                write_in(candidate, i, j);
                ind = true;
            }
        }
    }

    return ind;
}

bool Sudoku::backtracking(int i, int j){

    // ako smo presli cijeli red...
    if(j == NUMBERS){

        // ...idemo na sledeci
        j = 0;
        i++;

        //Ako smo presli cijeli sudoku, onda izlazimo iz funkcije sudoku je resen
        if(i == NUMBERS)
            return true;
    }


    // ako je polje vec popunjeno idemo na sljedece
    if(_elements[i][j] > 0){
        return backtracking(i, j+1);
    }

    //proveravamo redom za sve brojeve
    for(int n = 1; n <= NUMBERS; n++){
        if(isCandidate(i, j, n)){

            write_in(n, i, j);
            if(backtracking(i, j+1))
                return true;

            else
                delete_in(n, i, j);
        }
    }

    return false;
}

// funkcija brise vrednost sa pozicije [i][j]
void Sudoku::delete_in(unsigned value, int i, int j){

    // smanjujemo vrijednost za 1 jer su indeksi od 0 do 8, a brojevi od 1 do 9
    value--;

    _inRow[i][value]  = false;                  // Red i sadrzi broj value
    _inColl[j][value] = false;                  // Kolona j sadrzi broj value
    _inBlock[(i/SQR)*SQR+j/SQR][value] = false; // Blok (i/SQR)*SQR+j/SQR sadrzi broj value
    _elements[i][j] = 0;

}

ostream& operator<<(ostream &ostr, const Sudoku& m){

    ostr<<" -----------------------------"<<endl;

    for(unsigned i=0; i<NUMBERS; i++){
        ostr<<"| ";
        for(unsigned j=0; j<NUMBERS; j++){
            ostr<<m._elements[i][j]<<" ";
            if(((j%SQR)==2))
                ostr<<"| ";
            else
                ostr<<" ";
        }

        ostr<<endl;
        if((i%SQR)==2)
            ostr<<" -----------------------------"<<endl;
    }

    return ostr;
}
// convert  -resize 30% background0.jpeg background0.jpeg

