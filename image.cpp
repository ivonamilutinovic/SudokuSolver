#include "image.h"
#include "sudoku.h"


// globalne promenljive, sirina i visina matrice
int width;
int height;

// funkcija cita bitmapu i pretvara sliku u crno-belu
std::vector<std::vector<unsigned char>> readImage(std::string pathname){

    std::ifstream file(pathname, std::ios::binary);

    if (!file.is_open()){
        std::cerr << "Cannot open file" << pathname << std::endl;
        exit(EXIT_FAILURE);
    }

    // informacije iz fileheader-a
    static constexpr size_t INFO_SIZE = 54;
    std::array<char, INFO_SIZE> headerInfo;

    file.read(headerInfo.data(), headerInfo.size());

    // provera da li je fajl .bmp fajl
    if(headerInfo[0] != 'B' || headerInfo[1] != 'M'){
        std::cerr << "Fajl mora imati .bmp ekstenziju" << std::endl;
        exit(EXIT_FAILURE);
    }

    // uzimanje informacija o velicini, sirini i visini slike iz fileheader-a
    int dataSize = *(int*)&headerInfo[2] - INFO_SIZE;
    width = *(int*)&headerInfo[18];
    height = *(int*)&headerInfo[22];

    // citanje podataka iz bitmape
    std::vector<char> img(dataSize);
    file.read(img.data(), img.size());

    std::vector<unsigned char> Buffer(dataSize);

    int padding = 0;
    int scanlinebytes = width * 3;
    while ( ( scanlinebytes + padding ) % 4 != 0 )
        padding++;

    int paddWidth = scanlinebytes + padding;

    // prepoznavanje slike
    size_t k;       //za upis pozicija u vektor pom
    size_t imgPos = 0;
    size_t newPos = 0;

    std::vector<std::vector<unsigned char>> matrix;
    std::vector<unsigned char> pom(width);
    for ( int x = 0; x < height; x++ ){
        k = 0;

        for ( int y = 0; y < 3 * width; y+=3 )
        {
            newPos = x * 3 * width + y;
            imgPos = ( height - x - 1 ) * paddWidth + y;

            Buffer[newPos]     = img[imgPos + 2];
            Buffer[newPos + 1] = img[imgPos + 1];
            Buffer[newPos + 2] = img[imgPos];


            //pretvaranje slike u crno-belu
            if((Buffer[newPos] + Buffer[newPos + 1] + Buffer[newPos + 2])/3 > 127)
                pom[k] = 255;
            else
                pom[k] = 0;
            k++;

        }
        
        //dodamo u matricu i ispraznimo pom
        matrix.push_back(pom);
        pom.empty();
    }
    return matrix;
    
}


//da li ima vodoravnu liniju preko cele kocke
bool horizontalLine(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){
    bool ind;
    for(int i=ul_r;i<ul_r+h;i++){
        ind=true;
        for(int j=ul_c;j<ul_c+w;j++){
            if(image[i][j]==255){
                ind=false;
                break;
            }
        }

        if(ind)
            return true;
    }

    return false;
}

//da li ima uspravnu liniju
bool verticalLine(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){
    bool ind;
    for(int j=ul_c;j<ul_c+w;j++){
        ind=true;
        for(int i=ul_r;i<ul_r+h;i++){
            if(image[i][j]==255){
                ind=false;
                break;
            }
        }

        if(ind)
            return true;
    }

    return false;
}

//koliko ima crnih tackica
int pixels(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){
    int br=0;

    for(int i=ul_r;i<ul_r+h;i++){

        for(int j=ul_c;j<ul_c+w;j++){
            if(image[i][j]==0){
                br++;
            }
        }

    }


    return br;
}

//da li ima liniju, takvu da s leve strane ima neku tacku, a sa desne nema
bool emptyRight(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){

    bool ind=false;
    for(int i=ul_r;i<ul_r+h;i++){

        int levi=pixels(i, ul_c, 1, w/2, image);
        int desni=pixels(i, ul_c+w/2,1, w-w/2, image);
        if((levi>0) && (desni==0)){

            ind=true;
            break;
        }

    }

    return ind;
}

