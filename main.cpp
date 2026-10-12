#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG


#include "stb_image.h"
#include <iostream>
#include <string>
#include <vector>
//nirgends mehr std nötig
using namespace std;

// const char* weil stbi_load diesen Datentyp erwartet
const char* bild_name = "Bild Mosaik schief.jpeg";
//Matrix mit den Farb Positonen

struct Bild {
    unsigned char* daten;
    int breite;
    int hoehe;
};

struct RGB_Wert {
    unsigned char rot;
    unsigned char gruen;
    unsigned char blau;
};

//RGB farben mit min und max werten
struct Farben_RGB {     
    unsigned char rMin, rMax;
    unsigned char gMin, gMax;
    unsigned char bMin, bMax;
};

namespace RGB {
    Farben_RGB grün = {
        .rMin = 55,    .rMax = 85,
        .gMin = 120,   .gMax = 255,
        .bMin = 120,   .bMax = 210
    };

    Farben_RGB gelb = {
        .rMin = 235, .rMax = 255,
        .gMin = 235,   .gMax = 255,
        .bMin = 60,   .bMax = 85
    };

    Farben_RGB blau = {
        .rMin = 30,    .rMax = 80,
        .gMin = 110,   .gMax = 148,
        .bMin = 160,   .bMax = 255
    };

    Farben_RGB dunkelgrau = {
        .rMin = 40,    .rMax = 70,
        .gMin = 40,   .gMax = 70,
        .bMin = 40,   .bMax = 70
    };
}

Bild bild_laden(const char* dateiname ) {
    Bild bild{};
    int farbkanaele;
    const int gewuenschteKanaele = 3;

    bild.daten = stbi_load(dateiname, &bild.breite, &bild.hoehe,&farbkanaele, gewuenschteKanaele);

    if (bild.daten == nullptr) {
        std::cout << stbi_failure_reason() << "\n";
    }

    return bild;
}

RGB_Wert pixel_holen(const Bild& gespeichertes_bild, int spalte, int zeile) {

    const int position = (zeile * gespeichertes_bild.breite + spalte) * 3;

    RGB_Wert pixel{};

    pixel.rot = gespeichertes_bild.daten[position];
    pixel.gruen = gespeichertes_bild.daten[position + 1];
    pixel.blau = gespeichertes_bild.daten[position + 2];

    return pixel;
}

std::string welche_farbe(const Bild& gespeicherte_bild, int Spalten, int Zeilen) {

    const RGB_Wert pixel = pixel_holen(gespeicherte_bild, Spalten, Zeilen);
    bool istGrün, istGelb, istBlau, istDunkelgrau;

    //für grün
    if( pixel.rot >= RGB::grün.rMin && pixel.rot <= RGB::grün.rMax &&
        pixel.gruen >= RGB::grün.gMin && pixel.gruen <= RGB::grün.gMax &&
        pixel.blau >= RGB::grün.bMin && pixel.blau <= RGB::grün.bMax 
    ) {
        istGrün = true;
    } else {
        istGrün = false;
    }
    
    //für gelb
    if( pixel.rot >= RGB::gelb.rMin && pixel.rot <= RGB::gelb.rMax &&
        pixel.gruen >= RGB::gelb.gMin && pixel.gruen <= RGB::gelb.gMax &&
        pixel.blau >= RGB::gelb.bMin && pixel.blau <= RGB::gelb.bMax 
    ) {
        istGelb = true;
    } else {
        istGelb = false;
    }

    //für blau
    if( pixel.rot >= RGB::blau.rMin && pixel.rot <= RGB::blau.rMax &&
        pixel.gruen >= RGB::blau.gMin && pixel.gruen <= RGB::blau.gMax &&
        pixel.blau >= RGB::blau.bMin && pixel.blau <= RGB::blau.bMax 
    ) {
        istBlau = true ;
    } else {
        istBlau = false;
    }

    //für grau
    if( pixel.rot >= RGB::dunkelgrau.rMin && pixel.rot <= RGB::dunkelgrau.rMax &&
        pixel.gruen >= RGB::dunkelgrau.gMin && pixel.gruen <= RGB::dunkelgrau.gMax &&
        pixel.blau >= RGB::dunkelgrau.bMin && pixel.blau <= RGB::dunkelgrau.bMax 
    ) {
        istDunkelgrau = true ;
    } else {
        istDunkelgrau = false;
    }
     
    std::string Gelb, Blau, Grün, Keine;
    if (istBlau == false && istGelb == false && istGrün == false && istDunkelgrau == false) {
        return "Keine"; 
    } else if (istBlau == true) {
        return "Blau";
    } else if (istGrün == true) {
        return "Gruen";
    } else if (istGelb == true) {
        return "Gelb";
    } else if (istDunkelgrau == true){
        return "Dunkelgrau";
    }
    
    //Soll verhindert die Warnung das wenn kein if läuft nichts zurückgegeben wird
    return "Keine"; 
    
}


int zähle_dunkelgraue_pixel_x(const Bild& bild) {
    bool nicht_grau = true;
    int MaxAnzahlGrauPixel = 0;
    int zeile;
    int spalte;

    while (nicht_grau) {
        for (zeile = 0; zeile < bild.hoehe; zeile++ ){
            for (spalte = 0; spalte < bild.breite; spalte++ ) {
                //stoppt bei dem ersten grauen pixel
                std::string farbe = welche_farbe(bild, spalte, zeile);
                if (farbe == "Dunkelgrau") {
                    nicht_grau = false;
                    break;
                }
            }
            //Sonst stoppt nur die Spalte das ! ist ein false
            //stoppt bei der zeile wo das erste grau ist
            if (!nicht_grau){
                break;
            }
        }
    }

    //geht weiter und schaut das wenn die zeile nicht grau ist diese geht bis sie grau ist dann bricht es ab
    while (!nicht_grau) {
        for (spalte = 0; spalte < bild.breite; spalte++ ) {
            std::string farbe = welche_farbe(bild, spalte, zeile);
            if (farbe == "Dunkelgrau") {
                MaxAnzahlGrauPixel++;
            }
        }
        nicht_grau = true;
    }

    return MaxAnzahlGrauPixel;
}

int zähle_dunkelgraue_pixel_y(const Bild& bild) {
    bool nicht_grau = true;
    int MaxAnzahlGrauPixel = 0;
    int spalte;
    int zeile;

    while (nicht_grau) {
        for (spalte = 0; spalte < bild.breite; spalte++ ){
            for (zeile = 0; zeile < bild.hoehe; zeile++ ) {
                std::string farbe = welche_farbe(bild, spalte, zeile);
                if (farbe == "Dunkelgrau") {
                    nicht_grau = false;
                    break;
                }
            }
            if (!nicht_grau){
                break;
            }
        }
    }

    while (!nicht_grau) {
        for (zeile = 0; zeile < bild.hoehe; zeile++ ) {
            std::string farbe = welche_farbe(bild, spalte, zeile);
            if (farbe == "Dunkelgrau") {
                MaxAnzahlGrauPixel++;
            }
        }
        nicht_grau = true;
    }

    return MaxAnzahlGrauPixel;
}


class Gridline_einteilung {

    private:
    double toleranz;
    int min_anzahl_Dg_x, min_anzahl_Dg_y; // 
    std::vector<int> gridlines_x;  // Speichert die Pos für X
    std::vector<int> gridlines_y;  // Speichert die Pos für Y
    // x wäre hier zeile
    std::vector<int> dunkelgraue_pixel_x(const Bild& bild) {
        std::vector<int> anzahl(bild.hoehe, 0 );

        for (int zeile = 0; zeile < bild.hoehe; zeile++) {
            for (int spalte = 0; spalte < bild.breite; spalte++) {
                if (welche_farbe(bild, spalte, zeile) == "Dunkelgrau") {
                    anzahl[zeile]++;
                }
            }
        }
        return anzahl;
    }
// wäre hier spalte
    std::vector<int> dunkelgraue_pixel_y(const Bild& bild) {
        std::vector<int> anzahl(bild.breite, 0); 

        for (int spalte = 0; spalte < bild.breite; spalte++) {
            for (int zeile = 0; zeile < bild.hoehe; zeile++) {
                if (welche_farbe(bild, spalte, zeile) == "Dunkelgrau") {
                    anzahl[spalte]++;
                }
            }
        }
        return anzahl;
    }   
    
    std::vector<int> kandidaten_x(const std::vector<int>& anzahl1) {
        std::vector<int> kandidaten;

        for (int i = 0; i < anzahl1.size(); i++) {
            if (anzahl1[i] > min_anzahl_Dg_x * toleranz) {
                kandidaten.push_back(i);
            }
        }
        return kandidaten;
    }
    