// Ojlerova funkcija iz knjige
int eulerTest(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){
    int bqone = 0, bqthree = 0, bqd = 0;
    for(int i=ul_r-1; i<=ul_r+h+1; i++){
        for(int j=ul_c-1; j<=ul_c+w+1; j++){

            if(image[i][j]==0 && image[i+1][j]==255 && image[i][j+1]==255 && image[i+1][j+1]==255)
                bqone++;
            if(image[i][j]==255 && image[i+1][j]==0 && image[i][j+1]==255 && image[i+1][j+1]==255)
                bqone++;
            if(image[i][j]==255 && image[i+1][j]==255 && image[i][j+1]==0 && image[i+1][j+1]==255)
                bqone++;
            if(image[i][j]==255 && image[i+1][j]==255 && image[i][j+1]==255 && image[i+1][j+1]==0)
                bqone++;

            if(image[i][j]==255 && image[i+1][j]==0 && image[i][j+1]==0 && image[i+1][j+1]==0)
                bqthree++;
            if(image[i][j]==0 && image[i+1][j]==255 && image[i][j+1]==0 && image[i+1][j+1]==0)
                bqthree++;
            if(image[i][j]==0 && image[i+1][j]==0 && image[i][j+1]==255 && image[i+1][j+1]==0)
                bqthree++;
            if(image[i][j]==0 && image[i+1][j]==0 && image[i][j+1]==0 && image[i+1][j+1]==255)
                bqthree++;

            if(image[i][j]==0 && image[i+1][j]==255 && image[i][j+1]==255 && image[i+1][j+1]==0)
                bqd++;
            if(image[i][j]==255 && image[i+1][j]==0 && image[i][j+1]==0 && image[i+1][j+1]==255)
                bqd++;
        }
    }
    return (bqone-bqthree-2*bqd)/4;


}

//funkcija smanjuje kockicu na minimalnu mogucu i prepoznaje broj
int numberRecognition(int ul_r, int ul_c, int h, int w, std::vector<std::vector<unsigned char>> image){


    //provera da izbrisemo prazne redove iz kockice
    bool prazno=true;
    //zdesna
    while(w>0){
        for(int i=ul_r;i<ul_r+h;i++){
            if(image[i][ul_c+w-1]==0){
                prazno=false;
            }
        }

        if(prazno==false)
            break;
        w--;
    }
    //sleva
    prazno=true;
    while(w>0){
        for(int i=ul_r;i<ul_r+h;i++){
            if(image[i][ul_c]==0){
                prazno=false;
            }
        }

        if(prazno==false)
            break;
        w--; ul_c++;
    }
    //odozdo
    prazno=true;
    while(h>0){
        for(int j=ul_c;j<ul_c+w;j++){
            if(image[ul_r+h-1][j]==0){
                prazno=false;
            }
        }

        if(prazno==false)
            break;
        h--;
    }
    //odozgo

    prazno=true;
    while(h>0){
        for(int j=ul_c;j<ul_c+w;j++){
            if(image[ul_r][j]==0){
                prazno=false;
            }
        }

        if(prazno==false)
            break;
        h--; ul_r++;
    }




    auto euler = eulerTest(ul_r, ul_c, h, w, image);

    // Euler=-1
    if(euler==-1)
        return 8;

    // Euler=0
    if(euler==0){
        bool iU, iV, iDP;
        int pL, pD;

        iU=verticalLine(ul_r, ul_c, h, w, image);
        iV=horizontalLine(ul_r, ul_c, h, w, image);
        iDP=emptyRight(ul_r+h/4, ul_c, h/2-h/4, w, image);
        //ako ima uspravnu i vodoravnu liniju od pocetka do kraja onda je 4

        pL=pixels(ul_r, ul_c, h, w/2, image);
        pD=pixels(ul_r, ul_c+w/2, h, w-(w/2), image);
        //  printf("iu %d iv %d  pl %d pd %d\n", iU, iV, pL, pD);

        //ako ima uspravnu i vodoravnu liniju od pocetka do kraja onda je 4
        if(iU && iV)
            return 4;
        // ako ima desno praznu liniju u drugoj cetvrtini visine
        else {
            if(iDP)
                return 6;
            // nema desno praznu i nije prazna kocka
            else if(pL+pD)
                return 9;
        }
    }

    // Euler=1
    if(euler==1){
        bool iU=verticalLine(ul_r, ul_c, h, w, image);
        bool iVD=horizontalLine(ul_r+h/2, ul_c, h-h/2, w, image);
        bool iV=horizontalLine(ul_r, ul_c, h, w, image);
        bool dP=emptyRight(ul_r, ul_c, h/2, w, image);
        int dc=pixels(ul_r+h/2, ul_c+w-2, h-h/2, 1, image);
        int uk=pixels(ul_r, ul_c, h, w, image);

        //printf("iu%d ivd%d iv%d dp%d dc%d uk%d\n", iU, iVD, iV, dP, dc, uk);

        //ako ima uspravnu preko cele kocke onda 1
        if(iU)
            return 1;
        //inace  ako ima na dnu vodravnu onda 2
        else {
            if(iVD)
                return 2;
            //ako ima bilo gde vodoravnu onda 7 i od desnog coska nema vise tacak crnih
            else if(iV && (dc==0))
                return 7;
            // postoji linija tako da sa leve polovine ima tackica a desna polovina je prazna onda 5
            else if(dP)
                return 5;
            // inace 3
            else if(uk>0)
                return 3;
        }


    }

    return 0;

}