    std::vector<int> kandidaten_y(const std::vector<int>& anzahl1) {
        std::vector<int> kandidaten;

        for (int i = 0; i < anzahl1.size(); i++) {
            if (anzahl1[i] > min_anzahl_Dg_y * toleranz) {
                kandidaten.push_back(i);
            }
        }
        return kandidaten;
    }

    std::vector<int> gruppieren_mitteln(const std::vector<int>& kandidaten1) { 
        std::vector<int> Dg_gruppen;
        std::vector<int> Dg_Pixel_resultat;

        for (int i = 0; i < kandidaten1.size(); i++) {
            if (i > 0 && kandidaten1[i] - kandidaten1[i-1] <= 2) {
                Dg_gruppen.push_back(kandidaten1[i]);   
            } else {
                if (!Dg_gruppen.empty()) {
                    int summe = 0;
                    for (int wert : Dg_gruppen) {
                        summe += wert;
                    }
                    int mittelwert = summe / Dg_gruppen.size();  
                    Dg_Pixel_resultat.push_back(mittelwert);
                }
                Dg_gruppen.clear();
                Dg_gruppen.push_back(kandidaten1[i]);
            }
        }
        
        if (!Dg_gruppen.empty()) {
            int summe = 0;
            for (int wert : Dg_gruppen) {
                summe += wert;
            }
            int mittelwert = summe / Dg_gruppen.size();  
            Dg_Pixel_resultat.push_back(mittelwert);
        }
        
        return Dg_Pixel_resultat;
    }
    
    public: 

    //Constructor wird aufgerufen wenn die Klasse aufgerufen wurde
    //die initializer liste sorgt dafür, dass wenn ich unten das Objekt erstelle, den Wert den ich in () eingebe für min anzhal Dg der parameter ist
    Gridline_einteilung(int min_dg_x, int min_dg_y, double tol) 
        : min_anzahl_Dg_x(min_dg_x), min_anzahl_Dg_y(min_dg_y), toleranz(tol){}
        
    void berechne_gridlines(const Bild& bild) {
        // X-Achse -> pro Zeile zählen
        std::vector<int> anzahl_x = dunkelgraue_pixel_x(bild);
        std::vector<int> kand_x = kandidaten_x(anzahl_x);
        gridlines_x = gruppieren_mitteln(kand_x);
        
        // Y-Achse -> pro Spalte zählen
        std::vector<int> anzahl_y = dunkelgraue_pixel_y(bild);
        std::vector<int> kand_y = kandidaten_y(anzahl_y);
        gridlines_y = gruppieren_mitteln(kand_y);
    }

    std::vector<int> get_gridlines_x() { return gridlines_x; }
    std::vector<int> get_gridlines_y() { return gridlines_y; }

};

std::vector<vector<string>> Kachel_grid_mitte() {
    vector<vector<string>> kachel_mittelpunkt;
    Bild bild = bild_laden(bild_name);
    int min_dg_pixel_x = zähle_dunkelgraue_pixel_x(bild);
    int min_dg_pixel_y = zähle_dunkelgraue_pixel_y(bild);
    
    Gridline_einteilung grid(min_dg_pixel_x, min_dg_pixel_y, 0.95);
    grid.berechne_gridlines(bild);
    vector<int> x = grid.get_gridlines_x();
    vector<int> y = grid.get_gridlines_y();

    for (int i = 0; i < x.size() - 1; i++) {
        vector<string> zeile;
        for (int j = 0; j < y.size() - 1; j++) {
            int mittel_x = (x[i] + x[i+1]) / 2;
            int mittel_y = (y[j] + y[j+1]) / 2;
            
            string farbe = welche_farbe(bild, mittel_y, mittel_x);
            zeile.push_back(farbe);
        }
        kachel_mittelpunkt.push_back(zeile);
    }

    stbi_image_free(bild.daten);
    return kachel_mittelpunkt;
}

int main() {
    vector<vector<string>> matrix = Kachel_grid_mitte();

    for (int i = 0; i < matrix.size(); i++) {

        for (int j = 0; j < matrix[i].size(); j++) {

            cout << matrix[i][j] << "   ";
        }
        cout << "\n";
        
    }
    return 0;

}