// prepoznaje linije sudokua i salje numberRecognizer funkciji poziciju levog gornjeg ugla kvadratica, kao i sirinu tog kvadratica
void lineRecognition(Sudoku& sudoku, std::vector<std::vector<unsigned char>> matrix){

    int PosI, PosJ;

    // racunamo poziciju prve crne tacke krenuvsi od gornjeg levog ugla slike, poziciju smestamo u (PosI, PosJ)
    int ind = false; //indikator da smo nasli prvi crni piksel kako bismo izasli iz spoljasnje petlje
    for(int i = 0; i< height; i++){
        if(ind)
            break;
        for(int j = 0; j< width; j++){
            if(matrix[i][j] == 0){
                ind = true;
                PosI = i;
                PosJ = j;
                break;
            }
        }
    }

    // racunamo sirinu sudokua
    int sudokuWidth = 0;
    for(int j = PosJ; j < width; j++){
        if(matrix[PosI][j] == 255)
            break;
        sudokuWidth++;
    }

    // racunamo visinu sudokua
    int sudokuHeight = 0;
    for(int i = PosI; i<height;i++){
        if(matrix[i][PosJ] == 255)
            break;
        sudokuHeight++;
    }

    // racunamo debljinu horizontalne ivice sudokua
    int borderHorizontalLineWidth = 0;
    for(int i = PosI; i < height; i++){
        if(matrix[i][PosI+sudokuWidth/2] == 255)
            break;
        borderHorizontalLineWidth++;
    }


    // racunamo debljinu vertikalne ivice sudokua
    int borderVerticalLineWidth = 0;
    for(int j = PosJ; j < width; j++){
        if(matrix[PosJ+sudokuHeight/2][j] == 255)
            break;
        borderVerticalLineWidth++;
    }

    int squareHeight, squareWidth;
    int innerHorizontalLineWidth, innerVerticalLineWidth;
    int hole;
    int numberPosI = 0, numberPosJ = 0; //pozicija na kojoj upisujemo broj u matricu sudokua
    
 
    for(int i = PosI + borderHorizontalLineWidth; i < (PosI + sudokuHeight) ; i+= (squareHeight + innerHorizontalLineWidth), numberPosI++){
        
        hole = 0; 
        innerHorizontalLineWidth = 0;
        
        // izracunavamo visinu kockice
        squareHeight = 0;
        while(matrix[i][PosJ+borderVerticalLineWidth+hole] == 0){
            hole++;
        }
        
        for(int k = i; k < (PosI + sudokuHeight); k++){

            if(matrix[k][PosJ+borderVerticalLineWidth+hole] == 0){
                squareHeight = k - i;
                
                while(((k+innerHorizontalLineWidth)<(PosI + sudokuHeight)) && (matrix[k+innerHorizontalLineWidth][PosJ+borderVerticalLineWidth+hole]==0)){

                    innerHorizontalLineWidth++;
                }

            
                if((k - i) > (sudokuHeight/9)){
                    k = i;
                    hole++;
                }
                else 
                    break;
            }
        }    
        numberPosJ=0;
        
        // racunamo sirinu za 9 kockica
        for(int j=PosJ+borderVerticalLineWidth;j<(sudokuWidth+PosJ);j+=(squareWidth+innerVerticalLineWidth), numberPosJ++){

            hole = 0; 
            innerVerticalLineWidth = 0;
            squareWidth = 0;
            while(matrix[i+hole][j]==0){

                hole++;
            }
            for(int k = j;k<(PosJ + sudokuWidth);k++){

                if(matrix[i+hole][k]==0){
                    
                    squareWidth = k - j;
                    while(((k+innerVerticalLineWidth)<(PosJ + sudokuWidth)) && (matrix[i+hole][k+innerVerticalLineWidth]==0)){

                        innerVerticalLineWidth++;
                    }
   
                
                    if((k-j)>(sudokuWidth/9)){
                        k = j;
                        hole++;
                    }
                    else 
                        break;
                }
            }

            //pozivamo funkciju koja prepoznaje broj
            int number = numberRecognition(i+1, j+1, squareHeight-2, squareWidth-2, matrix);
            sudoku.write_in(number, numberPosI, numberPosJ);

        }

    }
}
